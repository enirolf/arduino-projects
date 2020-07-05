# SNEK

Arduino-controlled game of Snake on an 8x8 LED matrix.

# Hardware

## Components

* Arduino UNO board
* 4 7-segment
* MAX 7219 LED matrix
* Joystick

## Wiring

![4 7-segment pins](https://www.circuitbasics.com/wp-content/uploads/2017/05/Arduino-7-Segment-Tutorial-4-Digit-Display-Pin-Diagram.png)

The diagram above shows the pinout of the 4 7-segment display. The wiring of all components to the UNO board is given below.

### 4 7-segment

Component pin | UNO pin
--- | ---
D4 | 13
D3 | 12
D2 | 11
D1 | 10
A | 7
B | 6
C | 5
D | 4
E | 3
F | 2
G | 1
DP | n/a

### MAX 7219 LED matrix

Component pin | UNO pin
--- | ---
VCC | 5V
GND | GND
DIN | 16 (A2)
CS | 15 (A1)
CLK | 14 (A0)

### Joystick

Component pin | UNO pin
--- | ---
GND | GND
5V | 5V
VRx | A4
VRy | A5
SW | 0

## Building the project

This project uses [Arduino-Makefile](https://github.com/sudar/Arduino-Makefile). Run `make upload` to compile and upload the binaries to the board. Make sure all env variables are correct. Run `make clean` to remove all auxillary files created by the Makefile.
