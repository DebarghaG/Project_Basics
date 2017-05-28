// pin definitions
// note: the echoPin has to be a pin which is allowed for interrupts, see here: https://www.arduino.cc/en/Reference/attachInterrupt
const int trigPin = 3;
const int echoPin = 2;
const int ledPin = 13;
const int speakerPin = 9;

// interrupt speed. the datasheet of the HC-SR04 suggests 60 ms, so chose 16 Hz
const float irqSpeed = 16.0;

// updated in the echoInterrupt function
volatile uint32_t echoStart;
volatile uint32_t echoTime;

void setup()
{
  // initialize serial communication:
  Serial.begin(115200);

  // configure pin input and output mode
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(speakerPin, OUTPUT);

  // initialize timer1
  noInterrupts(); // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 15625.0f / irqSpeed; // compare match register for IRQ
  TCCR1B |= (1 << WGM12); // CTC mode
  TCCR1B |= (1 << CS10) | (1 << CS12); // prescaler 1024
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  interrupts(); // enable all interrupts

  // attach interrupt to echoPin changes
  attachInterrupt(digitalPinToInterrupt(echoPin), echoInterrupt, CHANGE);
}

// start measuring the echo time on rising edge, stop measuring on falling edge and store in echoTime
void echoInterrupt()
{
  if (digitalRead(echoPin) == LOW) {
    echoTime = micros() - echoStart;
  } else {
    echoStart = micros();
  }
}

// timer 1 interrupt, called with 16 Hz
ISR(TIMER1_COMPA_vect)
{
  // test if the echo pin is not asserted (could be if timeout)
  if (digitalRead(echoPin) == LOW) {
    // send a trigger
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // output last tone
    uint32_t mm = microsecondsToMillimeters(echoTime);
    uint32_t pitch = map(mm, 20, 4000, 150, 5000);
    if (mm > 4000) {
      noTone(speakerPin);
    } else {
      tone(speakerPin, pitch);
    }
  }
}

void loop()
{
  digitalWrite(ledPin, HIGH);
  delay(100);
  digitalWrite(ledPin, LOW);
  delay(100);

  uint32_t mm = microsecondsToMillimeters(echoTime);
  Serial.print(mm);
  Serial.println();
}

uint32_t microsecondsToMillimeters(uint32_t microseconds)
{
  // speed of sound: 340 m/s
  return 10 * microseconds / 34 / 2;
}