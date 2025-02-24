#include "ConsoleLogger.hpp"
#include "FileLogger.hpp"
#include "Messages.hpp"
#include "Sources.hpp"

int main() {
  Spektral::Log::FileLogger fl =
      Spektral::Log::FileLogger("output_logs/file_demo.log");
  for (int ii = 0; ii <= 500000; ++ii)
    fl.insert({Spektral::Log::LogLevel::INFO,
               Spektral::Log::Source<std::string>::Make("main"),
               Spektral::Log::Message<int>::Make(std::move(ii))});
}
