/**
 * lcdRpi.c - Controleur pour LCD HD44780 ( 16x2 )
 *
 * Driver pour LCD.
 *
 * @author Alexandra Hospital <hospital.alex@gmail.com>
 * @author Ilyas Toumlilt     <toumlilt.ilyas@gmail.com>
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <asm/io.h>
#include <mach/platform.h>
#include <asm/uaccess.h>
#include <linux/delay.h>

/*******************************************************************************
 * Headers
 ******************************************************************************/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hospital-Toumlilt, 2015");
MODULE_DESCRIPTION("RPi Driver for HD44780 LCD");

/*******************************************************************************
 * Parameters
 ******************************************************************************/
/**
 * @param nombre de lignes du LCD
 */
static int nbLines;
module_param(nbLines, int, 0);
MODULE_PARM_DESC(nbLines, "Nombre de lignes du LCD");

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

/*******************************************************************************
 * GPIO Operations ( From TME 02/03 )
 ******************************************************************************/
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
void lcd_strobe(void)
{
  gpio_update(E, 0);
  udelay((long)1);
  gpio_update(E, 1);
  udelay((long)1);
  gpio_update(E, 0);
  udelay((long)1);
}

/* send 4bits to LCD */
void lcd_write4bits(char* cmd, int mode)
{
  /* 1000 microseconds sleep */
  udelay((long)1000);
  
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
void lcd_init(void)
{
  lcd_command("00110011"); /* initialization */
  lcd_command("00110010"); /* initialization */
  lcd_command("00101000"); /* 2 line 5x7 matrix */
  lcd_command("00001100"); /* turn cursor off */
  lcd_command("00000110"); /* shift cursor right */
}

void lcd_clear(void)
{
  lcd_command(LCD_CLEARDISPLAY);
  mdelay((long)3); /* ça prend du temps */
}

void lcd_message(const char* txt, size_t count)
{
  int i, j, l;
  printk(KERN_DEBUG "%s", txt);

  for(i=0; i<16 && i<count; i++){
    if(txt[i] == '\\' || txt[i] == '\0')
      break;
    decimal2binary(txt[i]);
    lcd_data(buffer);
  }
  for(l=1; l<nbLines && i<count; l++){
    lcd_command("11000000");
    for(j=i+1; j<i+16 && i<count; j++){
      if(txt[j] == '\\' || txt[j] == '\0')
        break;
      decimal2binary(txt[j]);
      lcd_data(buffer);
    }
    i=j;
  }
}

/*******************************************************************************
 * I/O Simple functions
 ******************************************************************************/
int i; /* this will be useful */
char* memory_buffer; /* memoire for the buffer ( read ) */

static int open_ledbtn(struct inode *inode, struct file *file)
{
  return 0;
}

static ssize_t read_ledbtn(struct file *file, char *buf,
			   size_t count, loff_t *ppos) {
  return 0;
}

static ssize_t write_ledbtn(struct file *file, const char *buf,
			    size_t count, loff_t *ppos) {
  printk(KERN_DEBUG "write_ledbtn()\n");

  int i;
  for(i=0; i<5; i++)
    printk(KERN_DEBUG "%c", buf[i]);

  lcd_clear();
  lcd_message(buf, count-1);

  return count;
}

static int release_ledbtn(struct inode *inode, struct file *file) {
  return 0;
}

struct file_operations fops_ledbtn =
  {
    .open    = open_ledbtn,
    .read    = read_ledbtn,
    .write   = write_ledbtn,
    .release = release_ledbtn
  };

/*******************************************************************************
 * Module Basics
 ******************************************************************************/
int major;

static int __init mon_module_init(void)
{
   printk(KERN_DEBUG "Hello World !\n");

   /* 4 lignes par défaut */
   if( nbLines == 0 ){
     nbLines = 4;
   }

   /* validité des arguments */
   if( nbLines != 2 && nbLines != 4 ){
     printk(KERN_ERR "ERROR: nbLines must be equal to 2 or 4\n");
     return -1;
   }

   /* initialisation de l'adresse de base */
   gpio_regs = (struct gpio_s *)__io_address(GPIO_BASE);

   /* configuration des registres GPIO */
   gpio_config(RS, GPIO_OUTPUT);
   gpio_config(E , GPIO_OUTPUT);
   gpio_config(D4, GPIO_OUTPUT);
   gpio_config(D5, GPIO_OUTPUT);
   gpio_config(D6, GPIO_OUTPUT);
   gpio_config(D7, GPIO_OUTPUT);

   /* default test direction */
   binaryOR(LCD_ENTRYLEFT, LCD_ENTRYSHIFTDECREMENT);
   binaryOR(buffer, LCD_ENTRYMODESET);
   lcd_command(buffer);
   
   lcd_clear();

   /* enregistrement du driver,
    * 0 est le num majeur qu'on laisse choisir par le kernel */
   major = register_chrdev(0, "lcdrpi", &fops_ledbtn);

   return 0;
}

static void __exit mon_module_cleanup(void)
{
  /* Freeing the major number */
  unregister_chrdev(major, "lcdrpi");

  printk(KERN_DEBUG "Goodbye World!\n");
}

module_init(mon_module_init);
module_exit(mon_module_cleanup);
