/**
 * lcdRpi.c - Controleur pour LCd HD44780 ( 16x2 )
 *
 * Compatible pour les deux version de Raspberry.
 * Ecrit la chaîne reçue en argument sur le petit ecran.
 *
 * @author Alexandra Hospital <hospital.alex@gmail.com>
 * @author Ilyas Toumlilt     <toumlilt.ilyas@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>


/*******************************************************************************
 * GPIO Pins
 ******************************************************************************/
#define RS 7
#define E  8
#define D4 22
#define D5 23
#define D6 24
#define D7 25

#define GPIO_INPUT  0
#define GPIO_OUTPUT 1

#define RPI_BLOCK_SIZE    0xB4
#define BCM2835_GPIO_BASE 0x20200000
#define BCM2836_GPIO_BASE 0x3F200000

struct gpio_s
{
  uint32_t gpfsel[7];
  uint32_t gpset[3];
  uint32_t gpclr[3];
  uint32_t gplev[3];
  uint32_t gpeds[3];
  uint32_t gpren[3];
  uint32_t gpfen[3];
  uint32_t gphen[3];
  uint32_t gplen[3];
  uint32_t gparen[3];
  uint32_t gpafen[3];
  uint32_t gppud[1];
  uint32_t gppudclk[3];
  uint32_t test[1];
};
volatile struct gpio_s* gpio_regs;
static uint32_t gpio_base_adr;

/*******************************************************************************
 * GPIO Operations ( From TME 02/03 )
 ******************************************************************************/
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

int gpio_setup ( void ){

  int PiRev = setGpioBaseAdrFromPiRevision();
  if( PiRev == 0 ){
    printf("ERROR: Unknown RPi Revision\n");
    return -1;
  }
  printf("INFO: Setting up RPi Rev %d ...\n", PiRev);

  int mmap_fd = open ("/dev/mem", O_RDWR | O_SYNC );
  if ( mmap_fd < 0 ) {
    return -1;
  }

  gpio_regs = mmap ( NULL, RPI_BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED,
		       mmap_fd, gpio_base_adr );
  if ( gpio_regs == MAP_FAILED ) {
    close ( mmap_fd );
    return -1;
  }

  return 0;
}

void  gpio_teardown ( void ){
  munmap((void*)gpio_regs, RPI_BLOCK_SIZE);
}


void gpio_config(int gpio, int value)
{
  gpio_regs->gpfsel[gpio/10] = 
    ( gpio_regs->gpfsel[gpio/10] 
      & ~(0x7 << ((gpio % 10) * 3)) )
    | (value << ((gpio % 10) * 3));
}

void gpio_update(int gpio, int value)
{
  if( value )
    gpio_regs->gpset[gpio/32] = (0x1 << (gpio % 32) );
  else
    gpio_regs->gpclr[gpio/32] = (0x1 << (gpio % 32) );
}

/*******************************************************************************
 * LCD's Instructions ( source = doc )
 ******************************************************************************/

/* commands */
#define LCD_CLEARDISPLAY        "00000001"
#define LCD_RETURNHOME          "00000010"
#define LCD_ENTRYMODESET        "00000100"
#define LCD_DISPLAYCONTROL      "00001000"
#define LCD_CURSORSHIFT         "00010000"
#define LCD_FUNCTIONSET         "00100000"
#define LCD_SETCGRAMADDR        "01000000"
#define LCD_SETDDRAMADDR        "10000000"

/* flags for display entry mode */
#define LCD_ENTRYRIGHT          "00000000"
#define LCD_ENTRYLEFT           "00000010"
#define LCD_ENTRYSHIFTINCREMENT "00000001"
#define LCD_ENTRYSHIFTDECREMENT "00000000"

/* flags for display on/off control */
#define LCD_DISPLAYON           "00000100"
#define LCD_DISPLAYOFF          "00000000"
#define LCD_CURSORON            "00000010"
#define LCD_CURSOROFF           "00000000"
#define LCD_BLINKON             "00000001"
#define LCD_BLINKOFF            "00000000"

/* flags for display/cursor shift */
#define LCD_DISPLAYMOVE         "00001000"
#define LCD_CURSORMOVE          "00000000"
#define LCD_MOVERIGHT           "00000100"
#define LCD_MOVELEFT            "00000000"

/* flags for function set */
#define LCD_8BITMODE            "00010000"
#define LCD_4BITMODE            "00000000"
#define LCD_2LINE               "00001000"
#define LCD_1LINE               "00000000"
#define LCD_5x10DOTS            "00000100"
#define LCD_5x8DOTS             "00000000"

/* 8 bits default buffer */
char buffer[8];

/*******************************************************************************
 * LCD's Operations
 ******************************************************************************/
/* binary OR between two commands */
void binaryOR(char* cmd1, char* cmd2){
  int i;
  for(i=0; i<8; i++){
    buffer[i] = (cmd1[i] == '1' || cmd2[i] == '1') ? '1' : '0';
  }
}

void decimal2binary(char dec)
{
  int i, j, k;
  for(i=7, j=0; i>=0; i--, j++){    k = dec >> i;
    buffer[j] = ( k & 1 ) ? '1' : '0';
  }
}

/* generate E signal */
void lcd_strobe()
{
  gpio_update(E, 0);
  usleep(1);
  gpio_update(E, 1);
  usleep(1);
  gpio_update(E, 0);
  usleep(1);
}

/* send 4bits to LCD */
void lcd_write4bits(char* cmd, int mode)
{
  /* 1000 microseconds sleep */
  usleep(1000);
  
  /* first 4 bits */
  gpio_update(RS, mode);
  gpio_update(D7, cmd[0]-48);
  gpio_update(D6, cmd[1]-48);
  gpio_update(D5, cmd[2]-48);
  gpio_update(D4, cmd[3]-48);
  lcd_strobe();

  /* second 4 bits */
  gpio_update(D7, cmd[4]-48);
  gpio_update(D6, cmd[5]-48);
  gpio_update(D5, cmd[6]-48);
  gpio_update(D4, cmd[7]-48);
  lcd_strobe();
}

void lcd_command(char* cmd)
{
  lcd_write4bits(cmd, 0);
}

void lcd_data(char* data)
{
  lcd_write4bits(data, 1);
}

/* initialization */
void lcd_init()
{
  lcd_command("00110011"); /* initialization */
  lcd_command("00110010"); /* initialization */
  lcd_command("00101000"); /* 2 line 5x7 matrix */
  lcd_command("00001100"); /* turn cursor off */
  lcd_command("00000110"); /* shift cursor right */
}

void lcd_clear()
{
  lcd_command(LCD_CLEARDISPLAY);
  usleep(3000); /* ça prend du temps */
}

void lcd_message(char* txt)
{
  int i, j;
  for(i=0; i<16 && i<strlen(txt); i++){
    if(txt[i] == '\\' || txt[i] == '\0')
      break;
    decimal2binary(txt[i]);
    lcd_data(buffer);
  }
  lcd_command("11000000");
  for(j=i+1; j<i+16 && i<strlen(txt); j++){
    if(txt[j] == '\\' || txt[j] == '\0')
      break;
    decimal2binary(txt[j]);
    lcd_data(buffer);
  }
}
/*******************************************************************************
 * Finally, the main function
 ******************************************************************************/
int main(int argc, char** argv)
{
  /* arg */
  if( argc < 2 ){
    fprintf(stderr, "ERROR: must take a string as argument\n");
    exit(1);
  }

  /* Retreive the mapped GPIO memory */
  if( gpio_setup() == -1 ){
    perror("ERROR: gpio_setup\n");
    exit(1);
  }

  /* Setting up GPIOs to output */
  gpio_config(RS, GPIO_OUTPUT);
  gpio_config(E , GPIO_OUTPUT);
  gpio_config(D4, GPIO_OUTPUT);
  gpio_config(D5, GPIO_OUTPUT);
  gpio_config(D6, GPIO_OUTPUT);
  gpio_config(D7, GPIO_OUTPUT);

  /* initialization */
  lcd_init();

  /* default test direction */
  binaryOR(LCD_ENTRYLEFT, LCD_ENTRYSHIFTDECREMENT);
  binaryOR(buffer, LCD_ENTRYMODESET);
  lcd_command(buffer);

  lcd_clear();

  lcd_message(argv[1]);

  /* Release the GPIO memory mapping */
  gpio_teardown();

  return 0;
}
