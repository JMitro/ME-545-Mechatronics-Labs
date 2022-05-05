/*
 * ME 545 Lab 5 - Tennis Ball Tracker
 * Receives coordinates from the Raspberry Pi about where the tennis ball is
 * Camera situated on servo moves accordingly to make tennis ball always in the center of the frame
 */

 #include <Servo.h>
 Servo servoTrack;

int servoX; // initialize variable to store data coming in from serial monitor
int pos_track; // initialie variable for position of the servo

void setup() {
  Serial.begin(9600); // turn on the serial monitor and ensure baud rate is = to Pi
  
  servoTrack.attach(9); // attach servo to pin 9
  servoTrack.write(90); // set the servo to the middle (90º) on start up
}

void loop() {
  // if there's data on the serial buffer, read and store the data
  if (Serial.available() > 0) {
    servoX = Serial.read();
  }
  // map the values from the serial port to have a range of 0-180 (for the servo)
  pos_track = map(servoX, 255, 0, 180, 0); 
  
  servoTrack.write(pos_track); // set the servo to the mapped position
  delay(55); // delay 55ms to allow servo to reach it's position
  
  Serial.println(servoX); // print the serial read data to the serial monitor to track performance
}
