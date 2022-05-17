//
// Created by EIT-416 on 5/2/22.
//

#include "Fsm.h"
#include "PowerManage.h"

bool PowerManage::enabled_modules_[SUBMODULES_NUM] = {true};
bool PowerManage::module_override_[SUBMODULES_NUM] = {true};
DigitalOut PowerManage::power_rails_[] = {
    RAIL_1_PIN
};

/**
 * @brief Henter den nuværende state fra FSM biblioteket og
 *        opdaterer enabled_modules_ listen efter denne. Kalder
 *        til sidst WriteSubState().
 */

void PowerManage::UpdateEnabled() {

  Fsm::State current_state = Fsm::GetCurrentState();

  switch (current_state) {

    case Fsm::EMERGENCY:enabled_modules_[0] = false;
//            enabled_modules_[1] = false;
//            enabled_modules_[2] = false;
//            enabled_modules_[3] = false;
//            enabled_modules_[4] = false;
      break;
    case Fsm::LOW:enabled_modules_[0] = true;
//            enabled_modules_[1] = false;
//            enabled_modules_[2] = false;
//            enabled_modules_[3] = false;
//            enabled_modules_[4] = false;
      break;
    case Fsm::MID_LOW:enabled_modules_[0] = true;
//            enabled_modules_[1] = true;
//            enabled_modules_[2] = false;
//            enabled_modules_[3] = false;
//            enabled_modules_[4] = false;
      break;
    case Fsm::MID:enabled_modules_[0] = true;
//            enabled_modules_[1] = true;
//            enabled_modules_[2] = true;
//            enabled_modules_[3] = false;
//            enabled_modules_[4] = false;
      break;
    case Fsm::MID_HIGH:enabled_modules_[0] = true;
//            enabled_modules_[1] = true;
//            enabled_modules_[2] = true;
//            enabled_modules_[3] = true;
//            enabled_modules_[4] = false;
      break;
    case Fsm::HIGH:enabled_modules_[0] = true;
//            enabled_modules_[1] = true;
//            enabled_modules_[2] = true;
//            enabled_modules_[3] = true;
//            enabled_modules_[4] = true;

      break;
  }
  WriteSubState();
}

/**
 * @brief Opdaterer et modul i module_override_ listen. Og Kalder WriteSubState().
 * 
 * @param module Det modul der skal aktiveres/deaktiveres
 * @param is_on  false = deaktiver | true = aktiver
 */

void PowerManage::SetOverride(PowerManage::Modules module, bool is_on) {
  switch (module) {
    case SUB_1:module_override_[0] = is_on;
      break;
//    case SUB_2:
//            module_override_[1] = is_on;
//      break;
//    case SUB_3:
//            module_override_[2] = is_on;
//      break;
//    case SUB_4:
//            module_override_[3] = is_on;
//      break;
//    case SUB_5:
//            module_override_[4] = is_on;
      break;
  }
  WriteSubState();
}

/**
 * @brief Omdanner modul til string med navnet for modulet
 * 
 * @param module Det modul der ønskes navn på
 * @return string med navnet for modulet
 */
string PowerManage::ToString(PowerManage::Modules module) {
  switch (module) {
    case SUB_1:return "Submodule 1";
    default: return "Unknown";
  }
}

/**
 * @brief Opdaterer den endelige tisltand af Power Rails ved at AND'e
 *        enabled_modules_ og module_override_
 * 
 */

void PowerManage::WriteSubState() {
  for (int i = 0; i < SUBMODULES_NUM; i++) {
    power_rails_[i].write(enabled_modules_[i] and module_override_[i]);
  }
}

/**
 * @brief Høj prioritetet deaktivering af en power rail ved OCE
 * 
 * @param module Modulet der blev målt OCE på
 */

void PowerManage::OceFromIsr(PowerManage::Modules module) {
  power_rails_[module].write(false);
}
