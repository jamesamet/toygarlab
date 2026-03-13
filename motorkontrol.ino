// ============ MOTOR FONKSİYONU ============
void setMotors(int solHiz, int sagHiz) {
  solHiz = constrain(solHiz, -255, 255);
  sagHiz = constrain(sagHiz, -255, 255);

  if (abs(solHiz) < 15) solHiz = 0;
  if (abs(sagHiz) < 15) sagHiz = 0;

  // ================== SOL MOTOR (Pin 9=IN_B1, Pin 10=IN_B2) ==================
  if (solHiz > 0) {
    // IN_B1(Pin9)=HIGH, IN_B2(Pin10)=PWM
    TCCR1A = (TCCR1A | _BV(COM1B1)) & ~_BV(COM1A1);  // Pin10 PWM, Pin9 normal
    PORTB |= _BV(PORTB5);                              // Pin 9 HIGH
    OCR1B = 255 - solHiz;
  }
  else if (solHiz < 0) {
    // IN_B1(Pin9)=PWM, IN_B2(Pin10)=HIGH
    TCCR1A = (TCCR1A | _BV(COM1A1)) & ~_BV(COM1B1);  // Pin9 PWM, Pin10 normal
    PORTB |= _BV(PORTB6);                              // Pin 10 HIGH
    OCR1A = 255 - abs(solHiz);
  }
  else {
    // Fren: Her ikisi HIGH
    TCCR1A &= ~(_BV(COM1A1) | _BV(COM1B1));           // PWM kapat
    PORTB |= _BV(PORTB5) | _BV(PORTB6);               // Pin 9+10 HIGH
  }

  // ================== SAĞ MOTOR (Pin 5=IN_A2, Pin 6=IN_A1) ==================
  if (sagHiz > 0) {
    // IN_A1(Pin6)=HIGH, IN_A2(Pin5)=PWM
    TCCR3A |= _BV(COM3A1);            // Pin 5 PWM aç
    TCCR4C &= ~_BV(COM4D1);           // Pin 6 PWM kapat
    PORTD |= _BV(PORTD7);             // Pin 6 HIGH
    OCR3A = 255 - sagHiz;
  }
  else if (sagHiz < 0) {
    // IN_A1(Pin6)=PWM, IN_A2(Pin5)=HIGH
    TCCR3A &= ~_BV(COM3A1);           // Pin 5 PWM kapat
    TCCR4C |= _BV(COM4D1);            // Pin 6 PWM aç
    PORTC |= _BV(PORTC6);             // Pin 5 HIGH
    OCR4D = 255 - abs(sagHiz);
  }
  else {
    // Fren: Her ikisi HIGH
    TCCR3A &= ~_BV(COM3A1);           // PWM kapat
    TCCR4C &= ~_BV(COM4D1);           // PWM kapat
    PORTC |= _BV(PORTC6);             // Pin 5 HIGH
    PORTD |= _BV(PORTD7);             // Pin 6 HIGH
  }
}
