//
// Created by EIT-416 on 04/04/2022.
//

#ifndef EPS_MCU_FSM_H
#define EPS_MCU_FSM_H

#include <string>
#include "ProjectConfig.h"
#include "AdcRead.h"
//#include "PowerManage.h"

class Fsm {
 public:
  enum State {
    EMERGENCY,
    LOW,
    MID_LOW,
    MID,
    MID_HIGH,
    HIGH
  };

  explicit Fsm(State start_state){
    current_state_ = start_state;
  }


  static void DetermineState(AdcRead::adc_reading reading);

  static void UpState();

  static void DownState();

  static State GetCurrentState();

  static std::string ToString();

 private:

  static std::string GetString(State name);

  static State current_state_;

  static void StateChanger(State target_state);

  static float GetPower(AdcRead::adc_reading reading);
};

#endif //EPS_MCU_FSM_H
