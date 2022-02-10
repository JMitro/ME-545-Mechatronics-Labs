/*
  ME 545 - Mechatronics
  Lab Group 5
  Juliette Mitrovich, Sijani, Sheila Moroney
  Task 2: Heartbeat like flashing
*/

int ledPin = 9;    // LED connected to digital pin 9
int fadeValue; // create variable for fade value

void setup() {
  // nothing happens in setup
}

void loop() {
//  // fade in from min to max in increments of 5 points:
//  for (fadeValue = 0 ; fadeValue <= 255; fadeValue += 5) {
//    // sets the value (range from 0 to 255):
//    analogWrite(ledPin, fadeValue);
//    // wait for 30 milliseconds to see the dimming effect
//    delay(30);
//  }

  // make the light blink twice
  delay(120);
  analogWrite(ledPin, 0);
  delay(100);
  analogWrite(ledPin, 255);
  delay(120);
  analogWrite(ledPin, 0);
  delay(100);
  analogWrite(ledPin, 255);

  // fade out from max to min in increments of 5 points:
  for (fadeValue = 255 ; fadeValue >= 0; fadeValue -= 20) {
    // sets the value (range from 0 to 255):
    analogWrite(ledPin, fadeValue);
    // wait for 30 milliseconds to see the dimming effect
    delay(30);
  }
}
