/*
   ME 545 Lab 6 - Wall Following Code
   Group 5 - Juliette Mitrovich, Sheila Moroney, Sujani Patel
   This code allows our custom-built robot to successfully  follow a straight wall, outer corners
   and inner corners.
*/
// Libraries
#include <math.h>

///// ULTRASONIC SENSOR /////
// Ultrasoinic sensor pins
const int echoPin_fwd = 32;
const int trigPin_fwd = 33;

// Ultrasonic sensor calculation variables
long duration_fwd;
float distance_fwd;

// Ultrasonic range condition
const float tooClose_fwd = 15; // how close the robot can get to a front facing wall

// FUNCTION //
float senseUltrasonic_frontWall() {
  // calculate the distance in front of the robot (cm)
  digitalWrite(trigPin_fwd, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin_fwd, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_fwd, LOW);
  duration_fwd = pulseIn(echoPin_fwd, HIGH);
  distance_fwd = duration_fwd * 0.034 / 2; // convert duration to distance
  // limit how far the front sensor can see because we only care about the close values for turning
  if (distance_fwd > 25) {
    distance_fwd = 25;
  }
  return distance_fwd;
}

///// IR SENSOR /////
// IR sensor 1 variables
float IRsensor = A0; // set IRsensor as analog pin 0
float sensorValue; // variable to store the output of IR sensor

// digital filter coefficients, (fc = 27.77Hz, fs = 125Hz )
const float a1 = -0.0875;
const float b0 = 0.4563;
const float b1 = 0.4563;

// digital filter variables
volatile float Vold = 0; // initialize the first input
volatile float yold = 0; // initialize the first output
volatile float digitalFilter; // initialize variable to store first order digital filter values
volatile float distance_right; // variable to store the distance conversion of the IR sensor

// FUNCTION //
float senseIR() {
  // Collect IR sensor data (through and analog filter
  sensorValue = analogRead(IRsensor);
  // digitially filter IR sensor data
  digitalFilter = (b0 * sensorValue) + (b1 * Vold) - (a1 * yold);
  yold = digitalFilter;
  Vold = sensorValue;
  // Convert filtered IR sensor data to distance
  distance_right = (16569.0 / (digitalFilter + 25)) - 11;
  // limit the max range the sensor can see to avoid large gains
  if (60 < distance_right) {
    distance_right = 60;
  }
  return distance_right;
}

///// PROPORTIONAL DERIVATIVE (PD) CONTROL /////
const float setPoint_side = 55; // always want to be ~20cm from the side wall
// number is > 20 because the sensor is on the back left side and this is a right wall following robot
// allows you to stay within the correct operating range of the IR sensor (18cm - 150cm)

volatile float P; // proportional term
volatile float D; // derivative term
float Kp = 6.0; // proportional gain
float Kd = 110.0; // derivative gain

volatile float error_side; // variable to store calculated error
volatile float lastError_side; // variable to store the previous error for the derivative term

volatile float motorSpd_gain; // calculated gain from PID control

// FUNCTION //
float PD_control() {
  // find the error and previous error and calculate the necessary gain for the motor speeds
  error_side = setPoint_side - distance_right; // error calculation
  P = error_side;
  D = error_side - lastError_side;
  lastError_side = error_side;
  motorSpd_gain = P * Kp + D * Kd;
  return motorSpd_gain;
}

///// H-BRIDGE AND MOTORS /////
const float avgSpd = 105.0; // set speed motors should move at when going straight
volatile float rightSpd;
volatile float leftSpd;

// Motor pins
const int leftMotor1 = 5; // OUT 3 pin
const int leftMotor2 = 4; // OUT 4 pin

const int rightMotor1 = 3; // OUT 1 pin
const int rightMotor2 = 2; // OUT 2 pin

const int ENB_left = 10; // PWN pin to control left motor speed
const int ENA_right = 9; // PWM pin to control right motor speed

/*
    Pin convention
    Motor A: direction controlled by pins IN1 and IN2
    Motor B: direction controlled by pins IN3 and IN4
    if IN1 is HIGH and IN2 is low (or IN3 and IN4), motor will move one direction
    if you switch it the motor will move in the other direction DEPENDING ON HOW THE POWER AND GROUND
    WIRES OF THE MOTOR ARE PLUGGED IN TO THE H-BRIDGE
*/

// FUNCTION //
void goFwd(float leftSpd, float rightSpd) {
  // set pins and speed to drive the robot forward (left fwd, right fwd)
  // convention for
  digitalWrite(leftMotor1, LOW);
  digitalWrite(leftMotor2, HIGH);
  digitalWrite(rightMotor1, LOW);
  digitalWrite(rightMotor2, HIGH);
  analogWrite(ENB_left, leftSpd);
  analogWrite(ENA_right, rightSpd);
}

void turnLeft(float leftSpd, float rightSpd) {
  // set pins and speed to allow robot to do a zero-point left turn (left fwd, right bwd)
  digitalWrite(leftMotor1, HIGH);
  digitalWrite(leftMotor2, LOW);
  digitalWrite(rightMotor1, LOW);
  digitalWrite(rightMotor2, HIGH);
  analogWrite(ENB_left, leftSpd);
  analogWrite(ENA_right, rightSpd);
}

void setup() {
  Serial.begin(115200); // begin serial monitor for debugging

  // set ultrasonic sensor pin modes
  pinMode(echoPin_fwd, INPUT);
  pinMode(trigPin_fwd, OUTPUT);

  // set motor pin modes
  pinMode(rightMotor1, OUTPUT);
  pinMode(rightMotor2, OUTPUT);
  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);

  pinMode(IRsensor, INPUT);
}

void loop() {
  // call the neccessary functions for the robot to complete wall following
  senseIR();
  senseUltrasonic_frontWall();
  PD_control();

  // set the speed of the wheels
  // abs() used because the H-bridge does not allow negative speed values
  rightSpd = abs(avgSpd + motorSpd_gain);
  leftSpd = abs(avgSpd - motorSpd_gain);

  // set the max speed of each motor to 255 because PWM range is 0-255
  if (rightSpd > 255.0) {
    rightSpd = 255.0;
  }
  if (leftSpd > 255.0) {
    leftSpd = 255.0;
  }

  // wall following conditions
  if (distance_fwd < tooClose_fwd) {
    // when you come to an inner corner, do a zero point left turn at a set speed
    turnLeft(150, 150);
  }
  if (distance_fwd > tooClose_fwd) {
    // drive forward with these gain values
    Kp = 6.0;
    Kd = 110.0;
    goFwd(leftSpd, rightSpd);
  }

  ///// DEBUGGING /////
  //  Serial.print("Distance R: ");
  //  Serial.print(distance_right);
  //  Serial.print(", ");
  //  Serial.print("Distance fwd: ");
  //  Serial.print(distance_fwd);
  //  Serial.print(", ");
  //  //  Serial.print("Gain: ");
  //  //  Serial.print(motorSpd_gain);
  //  //  Serial.print(", ");
  //  Serial.print("Motor Speeds: ");
  //  Serial.print(leftSpd);
  //  Serial.print(", ");
  //  Serial.print(rightSpd);
  //  Serial.println();
}
