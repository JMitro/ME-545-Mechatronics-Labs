/*
   ME 545 Lab 4 Group 5
   2 floor elevator finite state machine
   Juliette Mitrovich, Sheila Moroney, Sujani Patel
*/

///// LIBRARIES /////
#include <Servo.h>

///// INITIALIZE INPUT AND OUTPUT ARDUINO PINS /////
int buttonUp_pin = 6; // button to move up
int buttonDown_pin = 7; // button to move down

int indicatorUp = 8; // LED to indicate moving up
int indicatorDown = 9; // LED to indicate moving down
int indicatorErr = 10; // LED to indicate and error

int FFS_pin = A0; // first floor sensor (FFS) pin
int SFS_pin = A1; // second floor sensor (SFS) pin

///// INITIALIZE VARIABLES TO STORE INPUTS AND OUTPUTS /////
int buttonUp;
int buttonDown;

int FFS;
int SFS;

///// INITIALIZE SERVO MOTOR /////
Servo myServo;  // create servo object to control a servo
int pos = 0;    // variable to store the servo position

///// FUNCTIONS /////
// make FFS output read 1 or 0
void readFFS() {
  FFS = analogRead(FFS_pin);
  if (500 < FFS) {
    FFS = 0;
  }
  else {
    FFS = 1;
  }
}

// make SFS output read 1 or 0
void readSFS() {
  SFS = analogRead(SFS_pin);
  if (500 < SFS) {
    SFS = 0;
  }
  else {
    SFS = 1;
  }
}

void setup() {
  Serial.begin(9600); // turn on serial monitor

  // set input pins
  pinMode(buttonUp_pin, INPUT);
  pinMode(buttonDown_pin, INPUT);
  pinMode(FFS_pin, INPUT);
  pinMode(SFS_pin, INPUT);

  // set output pins
  pinMode(indicatorUp, OUTPUT);
  pinMode(indicatorDown, OUTPUT);
  pinMode(indicatorErr, OUTPUT);

  myServo.attach(2);  // attaches the servo on pin 2 to the servo object
  myServo.write(pos); // set the servo to position 0 when powered on

}

void loop() {
  // contantly read the buttons and the floor sensors
  buttonUp = digitalRead(buttonUp_pin);
  buttonDown = digitalRead(buttonDown_pin);
  readFFS();
  readSFS();

  switch (buttonUp) {
    case 0: // button up case 0
      // watch button down
      switch (buttonDown) {
        case 0: // button down
          // watch first floor sensor
          switch (FFS) {
            case 0: // ffs
              // watch second floor sensor
              switch (SFS) {
                case 0: // sfs (0000)
                  Serial.println("0000: moving floors");
                  // watch first floor for interruptions
                  switch (FFS) {
                    case 0:
                      // do nothing
                      break;

                    case 1:
                      // something has blocked the floor before you got there
                      // stop the servo (elevator) and turn on error light
                      digitalWrite(indicatorErr, HIGH);
                      digitalWrite(indicatorUp, LOW);
                      digitalWrite(indicatorDown, LOW);
                      myServo.detach();
                      break;

                  }
                  break;
                  // watch second floor for interruptions
                  switch (SFS) {
                    case 0:
                      // do nothing
                      break;

                    case 1:
                      // something has blocked the floor before you got there
                      // stop the servo (elevator) and turn on error light
                      digitalWrite(indicatorErr, HIGH);
                      digitalWrite(indicatorUp, LOW);
                      digitalWrite(indicatorDown, LOW);
                      myServo.detach();
                      break;
                  }
                  break;
                  break;

                case 1: // sfs (0001)
                  digitalWrite(indicatorErr, LOW); // turn error light off if it was on
                  digitalWrite(indicatorUp, LOW); // turn up light off
                  Serial.println("0001: at second floor or, coming out of error");
                  break;
              }
              break;

            case 1: // ffs
              // watch second floor sensor
              switch (SFS) {
                case 0: // sfs (0010)
                  digitalWrite(indicatorErr, LOW); // turn error light off if it was on
                  digitalWrite(indicatorDown, LOW); // turn down light off
                  Serial.println("0010: down button pressed, move down, or coming out of error");
                  break;


                case 1: // sfs (0011)
                  digitalWrite(indicatorErr, HIGH); // turn on error light
                  digitalWrite(indicatorDown, LOW); // turn off down light if it was on
                  digitalWrite(indicatorUp, LOW); // turn off up light if it was on
                  Serial.println("0011 error: both floors");
                  break;
              }
          }
          break;


        case 1: // button down
          // watch first floor sensor
          switch (FFS) {
            case 0: // ffs
              // watch second floor sensor
              switch (SFS) {
                case 0: // sfs (0100)
                  Serial.println("0100: button press while moving, stay in state");
                  break;

                case 1: // sfs (0101)
                  digitalWrite(indicatorDown, HIGH);
                  digitalWrite(indicatorErr, LOW); // turn error light off if it was on
                  myServo.attach(2);
                  myServo.write(0); // move to the down (first floor) position
                  Serial.println("0101: down button pressed, move down, or coming out of error");
                  delay(500);
                  break;
              }
              break;

            case 1: // ffs
              // watch second floor sensor
              switch (SFS) {
                case 0: // sfs (0110)
                  digitalWrite(indicatorErr, LOW); // turn error light off if it was on
                  Serial.println("0110: down button pressed, move down, or coming out of error");
                  break;

                case 1: // sfs (0111)
                  digitalWrite(indicatorErr, HIGH);
                  digitalWrite(indicatorUp, LOW);
                  digitalWrite(indicatorDown, LOW);
                  Serial.println("0111 error: both floors and button down");
                  break;
              }
              break;

          }
          break;

      }
      break;

    case 1: // button up
      // watch button down
      switch (buttonDown) {
        case 0: // button down
          // watch first floor sensor
          switch (FFS) {
            case 0: // ffs
              // watch second floor sensor
              switch (SFS) {
                case 0: // sfs (1000)
                  Serial.println("1000: button pressed while moving, don't care");
                  break;

                case 1: // sfs (1001)
                  digitalWrite(indicatorErr, LOW); // turn error light off if it was on
                  Serial.println("1001: up pressed at 2nd floor, don't move");
                  Serial.println("or, coming out of error");
                  break;

              }
              break;

            case 1: // ffs
              // watch second floor sensor
              switch (SFS) {
                case 0: // sfs (1010)
                  digitalWrite(indicatorUp, HIGH);
                  digitalWrite(indicatorErr, LOW); // turn error light off if it was on
                  myServo.attach(2);
                  myServo.write(180); // move to the up (second floor) position
                  Serial.println("1010: on 1st and pressed up, move up");
                  delay(500);
                  break;

                case 1: // sfs (1011)
                  digitalWrite(indicatorErr, HIGH);
                  digitalWrite(indicatorUp, LOW);
                  digitalWrite(indicatorDown, LOW);
                  Serial.println("1011 error: both floors and button up");

                  break;
              }
              break;

          }
          break;


        case 1: // button down
          // watch first floor sensor
          switch (FFS) {
            case 0: // ffs
              // watch second floor sensor
              switch (SFS) {
                case 0: // sfs (1100)
                  Serial.println("1100: button pressed when moving, don't care");
                  break;

                case 1: // sfs (1101), even though you pressed both buttons, still move down
                  digitalWrite(indicatorDown, HIGH);
                  digitalWrite(indicatorErr, LOW); // turn error light off if it was on
                  myServo.attach(2);
                  myServo.write(0); // move to the down (first floor) position
                  Serial.println("1101: down button pressed, move down or, coming out of error");
                  delay(500);
                  break;
              }
              break;

            case 1: // ffs
              // watch second floor sensor
              switch (SFS) {
                case 0: // sfs (1110), even though you pressed both buttons, still move up
                  digitalWrite(indicatorUp, HIGH);
                  digitalWrite(indicatorErr, LOW); // turn error light off if it was on
                  myServo.attach(2);
                  myServo.write(180); // move to the up (second floor) position
                  Serial.println("1110: on 1st and pressed up, move up");
                  delay(500);
                  break;

                case 1: // sfs (1111)
                  digitalWrite(indicatorErr, HIGH);
                  digitalWrite(indicatorUp, LOW);
                  digitalWrite(indicatorDown, LOW);
                  Serial.println("1111 error: both floors and button up");
                  break;
              }
              break;
          }
          break;
      }
      break;
  }
}
