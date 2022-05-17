//
// Created by EIT-416 on 01/04/2022.
//

#ifndef EPS_MCU_INCLUDE_ADC_READ_ADC_READ_H_
#define EPS_MCU_INCLUDE_ADC_READ_ADC_READ_H_

#include "ProjectConfig.h"
#include "mbed.h"

class AdcRead {
 public:
  AdcRead() = default;

  struct adc_reading {
    uint16_t bat_v_1;
    uint16_t bat_i_1;
    uint16_t mppt_v_1;
    uint16_t mppt_i_1;
    uint16_t sub_v_1;
    uint16_t sub_i_1;
    time_t timestamp;
  };

  enum AdcName {
    BAT_V_1,
    BAT_I_1,
    MPPT_V_1,
    MPPT_I_1,
    SUB_V_1,
    SUB_I_1
  };

  struct adc_thread_args {
    AnalogIn *device;
    uint16_t *dest;
  };

//  TODO: make asynchronous
  static AdcRead::adc_reading DoRead();
  static CircularBuffer<adc_reading, ADC_BUFFER_SIZE> adc_buffer_;
  static void ReadU16(adc_thread_args *args);

 private:

  static AnalogIn *GetDevice(AdcName name);

  static float ReadFloat(AnalogIn *device);
  static AdcName adc_list_[6];
  static void AsyncADC(int adc_count);
};

#endif //EPS_MCU_INCLUDE_ADC_READ_ADC_READ_H_
