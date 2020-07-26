ESP32 program for controlling 4-pin PC fan. 

Wiring: 
* Hook GPIO16 to the blue (RPM input) pin of the fan
* Connect ESP32 GND and fan GND
* Wire a push button between GPIO27 and GND

Usage:
Press the button to choose the fan output level between 0%, 10%, 20%, ..., 100%. 

Caveats:
* 3.3V GPIO works for the fan we tested; but you might need a level shifter
* Software debouncing for the button is implemented but hardware debouncing is still favored
