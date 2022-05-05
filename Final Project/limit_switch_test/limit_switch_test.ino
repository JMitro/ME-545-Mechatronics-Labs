
int L_switch = 40;
int L_switch_flag = 0;
void setup() {
  Serial.begin(115200);
  pinMode(L_switch, INPUT);
}

void loop() {
  if (digitalRead(L_switch) == HIGH) {
    Serial.print("OFF");
    Serial.println();
  }
  if (digitalRead(L_switch) == LOW) {
    Serial.print("ON");
    Serial.println();
  }

//Serial.println(digitalRead(L_switch));
}
