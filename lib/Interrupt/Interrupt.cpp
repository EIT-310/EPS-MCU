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
//  LOG(LOG_INFO, "Event Handler Context: ");
  printf("\n\nHelløj\n\n");
//  printf("Handle in context %p\n", ThisThread::get_id());
};

void OceIsr::Print(){
    this->irq_happened_ = true;
    // Do something
};

bool OceIsr::read_pin() const {
  return irq_happened_;
  Thread thread;
}

void OceIsr::reset_interrupt() {
  irq_happened_ = false;

}
