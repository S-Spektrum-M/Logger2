#include "LogEvent.hpp"
#include <atomic>
#include <cstddef>
#include <memory>

namespace Spektral::Log {
class LogQueue {
public:
    LogQueue(size_t max_capacity = 50000);
    ~LogQueue();
    void emplace_back(LogEvent &&log_e);
    // void emplace_back(LogEvent &&log_e);
    LogEvent &pop();
    void clear();
    bool empty();
private:
  [[maybe_unused]] std::atomic<size_t> _start_iterator;
  [[maybe_unused]] std::atomic<size_t> _end_iterator;
  size_t _max_capacity;
  std::unique_ptr<LogEvent> *arr;
};
}; // namespace Spektral::Log
