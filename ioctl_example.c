#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>

#define DEVICE_NAME "ioctl_example"
#define IOCTL_MAGIC 'k'
#define IOCTL_SET_VALUE _IOW(IOCTL_MAGIC, 0, int)
#define IOCTL_GET_VALUE _IOR(IOCTL_MAGIC, 1, int)

static int device_value = 0;

static long device_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    int value;

    switch (cmd) {
        case IOCTL_SET_VALUE:
            if (copy_from_user(&value, (int __user *)arg, sizeof(value)))
                return -EFAULT;
            device_value = value;
            printk(KERN_INFO "Device value set to %d\n", device_value);
            break;
        
        case IOCTL_GET_VALUE:
            if (copy_to_user((int __user *)arg, &device_value, sizeof(device_value)))
                return -EFAULT;
            printk(KERN_INFO "Device value sent to user: %d\n", device_value);
            break;
        
        default:
            return -EINVAL;
    }
    return 0;
}

static int device_open(struct inode *inode, struct file *file) {
    printk(KERN_INFO "Device opened\n");
    return 0;
}

static int device_release(struct inode *inode, struct file *file) {
    printk(KERN_INFO "Device closed\n");
    return 0;
}

static struct file_operations fops = {
    .unlocked_ioctl = device_ioctl,
    .open = device_open,
    .release = device_release,
};

static int major_number;

static int __init ioctl_example_init(void) {
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "Failed to register character device\n");
        return major_number;
    }

    printk(KERN_INFO "Registered device with major number %d\n", major_number);
    return 0;
}

static void __exit ioctl_example_exit(void) {
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "Unregistered device\n");
}

module_init(ioctl_example_init);
module_exit(ioctl_example_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("IOCTL Example Device Driver");
