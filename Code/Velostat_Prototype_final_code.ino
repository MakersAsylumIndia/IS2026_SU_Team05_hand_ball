// Velostat Pressure Sensor - Arduino Nano
// Output only to Serial Monitor
// NOTE Hard touches respond well to low resistance, while soft touches respond well to high resistance.
const int row1 = A8;
const int row2 = A9;
const int col1= A10; 
const int col2= A11; 

int point_of_contactX{};
int point_of_contactY{};
void clearSerial() {
  for (int i = 0; i < 22; i++) {
    Serial.println();
  }
}

void setup() {
  Serial.begin(9600);
}

void loop() {
  int pressure_row1 = analogRead(row1);
  int pressure_row2 = analogRead(row2);
  int pressure_col1 = analogRead(col1);
  int pressure_col2 = analogRead(col2);
  if (pressure_row1<=120){
    point_of_contactX=0;
  }
  else if(pressure_row2<=120){
    point_of_contactX=1;
  }
  if(pressure_col1<=120){
    point_of_contactY=0;
  }
  else if(pressure_col2<=120){
    point_of_contactY=1;
  }
  Serial.print("Row1: ");
  Serial.println(pressure_row1);

  Serial.print("Row2: ");
  Serial.println(pressure_row2);

  Serial.print("Col1: "); 
  Serial.println(pressure_col1);

  Serial.print("Col2: "); 
  Serial.println(pressure_col2);

  // Serial.print("Point of contact X at: ");
  // Serial.println(point_of_contactX);

  // Serial.print("Point of contact Y at: ");
  // Serial.println(point_of_contactY);
  delay(2000);
}