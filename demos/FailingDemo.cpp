#include "Logger.hpp"
#include "Sources.hpp"
#include <fstream>
#include <print>

class Source_int : public Spektral::Log::ISource {
  int val;

public:
  operator std::string() { return std::to_string(val); }
  Source_int(int v) { val = v; }
  static std::unique_ptr<Source_int> Make(int v) {
    return std::make_unique<Source_int>(v);
  }
};

class Message_int : public Spektral::Log::IMessage {
  int val;

public:
  operator std::string() { return std::to_string(val); }
  Message_int(int v) { val = v; }
  static std::unique_ptr<Message_int> Make(int v) {
    return std::make_unique<Message_int>(v);
  }
};

auto main() -> int {
  // In commits before this(51fef4378fd26ad48b737a42696f533631ac842d and below)
  // this will segfault since the underlying ofstream is deleted before the
  // thread stops logging. We want to fix this by using shared_ptrs.
  Spektral::Log::Logger i(std::ofstream{"output_logs/demo.log"});
  for (int ii = 0; ii < 500000; ++ii)
    i.insert_INFO({Spektral::Log::INFO,
                   Spektral::Log::Source<std::string>::Make("main"),
                   Message_int::Make(ii)});
}
