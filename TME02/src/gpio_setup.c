/**
 * TP02 - EXO05 - Configuration File
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
#include <string.h>

#define RPI_BLOCK_SIZE    0xB4
#define BCM2835_GPIO_BASE 0x20200000
#define BCM2836_GPIO_BASE 0x3F200000

static uint32_t gpio_base_adr;

uint32_t volatile* gpio_base_p;

/**
 * Affecte la bonne adresse de base selon la revision de la RPi
 *
 * @return le numéro de révision de la RPi
 */
int setGpioBaseAdrFromPiRevision(void)
{
  FILE* filp = fopen("/proc/cpuinfo", "r");
  if( filp ){
    char buf[512];
    while(fgets(buf, sizeof(buf), filp)){
      if( !strncasecmp("model name", buf, 10)){
	if( strstr(buf, "ARMv6") ){
	  gpio_base_adr = BCM2835_GPIO_BASE;
	  fclose(filp);
	  return 1;
	}
	else if( strstr(buf, "ARMv7") ){
	  gpio_base_adr = BCM2836_GPIO_BASE;
	  fclose(filp);
	  return 2;
	}
      }
    }
  }
  printf("ERROR: can't open /proc/cpuinfo\n");
  return 0;
}

/*
 * Set up the GPIO memory mapping.
 *
 * Returns -1 in case of error, 0 otherwise.
 *
 * Note: this function must be called before any other
 * code related to GPIO.
 */
int gpio_setup ( void ){

  int PiRev = setGpioBaseAdrFromPiRevision();
  if( PiRev == 0 ){
    printf("ERROR: Unknown RPi Revision\n");
    return -1;
  }
  printf("Setting up RPi Rev %d ...\n", PiRev);

  int mmap_fd = open ("/dev/mem", O_RDWR | O_SYNC );
  if ( mmap_fd < 0 ) {
    return -1;
  }

  gpio_base_p = mmap ( NULL, RPI_BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED,
		       mmap_fd, gpio_base_adr );
  if ( gpio_base_p == MAP_FAILED ) {
    close ( mmap_fd );
    return -1;
  }

  return 0;
}

/*
 * Tear down the GPIO memory mapping.
 */
void  gpio_teardown ( void ){
  munmap((void*)gpio_base_p, RPI_BLOCK_SIZE);
}
