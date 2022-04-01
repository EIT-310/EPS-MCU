//
// Created by stormand on 01/04/2022.
//

#include "adc_read.h"
#include "project_config.h"
#include <mbed.h>
#include <rtos.h>


AnalogIn *_get_device(AdcRead::ADC_NAME name){

  AnalogIn *device_ptr; 

  switch (name)
  {
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
  
  default:
    throw new exception();
    return;
    break;
  }

  return device_ptr;

}

float _read_float(AnalogIn *_device){

  float sum = 0;
  for (int i = 0; i < ADC_MEASURE_LOOPS; ++i) {
    sum += _device->read() / (float) ADC_MEASURE_LOOPS;
  }

  return sum;
};
