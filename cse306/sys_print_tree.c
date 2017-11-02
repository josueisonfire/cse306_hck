#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/fs.h>
#include <linux/fs_struct.h>
#include <linux/string.h>
#include <linux/tty.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

asmlinkage long sys_print_tree(char *buf, int buflen, struct task_struct *task)
{
    //init stack
    int sp = 0;
    struct list_head *pos;
    struct task_struct **tstack;
    int *tabstack;
    char *buffer;
    char *initBuffer;
    tstack = kmalloc(sizeof(struct task_struct*)*1000, GFP_KERNEL);
    tabstack = kmalloc(sizeof(int)*1000, GFP_KERNEL);
    buffer = kmalloc(sizeof(char)*buflen, GFP_KERNEL);
    initBuffer = buffer;
    //init stack
    if(task == NULL)
    {
        tstack[sp] = &init_task;
    }    
    else
    {
        tstack[sp] = task;
    }
    tabstack[sp] = 0;
    sp++;

    //loop
    while(sp > 0)
    {
        struct task_struct *t;
        int tabq;
        int i;
        int n;
        int ti;
        long seconds;
        long decimals;
        sp--;
        t = tstack[sp];
        tabq = tabstack[sp];
        i = tabq;
        while(i>0)
        {
            printk("%s", "\t");
            n = snprintf(buffer, buflen, "%s", "\t");
            buffer += n;
            buflen -= n;
            i--;
        }     
        printk("%d", t->pid);
        n = snprintf(buffer, buflen, "%d", t->pid);
        buffer += n;
        buflen -= n;
        printk("%s", ":");
        n = snprintf(buffer, buflen, "%s", ":");
        buffer += n;
        buflen -= n;
        if(t->signal->tty->name == NULL)
        {
            printk("%s", ".");
            n = snprintf(buffer, buflen, "%s", ".");
            buffer += n;
            buflen -= n;
        }
        else
        {
            printk("%s", t->signal->tty->name);
            n = snprintf(buffer, buflen, "%s", t->signal->tty->name);
            buffer += n;
            buflen -= n;
        }
        printk("%s", ":");
        n = snprintf(buffer, buflen, "%s", ":");
        buffer += n;
        buflen -= n;
        ti = (cputime_to_nsecs(t->utime + t->stime));
            seconds = ti / 1000000000; //obtain seconds
            ti = ti - seconds*1000000000;
            decimals = ti / 1000000;
            //append into a custom buffer:
            //init buffer:
            printk("%ld", seconds);
            n = snprintf(buffer, buflen, "%ld", seconds);
            buffer += n;
            buflen -= n;
            printk("%s", ".");
            n = snprintf(buffer, buflen, "%s", ".");
            buffer += n;
            buflen -= n;
            printk("%ld", decimals);
            n = snprintf(buffer, buflen, "%ld", decimals);
            buffer += n;
            buflen -= n;
        printk("%s", ":");
        n = snprintf(buffer, buflen, "%s", ":");
        buffer += n;
        buflen -= n;
        printk("%s", t->comm);
        n = snprintf(buffer, buflen, "%s", t->comm);
        buffer += n;
        buflen -= n;
        printk("%s", "\n");
        n = snprintf(buffer, buflen, "%s", "\n");
        buffer += n;
        buflen -= n;
        
        list_for_each(pos, &t->children)
        {
            struct task_struct *child;
            child = list_entry(pos, struct task_struct, sibling);
            //push
            tstack[sp] = child;
            tabstack[sp] = tabq+1;
            sp++;
        }
    }
    printk("%s", initBuffer);
    if(copy_to_user(buf, initBuffer, buflen))
    {
        return -EFAULT;
    }
    return 0;
}

    // // after such implementation, you should:
    //
    //      add buffer += snprintf(buffer, buflen, "\t"); after we print everything.
    //     1. implement the method in the kernel with its appropriate system call.
    //     2. Test run it. (user code provided in the slides)
    //     3. Finish by making the package file.