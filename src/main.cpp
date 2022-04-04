#include "mbed.h"
#include "platform.h"

int main()
{
    while (1)
    {
        printf("Jeg Lever\n");
        ThisThread::sleep_for(500);
        sleep_manager_unlock_deep_sleep();
        

    }
}
