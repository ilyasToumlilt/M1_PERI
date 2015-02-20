/**
 * TP02 - EXO02 - Contrôler une LED
 *
 * Authors:
 * Julien    Peeters  <julien.peeters@lip6.fr>
 * Alexandra Hospital <hospital.alex@gmail.com>
 * Ilyas     Toumlilt <toumlilt.ilyas@gmail.com>
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <gpio.h>

static void delay ( unsigned int milisec )
{
  struct timespec ts, dummy;
  ts.tv_sec = ( time_t ) milisec / 1000;
  ts.tv_nsec = ( long ) ( milisec % 1000 ) * 1000000;
  nanosleep ( &ts, &dummy );
}

#define GPIO_LED0 4

/*
 * Main program.
 */
int main ( int argc, char **argv )
{
  int period, half_period;
  
  /* Retreive the mapped GPIO memory. */
  if( gpio_setup() == -1 ){
    perror("Error: gpio_setup\n");
    exit(1);
  }
  period = 1000; /* default = 1Hz */
  if ( argc > 1 ) {
    period = atoi ( argv[1] );
  }
  half_period = period / 2;
  
  /* Setup GPIO of LED0 to output. */
  gpio_config(GPIO_LED0, GPIO_OUTPUT_PIN);

  /* PART 01 : LEDs tests */
  printf("----> PART 01 : LED testing, for 10 blinks <----\n");
  printf( "-- info: start blinking @ %f Hz.\n", ( 1000.0f / period ) );

  /* Blink led at frequency of 1Hz. */
  int nb_blinks = 10;
  while ( --nb_blinks > 0 ) {
    gpio_update( GPIO_LED0, 1);
    usleep(half_period * 1000);
    gpio_update( GPIO_LED0, 0);
    usleep(half_period * 1000);
  }

  /* reset state of GPIOs. */
  gpio_update(GPIO_LED0, 0);

  /* Release the GPIO memory mapping. */
  gpio_teardown();
  
  return 0;
}
