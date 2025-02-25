MAKEFLAGS=-j 32
CXXFLAGS_WARN 			:= -Wall -Werror -Wpedantic
CXXFLAGS_OPTIM 			:= -O3
CXXFLAGS_INCLUDE_PATHS 	:= -Iinclude
CXXFLAGS_VERSION		:= -std=c++23
# CXXFLAGS_SAN 			:= -fsanitize=address -g
CXXFLAGS := $(CXXFLAGS_WARN) $(CXXFLAGS_OPTIM) $(CXXFLAGS_INCLUDE_PATHS)\
			$(CXXFLAGS_VERSION) $(CXXFLAGS_SAN)
CXX := /usr/bin/clang++-18 $(CXXFLAGS)
LOG_LIB := build/SpektralLogger.so

all: $(LOG_LIB)
demos: build/console_log_demo build/file_log_demo
tests: build/perfTest

check:
	cppcheck -Iinclude/ --enable=all --suppress=missingIncludeSystem \
		--suppress=checkersReport src tests

docs: Doxyfile
	doxygen Doxyfile

build/console_log_demo: $(LOG_LIB) demos/console_log_demo.cpp
	$(CXX) $^ -o $@

build/file_log_demo: $(LOG_LIB) demos/file_log_demo.cpp
	$(CXX) $^ -o $@

build/perfTest: $(LOG_LIB) tests/Perf.cpp
	$(CXX) $^ -o $@ -lbenchmark

$(LOG_LIB): build/BinaryLogger.o build/FileLogger.o build/ConsoleLogger.o\
	build/LogEvent.o
	$(CXX) -shared -fPIC $^ -o $@

build/BinaryLogger.o: src/BinaryLogger.cpp include/BinaryLogger.hpp
	$(CXX) -c -fPIC $< -o $@

build/ConsoleLogger.o: src/ConsoleLogger.cpp include/ConsoleLogger.hpp
	$(CXX) -c -fPIC $< -o $@

build/FileLogger.o: src/FileLogger.cpp include/FileLogger.hpp
	$(CXX) -c -fPIC $< -o $@

build/LogEvent.o: src/LogEvent.cpp include/LogEvent.hpp
	$(CXX) -c -fPIC $< -o $@

clean:
	rm -rf build/*

.PHONY: clean check

