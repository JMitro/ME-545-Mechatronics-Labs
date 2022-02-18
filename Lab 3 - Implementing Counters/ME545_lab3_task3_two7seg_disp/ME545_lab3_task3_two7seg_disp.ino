
///// SEVEN SEGMENT DISPLAY VARIABLES /////
#include "SevSeg.h"
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

int i = 0;

///// PUSH BUTTON VARIABLES /////
int pushButtonEnable = 0;
int flagEnable;
int stateButtonEnable;

int pushButtonClear = 0;
int flagClear;
int stateButtonClear;

///// PHOTO INTERRUPTOR VARIABLES /////
int photoInterrupt = A1;
int photoInterruptVal;

void isr () {
  if (flagEnable == 1) {
    if (i <= 99) {
      i++;
      int disp1 = i % 10;
      int disp2 = i / 10;
      sevseg1.setNumber(disp1);
      sevseg2.setNumber(disp2);
      sevseg1.refreshDisplay();
      sevseg2.refreshDisplay();
    }
  }
}

void setup()
{
  Serial.begin(9600);

  //Initialize sevseg object. Uncomment second line if you use common cathode 7 segment
  sevseg2.begin(COMMON_ANODE, numDigits1, digitPins1, segmentPins1, resistorsOnSegments1);
  sevseg1.begin(COMMON_ANODE, numDigits2, digitPins2, segmentPins2, resistorsOnSegments2);
  //sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg1.setBrightness(90);
  sevseg2.setBrightness(90);

  // Initialize push buttons
  pinMode(pushButtonEnable, INPUT_PULLUP);
  pinMode(pushButtonClear, INPUT_PULLUP);

  pinMode(photoInterrupt, INPUT);
  attachInterrupt(digitalPinToInterrupt(photoInterrupt), isr, CHANGE);
}

void loop() {
  stateButtonEnable = digitalRead(pushButtonEnable);
  stateButtonClear = digitalRead(pushButtonClear);

  // If the enable button is pushed, add one to the flag
  if (stateButtonEnable == 0) {
    flagEnable = flagEnable + 1;
  }
  if (flagEnable == 1) {
    sevseg1.setNumber(i);
    sevseg2.setNumber(i);
  }

  if (stateButtonClear == 0) {
    flagClear = flagClear + 1;
  }

  if (flagClear == 1) {
    i = 0;
    sevseg1.setNumber(i);
    sevseg2.setNumber(i);
  }



}
