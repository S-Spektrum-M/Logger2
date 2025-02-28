#include "LogEvent.hpp"
#include "LogCustomErrors.hpp"
#include <format>
#include <iostream>

Spektral::Log::LogEvent::LogEvent(LogLevel level,
                                  std::shared_ptr<ISource> source,
                                  std::shared_ptr<IMessage> message) {
  if (message == nullptr)
    throw message_nullptr_exception();
  if (source == nullptr)
    throw source_nullptr_exception();
  time = std_clock::now();
  this->level = std::move(level);
  this->source = std::move(source);
  this->message = std::move(message);
}

Spektral::Log::LogEvent::LogEvent(LogEvent &&l) {
  if (l.message == nullptr)
    throw message_nullptr_exception();
  if (l.source == nullptr)
    throw source_nullptr_exception();

  message = std::move(l.message);
  source = std::move(l.source);
  time = std::move(l.time);
  level = std::move(l.level);
}

Spektral::Log::LogEvent::operator std::string() {
  switch (level) {
    using enum LogLevel;
  case INFO:
    return std::format("INFO: {} {} from {}\n", time,
                       message->operator std::string(),
                       source->operator std::string());
  case WARN:
    return std::format("WARN: {} {} from {}\n", time,
                       message->operator std::string(),
                       source->operator std::string());
  case DEBUG:
    return std::format("DEBUG: {} {} from {}\n", time,
                       message->operator std::string(),
                       source->operator std::string());
  case ERROR:
    return std::format("ERROR: {} {} from {}\n", time,
                       message->operator std::string(),
                       source->operator std::string());
  }
  return std::format("UNKOWN_LEVEL: {} {} {}\n", time,
                     message->operator std::string(),
                     source->operator std::string());
}

auto Spektral::Log::LogEvent::operator=(const LogEvent &copy) -> LogEvent & {
    this->level = copy.level;
    this->time = copy.time;
    this->message = std::shared_ptr<IMessage>(copy.message.get());
    this->source = std::shared_ptr<ISource>(copy.source.get());
    return *this;
}
