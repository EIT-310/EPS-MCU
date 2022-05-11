//
// Created by stormand on 12/04/2022.
//
#include <NoMutexCAN.h>
#include "mbed.h"
#include "reading.pb.h"
#include "pb_decode.h"
#include "rtos.h"
#ifndef EPS_MCU_INTERRUPT_H
#define EPS_MCU_INTERRUPT_H

#endif //EPS_MCU_INTERRUPT_H
#include "PowerManage.h"

class OceIsr {
 public:

  OceIsr(PinName irq_pin, EventQueue *queue, PowerManage::Modules module) : interrupt_(irq_pin, PullDown){
    module_ = module;
    irq_happened_ = false;
    queue_ = queue;
    interrupt_.rise(callback(this, &OceIsr::Handle));

  }

  static EventQueue *queue_;


  void Handle();

 private:
  PowerManage::Modules module_;
  InterruptIn interrupt_;
  bool irq_happened_;

  void EventHandler();
};

