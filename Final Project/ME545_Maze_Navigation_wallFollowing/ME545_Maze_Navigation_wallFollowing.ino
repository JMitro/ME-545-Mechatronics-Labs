/*
   ME 545 Final Lab Competition - Maze navigation
   Juliette Mitrovich, Sheila Moroney, Sujani Patel
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
const float tooClose_fwd = 26; // how close the robot can get to a front facing wall

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
  if (distance_fwd > 35) {
    distance_fwd = 35;
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
const float setPoint_side = 39; // always want to be ~15cm from the side wall
// number is > 15 because the sensor is on the back left side and this is a right wall following robot
// allows you to stay within the correct operating range of the IR sensor (18cm - 150cm)

volatile float P; // proportional term
volatile float D; // derivative term
float Kp = 3.0; // proportional gain
float Kd = 55.0; // derivative gain

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
const float avgSpd = 110.0; // set speed motors should move at when going straight
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

void goBwd(float leftSpd, float rightSpd) {
  // set pins and speed to drive the robot backward (left bwd, right bwd)
  // convention for
  digitalWrite(leftMotor1, HIGH);
  digitalWrite(leftMotor2, LOW);
  digitalWrite(rightMotor1, HIGH);
  digitalWrite(rightMotor2, LOW);
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

///// LIMIT SWITCH /////
int L_switch_pin = 40; // limit switch input pin
int L_switch;

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

  // set IR sensor pin mode
  pinMode(IRsensor, INPUT);

  // set limit switch pin mode
  pinMode(L_switch_pin, INPUT);
}

void loop() {
  // call the neccessary functions for the robot to complete wall following
  senseIR();
  senseUltrasonic_frontWall();
  PD_control();
  L_switch = digitalRead(L_switch_pin);

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

  if (distance_fwd > tooClose_fwd) {
    // if you are not too close to the front wall, follow the right side wall
    Kp = 3.0;
    Kd = 60.0;
    goFwd(leftSpd, rightSpd);
  }
  if (distance_fwd < tooClose_fwd) {
    // when you come to an inner corner, do a zero point left turn at a set speed
    turnLeft(180, 180);
  }
  if (L_switch == LOW) {
    // if you hit the wall on the right side, back up, turn left slightly and drive forward
    delay(100);
    goBwd(150, 150);
    delay(500);
    turnLeft(110, 110);
    delay(200);
    goFwd(150, 150);
    delay(300);
  }

  ///// DEBUGGING /////
  //  Serial.print("Distance R: ");
  //  Serial.print(distance_right);
  //  Serial.print(", ");
  //  Serial.print("Distance fwd: ");
  //  Serial.print(distance_fwd);
  //  Serial.print(", ");
  //  Serial.print("Gain: ");
  //  Serial.print(motorSpd_gain);
  //  Serial.print(", ");
  //  Serial.print("Motor Speeds: ");
  //  Serial.print(leftSpd);
  //  Serial.print(", ");
  //  Serial.print(rightSpd);
  //  Serial.println();
}

/*
   Functions/code we did not use for the final wall following
   Kept for memory of what we tried
*/

/// ULTRASONIC SENSOR FUNCTIONS ///
//float senseUltrasonic_rightWall() {
//  digitalWrite(trigPin_r, LOW);
//  delayMicroseconds(2);
//  digitalWrite(trigPin_r, HIGH);
//  delayMicroseconds(10);
//  digitalWrite(trigPin_r, LOW);
//  duration_r = pulseIn(echoPin_r, HIGH);
//  distance_right = duration_r * 0.034 / 2;
//
//  return distance_right;
//}


//void turnRight(float leftSpd, float rightSpd) {
//  digitalWrite(leftMotor1, LOW);
//  digitalWrite(leftMotor2, HIGH);
//  digitalWrite(rightMotor1, HIGH);
//  digitalWrite(rightMotor2, LOW);
//  analogWrite(ENB_left, leftSpd);
//  analogWrite(ENA_right, rightSpd);
//}

//  outer corner condition
//    if (distance_right > tooFar_side) {
//      rightSpd = 0;
//      leftSpd = 255;
//      goFwd(leftSpd, rightSpd);
//    }
