//
// Created by EIT-416 on 5/2/22.
// Inspired by https://www.drdobbs.com/cpp/logging-in-c/201804215
//

#include "Log.h"
Log::Log() = default;

UnbufferedSerial Log::serial_(USBTX, USBRX);

TLogLevel Log::reporting_level_;

/**
 * @brief Opretter log beskeden
 * 
 * @param level neveau for informationen
 * @param str   string med det der skal logges
 */
void Log::Get(TLogLevel level, std::string str) {
  msg_.append("- " + TimeToString(time(NULL)));
  msg_.append(" " + EnumToString(level) + ": ");
  msg_.append("\t" + str + "\n");
  message_level_ = level;
}
/**
 * @brief Destroy the Log:: Log object. 
 *        Skriver hele beskeden som blev samlet i Log::Get()
 * 
 */
Log::~Log() {
  serial_.write(msg_.c_str(), msg_.length());
}

/**
 * @brief Omdanner log neveau til string med navnet for niveauet
 * 
 * @param level log niveuet der ønskes navn for
 * @return std::string String med navnet
 */

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

/**
 * @brief Omdanner UNIX sekund timestamp til formateret string
 * 
 * @param time UNIX timestamp
 * @return std::string formateret tid 
 */
std::string Log::TimeToString(time_t time) {
  char buf[32];
  strftime(buf, 32, "%c", localtime(&time));
  string str(buf);
  return str;
}
