/**
 * blink.ino - Fait clignoter la LED pin13.
 *
 * @author Alexandra Hospital <hospital.alex@gmail.com>
 * @author Ilyas Toumlilt     <toumlilt.ilyas@gmail.com>
 */

int ledPin = 13;

void setup()
{
  pinMode(13, OUTPUT);
}

void loop()
{
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(500);
}
  
