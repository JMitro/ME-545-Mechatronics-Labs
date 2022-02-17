/*
 * Find the voltage value of a potentiometer
 * Scale up by 1000 to have voltages act as milliseconds
 * 
 */


void find_pot_delay_val_range () {
  pot_bit = analogRead(pot);
  pot_voltage = AREF * (pot_bit / 1023.0);
  pot_voltage_scaled = pot_voltage * 1000.0;

// Keeps value within a range of 500ms and 2000ms
// change as needed
  if (pot_voltage_scaled > 2000.0) {
    pot_voltage_scaled = 2000.0;
  }
  if (pot_voltage_scaled < 500) {
    pot_voltage_scaled = 500;
  }
  else {
    pot_voltage_scaled = pot_voltage_scaled;
  }

  Serial.println(pot_voltage_scaled);
}

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
