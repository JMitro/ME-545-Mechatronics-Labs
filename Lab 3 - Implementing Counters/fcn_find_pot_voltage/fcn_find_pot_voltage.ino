/*
   Calculate the voltage output of a potentiometer
*/


float AREF = 5.0; // reference voltage of Arduino
int pot = A0;
float pot_bit;
float pot_voltage;
float pot_voltage_scaled;

void find_pot_voltage () {
  pot_bit = analogRead(pot);
  pot_voltage = AREF * (pot_bit / 1023.0);
}

void setup() {

}

void loop() {

}
