#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/interrupt.h> /* IRQ */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Elvire");
MODULE_DESCRIPTION("ch  nge state of output pin on interrupt");


#define GPIO_IN 516
#define GPIO_OUT 529

static unsigned int irq_number;
static int pin_value = 0;
static irqreturn_t gpio_irq_handler(int irq, void *dev_id) {
	pin_value = !(pin_value);
    gpio_set_value(GPIO_OUT, pin_value);

	return IRQ_HANDLED;
}

static int __init drv_init(void) {
	/* Init GPIO */
	if(gpio_request(GPIO_IN, "rpi-gpio-27")) {
		printk("dummy_gpio_irq: can't allocate GPIO 4\n");
		return -1;
	}

	/* Set GPIO direction */
	if(gpio_direction_input(GPIO_IN)) {
		printk("dummy_gpio_irq: can't set GPIO 4 to input\n");
		gpio_free(GPIO_IN);
		return -1;
	}

	if(gpio_request(GPIO_OUT, "rpi-gpio-17")) {
		printk("dummy_gpio_irq: can't allocate GPIO 27\n");
		return -1;
	}

	/* Set GPIO direction */
	if(gpio_direction_output(GPIO_OUT,1)) {
		printk("dummy_gpio_irq: can't set GPIO 17 to output\n");
		gpio_free(GPIO_OUT);
		return -1;
	}
	/* Set IRQ */
	irq_number = gpio_to_irq(GPIO_IN);
	if(request_irq(irq_number, gpio_irq_handler, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, "dummy_gpio_irq", NULL) != 0) {
		printk("dummy_gpio_irq: can't request interrupt %d.\n", irq_number);
		gpio_free(GPIO_IN);
		return -1;
	}
	printk("dummy_gpio_irq: GPIO button is mapped to IRQ %d.\n", irq_number);

	printk("dummy_gpio_irq: module is initialized into the kernel.\n");
	return 0;
}

static void __exit drv_exit(void) {
	printk("dummy_gpio_irq: module is removed from the kernel.\n");
    gpio_free(GPIO_OUT);
    gpio_free(GPIO_IN);

}

module_init(drv_init);
module_exit(drv_exit);