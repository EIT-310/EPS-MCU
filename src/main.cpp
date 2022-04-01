#include "mbed.h"

class ISR {
public:
    ISR(PinName pin, PinName sPin) : _interrupt(pin), swPin(sPin)           // create the InterruptIn on the pin specified to Counter
    
    {
        _interrupt.rise(callback(this, &ISR::handle)); // attach increment function of this counter instance
    }

    void handle()
    {
        swPin=1;
    }
private:
    InterruptIn _interrupt;
    DigitalOut swPin;
};
// struct ISRPIN
// {
//     InterruptIn chnl;
//     DigitalOut pin;

//     ISRPIN(const PinName interruptPin, const PinName GPIOPin) 
//     : chnl(interruptPin), pin(GPIOPin)
//     {}
//     chnl.rise(&doInter);

//     void doInter(){
//         pin=1;
//     }
// }
ISR isr1(A0,A1);
ISR isr2(A2,A3);    

int main()
{
    while (1)
    {
        // hal_sleep();
        ThisThread::sleep_for(5);
    }
}
