//
// Created by stormand on 01/04/2022.
//

#ifndef EPS_MCU_INCLUDE_ADC_READ_ADC_READ_H_
#define EPS_MCU_INCLUDE_ADC_READ_ADC_READ_H_

class AdcRead {
 public:
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
 private:
  float _get_float(ADC_NAME name);
};

#endif //EPS_MCU_INCLUDE_ADC_READ_ADC_READ_H_
