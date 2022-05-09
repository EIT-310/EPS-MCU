//
// Created by stormand on 5/3/22.
//

#ifndef EPS_MCU_LIB_CANCOM_CANCOM_H_
#define EPS_MCU_LIB_CANCOM_CANCOM_H_

#include <reading.pb.h>
#include <ProjectConfig.h>
#include <AdcRead.h>
#include "mbed.h"
#include "NoMutexCAN.h"

class CanCom {
 public:

  CanCom(NoMutexCAN *can){
    can_ = can;
  };

  enum CanMessageId {
    CAN_TIME_SYNC = 1,
    CAN_SET_CONFIG,
    CAN_GET_CONFIG,
    CAN_GET_READING,
    CAN_LOG
  };

  static uint8_t serialized_buffer_[sizeof(send_reading) * ADC_BUFFER_SIZE];

  static void SendBuffer(size_t size);

  static void SendReadings();

  static AdcRead::adc_reading GetReading();
//  static std::string Serialize(char *data, int size);

  static void CanTimeSync(CANMessage *msg);
  static NoMutexCAN *can_;

 private:
  void CanIsr();
  static size_t GetFrameSize();
  static void ShiftBuffer(size_t shift_bytes);
};


#endif //EPS_MCU_LIB_CANCOM_CANCOM_H_
