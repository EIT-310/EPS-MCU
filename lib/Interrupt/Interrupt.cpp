//
// Created by stormand on 12/04/2022.
//

#include "Interrupt.h"
#include "mbed.h"
#include "ProjectConfig.h"
#include "Log.h"

//volatile bool CanIsr::can_rx_isr_;
//time_t CanIsr::current_time_;

EventQueue *OceIsr::queue_;

void OceIsr::Handle() {
//  if (!irq_happened_) {
//    irq_happened_ = true;
    queue_->call(this, &OceIsr::EventHandler);
//  }
}

void OceIsr::EventHandler(){
  LOG(LOG_WARNING, "OCE on module: " + PowerManage::ToString(module_));
  LOG(LOG_INFO, "Setting override on " + PowerManage::ToString(module_));
  PowerManage::SetOverride(module_, false);
};

