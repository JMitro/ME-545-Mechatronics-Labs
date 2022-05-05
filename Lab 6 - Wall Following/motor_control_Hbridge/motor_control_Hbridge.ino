/*
   ME 545 Lab 6
   Testing DC motor control with H-Bridge
*/

// Motor pins
const int leftMotor1 = 5; // OUT 1 pin, GND
const int leftMotor2 = 4; // OUT 2 pin, POWER

const int rightMotor1 = 3; // OUT 3 pin, POWER
const int rightMotor2 = 2; // OUT 4 pin, GND

const int ENB_left = 10; // PWN pin to control left motor speed
const int ENA_right = 9; // PWM pin to control right motor speed

// Motor variables
const int spd = 125;

void setup() {
  Serial.begin(9600);
  // set motor pin modes
  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);
  pinMode(rightMotor1, OUTPUT);
  pinMode(rightMotor2, OUTPUT);
}

void loop() {
  // move both motors forward
  digitalWrite(leftMotor1, LOW);
  digitalWrite(leftMotor2, HIGH);
  digitalWrite(rightMotor1, LOW);
  digitalWrite(rightMotor2, HIGH);
  analogWrite(ENB_left, spd);
  analogWrite(ENA_right, spd);

  delay(2000); // wait for a second

  // move both motors backward
  digitalWrite(leftMotor1, HIGH);
  digitalWrite(leftMotor2, LOW);
  digitalWrite(rightMotor1, HIGH);
  digitalWrite(rightMotor2, LOW);
  analogWrite(ENB_left, spd);
  analogWrite(ENA_right, spd);

  delay(2000);

  // turn left
  digitalWrite(leftMotor1, HIGH);
  digitalWrite(leftMotor2, LOW);
  digitalWrite(rightMotor1, LOW);
  digitalWrite(rightMotor2, HIGH);
  analogWrite(ENB_left, spd);
  analogWrite(ENA_right, spd);

  delay(2000);

  // turn right
  digitalWrite(leftMotor1, LOW);
  digitalWrite(leftMotor2, HIGH);
  digitalWrite(rightMotor1, HIGH);
  digitalWrite(rightMotor2, LOW);
  analogWrite(ENB_left, spd);
  analogWrite(ENA_right, spd);


  delay(2000);

}
