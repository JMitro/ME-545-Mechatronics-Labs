/*
   ME 545 Lab 6 - Wall Following
   Getting the ultrasonic sensors to work
*/
#include <math.h>
const int echoPin_r = 30;
const int trigPin_r = 31;

const int echoPin_fwd = 32;
const int trigPin_fwd = 33;

long duration_r;
long duration_fwd;

float distance_r_fwd;
float distance_fwd;
float distance_right;
// float distance_left;

int count = 0;

//float senseUltrasonic() {
//  digitalWrite(trigPin, LOW);
//  delayMicroseconds(2);
//  // Sets the trigPin on HIGH state for 10 micro seconds
//  digitalWrite(trigPin, HIGH);
//  delayMicroseconds(10);
//  digitalWrite(trigPin, LOW);
//  // Reads the echoPin, returns the sound wave travel time in microseconds
//  duration = pulseIn(echoPin, HIGH);
//  // Calculating the distance
//  distance = duration * 0.034 / 2;
//  // Prints the distance on the Serial Monitor
//  Serial.print(count); // print 'count' to the serial monitor
//  Serial.print(", ");
//  Serial.println(distance);
//  return distance;
//}

float senseUltrasonic_rightWall() {
  digitalWrite(trigPin_r, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin_r, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin_r, LOW);
  duration_r = pulseIn(echoPin_r, HIGH);
  distance_r_fwd = duration_r * 0.034 / 2;
  distance_right = distance_r_fwd;
//  * sin(45);
  Serial.println(distance_right);
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
  Serial.println(distance_fwd);
  return distance_fwd;
}

//float senseUltrasonic_wallFollow() {
//  digitalWrite(trigPin_r, LOW);
//  digitalWrite(trigPin_l, LOW);
//  delayMicroseconds(2);
//  // Sets the trigPin on HIGH state for 10 micro seconds
//  digitalWrite(trigPin_r, HIGH);
//  digitalWrite(trigPin_l, HIGH);
//  delayMicroseconds(10);
//
//  digitalWrite(trigPin_r, LOW);
//  digitalWrite(trigPin_l, LOW);
//
//  // Reads the echoPin, returns the sound wave travel time in microseconds
//  duration_r = pulseIn(echoPin_r, HIGH);
//  duration_l = pulseIn(echoPin_l, HIGH);
//  // Calculating the distance
//  distance_r_fwd = duration_r * 0.034 / 2;
//  distance_l_fwd = duration_l * 0.034 / 2;
//
//  distance_right = distance_r_fwd * sin(45);
//  distance_left = distance_l_fwd * sin(45);
//  // Prints the distance on the Serial Monitor
//
//  return distance_r_fwd, distance_l_fwd, distance_right, distance_left;
//}


void setup() {
  Serial.begin(115200);

  pinMode(echoPin_r, INPUT);
  pinMode(trigPin_r, OUTPUT);

  pinMode(echoPin_fwd, INPUT);
  pinMode(trigPin_fwd, OUTPUT);

}

void loop() {
  senseUltrasonic_rightWall();

}
