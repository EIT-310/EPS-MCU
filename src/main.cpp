#include "mbed.h"
#include "Fsm.h"
#include "NVStore.h"
#include "AdcRead.h"
#include "mbed_stats.h"
#include <ProjectConfig.h>
#include "Interrupt.h"
#include "PowerManage.h"
#include "Log.h"
#include "CanCom.h"
#include "reading.pb.h"
#include "NoMutexCAN.h"
#include "rtos.h"

Fsm fsm(FSM_START_STATE);
EventQueue IsrQueue,
    AdcQueue;
NoMutexCAN can(CAN_RD_PIN, CAN_TD_PIN, CAN_BITRATE);
Watchdog &watchdog = Watchdog::get_instance();
CANMessage can_time_sync, can_req;
Thread t_can_handler(osPriorityNormal,OS_STACK_SIZE, nullptr,"CAN Handler"),
    t_ISR_handler(osPriorityISR, 1024, nullptr, "OCE Handler"),
    t_adc_main(osPriorityNormal,1024, nullptr,"ADC Handler"),
    t_state_subs(osPriorityNormal, 1024, nullptr,"State Updater");
Ticker adc_ticker;
OceIsr oce1(OCE_1_PIN, &IsrQueue, PowerManage::SUB_1);
CanCom cancom(&can);
Mail<AdcRead::adc_reading, 8> adc_mail;
bool can_time_isr, can_req_isr;
void ReadAdc();
void UpdateSubs();
void StartWatchdog();

/**
 * @brief ISR deferred handler for at decode time sync 
 *        package og opdatere RTC med den modtagede værdi 
 */
void CanTimeSync(){
  time_sync time_packet;
  pb_istream_t decode = pb_istream_from_buffer(\
    can_time_sync.data, can_time_sync.len);
  pb_decode(&decode, time_sync_fields, &time_packet);
  set_time(time_packet.current_time);
};

/**
 * @brief ISR deferred handler der reagerer på flag fra OnCanRec()
 *  
 * @note Lettere overflødig og lævn fra tidligere metode 
 *       uden brug af EventQueue
 * 
 */
void CanHandler(){
    if (can_time_isr) {
      LOG(LOG_DEBUG, "Received Time Sync CAN");
      can_time_isr = false;
      CanTimeSync();
    } else if (can_req_isr) {
      LOG(LOG_DEBUG, "Received Read Req CAN");
      can_req_isr = false;
      CanCom::SendReadings();
    }
}

/**
 * @brief ISR deferred handler til at beslutte handling
 *        på baggrund af CAN framets ID
 */
void OnCanRec() {
  CANMessage buf;
  can.read(buf);
  if (buf.id == CanCom::CAN_TIME_SYNC){
    can_time_isr = true;
    can_time_sync = buf;
  } else if (buf.id == CanCom::CAN_GET_READING) {
    can_req_isr = true;
    can_req = buf;
  }
  IsrQueue.call(&CanHandler);
}

/**
 * @brief Køres en enkelt gang når systemet starter
 * 
 */

void Setup(){
  // TODO læs module_override fra NVM
  Log::serial_.format(8, SerialBase::None, 1);
  Log::reporting_level_ = LOG_LEVEL; // TODO Hent fra config
  time_t now = 0; // TODO anmod om tid
  set_time(now);
  t_ISR_handler.start(callback(&IsrQueue, &EventQueue::dispatch_forever));
  StartWatchdog();
  PowerManage::UpdateEnabled();
}

/**
 * @brief Starter Watchdog hvis timeout er for langt sættes den
 *        istedet til den længste tilladte værdi
 * 
 */

void StartWatchdog(){
  //  Start Watchdog
  uint32_t max_timeout = watchdog.get_max_timeout();
  if (WATCHDOG_TIMEOUT > max_timeout) {
    LOG(LOG_WARNING,"Watchdog timeout too long, setting to " + to_string(max_timeout));

    LOG(LOG_DEBUG, "Starting watchdog with timeout " + to_string(max_timeout));
    watchdog.start(max_timeout);
  } else {
    LOG(LOG_DEBUG, "Starting watchdog with timeout " + to_string(WATCHDOG_TIMEOUT));
    watchdog.start(WATCHDOG_TIMEOUT);
  }
}

/**
 * @brief Sætter ADC måling igang. Kører hvert 5. sekundt med mindre
 *        en vigtigere thread pre-empt'er den. Lægger den nye måling
 *        i kø til NewState()
 * 
 */

void ReadAdc(){
  LOG(LOG_DEBUG, "Reading ADC values by state");
//  Read ADC Values
  AdcRead::adc_reading new_read = AdcRead::DoRead();
  adc_mail.put(&new_read);
}

/**
 * @brief Modtager ny måling fra ReadAdc() og bestemmer ny state på 
 *        baggrund af disse data
 * 
 */
void NewState(){
  while (true) {
    if (adc_mail.empty()) {
      ThisThread::sleep_for(500ms);
      continue;
    }
    AdcRead::adc_reading *new_read = adc_mail.try_get();
    LOG(LOG_DEBUG, "Updating state by ADC readings");

    //  Set new State
    Fsm::DetermineState(*new_read);
    UpdateSubs();
  }
}

/**
 * @brief Opdaterer de aktiverede moduler på baggrund af ny state og
 *        kicker watchdog
 * 
 */

void UpdateSubs(){
  LOG(LOG_DEBUG, "Updating enabled power rails by state");
//  Update enabled rails
  PowerManage::UpdateEnabled();

  LOG(LOG_DEBUG, "Kicking Watchdog");
  watchdog.kick();
}

/**
 * @brief Sætter threads igang og sover
 * 
 * @return int standard
 */

int main() {
  LOG(LOG_INFO, "STARTING");
  Setup();
  can.attach(OnCanRec);
  t_state_subs.start(NewState);
  t_adc_main.start(callback(&AdcQueue, &EventQueue::dispatch_forever));
  adc_ticker.attach(AdcQueue.event(ReadAdc), 5s); // TODO Skift tid til macro

  while (true) {
    ThisThread::sleep_for(5s);
  }

}

/*
void Loop(){

//  Start Watchdog
//  Watchdog &watchdog = Watchdog::get_instance();
  uint32_t max_timeout = watchdog.get_max_timeout();
  if (WATCHDOG_TIMEOUT > max_timeout) {
    LOG(LOG_WARNING,"Watchdog timeout too long, setting to " + to_string(max_timeout));

    LOG(LOG_DEBUG, "Starting watchdog with timeout " + to_string(max_timeout));
    watchdog.start(max_timeout);
  } else {
    LOG(LOG_DEBUG, "Starting watchdog with timeout " + to_string(WATCHDOG_TIMEOUT));
    watchdog.start(WATCHDOG_TIMEOUT);
  }

//  Get current state
  LOG(LOG_DEBUG, "Getting Current State");
  Fsm::State current_state = Fsm::GetCurrentState();
  LOG(LOG_INFO, "Current State: " + Fsm::ToString());

  LOG(LOG_DEBUG, "Reading ADC values by state");
//  Read ADC Values
  AdcRead::adc_reading new_read = AdcRead::DoRead();

  LOG(LOG_DEBUG, "Updating state by ADC readings");
//  Set new State
  Fsm::DetermineState(new_read);

  LOG(LOG_DEBUG, "Updating enabled power rails by state");
//  Update enabled rails
  PowerManage::UpdateEnabled();


  LOG(LOG_DEBUG, "Stopping watchdog");
//  Stop Watchdog
  watchdog.stop();

  LOG(LOG_DEBUG, "Starting Deep Sleep");
//  Deep Sleep
  ThisThread::sleep_for(5s);

};

*/



//  Testing();
//#pragma clang diagnostic push
//#pragma ide diagnostic ignored "EndlessLoop"
//  while (true) {
//    Loop();
//  }
//#pragma clang diagnostic pop

// Hej Abber Labber :)
//
//▬▬▬▬.◙.▬▬▬▬
// ▂▄▄▓▄▄▂
//◢◤ █▀▀████▄▄▄▄◢◤
//█▄ █ █▄ ███▀▀▀▀▀▀▀╬
//◥█████◤
//══╩══╩═
//╬═╬
//╬═╬
//╬═╬
//╬═╬       Hello?
//╬═╬  ☻/
//╬═╬ /▌
//╬═╬ / \