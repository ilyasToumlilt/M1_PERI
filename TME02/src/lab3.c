/**
 * TP02 - EXO04 - Contrôler les entrées.
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

#define GPIO_BTN0 18
#define GPIO_BTN1 23

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
  
  /* Setup GPIO of BTN0 and BTN1 to input. */
  gpio_config(GPIO_BTN0, GPIO_INPUT_PIN);
  gpio_config(GPIO_BTN1, GPIO_INPUT_PIN); 

  /* BTNs tests */
  int btn0_input, btn1_input;
  printf("----> PART 02 : BTN testing, for 20 secs <----\n");
  printf("-- info: Press BTN to test reaction.\n");
  int nb_secs = 200;
  while( --nb_secs >= 0 ){
    gpio_value(GPIO_BTN0, &btn0_input);
    gpio_value(GPIO_BTN1, &btn1_input);
    printf("-> (%d, %d)\n", btn0_input, btn1_input);
    usleep(period * 100);
  } 
  
  /* Release the GPIO memory mapping. */
  gpio_teardown();
  
  return 0;
}
