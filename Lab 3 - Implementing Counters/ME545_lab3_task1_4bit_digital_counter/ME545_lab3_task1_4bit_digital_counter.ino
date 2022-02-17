/*
   ME 545 Lab 4
   Task 1: Binary counter, 4 bits, 0-15
   Juliette Mitrovich, Sheila Moroney, Sujani Patel
*/

const int ledPin1 = 8; // the number of the LED pin
const int ledPin2 = 9;
const int ledPin3 = 10;
const int ledPin4 = 11;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
}

void loop() {
  digitalWrite(ledPin1, HIGH);
  digitalWrite(ledPin2, HIGH);
  digitalWrite(ledPin3, HIGH);
  digitalWrite(ledPin4, HIGH);

  for (int i = 0; i <= 15; i++) {
    if ((i % 2) == 1) {
      digitalWrite(ledPin1, HIGH);
    }
    else {
      digitalWrite(ledPin1, LOW);
    }

    if ((i % 4) == 2 | (i % 4) == 3) {
      digitalWrite(ledPin2, HIGH);
    }
    else {
      digitalWrite(ledPin2, LOW);
    }

    if ((i % 8) == 4 | (i % 8) == 5 | (i % 8) == 6 | (i % 8) == 7) {
      digitalWrite(ledPin3, HIGH);
    }
    else {
      digitalWrite(ledPin3, LOW);
    }

    if ((i % 16) >= 8) {
      digitalWrite(ledPin4, HIGH);
    }
    else {
      digitalWrite(ledPin4, LOW);
    }
    Serial.println(i);
    delay(500);
  }
}
