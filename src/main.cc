#include "mbed.h"
#include "rtos.h"
#include "adc_read.h"
UnbufferedSerial serial(USBTX, USBRX, 2e6);
AdcRead adc;


int main(){

    uint16_t volt;

    while (true) {
        std::string buf = "Voltage: ";
        volt = adc.read_int(adc.one);
//        volt = 5;
        buf.append(to_string(volt) + "\n");
        serial.write(buf.c_str(), buf.size());
//        printf("%s", buf.c_str());
        ThisThread::sleep_for(1s);
    }
}
