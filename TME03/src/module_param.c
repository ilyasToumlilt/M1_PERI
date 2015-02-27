/**
 * module_param.c - Exo_2 - Module minimal avec params
 *
 * Prend en argument le nombre de LEDs et le nombre de BTNs
 *
 * @author Alexandra Hospital <hospital.alex@gmail.com>
 * @author Ilyas     Toumlilt <toumlilt.ilyas@gmail.com>
 *
 * @version 1.0
 * @package M1/PERI
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hospital-Toumlilt, 2015");
MODULE_DESCRIPTION("Module avec arguments");

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

static int __init mon_module_init(void)
{
   printk(KERN_DEBUG "Hello World !\n");
   printk(KERN_DEBUG "nbLed=%d; nbBtn=%d;\n", nbLed, nbBtn);
   return 0;
}

static void __exit mon_module_cleanup(void)
{
   printk(KERN_DEBUG "Goodbye World!\n");
}

module_init(mon_module_init);
module_exit(mon_module_cleanup);
