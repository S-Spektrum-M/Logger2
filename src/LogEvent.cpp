#include "LogEvent.hpp"
#include "LogCustomErrors.hpp"
#include <format>
#include <iostream>

Spektral::Log::LogEvent::LogEvent(LogLevel level,
                                  std::unique_ptr<ISource> source,
                                  std::unique_ptr<IMessage> message) {
  if (message == nullptr)
    throw message_nullptr_excpetion();
  if (source == nullptr)
    throw source_nullptr_excpetion();
  time = std_clock::now();
  this->level = level;
  this->source = std::move(source);
  this->message = std::move(message);
}

Spektral::Log::LogEvent::LogEvent(LogEvent &&l) {
  if (l.message == nullptr)
    throw message_nullptr_excpetion();
  if (l.source == nullptr)
    throw source_nullptr_excpetion();
  message = std::move(l.message);
  source = std::move(l.source);
  time = std::move(l.time);
  level = std::move(l.level);
}

Spektral::Log::LogEvent::operator std::string_view() {
  std::string_view ret_strv;
  switch (level) {
  case INFO:
    ret_strv = std::format("INFO: {} {} from {}\n", time,
                           message->operator std::string_view(),
                           source->operator std::string_view());
    break;
  case WARN:
    ret_strv = std::format("WARN: {} {} from {}\n", time,
                           message->operator std::string_view(),
                           source->operator std::string_view());
    break;
  case DEBUG:
    ret_strv = std::format("DEBUG: {} {} from {}\n", time,
                           message->operator std::string_view(),
                           source->operator std::string_view());
    break;
  case ERROR:
    ret_strv = std::format("ERROR: {} {} from {}\n", time,
                           message->operator std::string_view(),
                           source->operator std::string_view());
    break;
  }
  ret_strv = std::format("UNKOWN_LEVEL: {} {} {}\n", time,
                         message->operator std::string_view(),
                         source->operator std::string_view());
  return ret_strv;
}
