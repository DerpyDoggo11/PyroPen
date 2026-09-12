// ============================================================================
//  PyroPen firmware
//  MCU : ATtiny1616 @ 10 MHz internal (3.3V), megaTinyCore
//
//  A pen-format non-contact thermometer: an MLX90614 IR sensor reads the
//  object temperature, which is shown as a left->right bar on a 30-LED
//  charlieplexed display. A piezo buzzer and a front LED warn when the
//  target is hot. A single button toggles the temperature unit (short press)
//  and mutes the buzzer (long press).
//
//  Hardware map (from the KiCad schematic/PCB netlist):
//    Charlieplex rails : PA4 PA5 PA6 PA7 PB5 PB4   (MATRIX_1..6, via R1..R6)
//    IR sensor (I2C)   : SDA=PB1  SCL=PB0          (MLX90614 on J3, addr 0x5A)
//    Buzzer (BTL)      : PC1 (BUZZ) + PC0 (BUZZ2)  (piezo across J4)
//    Front LED         : PC2                        (via R10 -> J2, active high)
//    Button            : PA2  (to GND, active low)
//    Serial debug      : TX=PB2  RX=PB3  @115200
// ============================================================================

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>

// ----------------------------------------------------------------------------
// Pins
// ----------------------------------------------------------------------------
static const uint8_t PIN_FRONT_LED = PIN_PC2;
static const uint8_t PIN_BUTTON    = PIN_PA2;
static const uint8_t PIN_BUZZ_A    = PIN_PC1;   // BUZZ
static const uint8_t PIN_BUZZ_B    = PIN_PC0;   // BUZZ2 (driven anti-phase)

// ----------------------------------------------------------------------------
// Charlieplex display: 6 rails, 30 LEDs (fully populated).
// The panel is an irregular dot cloud, so LEDs are ordered here by their
// physical X position: index 0 = leftmost dot ... 29 = rightmost. That makes
// a temperature "bar" as simple as lighting indices 0..n-1.
// LED_AC[i] = {anode rail, cathode rail}. Rails: 0=PA4 1=PA5 2=PA6 3=PA7 4=PB5 5=PB4
// ----------------------------------------------------------------------------
#define NUM_RAILS 6
#define NUM_LEDS  30

static const uint8_t LED_AC[NUM_LEDS][2] = {
  {5,0}, // [ 0] D36
  {4,0}, // [ 1] D35
  {0,1}, // [ 2] D2
  {5,4}, // [ 3] D41
  {1,0}, // [ 4] D32
  {1,5}, // [ 5] D11
  {5,1}, // [ 6] D26
  {4,5}, // [ 7] D20
  {2,5}, // [ 8] D15
  {0,2}, // [ 9] D3
  {4,2}, // [10] D29
  {2,4}, // [11] D14
  {0,4}, // [12] D5
  {0,5}, // [13] D6
  {2,0}, // [14] D33
  {5,2}, // [15] D30
  {0,3}, // [16] D4
  {3,0}, // [17] D34
  {3,2}, // [18] D28
  {2,3}, // [19] D13
  {3,5}, // [20] D18
  {3,1}, // [21] D24
  {2,1}, // [22] D23
  {1,2}, // [23] D8
  {4,1}, // [24] D25
  {3,4}, // [25] D17
  {1,3}, // [26] D9
  {1,4}, // [27] D10
  {4,3}, // [28] D38
  {5,3}, // [29] D39
};

// Direct-port descriptors for each rail (fast, glitch-free multiplexing).
// PA4..PA7 live on PORTA bits 4..7; PB4/PB5 on PORTB bits 4/5.
struct RailIO { PORT_t* port; uint8_t bm; };
static RailIO rail[NUM_RAILS] = {
  { &PORTA, PIN4_bm }, // rail0 PA4
  { &PORTA, PIN5_bm }, // rail1 PA5
  { &PORTA, PIN6_bm }, // rail2 PA6
  { &PORTA, PIN7_bm }, // rail3 PA7
  { &PORTB, PIN5_bm }, // rail4 PB5
  { &PORTB, PIN4_bm }, // rail5 PB4
};
static const uint8_t PORTA_RAIL_MASK = PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm;
static const uint8_t PORTB_RAIL_MASK = PIN4_bm | PIN5_bm;

// Frame buffer: one bit per LED.
static uint32_t frame = 0;

static inline void railsAllHiZ() {
  // Set every rail pin back to input (Hi-Z). Leaves PB0..PB3 (I2C/serial) alone.
  PORTA.DIRCLR = PORTA_RAIL_MASK;
  PORTB.DIRCLR = PORTB_RAIL_MASK;
}

static inline void lightPair(uint8_t a, uint8_t c) {
  railsAllHiZ();
  rail[a].port->OUTSET = rail[a].bm;  // anode high
  rail[a].port->DIRSET = rail[a].bm;
  rail[c].port->OUTCLR = rail[c].bm;  // cathode low
  rail[c].port->DIRSET = rail[c].bm;
}

// Display timing: 30 equal slots keep brightness uniform regardless of how
// many LEDs are on. ~250us/slot -> ~7.5ms/frame -> ~130 Hz (flicker-free).
static const uint16_t SLOT_US = 250;

// Render one full frame, then park the display Hi-Z.
static void renderFrame() {
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    if (frame & ((uint32_t)1 << i)) {
      lightPair(LED_AC[i][0], LED_AC[i][1]);
    } else {
      railsAllHiZ();
    }
    delayMicroseconds(SLOT_US);
  }
  railsAllHiZ();
}

// Show 'n' LEDs as a bar from the left.
static void setBar(uint8_t n) {
  if (n > NUM_LEDS) n = NUM_LEDS;
  frame = (n >= 32) ? 0xFFFFFFFFul : (((uint32_t)1 << n) - 1);
}

// ----------------------------------------------------------------------------
// Buzzer: piezo is wired across PC1/PC0. Driving them anti-phase gives a
// bridge-tied load (roughly double the voltage swing) for a louder beep.
// Blocking, so keep beeps short.
// ----------------------------------------------------------------------------
static void beep(uint16_t freqHz, uint16_t ms) {
  if (freqHz == 0 || ms == 0) return;
  pinMode(PIN_BUZZ_A, OUTPUT);
  pinMode(PIN_BUZZ_B, OUTPUT);
  uint32_t halfUs   = 500000UL / freqHz;
  uint32_t cycles   = (uint32_t)ms * 1000UL / (halfUs * 2);
  for (uint32_t i = 0; i < cycles; i++) {
    digitalWrite(PIN_BUZZ_A, HIGH); digitalWrite(PIN_BUZZ_B, LOW);
    delayMicroseconds(halfUs);
    digitalWrite(PIN_BUZZ_A, LOW);  digitalWrite(PIN_BUZZ_B, HIGH);
    delayMicroseconds(halfUs);
  }
  // Rest both terminals low so the piezo isn't left charged.
  digitalWrite(PIN_BUZZ_A, LOW);
  digitalWrite(PIN_BUZZ_B, LOW);
}

// ----------------------------------------------------------------------------
// Application state
// ----------------------------------------------------------------------------
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

static bool  sensorOK   = false;
static bool  useFahren  = false;   // false = Celsius (short-press toggles)
static bool  buzzerMute = false;   // long-press toggles

// Bar range (object temperature in Celsius): tuned for soldering/heat work.
static const float TEMP_MIN_C = 25.0f;
static const float TEMP_MAX_C = 250.0f;
// Warning threshold: at/above this the front LED lights and it beeps.
static const float HOT_C      = 45.0f;

// ----------------------------------------------------------------------------
// Button: short press < 800ms -> toggle unit; long press -> toggle mute.
// Non-blocking, sampled once per frame.
// ----------------------------------------------------------------------------
static bool     btnPrev      = false;
static uint32_t btnDownAt    = 0;
static bool     btnLongFired = false;
static const uint16_t LONG_MS = 800;

static void pollButton() {
  bool down = (digitalRead(PIN_BUTTON) == LOW);   // active low
  uint32_t now = millis();

  if (down && !btnPrev) {                 // press edge
    btnDownAt = now;
    btnLongFired = false;
  } else if (down && btnPrev) {           // held
    if (!btnLongFired && (now - btnDownAt) >= LONG_MS) {
      buzzerMute = !buzzerMute;
      btnLongFired = true;
      if (!buzzerMute) beep(2000, 40);    // confirm un-mute audibly
    }
  } else if (!down && btnPrev) {          // release edge
    if (!btnLongFired && (now - btnDownAt) >= 30) {  // debounced short press
      useFahren = !useFahren;
      beep(2500, 25);
    }
  }
  btnPrev = down;
}

// ----------------------------------------------------------------------------
// Startup sweep: chase the bar left->right so a builder can confirm every
// rail and LED works right after flashing.
// ----------------------------------------------------------------------------
static void startupSweep() {
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    frame = ((uint32_t)1 << i);
    for (uint8_t r = 0; r < 3; r++) renderFrame();   // ~22ms per dot
  }
  frame = 0;
}

// ----------------------------------------------------------------------------
void setup() {
  pinMode(PIN_FRONT_LED, OUTPUT);
  digitalWrite(PIN_FRONT_LED, LOW);
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  pinMode(PIN_BUZZ_A, OUTPUT);
  pinMode(PIN_BUZZ_B, OUTPUT);
  digitalWrite(PIN_BUZZ_A, LOW);
  digitalWrite(PIN_BUZZ_B, LOW);
  railsAllHiZ();

  Serial.begin(115200);
  Serial.println(F("PyroPen boot"));

  Wire.begin();
  sensorOK = mlx.begin();               // default I2C addr 0x5A
  Serial.println(sensorOK ? F("MLX90614 OK") : F("MLX90614 NOT FOUND"));

  beep(2000, 30);
  startupSweep();
}

// ----------------------------------------------------------------------------
void loop() {
  static uint32_t lastRead = 0;
  static float    objC     = NAN;
  static uint32_t lastBeep = 0;

  // Read the sensor a few times per second (blocking read is a few ms; it
  // briefly pauses the display, which is imperceptible at this cadence).
  uint32_t now = millis();
  if (sensorOK && (now - lastRead) >= 200) {
    lastRead = now;
    objC = mlx.readObjectTempC();

    float shown = useFahren ? (objC * 9.0f / 5.0f + 32.0f) : objC;
    Serial.print(F("obj="));
    Serial.print(shown, 1);
    Serial.println(useFahren ? F(" F") : F(" C"));
  }

  // --- map temperature to bar + warnings ---
  if (!sensorOK || isnan(objC)) {
    // Error: flash the two end LEDs, no bar.
    frame = (uint32_t)1 | ((uint32_t)1 << (NUM_LEDS - 1));
    if ((now / 300) & 1) frame = 0;
    digitalWrite(PIN_FRONT_LED, LOW);
  } else {
    float t = objC;
    int n = (int)((t - TEMP_MIN_C) / (TEMP_MAX_C - TEMP_MIN_C) * NUM_LEDS + 0.5f);
    if (t > TEMP_MIN_C && n < 1) n = 1;   // show at least one dot once above floor
    if (n < 0) n = 0;
    setBar((uint8_t)n);

    bool hot = (t >= HOT_C);
    digitalWrite(PIN_FRONT_LED, hot ? HIGH : LOW);
    if (hot && !buzzerMute && (now - lastBeep) >= 1500) {
      lastBeep = now;
      beep(3200, 60);
    }
  }

  pollButton();
  renderFrame();
}
