#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

void delayLoop (int n) {
    for (int i = 0; i < n * 1000; ++i)
        asm ("");
}

int main () {
    rcc_periph_clock_enable(RCC_GPIOB);
    gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO3);

    while (true) {
        gpio_set(GPIOB, GPIO3);
        delayLoop(500);
        gpio_clear(GPIOB, GPIO3);
        delayLoop(500);
    }
}
