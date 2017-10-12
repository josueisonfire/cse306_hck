#include <linux/kernel.h>
#include <linux/uaccess.h>
asmlinkage long sys_hello_name(char *name, int namelen, char *msg, int msglen)
{
    unsigned char nameBuffer[100];
    unsigned char messageBuffer[100];
    if(sizeof(nameBuffer) < namelen)
    {
        return -EINVAL;
    }
    if(sizeof(messageBuffer) < msglen)
    {
        return -EINVAL; 
    }
    
    // Copies contents of name string to nameBuffer buffer.
    if(copy_from_user(nameBuffer, name, namelen)) 
    {
        return -EFAULT;
    }
    sprintf(messageBuffer, "Hello! %s",nameBuffer);
    
    if(copy_to_user(msg, messageBuffer, msglen))
    {
        return -EFAULT;
    }
    return 0;
}
