//
// Created by EIT-416 on 12/04/2022.
//

#include "Interrupt.h"
#include "mbed.h"
#include "ProjectConfig.h"
#include "Log.h"

EventQueue *OceIsr::queue_;

/**
 * @brief ISR context der deactiverer modulet der blev målt OCE på.
 *        Sætter EventHandler() i kø til de ting der skal foregå 
 *        uden for ISR kontekst.
 * 
 */
void OceIsr::Handle() {
  PowerManage::OceFromIsr(module_);
  queue_->call(this, &OceIsr::EventHandler);
}

/**
 * @brief ISR deferred handler der logger OCE'et og gemmer hvilket ben der
 *        blev målt oce på.
 * 
 */

void OceIsr::EventHandler(){
  LOG(LOG_WARNING, "OCE on module: " + PowerManage::ToString(module_));
  LOG(LOG_INFO, "Setting override on " + PowerManage::ToString(module_));
  PowerManage::SetOverride(module_, false);

//  LOG(LOG_DEBUG, "Updating enabled modules");
//  PowerManage::UpdateEnabled();
};

