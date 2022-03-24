/*
 * ME 545 Lab 5 - Tennis Ball Tracker
 * Receives coordinates from the Raspberry Pi about where the tennis ball is
 * Camera situated on servo moves accordingly to make tennis ball always in the center of the frame
 */

 #include <Servo.h>
 Servo servo1;

int servoX; // initialize variable to store position
int pos; 

void setup() {
  Serial.begin(9600); // turn on the serial monitor
  
  servo1.attach(9); // attach servo to pin 9
  servo1.write(90); // set the servo to the middle (90º) on start up

}

void loop() {
  if (Serial.available() > 0) {
    servoX = Serial.read();
  }
  pos = map(servoX, 255, 0, 180, 0); // map the values from the serial to have a range of 0-180 (for the servo)
  servo1.write(pos);
  

}
