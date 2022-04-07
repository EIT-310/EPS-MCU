//
// Created by stormand on 01/04/2022.
//

#ifndef EPS_MCU_LIB_PROJECT_CONFIG_H_
#define EPS_MCU_LIB_PROJECT_CONFIG_H_

#define ADC_MEASURE_LOOPS   5
#define SUBMODULES_NUM      5
#define VBAT_MID_HIGH       4.0f //TODO: Ret så det passer med hvad Vref på ADC er
#define VBAT_MID            3.7f //TODO: Ret så det passer med hvad Vref på ADC er
#define VBAT_MID_LOW        3.4f //TODO: Ret så det passer med hvad Vref på ADC er
#define VBAT_EMERGENCY      3.0f //TODO: Ret så det passer med hvad Vref på ADC er
#define WATCHDOG_TIMEOUT    10000  //TODO: Evt ret så det giver mening

#endif //EPS_MCU_LIB_PROJECT_CONFIG_H_
