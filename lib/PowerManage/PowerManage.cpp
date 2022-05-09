//
// Created by stormand on 5/2/22.
//

#include "Fsm.h"
#include "PowerManage.h"

bool PowerManage::enabled_modules_[SUBMODULES_NUM];

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
}
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
}
