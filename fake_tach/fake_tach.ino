#define PWM_CYCLE 40.0 	// 40 us/cycle (PWM Control Fan: 25kHz)
#define ONE_STEP 64 	// 64 us/step (TIMER)
#define SEMIPULSE 2500 	// 2500 us/semipulse (TACH at 100%)

#define INPUT_PIN0 A7
#define OUTPUT_PIN0 6

#define INPUT_PIN1 A6
#define OUTPUT_PIN1 9

void setup() {
  pinMode(INPUT_PIN0,INPUT);
  pinMode(OUTPUT_PIN0,OUTPUT);
  
  pinMode(INPUT_PIN1,INPUT);
  pinMode(OUTPUT_PIN1,OUTPUT);
  
  noInterrupts();
  // TIMER 0
  TCCR0A = B01000010; // COM Toggle - CTC
  TCCR0B = B00000101; // CTC - 1024 Prescaler
  TCNT0 = 0;          // Contador inicia 0
  OCR0A = 78;         // Steps 50% (default)
  TIFR0 = 0;          // Interruption flags
  TIMSK0 = B00000010; // Mask: CTC Interruption - Channel: A

  // TIMER 1
  TCCR1A = B01000000;
  TCCR1B = B00001101;
  TCNT1 = 0;
  OCR1A = 78;
  TIFR1 = 0;
  TIMSK1 = B00000010;
  
  interrupts();
}

void loop() {
  unsigned long val0 = pulseIn(INPUT_PIN0,HIGH);
  if (val0 >= 8.0) {
    double percentage = val0/PWM_CYCLE;
    double semipulse_time = SEMIPULSE/percentage;
    double steps = 2*semipulse_time/ONE_STEP;
    int steps0 = (int) steps;
    
    char sreg = SREG;
    noInterrupts();
    OCR0A = steps0;
    SREG=sreg;
  }

  unsigned long val1 = pulseIn(INPUT_PIN1,HIGH);
  if (val1 >= 8.0) {
    double percentage = val1/PWM_CYCLE;
    double semipulse_time = SEMIPULSE/percentage;
    double steps = semipulse_time/ONE_STEP;
    int steps1 = (int) steps;
    
    char sreg = SREG;
    noInterrupts();
    OCR1A = steps1;
    SREG=sreg;
  }


}

