#include "FileLogger.hpp"
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
  Spektral::Log::FileLogger logger("output_logs/demo.log");
  for (int i = 0; i < 10; ++i) {
    for (int ii = 0; ii < 50000; ++ii)
      logger.insert_INFO({Spektral::Log::INFO,
                          Spektral::Log::Source<std::string>::Make("main"),
                          Message_int::Make(ii)});
    std::cout << "Sleeping" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
}
