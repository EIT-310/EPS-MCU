#include "mbed.h"

// class ISR {
// public:
//     ISR(PinName pin) : _interrupt(pin)          // create the InterruptIn on the pin specified to Counter
//     {
//         _interrupt.rise(callback(this, &ISR::handle)); // attach increment function of this counter instance
//     }

//     void handle()
//     {

//     }
// private:
//     InterruptIn _interrupt;
//     DigitalOut
// };
struct ISRPIN
{
    InterruptIn chnl;
    DigitalOut pin;

    ISRPIN(const PinName interruptPin, const PinName GPIOPin) 
    : chnl(interruptPin), pin(GPIOPin)
    {}
    void doInter(){
        pin=1;
    }
}
isr1(A0,A1);
    

int main()
{
    while (1)
    {
        isr1.chnl.rise(isr1.doInter);
    
            printf("Nothing so far %\n");
        // hal_sleep();
    }
}
