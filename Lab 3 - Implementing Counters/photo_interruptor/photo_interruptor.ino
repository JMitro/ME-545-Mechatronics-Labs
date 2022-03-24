/*
   Code to read an optical interruptor
   Built in LED on pin 13 will turn on when something is placed between the optical interrupt sensor
*/

int photoInterrupt = A1;
int photoInterruptVal;
int led = 10;

void setup() {
  Serial.begin(9600);
  pinMode (led, OUTPUT);
}

void loop() {
  photoInterruptVal = (analogRead(photoInterrupt));
  Serial.println(photoInterruptVal);
  
  if (photoInterruptVal < 500)
  {
    digitalWrite(led, HIGH);
  }
  else
  {
    digitalWrite(led, LOW);
  }
  delay(250);

}
