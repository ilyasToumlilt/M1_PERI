/**
 * brightness.ino - Allume la LED s'il fait sombre.
 *
 * @author Alexandra Hospital <hospital.alex@gmail.com>
 * @author Ilyas Toumlilt     <toumlilt.ilyas@gmail.com>
 */
 
int ledPin = 13;
int brightValue;
const int BRIGHT_LIMIT = 100;

void setup()
{
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop()
{
  brightValue = analogRead(A0);
  if( brightValue < BRIGHT_LIMIT )
    digitalWrite(ledPin, HIGH);
  else
    digitalWrite(ledPin, LOW);
   delay(100);
}

