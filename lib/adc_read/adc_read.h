//
// Created by stormand on 01/04/2022.
//

#ifndef EPS_MCU_INCLUDE_ADC_READ_ADC_READ_H_
#define EPS_MCU_INCLUDE_ADC_READ_ADC_READ_H_

#include "mbed.h"

class AdcRead {
public:
//  TODO: give proper names
    enum ADC_NAME {
        one,
        two,
        three,
        four,
        five,
        six,
        seven,
        eight
    };

//  TODO: make asynchronous
    float read(ADC_NAME name);

    uint16_t read_int(ADC_NAME name);

private:
    static AnalogIn *get_device_(ADC_NAME name);

    static float read_float_(AnalogIn *_device);

    static uint16_t read_u16_(AnalogIn *_device);
};

#endif //EPS_MCU_INCLUDE_ADC_READ_ADC_READ_H_
