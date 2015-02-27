/**
 * ledbtn_lite.c - Exo_4 - Accès aux GPIO depuis les fonctions du pilote
 *
 * 
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

/*******************************************************************************
 * Headers
 ******************************************************************************/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hospital-Toumlilt, 2015");
MODULE_DESCRIPTION("RPi LDT-BTN simple driver");

/*******************************************************************************
 * Parameters
 ******************************************************************************/
/**
 * @param nombre de LEDs
 */
static int nbLed;
module_param(nbLed, int, 0);
MODULE_PARM_DESC(nbLed, "Nombre de LEDs");

/**
 * @param nombre de BTNs
 */
static int nbBtn;
module_param(nbBtn, int, 0);
MODULE_PARM_DESC(nbBtn, "Nombre de BTNs");

/*******************************************************************************
 * GPIO access
 ******************************************************************************/
/* GPIO_LED Pins */
static const int GPIO_LED[4] = {4, 17, 22, 27};

/* GPIO_BTN Pins */
static const int GPIO_BTN[2] = {18, 23};

/* I/O GPIO States configuration */
static const int GPIO_INPUT  = 0;
static const int GPIO_OUTPUT = 1;

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
 * GPIO Operations ( from TME02 )
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

void gpio_value(int gpio, char* value)
{
  *value = ( gpio_regs->gplev[gpio/32] >> (gpio % 32) ) & 0x1;
}

/*******************************************************************************
 * I/O Simple functions
 ******************************************************************************/
int i; /* this will be useful */
char* memory_buffer; /* memoire for the buffer ( read ) */

static int open_ledbtn(struct inode *inode, struct file *file)
{
  printk(KERN_DEBUG "open_ledbtn()\n");
  return 0;
}

static ssize_t read_ledbtn(struct file *file, char *buf,
			   size_t count, loff_t *ppos) {
  printk(KERN_DEBUG "read_lebtn()\n");

  /* fetching data from GPIO */
  for(i=0; i<nbBtn; i++){
    gpio_value(GPIO_BTN[i], memory_buffer+i);
    memory_buffer[i] += 48;
  }

  /* transfering data to user space */
  i = copy_to_user(buf, memory_buffer, 1);
  if( i ){
    printk(KERN_WARNING "WARNING: copy_to_user not completed\n");
  }

  /* changing reading position as best suits */
  if( *ppos == 0 ){
    *ppos += 1;
    return 1;
  }
  return 0;
}

static ssize_t write_ledbtn(struct file *file, const char *buf,
			    size_t count, loff_t *ppos) {
  printk(KERN_DEBUG "write_ledbtn()\n");

  /** analyse de l'entrée **/
  /* on doit avoir une entrée de taille au moins égale
     au nombre de LEDs */
  if( count < nbLed ){
    printk(KERN_ERR "ERROR: invalid entry:\n");
    printk(KERN_ERR "->You must at least give %d chars\n", nbLed);
    return count;
  }
  /* les entrées sont binaires */
  for(i=0; i<nbLed; i++){
    if( buf[i] != '0' && buf[i] != '1' ){
      printk(KERN_ERR "ERROR: invalid entry:\n");
      printk(KERN_ERR "-> entry %c is not C {0,1}\n", buf[i]);
      return count;
    }
  }

  /** tout est bon je fais mon travail **/
  for(i=0; i<nbLed; i++){
    gpio_update(GPIO_LED[i], buf[i]-48);
  }

  return count;
}

static int release_ledbtn(struct inode *inode, struct file *file) {
  printk(KERN_DEBUG "close_ledbtn()\n");
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

   /* validité des arguments */
   if( nbLed == 0 && nbBtn == 0 ){
     printk(KERN_ERR "ERROR: nbLed && nbBtn equals 0\n");
     printk(KERN_ERR "-> You must at least assign one entry\n");
     return -1;
   }

   /* initialisation de l'adresse de base */
   gpio_regs = (struct gpio_s *)__io_address(GPIO_BASE);

   /* configuration des registres GPIO */
   for(i=0; i<nbLed; i++)
     gpio_config(GPIO_LED[i], GPIO_OUTPUT);
   for(i=0; i<nbBtn; i++)
     gpio_config(GPIO_BTN[i], GPIO_INPUT);

   /* allocation de memoire pour le buffer */
   memory_buffer = kmalloc(nbBtn * sizeof(char), GFP_KERNEL);
   if( !memory_buffer ){
     printk(KERN_ERR "ERROR: cannot allocate memory for the buffer\n");
     return -1;
   }
   memset(memory_buffer, 0, nbBtn * sizeof(char));

   /* enregistrement du driver,
    * 0 est le num majeur qu'on laisse choisir par le kernel */
   major = register_chrdev(0, "ledbtn", &fops_ledbtn);

   return 0;
}

static void __exit mon_module_cleanup(void)
{
  /* Freeing the major number */
  unregister_chrdev(major, "ledbtn");

  /* Freeing buffer memory */
  if( memory_buffer ){
    kfree(memory_buffer);
  }

  printk(KERN_DEBUG "Goodbye World!\n");
}

module_init(mon_module_init);
module_exit(mon_module_cleanup);
