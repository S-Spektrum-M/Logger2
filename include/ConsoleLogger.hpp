#include "LogEvent.hpp"
#include <atomic>
#include <deque>
#include <future>

namespace Spektral::Log {
// singleton class since we shouldn't have multiple things trying to
class ConsoleLogger {
private:
  using log_t = std::deque<std::shared_ptr<LogEvent>>;

  static ConsoleLogger *inst;
  ConsoleLogger();
  log_t _stdout_log;
  log_t _stderr_log;


public:
  ConsoleLogger &get_inst();
  void insert_INFO(LogEvent &l);
  void insert_INFO(LogEvent &&l);
  void insert_WARN(LogEvent &l);
  void insert_WARN(LogEvent &&l);
  void insert_DEBUG(LogEvent &l);
  void insert_DEBUG(LogEvent &&l);
  void insert_ERROR(LogEvent &l);
  void insert_ERROR(LogEvent &&l);

private:
  std::atomic<bool> _can_continue;
  /// @brief Start logging to the sink.
  [[nodiscard("Return is the value to stop the thread. Do not discard.")]]
  std::future<void> start_backend(std::atomic<bool> &can_continue);
  std::future<void> _ref;
};

}; // namespace Spektral::Log
