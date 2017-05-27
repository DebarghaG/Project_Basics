
const int trigPin = 3;
const int echoPin = 2;
void setup() 
{
  // initialize serial communication:
  Serial.begin(9600);
}
void loop()
{
  // establish variables for duration of the ping, 
  // and the distance result in inches and centimeters:
  long duration, inches, cm;

  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  repeat:
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);

  // convert the time into a distance
  cm = microsecondsToCentimeters(duration);

  if( duration>10000) // If the pulse doesnot come back - i.e. it has either encountered a soft object or probably is too far away.
  {
    goto repeat;
  }
  
  Serial.print(cm);
  Serial.print(" ");
  
  
  Serial.println();
  int OutputPin=9;
  unsigned int thisPitch = map(cm, 2, 400, 150, 15000);
   // play the pitch:
  tone(OutputPin, thisPitch, 10);       // delay in between reads for stability
  Serial.print(" ");
  delay(5);
}

long microsecondsToCentimeters(long microseconds)
{
  return microseconds/29/2;
} 

