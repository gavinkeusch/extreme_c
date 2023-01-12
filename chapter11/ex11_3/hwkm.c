#include <linux/version.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gavin");
MODULE_DESCRIPTION("HWKM");

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,6,0)
#define HAVE_PROC_OPS
#endif

// The structure pointing to the proc file
struct proc_dir_entry *proc_file;

// The read callback function
ssize_t proc_file_read(struct file *file, char __user *ubuf, size_t count, loff_t *ppos) {
    int copied = 0;

    if (*ppos > 0) {
        return 0;
    }

    copied = sprintf(ubuf, "Hello World From Kernel Module!\n");
    *ppos = copied;

    return copied;
}

#ifdef HAVE_PROC_OPS
static const struct proc_ops proc_file_fops = {
    .proc_read = proc_file_read
};
#else
static const struct file_operations proc_file_fops = {
    .owner = THIS_MODULE,
    .read = proc_file_read
};
#endif

// The module initialization callback
static int __init hwkm_init(void) {
    proc_file = proc_create("hwkm", 0, NULL, &proc_file_fops);

    if (!proc_file) {
        return -ENOMEM;
    }

    printk("Hello World module is loaded.\n");

    return 0;
}

// The module exit callback
static void __exit hwkm_exit(void) {
    proc_remove(proc_file);
    printk("Goodbye World!\n");
}

// Defining module callbacks
module_init(hwkm_init);
module_exit(hwkm_exit);

