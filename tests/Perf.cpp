#include "ConsoleLogger.hpp"
#include "FileLogger.hpp"
#include "LogCustomErrors.hpp"
#include "Messages.hpp"
#include "Sources.hpp"
#include <benchmark/benchmark.h>
#include <fstream>
#include <iostream>
#include <random>
#define NUM_BENCH_ITERS 100000

void BM_MakeSrc(benchmark::State &state) {
  for (auto _ : state) {
    Spektral::Log::Source<std::string>::Make("main");
  }
}

void BM_MakeMessage(benchmark::State &state) {
  for (auto _ : state) {
    Spektral::Log::Message<std::string>::Make("main");
  }
}

void BM_Console(benchmark::State &state) {
  Spektral::Log::ConsoleLogger &cl =
      Spektral::Log::ConsoleLogger::get_inst(Spektral::Log::LogLevel::INFO);
  for (const auto &_ : state) {
    try {
      cl.insert({Spektral::Log::LogLevel::INFO,
                     Spektral::Log::Source<std::string>::Make("main"),
                     Spektral::Log::Message<std::string>::Make("Hi")});
    } catch (const Spektral::Log::full_queue_exception &e) {
      return;
    }
  }
}

static Spektral::Log::FileLogger logger("output_logs/demo.log");
void BM_File(benchmark::State &state) {
  for (const auto &_ : state) {
    try {
      logger.insert({Spektral::Log::LogLevel::INFO,
                     Spektral::Log::Source<std::string>::Make("main"),
                     Spektral::Log::Message<std::string>::Make("Hi")});
    } catch (const Spektral::Log::full_queue_exception &e) {
      return;
    }
  }
}

BENCHMARK(BM_MakeSrc);
BENCHMARK(BM_MakeMessage);
BENCHMARK(BM_Console);
BENCHMARK(BM_File);
BENCHMARK_MAIN();
