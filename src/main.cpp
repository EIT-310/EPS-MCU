#include "mbed.h"

// class ISR {
// public:
//     ISR(PinName pin, PinName sPin) : _interrupt(pin), swPin(sPin)           // create the InterruptIn on the pin specified to Counter
    
//     {
//         _interrupt.rise(callback(this, &ISR::handle)); // attach increment function of this counter instance
//     }

//     void handle()
//     {
//         swPin=1;
//     }
// private:
//     InterruptIn _interrupt;
//     DigitalOut swPin;
// };

// ISR isr1(A0,A1);
// ISR isr2(A2,A3);    

int main()
{
    while (1)
    {
        printf("Jeg Lever");
        ThisThread::sleep_for(5);
        hal_sleep();
    }
}
