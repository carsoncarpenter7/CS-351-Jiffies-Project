/**
 * simple.c
 *
 * A simple kernel module. 
 * 
 * To compile, run makefile by entering "make"
 *
 * Operating System Concepts - 10th Edition
 * Copyright John Wiley & Sons - 2018
 */



#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/gcd.h>
#include <linux/hash.h>
#include <linux/jiffies.h>

#define BUFFER_SIZE 128

#define PROC_NAME "jiffies"


unsigned long start;
unsigned long end;
unsigned long total;
// #define MESSAGE "Hello World\n"

/**
 * Function prototypes (LEAVE)
 */
ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos);

static struct file_operations proc_ops = {
    .owner = THIS_MODULE,
    .read = proc_read,
};

/* This function is called when the module is loaded. */
int proc_init(void)
{
       if (strcmp(PROC_NAME, "jiffies") == 0) 
       {
              proc_create("jiffies", 0, NULL, &proc_ops);
              printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
              printk(KERN_INFO "Jiffies is : %lu\n", jiffies);
              printk("HZ is: %d\n", HZ);
       }
       else if (strcmp(PROC_NAME, "seconds") == 0)
       {
              proc_create(PROC_NAME, 0, NULL, &proc_ops);
              start = jiffies;
              printk(KERN_INFO "Jiffies start time: %lu\n", jiffies);
       }
       else
       {
              printk(KERN_INFO "Proc name cannot be found. Retry");
       }
       return 0;
}

/* This function is called when the module is removed. */
void proc_exit(void)
{
       printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
       printk(KERN_INFO "Jiffies end time: %lu\n", jiffies);

       end = jiffies;
       total = end - start;

       printk(KERN_INFO "Total time: %lu seconds \n", total / HZ);
       //removes the /proc/hello entry
       remove_proc_entry(PROC_NAME, NULL);
       printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
       int rv = 0;
       char buffer[BUFFER_SIZE];
       static int completed = 0;

       if (completed)
       {
              completed = 0;
              return 0;
       }

       completed = 1;

       if (strcmp(PROC_NAME, "jiffies") == 0)
       {
              rv = sprintf(buffer, "Jiffies current values is:%lu\n", jiffies);
       }
       else
       {
              rv = sprintf(buffer, "Seconds is calculated after proc is closed and jiffies is: %lu\n", jiffies);
       }
       // Reference
       //rv = sprintf(buffer, "Hello World\n");

       // copies the contents of buffer to userspace usr_buf
       copy_to_user(usr_buf, buffer, rv);

       return rv;
}

/* Macros for registering module entry and exit points. */
module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Display Seconds");
MODULE_AUTHOR("SGG");

