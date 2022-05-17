//
// Created by EIT-416 on 5/2/22.
//

#ifndef EPS_MCU_LIB_LOG_LOG_H_
#define EPS_MCU_LIB_LOG_LOG_H_

#include "mbed.h"
enum TLogLevel {LOG_ERROR, LOG_WARNING, LOG_INFO, LOG_DEBUG};

enum TLogDevice {
  LOG_SERIAL,
  LOG_CAN
};
class Log
{
 public:
  Log();
  static UnbufferedSerial serial_;
  virtual ~Log();
  void Get(TLogLevel level, std::string str);
 public:
  static TLogLevel reporting_level_;
 protected:
  std::string msg_;
 private:
  Log(const Log&);
  Log& operator =(const Log&);
 private:
  TLogLevel message_level_;
  static string EnumToString(TLogLevel level);
  std::string TimeToString(time_t time);
};

#define LOG(level, string) \
if ((level) > Log::reporting_level_) ; \
else Log().Get(level, string)

#endif //EPS_MCU_LIB_LOG_LOG_H_
