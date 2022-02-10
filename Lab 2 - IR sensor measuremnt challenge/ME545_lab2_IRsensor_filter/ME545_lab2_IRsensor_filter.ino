/*
   ME 545 Lab 2
   IR sensor measurement
   Filtering - raw data, analog filter, digital filter (1st order)
   Juliette Mitrovich, Sujani, Sheila Moroney
*/

// Polling loop variables (f = 333Hz)
unsigned long startTime = 0; // variable for the start time
unsigned long currentTime = 0; // variable for the current time
unsigned long elapsedTime; // variable to store the elapsed time (currentTime - startTime)
int count = 0; // initialize a counter

// IR sensor, Raw data pins and variables
float IRsensorRaw = A0; // set IRsensor as analog pin 0
float sensorValueRaw; // variable to store the output of IRsensor
//float voltageRaw; // variable to store the output voltage

// IR sensor, Analog filtered (AF) data pins and variables
float IRsensorAF = A1;
float sensorValueAF;
//float voltageAF;

// First order digital filter coefficients, found in Matlab
//float a1 = -0.7508;
//float b0 = 0.1246;
//float b1 = 0.1246;
// digital filter coefficients, analog filtered data, sensor 1
float a1 = -0.6314;
float b0 = 0.1843;
float b1 = 0.1843;

float Vold = 0; // initialize the first input
float yold = 0; // initialize the first output
float yfilt_f; // initialize variable for the filter equation
float digitalFilter; // initialize variable to store first order digital filter values


void setup() {
  Serial.begin(115200); // initialize the serial monitor
  currentTime = millis(); // set the currentTime equal to millis function
}

void loop() {
  currentTime = millis();
  elapsedTime = currentTime - startTime; // calculate the elapsed time

  // collect 200 sampling points
  if (3 <= elapsedTime && 805 >= count) {

    // Collect raw voltage data
    sensorValueRaw = analogRead(IRsensorRaw);
//    voltageRaw = sensorValueRaw * (5.0 / 1023.0);

    // Collect analog filter voltage data
    sensorValueAF = analogRead(IRsensorAF);
//    voltageAF = sensorValueAF * (5.0 / 1023.0);

    // collect digitally filtered data (1st order), MATLAB coefficients
    digitalFilter = (b0 * sensorValueRaw) + (b1 * Vold) - (a1 * yold);
    yold = digitalFilter;
    Vold = sensorValueRaw;

    // Print important data to serial monitor
    Serial.print(currentTime);
    Serial.print(",");
    Serial.println(sensorValueRaw);
//    Serial.print(", ");
//    Serial.print(sensorValueAF);
//    Serial.print(", ");
//    Serial.println(digitalFilter);

    startTime = millis();
    count += 1; // add 1 to variable ‘count’
  }
}
