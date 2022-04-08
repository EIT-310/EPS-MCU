#include "mbed.h"

int main()
{
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (1)
    {
        printf("Going to deeep sleep");
        lp_ticker_free();
        hal_deepsleep();
        lp_ticker_init();
        printf("Waking up from deeep sleeep");

    }
#pragma clang diagnostic pop
}
