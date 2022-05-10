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
#if LOG_COM_TYPE == SERIAL
UnbufferedSerial serial(USBTX, USBRX);
#endif

Fsm fsm(FSM_START_STATE);

/// CAN stuff her da jeg ikke kan få det til at virke med
/// med biblioteker
EventQueue OceQueue;
NoMutexCAN can(CAN_RD_PIN, CAN_TD_PIN, CAN_BITRATE);
CANMessage can_time_sync, can_req;
Thread t_can_handler(osPriorityAboveNormal),
        oce_thread(osPriorityISR);
OceIsr oce1(OCE_1_PIN, &OceQueue, PowerManage::SUB_1);
CanCom cancom(&can);
bool can_time_isr, can_req_isr;


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
}

void CanTimeSync(){
  time_sync time_packet;
  pb_istream_t decode = pb_istream_from_buffer(\
    can_time_sync.data, can_time_sync.len);
  pb_decode(&decode, time_sync_fields, &time_packet);
  set_time(time_packet.current_time);
};

[[noreturn]] void CanHandler(){
  can.attach(OnCanRec);
  while (true) {
    if (can_time_isr) {
      LOG(LOG_DEBUG, "Received CAN");
      can_time_isr = false;
      CanTimeSync();
    } else if (can_req_isr) {
      can_req_isr = false;
      CanCom::SendReadings();
    } else {
      ThisThread::sleep_for(1ms);
    }
  }
}

void Setup(){
  // TODO læs module_override fra NVM
  Log::reporting_level_ = LOG_LEVEL; // TODO Hent fra config
  time_t now = 0; // TODO anmod om tid
  set_time(now);
  oce_thread.start(callback(&OceQueue, &EventQueue::dispatch_forever));
  t_can_handler.start(CanHandler);
}

[[noreturn]] void Testing(){
//  thread.start(callback(&eventQueue, &EventQueue::dispatch_forever));
//  can.attach(OnCanRec);

  while (true) {
    LOG(LOG_INFO, "Thread: " + string(ThisThread::get_name()));
    ThisThread::sleep_for(1s);
  }
//  while (true) {
//    if (isr) {
//      LOG(LOG_INFO,"ISR ID: " + to_string(msg.id));
//      CanTimeSync(&msg);
//      isr = false;
//    } else {
//      LOG(LOG_INFO,"waiting");
//    }
//    ThisThread::sleep_for(1s);
//  }

//  while (true) {
//    LOG(LOG_INFO, "Waiting 1 sec.");
//    if (CanIsr::can_rx_isr_) {
//      LOG(LOG_INFO, "CAN recieved");
//      CanIsr::can_rx_isr_ = false;
////      auto *p_message = const_cast<CANMessage *>(CanIsr::last_message_);
//      CanCom::CanTimeSync(&can_isr.last_message_);
////      (CanIsr(&can));
//    }
//    ThisThread::sleep_for(1s);
//  }
//  for (int i = 0; i < 3; ++i) {
//
//    AdcRead::DoRead();
//  }
//    AdcRead::adc_reading reading{};
//    AdcRead::adc_buffer_.peek(reading);
//
//    printf("%d, %d, %d, %d, %d, %d, %lld\n", \
            reading.bat_v_1, reading.bat_i_1, \
            reading.mppt_v_1, reading.mppt_i_1, \
            reading.sub_v_1, reading.sub_i_1, \
            reading.timestamp);
//  CanCom::SendReadings();

//  for (int i = 0; i < 5; ++i) {
//    AdcRead::adc_reading rec = CanCom::GetReading();
//    printf("%d, %d, %d, %d, %d, %d, %lld\n", \
//          rec.bat_v_1, rec.bat_i_1, \
//          rec.mppt_v_1, rec.mppt_i_1, \
//          rec.sub_v_1, rec.sub_i_1, \
//          rec.timestamp);
//  }
//  printf("\n");

}

void Loop(){

//  Start Watchdog
  Watchdog &watchdog = Watchdog::get_instance();
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


int main() {
  Setup();
  Testing();
//#pragma clang diagnostic push
//#pragma ide diagnostic ignored "EndlessLoop"
//  while (true) {
//    Loop();
//  }
//#pragma clang diagnostic pop
}

// Hej Abber Labber :)
//
//▬▬▬.◙.▬▬▬
//═▂▄▄▓▄▄▂
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