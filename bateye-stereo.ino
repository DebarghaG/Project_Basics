// pin definitions
// note: the echoPin has to be a pin which is allowed for interrupts, see here: https://www.arduino.cc/en/Reference/attachInterrupt
const uint8_t trigPinLeft = 4;
const uint8_t echoPinLeft = 2;
const uint8_t speakerPinLeft = 9;

const uint8_t trigPinRight = 5;
const uint8_t echoPinRight = 3;
const uint8_t speakerPinRight = 10;

// samplerate for speaker output
const float samplerate = 10000.0f;

// all information for one side
struct SideInfo_t {
  // current echo start time in microseconds
  uint32_t echoStart;

  // last measured echo time in microseconds
  uint32_t echoTime;

  // pin for trigger output
  uint8_t trigPin;

  // pin for echo input
  uint8_t echoPin;

  // pin for speaker output
  uint8_t speakerPin;

  // tone generator
  uint32_t toneAccu;
  uint32_t toneIncrement;
};

// the two sensors and speakers
volatile SideInfo_t leftSide = { 0, 0, trigPinLeft, echoPinLeft, speakerPinLeft };
volatile SideInfo_t rightSide = { 0, 0, trigPinRight, echoPinRight, speakerPinRight };

// current side for echo test
volatile SideInfo_t* echoSide = &leftSide;

// setup pin directions and attach interrupt to echo pins
void setupSide(volatile SideInfo_t& side)
{
  pinMode(side.trigPin, OUTPUT);
  pinMode(side.echoPin, INPUT);
  pinMode(side.speakerPin, OUTPUT);

  // attach interrupt to echoPin changes
  attachInterrupt(digitalPinToInterrupt(side.echoPin), echoInterrupt, CHANGE);

  // reset tone generators
  side.toneAccu = 0;
  side.toneIncrement = 0;
}

void setup()
{
  // initialize serial communication:
  Serial.begin(115200);

  // configure pin input and output mode
  setupSide(leftSide);
  setupSide(rightSide);

  // initialize timer1
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 16000000.0f / samplerate; // compare match register for IRQ with selected samplerate
  TCCR1B |= (1 << WGM12); // CTC mode
  TCCR1B |= (1 << CS10); // no prescaler
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  interrupts();
}

// start measuring the echo time on rising edge, stop measuring on falling edge and store in echoTime
void echoInterrupt()
{
  if (digitalRead(echoSide->echoPin) == LOW) {
    echoSide->echoTime = micros() - echoSide->echoStart;
  } else {
    echoSide->echoStart = micros();
  }
}

// send a trigger pulse to one side, wait for the echo, and update the speaker frequency
uint32_t doSide(volatile SideInfo_t& side)
{
  // test if the echo pin is not asserted (could be if timeout)
  if (digitalRead(side.echoPin) == LOW) {
    // output last tone
    uint32_t echoTime = side.echoTime;
    float mm = microsecondsToMillimeters(echoTime);

    // if below 10 cm, it is too close, then max frequency, otherwise offset by 10 cm
    mm -= 100.0f;
    if (mm < 0.0f) mm = 0.0f;

    // calculate pitch
    float pitch = fscale(20.0f, 4000.0f, 1000.0f, 150.0f, mm, 7.0f);

    // output tone, if less than 4 m distance, otherwise mute speaker
    if (mm > 4000) {
      side.toneIncrement = 0;
    } else {
      side.toneIncrement = freqToIncrement(pitch);
    }

    // set echo side to test
    echoSide = &side;

    // send a trigger
    digitalWrite(side.trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(side.trigPin, LOW);

    // wait for echo
    delay(60);

    return echoTime;
  }
  return 0;
}

// update tone generator and control speaker pins
void toneOutput(volatile SideInfo_t& side)
{
  digitalWrite(side.speakerPin, side.toneAccu & 0x80000000 ? HIGH : LOW);
  side.toneAccu += side.toneIncrement;
}

// timer 1 interrupt, called with 16 Hz
ISR(TIMER1_COMPA_vect)
{
  toneOutput(leftSide);
  toneOutput(rightSide);
}

void loop()
{
  uint32_t leftEcho = doSide(leftSide);
  uint32_t rightEcho = doSide(rightSide);

  // serial output for debugging
  /*
  Serial.print("left: ");
  Serial.print(microsecondsToMillimeters(leftEcho));
  Serial.print(", right: ");
  Serial.print(microsecondsToMillimeters(rightEcho));
  Serial.println();
  */
}

// calculate accumumator increment for tone generator by frequency
uint32_t freqToIncrement(float freq)
{
  return 4294967296.0f / samplerate * freq;
}

// calculate millimeter distance from microseconds delay
float microsecondsToMillimeters(float microseconds)
{
  // speed of sound: 340 m/s
  return 0.14705882352941176471f * microseconds;  // same as 10.0f * microseconds / 34.0f / 2.0f;
}

// logarithmic scaling from http://playground.arduino.cc/Main/Fscale
float fscale( float originalMin, float originalMax, float newBegin, float newEnd, float inputValue, float curve)
{
  float OriginalRange = 0;
  float NewRange = 0;
  float zeroRefCurVal = 0;
  float normalizedCurVal = 0;
  float rangedValue = 0;
  boolean invFlag = 0;

  // condition curve parameter
  // limit range

  if (curve > 10) curve = 10;
  if (curve < -10) curve = -10;

  curve = (curve * -.1) ; // - invert and scale - this seems more intuitive - postive numbers give more weight to high end on output 
  curve = pow(10, curve); // convert linear scale into lograthimic exponent for other pow function

  // Check for out of range inputValues
  if (inputValue < originalMin) {
    inputValue = originalMin;
  }
  if (inputValue > originalMax) {
    inputValue = originalMax;
  }

  // Zero Refference the values
  OriginalRange = originalMax - originalMin;

  if (newEnd > newBegin){ 
    NewRange = newEnd - newBegin;
  }
  else
  {
    NewRange = newBegin - newEnd; 
    invFlag = 1;
  }

  zeroRefCurVal = inputValue - originalMin;
  normalizedCurVal  =  zeroRefCurVal / OriginalRange;   // normalize to 0 - 1 float

  // Check for originalMin > originalMax  - the math for all other cases i.e. negative numbers seems to work out fine 
  if (originalMin > originalMax ) {
    return 0;
  }

  if (invFlag == 0){
    rangedValue =  (pow(normalizedCurVal, curve) * NewRange) + newBegin;

  }
  else     // invert the ranges
  {   
    rangedValue =  newBegin - (pow(normalizedCurVal, curve) * NewRange); 
  }

  return rangedValue;
}

