/*
  ME 545 - Mechatronics
  Lab Group 5
  Juliette Mitrovich, Sijani, Sheila Moroney
  Task 3: Blink LED, 1 second on, .5 second off
*/

int ledPin = 7;
// int pot = A2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);
  delay(500);

  //  Serial.println(pot);
}
