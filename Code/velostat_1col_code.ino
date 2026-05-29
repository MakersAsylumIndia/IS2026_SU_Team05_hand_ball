// Velostat Pressure Sensor - Arduino Nano
// Output only to Serial Monitor
//NOTE Hard touches respond well to low resistance, while soft touches respond well to high resistance.
const int sensorPin = A8;

void clearSerial() {
  for (int i = 0; i < 22; i++) {
    Serial.println();
  }
}

void setup() {
  Serial.begin(9600);
}

void loop() {

  // Read pressure sensor
  // int total = 0;
  // int pressure=0;
  // for (int i=1; i<=10; i++){
  //   pressure = analogRead(sensorPin);
  //   total += pressure;
  //   // Print value to Serial Monitor

  //   delay(30);
  // }
  // // clearSerial();
  // Serial.println(total/10);
  int pressure = analogRead(sensorPin);
  float voltage = analogRead(sensorPin) * (5.0 / 1023.0);
  Serial.println(pressure);
  delay(500);
}