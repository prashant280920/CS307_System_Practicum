// Crashing  sysytem 
// Idea : dereference a NULL pointer;

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
 
static int __init load(void){
  printk(KERN_INFO "Crashing using module\n");
  *(int *)0=0;
   return 0;
}
 
 
static void unload(void){
   printk(KERN_INFO "Unload Crash module\n");
}
 
module_init(load);
module_exit(unload);