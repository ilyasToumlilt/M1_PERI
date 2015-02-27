/**
 * ledbtn_lite.c - Exo_3 - Driver qui ne fait rien mais dans le Noyau
 *
 * Test des fonction I/O avec des printk.
 *
 * @author Alexandra Hospital <hospital.alex@gmail.com>
 * @author Ilyas Toumlilt     <toumlilt.ilyas@gmail.com>
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>

/*******************************************************************************
 * Headers
 ******************************************************************************/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hospital-Toumlilt, 2015");
MODULE_DESCRIPTION("RPi LDT-BTN simple driver");

/*******************************************************************************
 * I/O Simple functions
 ******************************************************************************/
static int open_ledbtn(struct inode *inode, struct file *file)
{
  printk(KERN_DEBUG "open()\n");
  return 0;
}

static ssize_t read_ledbtn(struct file *file, char *buf,
			   size_t count, loff_t *ppos) {
  printk(KERN_DEBUG "read()\n");
  return count;
}

static ssize_t write_ledbtn(struct file *file, const char *buf,
			    size_t count, loff_t *ppos) {
  printk(KERN_DEBUG "write()\n");
  return count;
}

static int release_ledbtn(struct inode *inode, struct file *file) {
  printk(KERN_DEBUG "close()\n");
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

   /* enregistrement du driver,
    * 0 est le num majeur qu'on laisse choisir par le kernel */
   major = register_chrdev(0, "ledbtn_lite", &fops_ledbtn);

   return 0;
}

static void __exit mon_module_cleanup(void)
{
  unregister_chrdev(major, "ledbtn_lite");

  printk(KERN_DEBUG "Goodbye World!\n");
}

module_init(mon_module_init);
module_exit(mon_module_cleanup);
