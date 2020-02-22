# ESP32_8x8_NTP_Clock
Basically, a NTP clock displayed on MAX7219 8x8 led matrix + some features

**It is a work in progress, requires fixes as with this version, the ESP32 craches on startup (heap memory overrange)**

## Hardware required
- ESP32, this project was developed on a "ESP32 devkit TV1" board
- 6x 8x8 les matrixes powered via a MAX7219
- Breadboard, jumper wires...
- Optionally, a switch for additional features (WIP -domoticz API client,  date display, temperature sensor...)

## Dependencies 
- https://github.com/danidask/MatrizLed
- https://github.com/MajicDesigns/MD_MAX72XX
- https://github.com/MajicDesigns/MD_MAXPanel
- https://github.com/LennartHennigs/Button2



## Use the program
Rename ConfigurationTemplate.h to Configuration.h and enter your wifi credentials to succed with compilation.