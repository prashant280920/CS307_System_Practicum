// Crashing  sysytem 
// Idea : Infinite loop


#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
 
static int __init load(void){
   while(1){
       printk(KERN_ALERT "Crashing using module\n");
   }
   return 0;
}
 
 
static void unload(void){
   printk(KERN_ALERT "Unload Crash module\n");
}
 
module_init(load);
module_exit(unload);