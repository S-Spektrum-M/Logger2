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
  Spektral::Log::Logger i(std::cout);
  for (int ii = 0; ii < 500000; ++ii)
    i.insert_INFO({Spektral::Log::INFO,
                   Spektral::Log::Source<std::string>::Make("main"),
                   Message_int::Make(ii)});
}
