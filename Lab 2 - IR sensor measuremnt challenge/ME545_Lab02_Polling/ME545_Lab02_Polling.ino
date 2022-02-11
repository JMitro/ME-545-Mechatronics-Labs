/*
   ME 545 Lab 2
   IR sensor measurement
   Polling loop - how to initiate data collection at a certain frequency
   Juliette Mitrovich, Sujani, Sheila Moroney
*/

unsigned long startTime = 0; // variable for the start time
unsigned long currentTime = 0; // variable for the current time
unsigned long elapsedTime; // variable to store the elapsed time (currentTime - startTime)
unsigned long samplingTime;

float IRsensorRaw = A0; // set raw IRsensor data as pin A0
float IRsensorAF = A1; // set analog filtered IRsensor data as pin A1

int count = 0; // initialize a counter

float sensorValueRaw; // variable to store raw output of IR sensor
float voltageRaw; // variable to store raw voltage of IR sensor

float sensorValueAF; // variable to store analog filtered output of IR sensor
float voltageAF; // variable to store analog filtered voltage of IR sensor

void setup() {
  Serial.begin(115200); // initialize the serial monitor
  currentTime = millis(); // set the currentTime equal to millis function
}

void loop() {
  currentTime = millis();
  elapsedTime = currentTime - startTime; // calculate the elapsed time

  // collect 500 data points at a frequency of 125 Hz
  if (8 <= elapsedTime && 500 >= count) {
    sensorValueRaw = analogRead(IRsensorRaw); // read the output of the IRsensor and store it
    voltageRaw = sensorValueRaw * (5.0 / 1023.0); // convert the sensorValue to a voltage

    sensorValueAF = analogRead(IRsensorAF); // read analog filtered output of IR sensor and store it
    voltageAF = sensorValueAF * (5.0 / 1023.0); // convert filtered sensor value to voltage

    Serial.print(count); // print 'count' to the serial monitor
    Serial.print(", ");
    Serial.print(currentTime); // Print data to serial monitor
    Serial.print(",");
    Serial.print(voltageRaw); // print raw voltage data to the serial monitor
    Serial.print(", ");
    Serial.println(voltageAF); // print filtered voltage data

    
    startTime = millis();
    count += 1; // add 1 to variable 'count'
  }
}
