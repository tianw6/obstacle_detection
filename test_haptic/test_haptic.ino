// the code to test the relationship between voltage and amplitude of vibration 
int haptic = 9;

void setup() {
  // put your setup code here, to run once:
  pinMode(haptic, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 170; i >= 0; i--) {
    analogWrite(haptic, i);
    delay(20);
  }
  for (int i = 0; i <= 170; i++) {
    analogWrite(haptic, i);
    delay(20);
  }
}
