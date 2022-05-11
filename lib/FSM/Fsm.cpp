//
// Created by stormand on 04/04/2022.
//

#include "Fsm.h"

Fsm::State Fsm::current_state_;

std::string Fsm::GetString(Fsm::State name) {
  switch (name) {

    case EMERGENCY:return "emergency";
      break;
    case LOW:return "low";
      break;
    case MID_LOW:return "mid_low";
      break;
    case MID:return "mid";
      break;
    case MID_HIGH:return "mid_high";
      break;
    case HIGH:return "high";
      break;
    default:return "unkown";
  }
}

std::string Fsm::ToString() {
  return GetString(current_state_);
}

void Fsm::UpState() {
  switch (current_state_) {

    case EMERGENCY:current_state_ = LOW;
      break;
    case LOW:current_state_ = MID_LOW;
      break;
    case MID_LOW:current_state_ = MID;
      break;
    case MID:current_state_ = MID_HIGH;
      break;
    case MID_HIGH:
    case HIGH:current_state_ = HIGH;
      break;
  }

}

void Fsm::DownState() {
  switch (current_state_) {

    case EMERGENCY:
    case LOW:current_state_ = EMERGENCY;
      break;
    case MID_LOW:current_state_ = LOW;
      break;
    case MID:current_state_ = MID_LOW;
      break;
    case MID_HIGH:current_state_ = MID;
      break;
    case HIGH:current_state_ = MID_HIGH;
      break;
  }

}

Fsm::State Fsm::GetCurrentState() {
  return current_state_;
}

void Fsm::DetermineState(AdcRead::adc_reading reading) {
  State target_state;
  float v_bat = ((float) reading.bat_v_1 * 3.3f) / (float) UINT16_MAX;
  float p_in = GetPower(reading);

  if (v_bat > VBAT_MID_HIGH) { //TODO: Add other dependencies
    target_state = HIGH;
  } else if (v_bat > VBAT_MID) {
    target_state = MID_HIGH;
  } else if (v_bat > VBAT_MID_LOW) {
    target_state = MID;
  } else if (v_bat > VBAT_EMERGENCY) {
    target_state = MID_LOW;
  } else if (v_bat > VBAT_LOW) {
    target_state = LOW;
  } else target_state = EMERGENCY;

  if (p_in < CRIT_MPPT_POW_SUM) {
    switch (target_state) {

      case EMERGENCY:
      case LOW:target_state = EMERGENCY;
        break;
      case MID_LOW:target_state = LOW;
        break;
      case MID:target_state = MID_LOW;
        break;
      case MID_HIGH:target_state = MID;
        break;
      case HIGH:target_state = MID_HIGH;
        break;
    }
  }
  StateChanger(target_state);
}

void Fsm::StateChanger(State target_state) {
  if (current_state_ < target_state) {
    UpState();
  } else if (target_state < current_state_) {
    DownState();
  }
}

float Fsm::GetPower(AdcRead::adc_reading reading) {
  uint16_t readings[][2]{
      {reading.mppt_v_1, reading.mppt_i_1},
//        TODO Indsæt de andre MPPT'er her
  };

  float volt = 0;
  float curr = 0;

//Convert to Volt and Ampere
  for (auto &&mppt : readings) {
    volt += ((float) mppt[0] * 3.3f) / (float) UINT16_MAX;
    curr += (((float) mppt[1] * 3.3f) / (float) UINT16_MAX)
        / (MPPT_CURR_AMP * MPPT_SHUNT_RES);
  }

  float power = volt * curr;

  return power;
}
