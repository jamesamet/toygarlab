// ================================================================SENSÖR KALİBRE İŞLEMLERİ

// ADC kanal dönüşüm tablosu (Pro Micro A0-A3)
const uint8_t pinToADC[] = {7, 6, 5, 4};  // A0, A1, A2, A3

// Hızlı ADC okuma
inline int fastAnalogRead(int pin) {
  uint8_t adcChannel = pinToADC[pin - A0];
  ADMUX = (1 << REFS0) | adcChannel;
  ADCSRA |= (1 << ADSC);
  while (ADCSRA & (1 << ADSC));
  return ADC;
}

// Sensör okuma 
int readRawSensor(int pin) {
  long total = 0;
  for (int i = 0; i < NUM_READINGS; i++) {
    total += fastAnalogRead(pin);
  }
  return total / NUM_READINGS;
}

// Kalibre edilmiş sensör okuma 
int readCalibratedSensor(int pin) {
  int rawValue = readRawSensor(pin);
  rawValue = constrain(rawValue, FIXED_MIN, FIXED_MAX);
  int calibrated = map(rawValue, FIXED_MIN, FIXED_MAX, 0, 1000);
  return calibrated;
}
// Sensör test
int sensortest() 
{
  while(1){
    Serial.print(readRawSensor(QTR_PIN)); Serial.print("\t");
    Serial.print(readRawSensor(ONS_PIN)); Serial.print("\t");
    Serial.print(readRawSensor(SAG_PIN)); Serial.print("\t");
    Serial.println(readRawSensor(SOL_PIN));
  }
}

int motortest() 
{
  while(1){ setMotors(150,150);}
}


void sensoroku(){
//Lsens=analogRead(A1);  
solsensor=constrain(analogRead(SOL_PIN),50,950); 
sagsensor= constrain(analogRead(SAG_PIN),50,950);
onsensor= constrain(analogRead(ONS_PIN),50,950);
//FLsens=constrain(analogRead(A3),50,950); 

solsensor=map(solsensor,50,950,0,1000); 
//Rsens= map(Rsens,50,950,0,1000);
sagsensor=map(sagsensor,50,950,0,1000); 
onsensor= map(onsensor,50,950,0,1000); 
//Fsens=(FRsens+FLsens)/2;
}
