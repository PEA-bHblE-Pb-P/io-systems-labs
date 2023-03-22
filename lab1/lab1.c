#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/string.h>
#include <linux/proc_fs.h>

static dev_t first;
static struct cdev c_dev; 
static struct class *cl;

static struct proc_dir_entry *entry;

static char sum_as_str[1024*16];
static size_t end_char;

static ssize_t read_proc(struct file *f, char __user *buf, size_t len, loff_t *off) {
    size_t size = end_char + 1;

    if (*off > 0 || len < size) {
        return 0;
    }

    if (copy_to_user(buf, sum_as_str, size) != 0) {
        return -EFAULT;
    }

    *off = len;
    return size;
}

static ssize_t read(struct file *f, char __user *buf, size_t len, loff_t *off) {
    printk(KERN_INFO "%s", sum_as_str);
    return 0;
}

static ssize_t write(struct file *f, const char __user *buf,  size_t len, loff_t *off) {
    char str[len];
    if (copy_from_user(str, buf, len) != 0) {
        return -EFAULT;
    }

    long sum = 0;
    size_t is_neg = false;
    long cur_sum = 0;
    size_t i = 0;
    for (i; i < len; i++) {
        if (str[i] == '-') {
            is_neg = true;
        } else if ('0' <= str[i] && str[i] <= '9') {
            cur_sum *= 10;
            cur_sum += str[i] - '0';
        } else {
            if (is_neg) {
                cur_sum *= -1;
            }
            sum += cur_sum;
            cur_sum = 0;
            is_neg = false;
        }
    }

    if (is_neg) {
        cur_sum *= -1;
    }
    sum += cur_sum;

    end_char += sprintf(sum_as_str + end_char, "%d\n", sum);    
    sum_as_str[end_char] = '\0';

    return len;
}

static int my_dev_uevent(struct device *dev, struct kobj_uevent_env *env) {
    add_uevent_var(env, "DEVMODE=%#o", 0666);
    return 0;
}

static struct file_operations mychdev_fops =
{
  .owner      = THIS_MODULE,
  .read       = read,
  .write      = write
};

static const struct proc_ops proc_fops = { 
    .proc_read = read_proc
};

static int __init ch_drv_init(void) {
    end_char = 0;
    sum_as_str[end_char] = '\0';

    printk(KERN_INFO "Driver inited\n");

    //create file in dev
    if (alloc_chrdev_region(&first, 0, 1, "ch_dev") < 0) {
        return -1;
    }

    if ((cl = class_create(THIS_MODULE, "chardrv")) == NULL) {
        unregister_chrdev_region(first, 1);
        return -1;
    }

    cl->dev_uevent = my_dev_uevent;

    if (device_create(cl, NULL, first, NULL, "var3") == NULL) {
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return -1;
    }
    
    cdev_init(&c_dev, &mychdev_fops);
    if (cdev_add(&c_dev, first, 1) == -1) {
        device_destroy(cl, first);
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return -1;
    }

    //create file in proc
    entry = proc_create("var3", 0666, NULL, &proc_fops);
    if (entry == NULL) {
        device_destroy(cl, first);
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        cdev_del(&c_dev);
        return -1;
    }

    return 0;
}
 
static void __exit ch_drv_exit(void)
{
    cdev_del(&c_dev);
    device_destroy(cl, first);
    class_destroy(cl);
    unregister_chrdev_region(first, 1);
    proc_remove(entry);
    printk(KERN_INFO "Drvier exited\n");
}
 
module_init(ch_drv_init);
module_exit(ch_drv_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("PEA-bHblE-Pb-P");
MODULE_DESCRIPTION("Character device driver");

