# Arduino Reaction Test

## About
This was a school project that I worked on to test reaction times using an Arduino. I'm publishing it here because why not? 🤷‍♂️ Maybe someone will find it useful or interesting.

## What it does
The device tests your reaction time by lighting up an LED after a random delay, and you have to press a button as quickly as possible. It runs the test 5 times with glasses on and 5 times without to see if there's any difference in reaction time.

## Hardware Requirements
- Arduino (any compatible board)
- 16x2 LCD display with I2C interface
- LED
- Push button
- Resistors and wires
- Breadboard or PCB

## Features
- Random delay between 1-5 seconds before the LED lights up
- LCD display showing instructions and results
- Prevents cheating by detecting early button presses
- Compares reaction times with and without glasses
- Outputs results in CSV format to Serial monitor
- Runs 5 tests in each condition

## How to use
1. Upload the code to your Arduino
2. Connect the hardware according to the pin definitions in the code:
   - LED on pin 8
   - Button on pin 13 (with internal pull-up)
   - LCD on I2C address 0x27
3. Open the Serial monitor at 9600 baud
4. Follow the on-screen instructions
5. Check the Serial monitor for CSV results

## Code Structure
- `setup()`: Initializes LCD, pins, and serial communication
- `loop()`: Runs the main test sequence
- `check()`: Handles individual reaction time measurements
- `printLcd()`: Custom function for LCD output with newline support

## Results
The device outputs reaction times in milliseconds as CSV data to the Serial monitor, making it easy to analyze the data in spreadsheet applications.

## Why I made this
It was for a school assignment, but honestly, it was pretty fun to build. The idea of testing whether glasses affect reaction time seemed interesting enough to actually implement properly.

Feel free to use, modify, or completely ignore this code. It's here because I can put it here. 🚀 