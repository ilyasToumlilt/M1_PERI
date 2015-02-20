/**
 * TP02 - EXO05 - GPIOs I/O Functions
 *
 * @author Alexandra Hospital <hospital.alex@gmail.com>
 * @author Ilyas     Toumlilt <toumlilt.ilyas@gmail.com>
 *
 * @version 1.1
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>

uint32_t volatile* gpio_base_p;

#define GPIO_GPSET0 0x1c
#define GPIO_GPSET1 0x20

#define GPIO_GPCLR0 0x28
#define GPIO_GPCLR1 0x2c

#define GPIO_GPLEV0 0x34
#define GPIO_GPLEV1 0x38

/*
 * Read the value of a given GPIO.
 */
int gpio_value ( int gpio, int * value )
{
  *value = (*( gpio_base_p + ( GPIO_GPLEV0 / 4 )) >> gpio) & 1 ;
  
  return 0;
}

/*
 * Update the value of the GPIO (if output).
 * Output value if zero if 'value' == 0, 1 otherwise.
 */
int gpio_update ( int gpio, int value )
{
  if( value )
    *( gpio_base_p + (GPIO_GPSET0 / 4) ) = (0x1 << gpio);
  else
    *( gpio_base_p + (GPIO_GPCLR0 / 4) ) = (0x1 << gpio);
  return 0;
}
