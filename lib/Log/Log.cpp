//
// Created by stormand on 5/2/22.
// Inspired by https://www.drdobbs.com/cpp/logging-in-c/201804215
//

#include "Log.h"
Log::Log() = default;

UnbufferedSerial Log::serial_(USBTX, USBRX);

TLogLevel Log::reporting_level_;

void Log::Get(TLogLevel level, std::string str) {
  msg_.append("- " + TimeToString(time(NULL)));
  msg_.append(" " + EnumToString(level) + ": ");
  msg_.append("\t" + str + "\n");
  message_level_ = level;
}

Log::~Log() {
#if LOG_COM_TYPE == SERIAL
  serial_.write(msg_.c_str(), msg_.length());
//  printf("%s", msg_.c_str());
#elif LOG_COM_TYPE == CAN
  //  TODO: CAN logging
#endif
}

std::string Log::EnumToString(TLogLevel level) {
  string str;
  switch (level) {

    case LOG_ERROR: str = "ERR";  break;
    case LOG_WARNING: str = "WARN"; break;
    case LOG_INFO: str = "INFO"; break;
    case LOG_DEBUG: str = "DEBUG"; break;

  }
  return str;
}
std::string Log::TimeToString(time_t time) {
  char buf[32];
  strftime(buf, 32, "%c", localtime(&time));
  string str(buf);
  return str;
}
