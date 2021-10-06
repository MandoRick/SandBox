word globalVariable = 1;

void setup() {
  delay(2000);
  Serial.begin(115200);
}

void loop() {
  delay(250);
  myFunction();
}

void myFunction() {
  Serial.println("my function retruns: " + (String)doSomeMath(2.0f, 9.7f, 93.572000));
}

double doSomeMath(float x, float y, double angle) {
  globalVariable++;
  x += globalVariable;
  return (x * y) + (x * angle) + (y * angle) - (x / y);
}
