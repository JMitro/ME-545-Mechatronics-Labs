/*
  ME 545 - Mechatronics
  Lab Group 5
  Juliette Mitrovich, Sijani, Sheila Moroney
  Task 4: Annoying Sound
*/

int speakerOut = 9; // assigned pin number for the speaker
int pulses = 1000; // number of times to pulsate the speaker
int i = 0; // initializing i variable for the for loop
int wait; // initialize variable for random delay
int delayTone1; // initialize variable for random tone
int delayTone2; // initialize variable for random tone

void setup() {
  Serial.begin(9600);
  pinMode(speakerOut, OUTPUT); // define speaker pin mode

}

void loop() {
  // generate random number of microseconds to create a new tone each time
  delayTone1 = random(200, 1000);
  delayTone2 = random(200, 1000);

  // loop the pulsating commands until the desired number is achieved
  for (int i = 0; i < pulses; i++)
  {
    //Pulsate up
    digitalWrite(speakerOut, HIGH);
    delayMicroseconds(delayTone1);

    //Pulsate down
    digitalWrite (speakerOut, LOW);
    delayMicroseconds(delayTone2);
  }
  // generate random number of milliseconds to wait, between 7000 and 15000
  wait = random(5000, 15000);
  delay(wait); // delay the random number of milliseconds.
}
