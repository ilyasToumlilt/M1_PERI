/**
 * module.c - Exo_1 - Le module le plus simple du monde !
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
MODULE_DESCRIPTION("Module, aussitot insere, aussitot efface");

static int __init mon_module_init(void)
{
   printk(KERN_DEBUG "Hello World !\n");
   return 0;
}

static void __exit mon_module_cleanup(void)
{
   printk(KERN_DEBUG "Goodbye World!\n");
}

module_init(mon_module_init);
module_exit(mon_module_cleanup);
