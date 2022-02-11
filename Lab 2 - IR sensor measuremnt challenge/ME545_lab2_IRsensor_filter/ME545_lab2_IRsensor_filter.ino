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

// IR sensor, Analog filtered (AF) data pins and variables
float IRsensorAF = A1;
float sensorValueAF;

// First order digital filter coefficients
// values found at different sampling and cut off frequencies, change as needed

//float a1 = -0.7050;
//float b0 = 0.1475;
//float b1 = 0.1475;

float a1 = -0.2309;
float b0 = 0.3846;
float b1 = 0.3846;

// fc = 27.77Hz, fs = 125
//float a1 = -0.0875;
//float b0 = 0.4563;
//float b1 = 0.4563;

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

  // collect 500 sampling points at a frequency of 125 Hz
  if (8 <= elapsedTime && 500 >= count) {

    // Collect raw voltage data
    sensorValueRaw = analogRead(IRsensorRaw);

    // Collect analog filter voltage data
    sensorValueAF = analogRead(IRsensorAF);

    // collect digitally filtered data (1st order), MATLAB coefficients
    digitalFilter = (b0 * sensorValueAF) + (b1 * Vold) - (a1 * yold);
    yold = digitalFilter;
    Vold = sensorValueAF;

    // Print important data to serial monitor
    Serial.print(currentTime);
    //    Serial.print(",");
    //    Serial.print(sensorValueRaw);
    //    Serial.print(", ");
    //    Serial.println(sensorValueAF);
    Serial.print(", ");
    Serial.println(digitalFilter);

    startTime = millis();
    count += 1; // add 1 to variable ‘count’
  }
}
