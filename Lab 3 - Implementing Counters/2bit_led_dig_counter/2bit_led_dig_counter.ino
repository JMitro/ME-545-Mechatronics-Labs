/*
   ME 545 Lab 4
   Binary counter, 2 bits, 0 to 3
*/

const int ledPin1= 8; // the number of the LED pin
const int ledPin2 = 9;
int Clock = LOW;
long previousMillis = 0;
int a = LOW;
int b = LOW;
int t = 0;
long interval = 1000;

void setup() {
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
}
void loop()
{
  a = Clock;
  if (a == LOW) {
    t = 1;
  }//
  if ((a == HIGH) && (t == 1)) {
    b = !b;
    t = 0;
  }
  digitalWrite(ledPin1, a);
  digitalWrite(ledPin2, b);
  if (millis() - previousMillis > interval) {
    previousMillis = millis();
    //
    if (Clock == LOW)
      Clock = HIGH;
    else
      Clock = LOW;
  }
}
