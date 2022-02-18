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


void setup()
{
  Serial.begin(9600);
  //Initialize sevseg object. Uncomment second line if you use common cathode 7 segment
  sevseg1.begin(COMMON_ANODE, numDigits1, digitPins1, segmentPins1, resistorsOnSegments1);
  sevseg2.begin(COMMON_ANODE, numDigits2, digitPins2, segmentPins2, resistorsOnSegments2);
  //sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins, resistorsOnSegments);

  sevseg1.setBrightness(90);
  sevseg2.setBrightness(90);
}

void loop()
{
  //Display numbers one by one with 2 seconds delay
  for (int i = 0; i < 10; i++) {
    sevseg1.setNumber(i);
    sevseg1.refreshDisplay();
    sevseg2.setNumber(i);
    sevseg2.refreshDisplay();
    delay(1000);
  }

}
