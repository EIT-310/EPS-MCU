#include "mbed.h"
#include "rtos.h"

#define MEAS_COUNT  5
#define CHANNELS    8

UnbufferedSerial pc(USBTX, USBRX, 2000000);

int counter = CHANNELS;

class Device {
public:
    Semaphore *my_mutex;
    uint16_t voltage{};

    Device(PinName pin, const std::string &name) :
            my_mutex(new Semaphore(1)),
            _thread(new Thread(osPriorityNormal, OS_STACK_SIZE, nullptr, name.c_str())),
            _driver(new AnalogIn(pin)) {
        _thread->start(callback(this, &Device::_start));
    }


private:
    Thread *_thread;
    AnalogIn *_driver;

    void _start(){
        while (true)
        {
            voltage = _adc_read();
        }
        
    }

    uint16_t _adc_read() {
        // while (true) {
            my_mutex->acquire();
            
            float sum = 0;
            for (int i = 0; i < MEAS_COUNT; ++i) {
                sum += _driver->read_u16() / (float) MEAS_COUNT;
            }
            counter--;

            return (uint16_t) sum;
        // }
    }
};


int main() {

    Device
        adc0(A0, "ADC0"),
        adc1(A1, "ADC1"),
        adc2(A2, "ADC2"),
        adc3(A3, "ADC3"),
        adc4(A4, "ADC4"),
        adc5(A5, "ADC5"),
        adc6(A6, "ADC6"),
        adc7(D3, "ADC7");

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (1) {

        while (counter) {
            ThisThread::sleep_for(5);
        }

        std::string str;

        str.append(to_string(adc0.voltage) + "\t");
        str.append(to_string(adc1.voltage) + "\t");
        str.append(to_string(adc2.voltage) + "\t");
        str.append(to_string(adc3.voltage) + "\t");
        str.append(to_string(adc4.voltage) + "\t");
        str.append(to_string(adc5.voltage) + "\t");
        str.append(to_string(adc6.voltage) + "\t");
        str.append(to_string(adc7.voltage) + "\t");

        str.append("\n");

        pc.write(str.c_str(),str.size());

        counter = CHANNELS;

        adc0.my_mutex->release();
        adc1.my_mutex->release();
        adc2.my_mutex->release();
        adc3.my_mutex->release();
        adc4.my_mutex->release();
        adc5.my_mutex->release();
        adc6.my_mutex->release();
        adc7.my_mutex->release();

        ThisThread::sleep_for(1);
    };
#pragma clang diagnostic pop
}