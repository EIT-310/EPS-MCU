//
// Created by EIT-416 on 5/3/22.
//

#include "CanCom.h"
#include "AdcRead.h"
#include "mbed.h"
#include <pb_encode.h>
#include <pb_decode.h>
#include "reading.pb.h"
#include "ProjectConfig.h"
#include "Log.h"

uint8_t CanCom::serialized_buffer_[send_reading_size * ADC_BUFFER_SIZE];

NoMutexCAN *CanCom::can_;

/**
 * @brief Send alle målinger i CircularBuffer'eren (adc_buffer_) over CAN.
 * 
 * Hver måling i adc_buffer_ enkodes med Google Protobuf structen _send_reading
 * og serialiseres. De serialiserede bytes lægges ind i serialized_buffer_. Hvis der
 * ikke blev målt på mppt og rails i en given måling sættes has_mppt_x_x og 
 * has_sub_x_x bools'ne til False, og protobufs sørger for at serialiseringen
 * fylder mindre.
 * 
 */
void CanCom::SendReadings() {
  auto size = AdcRead::adc_buffer_.size();


  AdcRead::adc_reading buf{};
  send_reading to_send;
  pb_ostream_t encode = pb_ostream_from_buffer(\
  serialized_buffer_, sizeof(serialized_buffer_));

  for (size_t i = 0; i < size; ++i) {
    AdcRead::adc_buffer_.pop(buf);
    to_send.bat_v_1 = buf.bat_v_1;
    to_send.bat_i_1 = buf.bat_i_1;
    to_send.timestamp = buf.timestamp;

    if (!buf.mppt_v_1) { // Don't send data if it wasn't read
      to_send.has_mppt_v_1 = false;
      to_send.has_mppt_i_1 = false;
      to_send.has_sub_v_1 = false;
      to_send.has_sub_i_1 = false;
    } else {
      to_send.has_mppt_v_1 = true;
      to_send.has_mppt_i_1 = true;
      to_send.has_sub_v_1 = true;
      to_send.has_sub_i_1 = true;

      to_send.mppt_v_1 = buf.mppt_v_1;
      to_send.mppt_i_1 = buf.mppt_i_1;
      to_send.sub_v_1 = buf.sub_v_1;
      to_send.sub_i_1 = buf.sub_i_1;
    }

    pb_encode(&encode, send_reading_fields, &to_send);

  }

  SendBuffer(encode.bytes_written);

}

/**
 * @brief Sender fra serializeret buffer over CAN ved at dele hvert
 *        frame op i 8 bytes
 * 
 * @param size Antale bytes at sende fra bufferen
 */
void CanCom::SendBuffer(size_t size) {
  auto bytes = size;
  auto can_frames = (bytes / 8) + ((bytes % 8) ? 1 : 0);
  uint8_t framebuffer[8];

  //  CANMessage *msg;
  LOG(LOG_DEBUG, "Sending CAN");
  for (size_t i = 0; i < can_frames; ++i) {
    auto frame_size = (bytes / 8) ? 8 : bytes;
    for (size_t j = 0; j < frame_size; ++j) {
      framebuffer[j] = serialized_buffer_[j];
    }
    ShiftBuffer(frame_size);
    CANMessage msg(CAN_GET_READING, framebuffer, frame_size);
    can_->write(msg);
    bytes -= frame_size;
  }
  LOG(LOG_DEBUG, "Done Sending");

}

/**
 * @brief Find størrelsen af en modtaget Protobuf pakke inden decode
 * 
 * @return size_t størrelsen af pakken i bytes
 * 
 * @note Anvdendes ikke da vi ikke modtager pakker der fylder mere end 1 CAN frame
 */
size_t CanCom::GetFrameSize(){
  if (serialized_buffer_[0] != '\b') {
    LOG(LOG_ERROR,"Can read buffer did not start with STX");
    return 0; // TODO better Error Reporting
  }

  for (int i = 1; i < send_reading_size; ++i) {
    auto index = serialized_buffer_[i];
    if (index == '\b' or index == '\0') {
      return i ;
    }
  }
}

/**
 * @brief Modtag en enkelt ADC måling fra serialized_buffer_
 * 
 * @return AdcRead::adc_reading decoded adc reading
 * 
 * @note Anvdendes ikke da vi ikke modtager ADC målinger igennem CAN
 */
AdcRead::adc_reading CanCom::GetReading() {
  AdcRead::adc_reading ret{};
  send_reading buf;

  auto frame_size = GetFrameSize();
  pb_istream_t decode = pb_istream_from_buffer(\
  serialized_buffer_, frame_size);


  pb_decode(&decode, send_reading_fields, &buf);

  ret.bat_v_1 = buf.bat_v_1;
  ret.bat_i_1 = buf.bat_i_1;
  ret.timestamp = buf.timestamp;

  if (buf.has_mppt_v_1) { // Don't send data if it wasn't read
    ret.mppt_v_1 = buf.mppt_v_1;
    ret.mppt_i_1 = buf.mppt_i_1;
    ret.sub_v_1 = buf.sub_v_1;
    ret.sub_i_1 = buf.sub_i_1;
  }

//  Shift buffer by amount read
  ShiftBuffer(frame_size);
  return ret;
}

/**
 * @brief Rykker alle bytes i serialized_buffer_ med et antal pladser
 * 
 * @param shift_bytes antal pladser at rykke pladser i bufferen
 */
void CanCom::ShiftBuffer(size_t shift_bytes) {
  for (auto i = shift_bytes; i < sizeof(serialized_buffer_) - shift_bytes; ++i) {
    serialized_buffer_[i - shift_bytes] = serialized_buffer_[i];
    serialized_buffer_[i] = '\0';
  }
}
