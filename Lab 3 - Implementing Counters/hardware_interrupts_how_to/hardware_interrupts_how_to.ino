/*
 * How to create a hardware interrupt
 */

// must be attached to a pin that allows hardware interrupts, MEGA: 2,3,18,19,20,21 (20 and 21 not available when being
// used for I2C)
int pushButtonEnable = 18;
int pushButtonClear = 19;

void setup() {
  // put your setup code here, to run once:
 // Initialize push buttons
  pinMode(pushButtonEnable, INPUT);
  pinMode(pushButtonClear, INPUT);

  // Attach a hardware interrupt to the push buttons, ISR will run whenever there is a change in pin value
  // attachInterrupt(digitalPinToInterrupt(pin), ISR, mode)
  // ISR = interrupt service routine, you can call the funciton whatever you want though
  attachInterrupt(digitalPinToInterrupt(pushButtonEnable), isrEnable, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pushButtonClear), isrClear, CHANGE);
}

void loop() {
  // put your main code here, to run repeatedly:

}

///// HARDWARE INTERRUPT FUNCITONS /////
// Define what you want the function to do when the specific pin interrupt is triggered

// If the enable button is pushed, turn the display on and set it to 0
void isrEnable () {
  if (stateButtonEnable == 1) {
    sevseg1.setNumber(0);
    sevseg1.refreshDisplay();
    sevseg2.setNumber(0);
    sevseg2.refreshDisplay();
  }
}

// If the clear button is pushed, change the display to 0 and set i back to 0
void isrClear() {
  if (stateButtonClear == 1) {
    sevseg1.setNumber(0);
    sevseg1.refreshDisplay();
    sevseg2.setNumber(0);
    sevseg2.refreshDisplay();
    i = 0; // set i back to 0 so the count will restart
  }
}
