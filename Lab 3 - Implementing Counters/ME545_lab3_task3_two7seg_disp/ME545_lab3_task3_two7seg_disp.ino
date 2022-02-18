/*
   ME 545 - Lab 4 Task 3 Group 5
   Juliette Mitrovich, Sheila Moroney, Sujani Patel
   Goal: Count, on the 7 segment display, every time something passes between the optical interrupter.
   You should be able to count from 0 to 99. You must also attach interrupts to two push buttons, one interrupt to
   clear the display, and one interrupt to enable the display

*/

///// SEVEN SEGMENT DISPLAY VARIABLES /////
#include "SevSeg.h"
// define the two 7 segment displays as different names to differentiate and not overwrite data
SevSeg sevseg1;
SevSeg sevseg2;

//Set to 1 for single digit display, 1st display
byte numDigits1 = 1;

//defines common pins while using multi-digit display. Left empty as we have a single digit display
byte digitPins1[] = {};

//Defines arduino pin connections in order: A, B, C, D, E, F, G, DP
byte segmentPins1[] = {3, 2, 8, 7, 6, 4, 5, 9};
bool resistorsOnSegments1 = true;

//Set to 1 for single digit display, 2nd display
byte numDigits2 = 1;

//defines common pins while using multi-digit display. Left empty as we have a single digit display
byte digitPins2[] = {};

//Defines arduino pin connections in order: A, B, C, D, E, F, G, DP
byte segmentPins2[] = {33, 32, 38, 37, 36, 34, 35, 39};
bool resistorsOnSegments2 = true;

int i = 0; // Counter to keep track of what number should be displayed

///// PUSH BUTTON VARIABLES /////
// must be attached to a pin that allows hardware interrupts (MEGA: 2,3,18,19)
int pushButtonEnable = 18;
int pushButtonClear = 19;

// variables to store output of the push buttons
int stateButtonEnable; 
int stateButtonClear;

///// OPTICAL INTERRUPT VARIABLES /////
int photoInterrupt = A4;
int photoInterruptVal;

void setup() {
  Serial.begin(9600); // Initialize serial monitor if you want to print anything

  //Initialize sevseg object. Uncomment second line if you use common cathode 7 segment
  sevseg2.begin(COMMON_ANODE, numDigits1, digitPins1, segmentPins1, resistorsOnSegments1);
  sevseg1.begin(COMMON_ANODE, numDigits2, digitPins2, segmentPins2, resistorsOnSegments2);
  //sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg1.setBrightness(90);
  sevseg2.setBrightness(90);

  // Initialize push buttons
  pinMode(pushButtonEnable, INPUT);
  pinMode(pushButtonClear, INPUT);

  // Attach a hardware interrupt to the push buttons, ISR will run whenever there is a change in pin value
  attachInterrupt(digitalPinToInterrupt(pushButtonEnable), isrEnable, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pushButtonClear), isrClear, CHANGE);

  // Initialize photo interrupt pin
  pinMode(photoInterrupt, INPUT);

}

void loop() {
  // Read data from all necessary sensors
  stateButtonEnable = digitalRead(pushButtonEnable);
  stateButtonClear = digitalRead(pushButtonClear);
  photoInterruptVal = analogRead(photoInterrupt);

  // If something passes between the optical interrupt and i <= 99, display and increase the value by 1
  if (500 > photoInterruptVal) {
    if (99 >= i) {
      int disp1 = i % 10;
      int disp2 = i / 10;
      sevseg1.setNumber(disp1);
      sevseg1.refreshDisplay();
      sevseg2.setNumber(disp2);
      sevseg2.refreshDisplay();
      i++;
      delay(500); // add a delay so the display doesn't count too fast
    }
  }
}

///// HARDWARE INTERRUPT FUNCITONS /////
// If the enable button is pushed, turn the display on and set it to 0
void isrEnable () {
  if (stateButtonEnable == 1) {
    sevseg1.setNumber(0);
    sevseg1.refreshDisplay();
    sevseg2.setNumber(0);
    sevseg2.refreshDisplay();
  }
}

// If the clear button is pushed, change the display to 0 and set i back to 0
void isrClear() {
  if (stateButtonClear == 1) {
    sevseg1.setNumber(0);
    sevseg1.refreshDisplay();
    sevseg2.setNumber(0);
    sevseg2.refreshDisplay();
    i = 0; // set i back to 0 so the count will restart
  }
}
