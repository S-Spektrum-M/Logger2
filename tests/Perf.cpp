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
  operator std::string() override { return std::to_string(val); }
  explicit Message_int(int v) { val = v; }
  static std::unique_ptr<Message_int> Make(int v) {
    return std::make_unique<Message_int>(v);
  }
};

std::random_device dev;
std::mt19937 rng(dev());
std::uniform_int_distribution<int> dist(0, 1000000000);

auto generateRandomInt() -> int { return dist(rng); }

void BM_X(benchmark::State &state) {
  Spektral::Log::Logger i("output_logs/demo.log");
  for (int ii = 0; auto _ : state) {
      i.insert_INFO({Spektral::Log::INFO,
                     Spektral::Log::Source<std::string>::Make("main"),
                     Message_int::Make(ii++)});
  }
}

BENCHMARK(BM_X)->Threads(1);
BENCHMARK_MAIN();
