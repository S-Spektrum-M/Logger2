CXXFLAGS_WARN 			:= -Wall -Werror -Wpedantic
CXXFLAGS_OPTIM 			:= -O3
CXXFLAGS_INCLUDE_PATHS 	:= -Iinclude
CXXFLAGS_VERSION		:= -std=c++23
# CXXFLAGS_SAN 			:= -fsanitize=address -g
CXXFLAGS := $(CXXFLAGS_WARN) $(CXXFLAGS_OPTIM) $(CXXFLAGS_INCLUDE_PATHS)\
			$(CXXFLAGS_VERSION) $(CXXFLAGS_SAN)
CXX := /usr/bin/clang++-18 $(CXXFLAGS)
LOG_LIB := build/SpektralLogger.so

all: build/LogDemo tests
tests: build/perfTest

check:
	cppcheck -Iinclude/ --enable=all --suppress=missingIncludeSystem \
		--suppress=checkersReport src tests

build/LogDemo: $(LOG_LIB) tests/Demo.cpp
	$(CXX) $^ -o $@

build/perfTest: $(LOG_LIB) tests/Perf.cpp
	$(CXX) $^ -o $@ -lbenchmark

$(LOG_LIB): build/FileLogger.o build/ConsoleLogger.o build/LogEvent.o
	$(CXX) -shared -fPIC $^ -o $@

build/ConsoleLogger.o: src/ConsoleLogger.cpp include/ConsoleLogger.hpp
	$(CXX) -c -fPIC $< -o $@

build/FileLogger.o: src/FileLogger.cpp include/FileLogger.hpp
	$(CXX) -c -fPIC $< -o $@

build/LogEvent.o: src/LogEvent.cpp include/LogEvent.hpp
	$(CXX) -c -fPIC $< -o $@

clean:
	rm -rf build/*

.PHONY: clean check

