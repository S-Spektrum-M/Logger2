#include "LogQueue.hpp"

namespace Spektral::Log {

LogQueue::LogQueue(size_t max_capacity)
    : _start_iterator(0), _end_iterator(0), _max_capacity(max_capacity) {
  _underlying.reserve(_max_capacity);
}

LogQueue::~LogQueue() { free(arr); }

void LogQueue::emplace_back(LogEvent &&log_e) {
  _underlying.emplace_back(std::make_shared<LogEvent>(std::move(log_e)));
}

std::shared_ptr<LogEvent> LogQueue::pop() {
    auto v = _underlying.back();
    _underlying.pop_back();
    return v;
}

void LogQueue::clear() {
    _underlying.clear();
}

bool LogQueue::empty() {
    return _underlying.empty();
}
}; // namespace Spektral::Log

// namespace Spektral::Log {
//
// LogQueue::LogQueue(size_t max_capacity)
//     : _start_iterator(0), _end_iterator(0), _max_capacity(max_capacity) {
//   arr = (LogEvent **)malloc(sizeof(std::unique_ptr<LogEvent>) *
//   max_capacity);
// }
//
// LogQueue::~LogQueue() { free(arr); }
//
// void LogQueue::emplace_back(LogEvent &&log_e) {
//   if (_end_iterator == _max_capacity)
//     throw std::exception();
//   arr[_end_iterator] = (LogEvent *)malloc(sizeof(log_e));
//   if (arr[_end_iterator])
//       *(arr[_end_iterator++]) = std::move(log_e);
// }
//
// LogEvent &LogQueue::pop() {
//   if (_start_iterator++ == _end_iterator) {
//     LogEvent &v = *(arr[_start_iterator]);
//     _start_iterator = 0;
//     _end_iterator = 0;
//     return v;
//   }
//   return *(arr[_start_iterator]);
// }
//
// void LogQueue::clear() {
//     while (_start_iterator != _end_iterator) {
//         free(arr[_start_iterator]);
//         arr[_start_iterator++] = nullptr;
//     }
//     _start_iterator = 0;
//     _end_iterator = 0;
// }
//
// bool LogQueue::empty() {
//     return (_start_iterator == 0) && (_end_iterator == 0);
// }
// }; // namespace Spektral::Log
