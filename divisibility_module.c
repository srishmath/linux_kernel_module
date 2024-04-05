#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/delay.h>

static int num1 = 9;
static int num2 = 3;
static int result;

static DECLARE_COMPLETION(child_completion);

static struct task_struct *child_thread;

// Function executed by the child process
static int child_function(void *data) {
    printk(KERN_INFO "Child process running\n");
    result = num1 % num2;
    if(result==0)
    {
      printk(KERN_INFO "Child process result: divisible");
    }
    else
    {
      printk(KERN_INFO "Child process result:not divisible");
    }
    
    printk(KERN_INFO "Signaling child completion\n");
    complete(&child_completion); // Signal completion
    return 0;
}

static int __init divide_numbers_init(void) {
    printk(KERN_INFO "Parent process running\n");

    // Create a child process
    child_thread = kthread_run(child_function, NULL, "child_thread");

    if (IS_ERR(child_thread)) {
        printk(KERN_ERR "Failed to create child process\n");
        return PTR_ERR(child_thread);
    }

    // Wait for the child process to complete
    printk(KERN_INFO "Waiting for child process to complete\n");
    wait_for_completion(&child_completion);

    printk(KERN_INFO "Child process completed\n");

    return 0;
}

static void __exit divide_numbers_exit(void) {
    printk(KERN_INFO "Exiting add_numbers module\n");
}

module_init(divide_numbers_init);
module_exit(divide_numbers_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Srishti Mathur");

MODULE_VERSION("0.1");

