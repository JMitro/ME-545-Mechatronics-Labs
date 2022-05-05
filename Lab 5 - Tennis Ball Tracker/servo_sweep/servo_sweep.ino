/*
   ME 545 Lab 5
   Group 5: Juliette Mitrovich, Sheila Moroney, Sujani Patel

   Create a function to sweep a servo at a fixed frequency
   Tennis ball will be attached to the end of the servo.
   Note: The whole range of the servo is not being used because we created a fake 2D tennis ball,
         (highlighter drawn on white paper) becuase the real one was too heavy to move.
*/

#include <Servo.h>

Servo servoMoveBall;  // create servo object to control a servo

int pos = 30;    // variable to store the servo position

void setup() {
  servoMoveBall.attach(9);  // attaches the servo on pin 9 to the servo object
  servoMoveBall.write(pos); // move ther servo to the starting position at start up
}

void loop() {
  // move servo up from from 30º - 160º in steps of 1
  // change step size to change frequency
  for (pos = 30; pos <= 160; pos += 1) {
    servoMoveBall.write(pos);
    delay(15); // waits 15ms for the servo to reach the position
  }
  // move servo down from from 160º - 30º in steps of 1
  for (pos = 160; pos >= 30; pos -= 1) {
    servoMoveBall.write(pos);
    delay(15); // waits 15ms for the servo to reach the position
  }
}
