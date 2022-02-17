/*
   ME 545 - Mechatronics
   Lab 2 - IR Sensors final code
   Lab Group 5
   Juliette Mitrovich, Sijani, Sheila Moroney
*/
// Polling loop variables
unsigned long startTime = 0; // variable for the start time
unsigned long currentTime = 0; // variable for the current time
unsigned long elapsedTime; // variable to store the elapsed time (currentTime - startTime)

// IR sensor 1 variables
float IRsensor1 = A0; // set IRsensor as analog pin 0
float sensorValue1; // variable to store the output of IR sensor 1

// IR sensor 2 variables
float IRsensor2 = A1; // set IRsensor as analog pin 0
float sensorValue2; // variable to store the output of IR sensor 1

// digital filter coefficients, sensor 1 (fc = 27.77Hz, fs = 125Hz )
float a11 = -0.0875;
float b01 = 0.4563;
float b11 = 0.4563;

// digital filter coefficients, sensor 2 (fc = 22.22Hz, fs = 125Hz)
float a12 = -0.2309;
float b02 = 0.3846;
float b12 = 0.3846;

// digital filter, IR sensor 1 values
float Vold1 = 0; // initialize the first input
float yold1 = 0; // initialize the first output
float digitalFilter1; // initialize variable to store first order digital filter values

// digital filter, IR sensor 2 values
float Vold2 = 0; // initialize the first input
float yold2 = 0; // initialize the first output
float digitalFilter2; // initialize variable to store first order digital filter values

// variables to store distance calculations
float distance1; // store distance calculation from IR sensor 1
float distance2; // store distance calculation from IR sensor 2
float distance2_adj; // adjusted distance from sensor 2, set 20cm back
float distance; // final distance calculated from both IR sensors

// weight variables for sensor fusion
float w1;
float w2;

// speaker variables for mapping tone
int speakerOut = 9;
int pitch;

void setup() {
  Serial.begin(115200); // initialize the serial monitor
  pinMode(speakerOut, OUTPUT);
  currentTime = millis(); // set the currentTime equal to millis function
}

void loop() {
  currentTime = millis();
  elapsedTime = currentTime - startTime; // calculate the elapsed time

  // collect data at a fequency of 125 Hz (every 8 milliseconds)
  if (0 == (elapsedTime % 100)) {

    // Collect IR sensor 1 data
    sensorValue1 = analogRead(IRsensor1);

    // Collect IR sensor 2 data
    sensorValue2 = analogRead(IRsensor2);

    // digitially filter IR sensor 1 data
    digitalFilter1 = (b01 * sensorValue1) + (b11 * Vold1) - (a11 * yold1);
    yold1 = digitalFilter1;
    Vold1 = sensorValue1;

    // digitally filter IR sensor 2 data
    digitalFilter2 = (b02 * sensorValue2) + (b12 * Vold2) - (a12 * yold2);
    yold2 = digitalFilter2;
    Vold2 = sensorValue2;

    // Convert filtered IR sensor data to distance
    distance1 = (16569.0 / (digitalFilter1 + 25)) - 11;
    distance2 = (16569.0 / (digitalFilter2 + 25)) - 11;
    distance2_adj = distance2 - 21;

    // Use two distances to calculate the final distance
    // if IR sensor 2 reads a distance of 65 or more, only use IR sensor 1 data
    if (120 < distance2) {
      w1 = 1.05;
      w2 = 0;
      distance = (w1 * distance1);
    }
    
    // if IR sensor 2 reads a distance of 40cm or closer, only use IR sensor 2 data
    if (40 >= distance2) {
      w1 = 0;
      w2 = 1;
      distance = ((w1 * distance1) + (w2 * distance2_adj)) / (w1 + w2);
    }

    // if IR sensor 2 reads a distance between 40cm and 60 cm, average the two sensor distances
    if (120 >= distance2 && 40 < distance2) {
      w1 = 1;
      w2 = 1;
      distance = ((w1 * distance1) + (w2 * distance2_adj)) / (w1 + w2);
    }

    // generate varying pitch from the varying distance
    pitch = map(distance, 0, 150, 120, 1500);
    tone(9, pitch);

    // Print data to the serial monitor
    //      Serial.print(distance1); // print when you want to test sensor 1 distance
    //      Serial.print(", ");
    //      Serial.print(distance2); // print when you want to test sensor 2 distance
    //      Serial.print(", ");
    Serial.println(distance); // print when you want to test sensor fusion distance

    startTime = millis(); // reset startTime to allow for 125Hz of polling
  }
}
