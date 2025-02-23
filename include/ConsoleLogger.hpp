#include "LogEvent.hpp"
#include <atomic>
#include <deque>
#include <future>

namespace Spektral::Log {
// singleton class since we shouldn't have multiple things trying to log to
// console.
class ConsoleLogger {
  // User Facing API
public:
  static ConsoleLogger &get_inst(LogLevel min_level = WARN);
  void insert(LogEvent &&l);
  ~ConsoleLogger();

private:
  using log_t = std::deque<std::shared_ptr<LogEvent>>;

  static ConsoleLogger *inst;
  ConsoleLogger(LogLevel min_level = INFO);
  log_t _stdout_log;
  log_t _stderr_log;

private:
  std::atomic<bool> _can_continue;
  [[nodiscard("Return is the value to stop the thread. Do not discard.")]]
  std::future<void> start_backend(std::atomic<bool> &can_continue);
  std::future<void> _ref;
  LogLevel _min_level;
};

}; // namespace Spektral::Log
