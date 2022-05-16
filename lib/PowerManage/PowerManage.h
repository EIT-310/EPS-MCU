//
// Created by stormand on 5/2/22.
//

#ifndef EPS_MCU_LIB_POWERMANAGE_POWERMANAGE_H_
#define EPS_MCU_LIB_POWERMANAGE_POWERMANAGE_H_

#include <string>
#include <Fsm.h>
#include "ProjectConfig.h"
#include "AdcRead.h"

class PowerManage {
 public:
  enum Modules {
    SUB_1,
//    SUB_2,
//    SUB_3,
//    SUB_4,
//    SUB_5
  };


  static string ToString(Modules module);
  static void OceFromIsr(Modules module);
  static void SetOverride(Modules module, bool is_on);
  static void UpdateEnabled();
  static void WriteSubState();

 private:
  static bool enabled_modules_[SUBMODULES_NUM];
  static bool module_override_[SUBMODULES_NUM]; //TODO: skal gemmes i NVM

  static DigitalOut power_rails_[1];
};

#endif //EPS_MCU_LIB_POWERMANAGE_POWERMANAGE_H_
