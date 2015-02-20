/**
 * TP02 - EXO06 - Amusez vous !
 *
 * Alors pour nous amuser on utilisera deux LEDs et les deux BTNs
 * pour implémenter un automate déterministe qui se base 
 * sur les états des noeuds.
 * Chaque bouton s'occupe d'allumer/éteindre une LED.
 * Le programme s'arrête quand on appuie sur les deux BTNS.
 *
 * @author Alexandra Hospital <hospital.alex@gmail.com>
 * @author Ilyas     Toumlilt <toumlilt.ilyas@gmail.com>
 *
 * @version 1.1
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <gpio.h>

#define GPIO_LED0 4
#define GPIO_LED1 17

#define GPIO_BTN0 18
#define GPIO_BTN1 23

/* LEDs states */
#define NOT_BLINKING 0
#define BLINKING     1
/* BTNs states */
#define PRESSED     0
#define NOT_PRESSED 1
/* Program state */
#define RUNNING 1
#define STOPPED 0

/*
 * Main program.
 * Alors pour nous amuser on utilisera deux LEDs et les deux BTNs
 * pour implémenter un automate déterministe qui se base 
 * sur les états des noeuds.
 * Chaque bouton s'occupe d'allumer/éteindre une LED.
 * Le programme s'arrête quand on appuie sur les deux BTNS.
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
  
  /* Setup GPIO of LED0 and LED1 to output. */
  gpio_config(GPIO_LED0, GPIO_OUTPUT_PIN);
  gpio_config(GPIO_LED1, GPIO_OUTPUT_PIN);

  /* Setup GPIO of BTN0 and BTN1 to input. */
  gpio_config(GPIO_BTN0, GPIO_INPUT_PIN);
  gpio_config(GPIO_BTN1, GPIO_INPUT_PIN); 

  /* states initialization */
  int led0_state = BLINKING;
  int led1_state = BLINKING;
  int btn0_state = NOT_PRESSED;
  int btn1_state = NOT_PRESSED;
  int prog_state = RUNNING;
  int btn0_input, btn1_input;

  /* main loop */
  while( prog_state == RUNNING ){
    gpio_value(GPIO_BTN0, &btn0_input);
    gpio_value(GPIO_BTN1, &btn1_input);
    
    led0_state = ((led0_state && ((btn0_input == btn0_state) ||
				  btn0_state == PRESSED))
		  || (!led0_state && (btn0_state != btn0_input) && 
		      (btn0_input == PRESSED))) ? BLINKING : NOT_BLINKING;
    led1_state = ((led1_state && ((btn1_state == btn1_input) ||
				  btn1_state == PRESSED))
		  || (!led1_state && (btn1_state != btn1_input) &&
		      (btn1_input == PRESSED))) ? BLINKING : NOT_BLINKING;

    prog_state = ((btn0_input == PRESSED) && (btn1_input == PRESSED))
      ? STOPPED : RUNNING;

    btn0_state = btn0_input;
    btn1_state = btn1_input;

    gpio_update(GPIO_LED0, led0_state);
    gpio_update(GPIO_LED1, led1_state);

    usleep(period * 100);
  }

  /* turning off LEDs */
  gpio_update(GPIO_LED0, NOT_BLINKING);
  gpio_update(GPIO_LED1, NOT_BLINKING);
  
  /* Release the GPIO memory mapping. */
  gpio_teardown();
  
  return 0;
}
