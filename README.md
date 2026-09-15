# Pyropen
A pen with an embedded charlieplexed screen and thermal sensor

## 3D Model Preview
![3D PCB](model.png)

## 3D PCB Preview
![3D PCB](model2.png)

## Wiring
![PCB](wiring.png)

## Schematic
![Schematic](schematic.png)

## Introduction
Hobby electronics projects can come with a lot of dangers, especially if there is a short between ground and the main voltage line (3.3v) through a component, which is one of the most common faults when hand soldering. These components can get really hot which means placing your finger to sense the temperature is not a super safe option, and is also not very possible for really small components in tight areas. Theres also hundreds of other applications like seeing if a soldering iron is at the good temperature or if something is being heated properly where touching it is not very recommended... 

My solution to this is the PyroPen - a portable thermal sensor that fits in the palm of your hand! It can also replace a regular pen, meaning that you have this sensor on you wherever you go. 

## Bill of Materials 

### 5 Pens:

| Quantity | Components | Price | Link |
|--------- |----------|----------| -----|
| 5  | D1 Mini Pen Refill  (around 67mm long) | $6 | [here](https://www.aliexpress.us/item/3256810502408645.html?spm=a2g0o.productlist.main.1.6d809qLy9qLyLj&algo_pvid=76f2cf68-db1d-471b-9ad9-c0e175543bda&algo_exp_id=76f2cf68-db1d-471b-9ad9-c0e175543bda-0&pdp_ext_f=%7B%22order%22%3A%22431%22%2C%22eval%22%3A%221%22%2C%22fromPage%22%3A%22search%22%7D&pdp_npi=6%40dis%21USD%218.80%216.16%21%21%2158.72%2141.10%21%402101e80b17891628832691912e0dab%2112000053197611078%21sea%21US%217493938711%21X%211%210%21n_tag%3A-29919%3Bd%3Ad260408f%3Bm03_new_user%3A-29895&curPageLogUid=INQ9floTcDND&utparam-url=scene%3Asearch%7Cquery_from%3A%7Cx_object_id%3A1005010688723397%7C_p_origin_prod%3A)
| 1 | Plexiglass sheet | $5 | [here](https://www.aliexpress.us/item/3256807722386926.html?spm=a2g0o.productlist.main.2.39f129e2SERr2S&algo_pvid=185f7068-e484-4aa3-9ff3-0e65a2acbd56&algo_exp_id=185f7068-e484-4aa3-9ff3-0e65a2acbd56-1&pdp_ext_f=%7B%22order%22%3A%22156%22%2C%22eval%22%3A%221%22%2C%22fromPage%22%3A%22search%22%7D&pdp_npi=6%40dis%21USD%218.44%215.23%21%21%2156.35%2134.94%21%402101c4ea17891631287926600e102c%2112000042797704390%21sea%21US%217493938711%21X%211%210%21n_tag%3A-29919%3Bd%3Ad260408f%3Bm03_new_user%3A-29895&curPageLogUid=IFVzF4QWo7zW&utparam-url=scene%3Asearch%7Cquery_from%3A%7Cx_object_id%3A1005007908701678%7C_p_origin_prod%3A)
| 1 | M10 tap and M10 die | $10-25 | [here](https://www.aliexpress.us/item/3256806490105856.html?spm=a2g0o.productlist.main.1.787f45362GMN3x&algo_pvid=6b4764b8-9f9b-4b4b-abaf-7ee7ff8e24d2&algo_exp_id=6b4764b8-9f9b-4b4b-abaf-7ee7ff8e24d2-0&pdp_ext_f=%7B%22order%22%3A%222091%22%2C%22eval%22%3A%221%22%2C%22fromPage%22%3A%22search%22%7D&pdp_npi=6%40dis%21USD%2191.27%2124.16%21%21%2191.27%2124.16%21%402101d9ef17891629153167158e0d3e%2112000037989416687%21sea%21US%217493938711%21X%211%210%21n_tag%3A-29919%3Bd%3Ad260408f%3Bm03_new_user%3A-29895%3BpisId%3A5000000216826354&curPageLogUid=6mPafcKNUYwu&utparam-url=scene%3Asearch%7Cquery_from%3A%7Cx_object_id%3A1005006676420608%7C_p_origin_prod%3A#nav-specification)
| 1 | Black Silicone Sheet | $6 | [here](https://www.aliexpress.us/item/3256803477740534.html?spm=a2g0o.productlist.main.2.18d8BtN8BtN8hz&algo_pvid=ee0522a2-58d0-4bcf-9e30-e4f22c28cf2e&algo_exp_id=ee0522a2-58d0-4bcf-9e30-e4f22c28cf2e-1&pdp_ext_f=%7B%22order%22%3A%22401%22%2C%22eval%22%3A%221%22%2C%22fromPage%22%3A%22search%22%7D&pdp_npi=6%40dis%21USD%216.57%216.57%21%21%216.57%216.57%21%402103081117891632978163784e1014%2112000026705712515%21sea%21US%217493938711%21X%211%210%21n_tag%3A-29919%3Bd%3Ad260408f%3Bm03_new_user%3A-29895&curPageLogUid=KVIebw9Xz1w2&utparam-url=scene%3Asearch%7Cquery_from%3A%7Cx_object_id%3A1005003664055286%7C_p_origin_prod%3A)
| 10 | Contact Springs | $3 | [here](https://www.aliexpress.us/item/3256811981198202.html?spm=a2g0o.productlist.main.22.77f10hiQ0hiQK3&algo_pvid=e18af909-f335-4400-8902-d4f17e02d908&algo_exp_id=e18af909-f335-4400-8902-d4f17e02d908-21&pdp_ext_f=%7B%22order%22%3A%22104%22%2C%22eval%22%3A%221%22%2C%22fromPage%22%3A%22search%22%7D&pdp_npi=6%40dis%21USD%212.11%212.11%21%21%2114.07%2114.07%21%402103094f17891662172853954e0ecd%2112000057659623320%21sea%21US%217493938711%21X%211%210%21n_tag%3A-29919%3Bd%3Ad260408f%3Bm03_new_user%3A-29895&curPageLogUid=K0owjtkuBRbz&utparam-url=scene%3Asearch%7Cquery_from%3A%7Cx_object_id%3A1005012167512954%7C_p_origin_prod%3A) 
| 5 | 5mm Piezoelectric Buzzers | $1 | [here](https://www.lcsc.com/product-detail/C76871.html?s_z=n_q_l_piezoelectric%2520ceramic%2520diaphragm&spm=wm.fly.bg.0.xh___wm.ssy.ml.0-0.ent&lcsc_vid=RwNdUQVeTwANA11XFlZZX1IHRwBZVVwDRlkNV1BWQQMxVlNeTlBYVFxWRldYUTsOAxUeFF5JWBYZEEoKFBINSQcJGk4%3D) 
| 5 | MLX Infrared Sensors | $44 | [here](https://www.lcsc.com/product-detail/C3663116.html?s_z=n_q_MLX90614%2520&spm=wm.fly.bg.0.xh&lcsc_vid=RwNdUQVeTwANA11XFlZZX1IHRwBZVVwDRlkNV1BWQQMxVlNeTlBYX1NQTlVfVjsOAxUeFF5JWBYZEEoKFBINSQcJGk4NBhADEA4cHktTRk8GEwkK)
| 5 | AAAA Batteries | $7 | [here](https://www.amazon.com/AmazonBasics-Everyday-Alkaline-Batteries-8-Pack/dp/B00LH3DTYS?crid=3SYUEZ4T84FRV&dib=eyJ2IjoiMSJ9.xJZDnwcXb6EhPXm3mujCAttCgFNsW5aKTzGClFgmbNEjNBc2EIEIb0whSA5f3VcD3A-52y31u_q1Zwa2tIzaYB4oMPsB5-F0LAKjcA06an0JA-MCbnHJLxW74Hy52UELYeB8vnAcLA--8-hO090HfCUW1_Jm4RTs1KzpswVWjnrPVgvfUBrpNFrdIDRPyOvtuVmp1lYjwbZ0h8qXp7lpeklCB14scTmb_KNGeEhvzpb2sc9qDL89ZA_GC0lTx49BjnFbOIMi3paBV0zKl9BSVV2KHnai9rwA-lfpRBzrJMg.HBTIVsixOTtFYGvLlxr646ElXtiAA0v_4nQW3vVoWGo&dib_tag=se&keywords=aaaa%2Bbatteries%2B10%2Bpack&qid=1789191829&rdc=1&sprefix=aaaa%2Bbatteries%2B10%2Bpac%2Caps%2C321&sr=8-9&th=1)
| 5 | Warm Leds | $1 | [here](https://www.lcsc.com/product-detail/C5440675.html?s_z=n_q_t_warm%2520white%2520led&spm=wm.fly.bg.10.stp___wm.ssy.tc.1.tz&lcsc_vid=RgRbBFdUEQJXXwUER1JXVV1QRQIPUgdUQgRYVAFRR1gxVlNeTlNdUVRWR1NcUzsOAxUeFF5JWBYZEEoKFBINSQcJGk4%3D) 
| 5 | PCBA for PyroPen | $60 | Uploaded PCB fabrication files to JLCPCB and assemble it.   
 
Total cost: **$157**

Cost per pen: **$31.4**

----

## Tools / Other parts:
- M10 Tap and Die (from BOM)
- Hot glue + hot glue gun (or super glue)
- Dremel
- Raspbbery Pi, or any other device that can be used to flash firmware to the ATTINY chip.
- 3D printer

## Assembly
1. 3D print the case and assembly 
2. Use the M10 Tap and Die to thread the three pieces at their corresponding places
3. Cut plexiglass and glue onto the screen case
4. Remove the tabs from the PCB with a dremel (required for meeting the PCBA size requirements)
5. Flash firmware to the PCB with a raspbbery pi (or other method)
6. Solder on the buzzer, MLX sensor, and contact springs to the PCB
7. Insert the assembly into the case.
8. Glue the PCB to the inside of the case with the screen facing out and the button in place
9. Insert the pen refill tightly into the case
10. Mount the remaining components into the case, insert battery, and seal
