#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>

#define LED0_NODE DT_ALIAS(led0)

#define LED0    DT_GPIO_LABEL(LED0_NODE, gpios)
#define PIN     DT_GPIO_PIN(LED0_NODE, gpios)
#define FLAGS   DT_GPIO_FLAGS(LED0_NODE, gpios)

void main () {
    struct device const* dev = device_get_binding(LED0);
    if (dev == NULL)
        return;
    if (gpio_pin_configure(dev, PIN, GPIO_OUTPUT_ACTIVE|FLAGS) < 0)
        return;

    while (true) {
        gpio_pin_set(dev, PIN, 1);
        k_msleep(100);
        gpio_pin_set(dev, PIN, 0);
        k_msleep(500);
    }
}
