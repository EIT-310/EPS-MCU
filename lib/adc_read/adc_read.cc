//
// Created by stormand on 01/04/2022.
//

#include "adc_read.h"
#include "../../include/project_config.h"
#include <mbed.h>
#include <rtos.h>


float AdcRead::read(AdcRead::ADC_NAME name) {
    AnalogIn *device = get_device_(name);
    float sum = read_float_(device);
    return sum;
}

uint16_t AdcRead::read_int(AdcRead::ADC_NAME name) {
    AnalogIn *device = get_device_(name);
    uint16_t sum = read_u16_(device);
    return sum;
}


AnalogIn *AdcRead::get_device_(AdcRead::ADC_NAME name) {

    AnalogIn *device_ptr;

    switch (name) {
        case 0:
            device_ptr = new AnalogIn(A0);
            break;

        case 1:
            device_ptr = new AnalogIn(A1);
            break;

        case 2:
            device_ptr = new AnalogIn(A2);
            break;

        case 3:
            device_ptr = new AnalogIn(A3);
            break;

        case 4:
            device_ptr = new AnalogIn(A4);
            break;

        case 5:
            device_ptr = new AnalogIn(A5);
            break;

        case 6:
            device_ptr = new AnalogIn(A6);
            break;

        case 7:
            device_ptr = new AnalogIn(D3);
            break;

    }

    return device_ptr;

}

float AdcRead::read_float_(AnalogIn *_device) {

    float sum = 0;
    for (int i = 0; i < ADC_MEASURE_LOOPS; ++i) {
        sum += _device->read() / (float) ADC_MEASURE_LOOPS;
    }
    return sum;
}

uint16_t AdcRead::read_u16_(AnalogIn *_device) {

    float sum = 0;
    for (int i = 0; i < ADC_MEASURE_LOOPS; ++i) {
        sum += (float) _device->read_u16() / (float) ADC_MEASURE_LOOPS;
    }

    return (uint16_t) sum;
}

