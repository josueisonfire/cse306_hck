#include <linux/kernel.h>
asmlinkage long sys_hello(void)
{
    printk("Hello WOrld!\n");
    return 0;
}
