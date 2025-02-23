#include "ConsoleLogger.hpp"
#include "FileLogger.hpp"
#include "Messages.hpp"
#include "Sources.hpp"
#include <fstream>
#include <print>

class Source_int : public Spektral::Log::ISource {
  int val;

public:
  operator std::string() override { return std::to_string(val); }
  Source_int(int v) : val(v) {}
  static std::unique_ptr<Source_int> Make(int &&v) {
    return std::make_unique<Source_int>(v);
  }
};

auto main() -> int {
  Spektral::Log::ConsoleLogger *cl =
      &Spektral::Log::ConsoleLogger::get_inst(Spektral::Log::INFO);
  for (int i = 0; i < 10; ++i) {
    for (int ii = 0; ii < 50000; ++ii)
      cl->insert({Spektral::Log::INFO,
                 Spektral::Log::Source<std::string>::Make("main"),
                 Spektral::Log::Message<int>::Make(std::move(ii))});
  }
  delete cl;
}
