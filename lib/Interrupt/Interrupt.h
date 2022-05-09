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

class OceIsr {
 public:

  OceIsr(PinName isr_pin, EventQueue *queue) : interrupt_(isr_pin, PullDown){
    irq_happened_ = false;
    queue_ = queue;
    interrupt_.rise(callback(this, &OceIsr::Handle));

  }

  static EventQueue *queue_;


//  Thread thread_;

  bool read_pin() const;

  void reset_interrupt();
  void Handle();

 private:
//  DigitalOut sw_pin_;
  InterruptIn interrupt_;
  bool irq_happened_;

  void Print();
  void EventHandler();
};

