/**
 * brightCodeLock.ino 
 *
 * @author Alexandra Hospital <hospital.alex@gmail.com>
 * @author Ilyas Toumlilt     <toumlilt.ilyas@gmail.com>
 */

int ledPin = 13;
int brightValue;
const int BRIGHT_LIMIT = 100;

int state = 0;
int reflex = 500;
unsigned long cur_time, start_time;

void setup()
{
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(ledPin, OUTPUT);

  start_time = millis();
}

void loop()
{
  brightValue = analogRead(A0);
  cur_time = millis();

  switch( state )
    {
      /* State 0 : masquer 4s */
    case 0:
      if( brightValue > BRIGHT_LIMIT ){
        if (cur_time > start_time + 4000 ){
          state = 1; 
        }
        start_time = cur_time;
      }
      break;
    case 1: /* State 1 : démasquer 4s */
      if( cur_time > start_time + 4000 + reflex ){
        state = 0;
        start_time = cur_time;
      } else if ( brightValue < BRIGHT_LIMIT ){
        if( cur_time > start_time + 4000 - reflex ){
          state = 2;
        } else {
          state = 0;
        }
        start_time = cur_time;
      }
      break;
    case 2: /* State 2 : masquer 6s */
      if( cur_time > start_time + 6000 + reflex ){
        state = 0; /* sans remettre le temps à 0 */
      } else if ( brightValue > BRIGHT_LIMIT ){
        if( cur_time > start_time + 6000 - reflex ){
	  state = 3;
	}
	else if ( cur_time > start_time + 4000 - reflex ){
	  state = 1;
	}
	else {
	  state = 0;
	}
	start_time = cur_time;
      }
      break;
    case 3: /* serrure ouverte pour 3s */
      if( cur_time < start_time + 3000 ){
	digitalWrite(ledPin, HIGH);
      }
      else {
	digitalWrite(ledPin, LOW);
	state = 0;
	start_time = cur_time;
      }
      break;
    default: 
      break;
    }

   delay(10);
}
