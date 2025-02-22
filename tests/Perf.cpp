#include "Sources.hpp"
#include "FileLogger.hpp"
#include <benchmark/benchmark.h>
#include <fstream>
#include <iostream>
#include <random>
#define NUM_BENCH_ITERS 100000

class Message_int : public Spektral::Log::IMessage {
  int val;

public:
  operator std::string_view() {
    std::string k = std::to_string(val);
    std::string_view s = k;
    return s;
  }
  Message_int(int v) { val = v; }
  static std::unique_ptr<Message_int> Make(int v) {
    return std::make_unique<Message_int>(v);
  }
};

std::random_device dev;
std::mt19937 rng(dev());
std::uniform_int_distribution<int> dist(0, 1000000000);

Spektral::Log::FileLogger logger("output_logs/demo.log");
auto generateRandomInt() -> int { return dist(rng); }

void BM_X(benchmark::State &state) {
  for (int ii = 0; auto _ : state) {
    logger.insert_INFO({Spektral::Log::INFO,
                        Spektral::Log::Source<std::string>::Make("main"),
                        Message_int::Make(ii++)});
  }
}

BENCHMARK(BM_X)->Iterations(1000000);
BENCHMARK_MAIN();
