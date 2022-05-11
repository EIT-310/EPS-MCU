//
// Created by stormand on 01/04/2022.
//

#include "AdcRead.h"
#include "../Config/ProjectConfig.h"
#include <mbed.h>
#include <rtos.h>
#include "Fsm.h"

CircularBuffer<AdcRead::adc_reading, ADC_BUFFER_SIZE> AdcRead::adc_buffer_;
uint16_t adc_read_list[ADC_NUM_CHANNELS];

Thread *t_adc_read[ADC_NUM_CHANNELS];
AdcRead::adc_thread_args arg_list[ADC_NUM_CHANNELS];
//Mutex adc_mutex;


AdcRead::adc_reading AdcRead::DoRead() {
  Fsm::State current_state = Fsm::GetCurrentState();
  adc_reading new_read{};

  if (current_state > Fsm::LOW) {
    int adc_count = ADC_NUM_CHANNELS;
    AsyncADC(adc_count);
    new_read = {
        adc_read_list[0],
        adc_read_list[1],
        adc_read_list[2],
        adc_read_list[3],
        adc_read_list[4],
        adc_read_list[5],
        time(NULL)
    };
  } else {
    int adc_count = 2;
    AsyncADC(adc_count);
    new_read = {
        adc_read_list[0],
        adc_read_list[1],
        0,
        0,
        0,
        0,
        time(NULL)
    };
  }

  adc_buffer_.push(new_read);
  return new_read;
}
void AdcRead::AsyncADC(int adc_count) {
  for (int i = 0; i < adc_count; ++i) {
    arg_list[i] = {
        GetDevice((AdcName) i),
        &adc_read_list[i]
    };
    t_adc_read[i] = new Thread();
    t_adc_read[i]->start(callback(ReadU16, &arg_list[i]));
  }

  for (int i = 0; i < adc_count; ++i) {
    t_adc_read[i]->join();
  }

  for (int i = 0; i < adc_count; ++i) {
    delete t_adc_read[i];
  }
}

AnalogIn *AdcRead::GetDevice(AdcRead::AdcName name) {

  AnalogIn *device_ptr;

  switch (name) {
    case BAT_V_1:device_ptr = new AnalogIn(BAT_V_1_PIN);
      break;

    case BAT_I_1:device_ptr = new AnalogIn(BAT_I_1_PIN);
      break;

    case MPPT_V_1:device_ptr = new AnalogIn(MPPT_V_1_PIN);
      break;

    case MPPT_I_1:device_ptr = new AnalogIn(MPPT_I_1_PIN);
      break;

    case SUB_V_1:device_ptr = new AnalogIn(SUB_V_1_PIN);
      break;

    case SUB_I_1:device_ptr = new AnalogIn(SUB_I_1_PIN);
      break;
  }

  return device_ptr;
}

void AdcRead::ReadU16(adc_thread_args *args) {
  float sum = 0;
  for (int i = 0; i < ADC_MEASURE_LOOPS; i++) {
//    adc_mutex.lock();
    sum += (float) args->device->read_u16() / (float) ADC_MEASURE_LOOPS;
//    adc_mutex.unlock();
  }

  delete args->device;

  if (sum == 0) {
    sum = 1;
  }

  *args->dest = (uint16_t) sum;
}
