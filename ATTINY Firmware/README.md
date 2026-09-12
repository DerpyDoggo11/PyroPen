# PyroPen firmware

C++ / PlatformIO firmware for the PyroPen — a pen-format non-contact IR
thermometer built around an **ATtiny1616** (tinyAVR 1-series).

## What it does
- Reads object temperature from the **MLX90614** IR sensor over I²C.
- Displays it as a left→right **bar** on the 30-LED charlieplexed panel
  (range `25–250 °C`, configurable in `src/main.cpp`).
- Warns when the target is hot (≥ 45 °C): lights the **front LED** and beeps.
- **Button** (PA2): short press toggles °C/°F, long press (≥0.8 s) mutes the buzzer.
- Prints readings on the serial port (115200 baud, TX=PB2).
- Runs a startup LED sweep so you can verify every dot after assembly.

## Pin map (from the KiCad netlist)
| Function            | MCU pin(s)                 |
|---------------------|----------------------------|
| Charlieplex rails   | PA4 PA5 PA6 PA7 PB5 PB4    |
| I²C (MLX90614)      | SDA=PB1, SCL=PB0 (addr 0x5A) |
| Buzzer (BTL)        | PC1 + PC0 (anti-phase)     |
| Front LED           | PC2                        |
| Button              | PA2 (active-low, pullup)   |
| Serial debug        | TX=PB2, RX=PB3             |
| UPDI (program)      | PA0                        |

## Build & flash
The board is programmed over **UPDI** (single wire), not ISP/SPI. Any
serial-UPDI adapter works, including a **Raspberry Pi**.

### Wiring the programmer to the DEBUG header (J5, 1.0 mm pitch)
J5 pinout: `1=UPDI  2=GND  3=RX  4=TX  5=+3.3V` — check pins 4/5 on the board
before powering. For flashing you only need UPDI + GND (+ 3.3V if the pen
isn't otherwise powered).

Classic serial-UPDI adapter (USB-serial or Pi UART):
```
  TX ──[1 kΩ]──┬── UPDI (J5.1)
  RX ──────────┘
  GND ── GND (J5.2)
```

### Set your port, then upload
Edit `upload_port` in `platformio.ini`:
- Raspberry Pi GPIO UART: `/dev/serial0` (enable UART, disable the serial console)
- USB-serial on Linux: `/dev/ttyUSB0`
- Windows: `COM4`

```bash
pio run                 # build
pio run -t upload       # flash over UPDI
pio device monitor      # 115200 baud
```

## Notes
- Clock is the **10 MHz** internal oscillator: at 3.3 V (MAX17222 boost) the
  part is not rated for 20 MHz. Change `board_build.f_cpu` only if you raise Vcc.
- The buzzer is bridge-tied across PC0/PC1; the `beep()` routine drives them
  anti-phase for a louder tone. This is why the standard Arduino `tone()` isn't
  used (it drives a single pin).
- Display multiplexing is cooperative (in `loop()`), so no hardware timer is
  claimed and it can't collide with `millis()`/`tone()`.
