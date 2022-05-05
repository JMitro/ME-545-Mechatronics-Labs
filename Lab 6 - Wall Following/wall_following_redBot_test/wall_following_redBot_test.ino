/*
   Test wall followng: red bot
*/
// Libraries
#include <math.h>
#include <RedBot.h>
#include <RedBotSoftwareSerial.h>

RedBotMotors motors; // initialize the motors

// Ultrasoinic sensor pins
const int echoPin_r = 30;
const int trigPin_r = 31;

const int echoPin_fwd = 32;
const int trigPin_fwd = 33;

// Ultrasonic sensor calculation variables
long duration_r;
long duration_fwd;

float distance_r_fwd;
float distance_fwd;
float distance_right;

// Proportional derivative control variables
const float setPoint_side = 40; // always want to be 8cm from the side wall

volatile float P; // proportional term
volatile float D; // derivative term
float Kp = 7.0; // proportional gain
float Kd = 55.0; // derivative gain

volatile float error_side; // variable to store calculated error
volatile float lastError_side;

volatile float motorSpd_gain; // calculated gain from PID control

const float avgSpd = 255.0; // set speed motors should move at when going straight
volatile float rightSpd;
volatile float leftSpd;

// Range conditions
const float tooFar_side = 45.0; // when no wall is detected, error basially infinite
const float tooClose_fwd = 8.0; // if fwd sensor

// Motor pins
const int leftMotor1 = 5; // OUT 3 pin
const int leftMotor2 = 4; // OUT 4 pin

const int rightMotor1 = 3; // OUT 1 pin
const int rightMotor2 = 2; // OUT 2 pin

const int ENB_left = 10; // PWN pin to control left motor speed
const int ENA_right = 9; // PWM pin to control right motor speed


// IR sensor 1 variables
float IRsensor1 = A0; // set IRsensor as analog pin 0
float sensorValue1; // variable to store the output of IR sensor 1

// digital filter coefficients, sensor 1 (fc = 27.77Hz, fs = 125Hz )
const float a11 = -0.0875;
const float b01 = 0.4563;
const float b11 = 0.4563;

// digital filter, IR sensor 1 values
volatile float Vold1 = 0; // initialize the first input
volatile float yold1 = 0; // initialize the first output
volatile float digitalFilter1; // initialize variable to store first order digital filter values
volatile float distance; // variable to store the distance conversion of the IR sensor

///// IR SENSOR FUNCTION /////
float senseIR() {
  // Collect IR sensor 1 data
  sensorValue1 = analogRead(IRsensor1);
  // digitially filter IR sensor 1 data
  digitalFilter1 = (b01 * sensorValue1) + (b11 * Vold1) - (a11 * yold1);
  yold1 = digitalFilter1;
  Vold1 = sensorValue1;
  // Convert filtered IR sensor data to distance
  distance = (16569.0 / (digitalFilter1 + 25)) - 11;

  if (80 < distance) {
    distance = 80;
  }

  return distance;
}

/// ULTRASONIC SENSOR FUNCTIONS ///
float senseUltrasonic_rightWall() {
  digitalWrite(trigPin_r, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin_r, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_r, LOW);
  duration_r = pulseIn(echoPin_r, HIGH);
  distance_right = duration_r * 0.034 / 2;

  return distance_right;
}

float senseUltrasonic_frontWall() {
  digitalWrite(trigPin_fwd, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin_fwd, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_fwd, LOW);
  duration_fwd = pulseIn(echoPin_fwd, HIGH);
  distance_fwd = duration_fwd * 0.034 / 2;

  return distance_fwd;
}

/// PROPORTIONAL CONTROL ///
float PD_control() {
  //  error_side = setPoint_side - distance_right; // error calculation with ultrasonic sensor
  error_side = setPoint_side - distance; // error calculation with IR sensor
  P = error_side;
  D = error_side - lastError_side;
  lastError_side = error_side;

  motorSpd_gain = P * Kp + D * Kd;

  return motorSpd_gain;
}

void goFwd(float leftSpd, float rightSpd) {
  digitalWrite(leftMotor1, LOW);
  digitalWrite(leftMotor2, HIGH);
  digitalWrite(rightMotor1, LOW);
  digitalWrite(rightMotor2, HIGH);
  analogWrite(ENB_left, leftSpd);
  analogWrite(ENA_right, rightSpd);
}

/// ZERO POINT TURN FUNCTIONS
void turnRight(float leftSpd, float rightSpd) {
  digitalWrite(leftMotor1, LOW);
  digitalWrite(leftMotor2, HIGH);
  digitalWrite(rightMotor1, HIGH);
  digitalWrite(rightMotor2, LOW);
  analogWrite(ENB_left, leftSpd);
  analogWrite(ENA_right, rightSpd);
}

void turnLeft(float leftSpd, float rightSpd) {
  digitalWrite(leftMotor1, HIGH);
  digitalWrite(leftMotor2, LOW);
  digitalWrite(rightMotor1, LOW);
  digitalWrite(rightMotor2, HIGH);
  analogWrite(ENB_left, leftSpd);
  analogWrite(ENA_right, rightSpd);
}

void setup() {
  Serial.begin(115200); // begn serial monitor for debugging

  // set ultrasonic sensor pin modes
  pinMode(echoPin_r, INPUT);
  pinMode(trigPin_r, OUTPUT);

  pinMode(echoPin_fwd, INPUT);
  pinMode(trigPin_fwd, OUTPUT);

  // set motor pin modes
  pinMode(rightMotor1, OUTPUT);
  pinMode(rightMotor2, OUTPUT);
  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);

  pinMode(IRsensor1, INPUT);
}

void loop() {
  //  senseUltrasonic_rightWall();
  //  senseUltrasonic_frontWall();
  senseIR();
  PD_control();

  rightSpd = avgSpd + motorSpd_gain;
  leftSpd = avgSpd - motorSpd_gain;
  motors.rightDrive(rightSpd);
  motors.leftDrive(leftSpd);


    if (rightSpd > 255) {
      rightSpd = 255;
    }
    if (leftSpd > 255) {
      rightSpd = 255;
    }

//    motors.rightDrive(260);
//    motors.leftDrive(260);

  // inner corner condition
  //  if (distance_fwd < tooClose_fwd) {
  //    // adjust your P and D gains and turn left
  //    Kp = 20;
  //  }
  //
  //  // outer corner condition
  //  if (error_side < tooFar_side) {
  //    // adjust P and D gains and turn right
  //    Kp = 50;
  //    Kd = 25;
  //    turnRight(leftSpd, rightSpd);
  //  }
  //
  //  else {
  //    // set P and D gain values back to original
  //    Kp = 5;
  //    Kd = 110;
  //  }

  ///// DEBUG /////
  //  Serial.print("Distance Right: ");
  //  Serial.println(distance_right);

  //  Serial.print("Distance: ");
  //  Serial.print(distance);
  //  Serial.print(", ");
  //  Serial.println(sensorValue1);
  //
  //  Serial.print("Gain: ");
  //  Serial.print(motorSpd_gain);
  //  Serial.print(", ");
  //
  //  Serial.print("Motor Speeds: ");
  //  Serial.print(leftSpd);
  //  Serial.print(", ");
  //  Serial.println(rightSpd);
}
