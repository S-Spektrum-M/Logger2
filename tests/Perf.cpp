#include "Sources.hpp"
#include <Logger.hpp>
#include <benchmark/benchmark.h>
#include <fstream>
#include <iostream>
#include <random>
#define NUM_BENCH_ITERS 100000

class Message_int : public Spektral::Log::IMessage {
  int val;

public:
  operator std::string() { return std::to_string(val); }
  explicit Message_int(int v) { val = v; }
  static std::unique_ptr<Message_int> Make(int v) {
    return std::make_unique<Message_int>(v);
  }
};

std::random_device dev;
std::mt19937 rng(dev());
std::uniform_int_distribution<int> dist(0, 1000000000);

auto generateRandomInt() -> int { return dist(rng); }

void BM_LogTestInt(benchmark::State &state) {
  using namespace Spektral::Log;
  std::ofstream l{"performace.log"};
  Logger logger(l);
  for (size_t ii = 0; auto _ : state) {
    logger.insert_INFO({Spektral::Log::INFO,
                   Spektral::Log::Source<std::string>::Make("main"),
                   Message_int::Make(ii++)});
  }
  l.close();
}

void BM_LogTestRandInt(benchmark::State &state) {
  using namespace Spektral::Log;
  std::ofstream l;
  l.open("log");
  Logger logger(l);
  std::vector<int> rands;
  for (int ii = 0; ii < NUM_BENCH_ITERS; ++ii) {
    rands.push_back(generateRandomInt());
  }
  int ii = 0;
  for (auto _ : state) {
    logger.insert(Spektral::Log::INFO, {Spektral::Log::INFO,
                        Spektral::Log::Source<std::string>::Make("benchmark"),
                        Message_int::Make(rands[ii++])});
  }
  l.close();
}

BENCHMARK(BM_LogTestInt);
// BENCHMARK(BM_LogTestRandInt)
//     ->Iterations(NUM_BENCH_ITERS)
//     ->Repetitions(5)
//     ->ReportAggregatesOnly(true);
BENCHMARK_MAIN();
