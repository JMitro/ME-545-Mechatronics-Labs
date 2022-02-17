/*
 * ME 545 - Lab 3 Task 2
 * Display values 0-9 on 7 segment display
 * Use potentiometer to change speed at which is counts (delay)
 */

#include "SevSeg.h"
SevSeg sevseg;

//Set to 1 for single digit display
byte numDigits = 1;

//defines common pins while using multi-digit display. Left empty as we have a single digit display
byte digitPins[] = {};

//Defines arduino pin connections in order: A, B, C, D, E, F, G, DP
byte segmentPins[] = {3, 2, 8, 7, 6, 4, 5, 9};
bool resistorsOnSegments = true;

float AREF = 5.0; // reference voltage of Arduino
int pot = A0;
float pot_bit;
float pot_voltage;
float pot_voltage_scaled;

void find_pot_delay_val_range () {
  pot_bit = analogRead(pot);
  pot_voltage = AREF * (pot_bit / 1023.0);
  pot_voltage_scaled = pot_voltage * 1000.0;

// Keeps value within a range of 500ms and 2000ms
// change as needed
  if (pot_voltage_scaled > 2000.0) {
    pot_voltage_scaled = 2000.0;
  }
  if (pot_voltage_scaled < 500) {
    pot_voltage_scaled = 500;
  }
  else {
    pot_voltage_scaled = pot_voltage_scaled;
  }

  Serial.println(pot_voltage_scaled);
}

void setup()
{
  Serial.begin(9600);
  //Initialize sevseg object. Uncomment second line if you use common cathode 7 segment
  sevseg.begin(COMMON_ANODE, numDigits, digitPins, segmentPins, resistorsOnSegments);
  //sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins, resistorsOnSegments);

  sevseg.setBrightness(90);
}

void loop()
{
  
  //Display numbers one by one with 2 seconds delay
  for (int i = 0; i < 10; i++) {
    find_pot_delay_val_range();
    sevseg.setNumber(i);
    sevseg.refreshDisplay();
    delay(pot_voltage_scaled);
  }
}
