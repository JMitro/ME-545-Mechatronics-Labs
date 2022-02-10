/*
   ME 545 - Mechatronics
   Lab 2 - IR Sensors
   Lab Group 5
   Juliette Mitrovich, Sijani, Sheila Moroney
*/
// Polling loop variables (f = 333Hz)
unsigned long startTime = 0; // variable for the start time
unsigned long currentTime = 0; // variable for the current time
unsigned long elapsedTime; // variable to store the elapsed time (currentTime - startTime)

// IR sensor 1 variables
float IRsensor1 = A1; // set IRsensor as analog pin 0
float sensorValue1; // variable to store the output of IR sensor 1

// IR sensor 2 variables
float IRsensor2 = A0; // set IRsensor as analog pin 0
float sensorValue2; // variable to store the output of IR sensor 1

// digital filter coefficients, both sensors
float a1 = -0.6314;
float b0 = 0.1843;
float b1 = 0.1843;

// digital filter, IR sensor 1 values
float Vold1 = 0; // initialize the first input
float yold1 = 0; // initialize the first output
float digitalFilter1; // initialize variable to store first order digital filter values

// digital filter, IR sensor 2 values
float Vold2 = 0; // initialize the first input
float yold2 = 0; // initialize the first output
float digitalFilter2; // initialize variable to store first order digital filter values

// variables to store distance calculations
float distance1; // store distance variables
float distance2;
float distance2_adj; // adjusted distance from sensor 2, set 20cm back
float distance;

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

  // collect 200 sampling points
  if (3 <= elapsedTime) {

    // Collect IR sensor 1 data
    sensorValue1 = analogRead(IRsensor1);

    // Collect IR sensor 2 data
    sensorValue2 = analogRead(IRsensor2);

    // collect digitally filtered data IR sensor 1
    digitalFilter1 = (b0 * sensorValue1) + (b1 * Vold1) - (a1 * yold1);
    yold1 = digitalFilter1;
    Vold1 = sensorValue1;

    digitalFilter2 = (b0 * sensorValue2) + (b1 * Vold2) - (a1 * yold2);
    yold2 = digitalFilter2;
    Vold2 = sensorValue2;

    // Convert filtered IR sensor data to distance
    distance1 = (16569.0 / (digitalFilter1 + 25)) - 11;
    distance2 = (16569.0 / (digitalFilter2 + 25)) - 11;
    distance2_adj = distance2 - 20;

    // Sensor Fusion
    if (135 <= distance2) {
      w1 = 1.0435;
      w2 = 0;
      distance = ((w1 * distance1) + (w2 * distance2_adj)) / (w1 + w2);
    }

    if (20 >= distance2_adj) {
      w1 = 0;
      w2 = 1;
      distance = ((w1 * distance1) + (w2 * distance2_adj)) / (w1 + w2);
    }

    // maybe not 1 and 1, probably a little more on the worse sensor
    else {
      w1 = 1;
      w2 = 1;
      distance = ((w1 * distance1) + (w2 * distance2_adj)) / (w1 + w2);
    }

    // generate varying pitch from the varying distance
    pitch = map(distance, 0, 150, 120, 1500);
    tone(9, pitch);

    distance = ((w1 * (distance1) + (w2 * distance2_adj)) / (w1 + w2));

    if (0 == (elapsedTime % 100)) {
      Serial.println(distance);
    }


    startTime = millis();
  }
}
