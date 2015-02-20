/**
 * TP02 - EXO05 - GPIOs Config File
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
#include <stdint.h>
#include <sys/ioctl.h>

uint32_t volatile* gpio_base_p;

int gpio_config ( int gpio, int value ) {
  *(gpio_base_p + (gpio / 10)) =
    ( *(gpio_base_p + (gpio/10) )
      & ~(0x7 << ((gpio % 10)*3)) )  
    | ( (value-1) << ((gpio%10)*3));

  return 0;
}
