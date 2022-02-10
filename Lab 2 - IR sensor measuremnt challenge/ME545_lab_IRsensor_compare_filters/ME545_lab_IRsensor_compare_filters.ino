/*
   ME 545 Lab 2
   IR sensor measurement
   Comparing Filters - Analog filter, digital filter, analog and digital filter
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
float voltageRaw; // variable to store the output voltage

// IR sensor, Analog filtered (AF) data pins and variables
float IRsensorAF = A1;
float sensorValueAF;
float voltageAF;

// First order digital filter coefficients, raw data
float a0 = 1.0;
float a1 = -0.6796;
float b0 = 0.1602;
float b1 = 0.1602;

// Digital filter, raw data variables
float Vold = 0; // initialize the first input
float yold = 0; // initialize the first output
float digitalFilter; // initialize variable to store first order digital filter values

// First order digital filter coefficients, analog filtered data
float a0f = 1.0;
float a1f = -0.7362;
float b0f = 0.1319;
float b1f = 0.1319;

// Digital filter, analog filtered data variables
float Voldf = 0; // initialize the first input
float yoldf = 0; // initialize the first output
float digitalFilterf; // initialize variable to store first order digital filter values

void setup() {
  Serial.begin(115200); // initialize the serial monitor
  currentTime = millis(); // set the currentTime equal to millis function
}

void loop() {
  currentTime = millis();
  elapsedTime = currentTime - startTime; // calculate the elapsed time

  // collect digitally filtered data, raw data as inputs
  digitalFilter = (1 / a0) * ((b0 * voltageRaw) + (b1 * Vold) - (a1 * yold));
  yold = digitalFilter;
  Vold = voltageRaw;
  
  // collect digitally filtered data, analog filtered data as inputs
  digitalFilterf = (1 / a0f) * ((b0f * voltageAF) + (b1f * Voldf) - (a1f * yoldf));
  yoldf = digitalFilterf;
  Voldf = voltageAF;

  // collect 200 sampling points
  if (3 <= elapsedTime && 805 >= count) {

    // Collect raw voltage data
    sensorValueRaw = analogRead(IRsensorRaw);
    voltageRaw = sensorValueRaw * (5.0 / 1023.0);

    // Collect analog filter voltage data
    sensorValueAF = analogRead(IRsensorAF);
    voltageAF = sensorValueAF * (5.0 / 1023.0);

    // Print important data to serial monitor
    Serial.print(currentTime);
    Serial.print(",");
    Serial.print(voltageAF);
    Serial.print(", ");
    Serial.print(digitalFilter);
    Serial.print(", ");
    Serial.println(digitalFilterf);

    startTime = millis();
    count += 1; // add 1 to variable ‘count’
  }
}
