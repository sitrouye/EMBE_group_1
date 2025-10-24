// bassam
#include <linux/module.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/interrupt.h>
#include <linux/gpio/consumer.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/spinlock.h>

#define DRV_NAME "encoder-irq"

static struct gpio_desc *enc_a, *enc_b;
static unsigned int irq_a, irq_b;
static atomic64_t pulse_count;
static u8 last_state;
static spinlock_t state_lock; // protects last_state if needed


static const s8 qlut[16] = {
    0, -1, +1,  0,
   +1,  0,  0, -1,
   -1,  0,  0, +1,
    0, +1, -1,  0
};

static irqreturn_t enc_isr(int irq, void *dev_id)
{
    
    int a = gpiod_get_value_cansleep(enc_a) ? 1 : 0;
    int b = gpiod_get_value_cansleep(enc_b) ? 1 : 0;
    u8 now = (a << 1) | b;

   
    u8 prev = last_state;
    s8 delta = qlut[(prev << 2) | now];
    if (delta)
        atomic64_add(delta, &pulse_count);
    last_state = now;

    return IRQ_HANDLED;
}


static ssize_t enc_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
    u64 v = (u64)atomic64_read(&pulse_count);
    if (len < sizeof(v)) return -EINVAL;
    if (copy_to_user(buf, &v, sizeof(v))) return -EFAULT;
    return sizeof(v);
}
static ssize_t enc_write(struct file *f, const char __user *ubuf, size_t len, loff_t *off)
{
    char kbuf[16] = {0};
    if (len > sizeof(kbuf)-1) len = sizeof(kbuf)-1;
    if (copy_from_user(kbuf, ubuf, len)) return -EFAULT;

    if (!strncmp(kbuf, "0", 1) || !strncasecmp(kbuf, "reset", 5)) {
        atomic64_set(&pulse_count, 0);
        return (ssize_t)len;
    }
    return -EINVAL;
}
static const struct file_operations enc_fops = {
    .owner = THIS_MODULE,
    .read  = enc_read,
    .write = enc_write,
};
static struct miscdevice enc_misc = {
    .minor = MISC_DYNAMIC_MINOR,
    .name  = "encoder0",
    .fops  = &enc_fops,
    .mode  = 0666,
};


static int enc_probe(struct platform_device *pdev)
{
    int ret;

    atomic64_set(&pulse_count, 0);
    spin_lock_init(&state_lock);

    enc_a = devm_gpiod_get(&pdev->dev, "a", GPIOD_IN);
    if (IS_ERR(enc_a)) {
        dev_err(&pdev->dev, "get A failed\n");
        return PTR_ERR(enc_a);
    }
    enc_b = devm_gpiod_get(&pdev->dev, "b", GPIOD_IN);
    if (IS_ERR(enc_b)) {
        dev_err(&pdev->dev, "get B failed\n");
        return PTR_ERR(enc_b);
    }

    
    {
        int a = gpiod_get_value_cansleep(enc_a) ? 1 : 0;
        int b = gpiod_get_value_cansleep(enc_b) ? 1 : 0;
        last_state = (a << 1) | b;
    }

    irq_a = gpiod_to_irq(enc_a);
    if ((int)irq_a < 0) return irq_a;
    irq_b = gpiod_to_irq(enc_b);
    if ((int)irq_b < 0) return irq_b;


    ret = devm_request_irq(&pdev->dev, irq_a, enc_isr,
                           IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
                           DRV_NAME "-A", NULL);
    if (ret) return ret;

    ret = devm_request_irq(&pdev->dev, irq_b, enc_isr,
                           IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
                           DRV_NAME "-B", NULL);
    if (ret) return ret;

    ret = misc_register(&enc_misc);
    if (ret) return ret;

    dev_info(&pdev->dev, "encoder ready (4x): /dev/encoder0, irqA=%u irqB=%u\n", irq_a, irq_b);
    return 0;
}
static void enc_remove(struct platform_device *pdev)
{
    misc_deregister(&enc_misc);
}

static const struct of_device_id enc_dt_ids[] = {
    { .compatible = "custom,encoder-irq" },
    {}
};
MODULE_DEVICE_TABLE(of, enc_dt_ids);

static struct platform_driver enc_driver = {
    .probe  = enc_probe,
    .remove = enc_remove,
    .driver = {
        .name = DRV_NAME,
        .of_match_table = enc_dt_ids,
    },
};
module_platform_driver(enc_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Bassam");
MODULE_DESCRIPTION("IRQ (A=GPIO17, B=GPIO27) quadrature encoder driver with /dev interface");
