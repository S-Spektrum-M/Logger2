/// @file: include/Sources.hpp
/// @brief: provides implemntations for the ISource Interface
///
/// 1. defines the StrConv<T> concept, which checks to make sure that a type
/// defines a string conversion operator.
/// 2. provides class Source<T>, a encapsulation around a value of type T,
/// which implements ISource.

#pragma once
#include "LogEvent.hpp"
#include <iostream>

namespace Spektral::Log {

#ifndef Spektral__Log__STRCONV
#define Spektral__Log__STRCONV
template <typename T>
concept StrConv = std::convertible_to<T, std::string>;
#endif

/**
 * @class Source
 * @brief A templated source class that implements the ISource interface from
 * the Spektral::Log namespace.
 *
 * The Source class provides a mechanism to encapsulate any value of type T,
 * where T must support conversion to std::string. It allows for creating
 * instances using a factory method and converting them to strings.
 *
 * @tparam T A template parameter that must be convertible to std::string via
 * the standard library's implicit or explicit string conversion mechanisms.
 *
 * @section Private Members
 * - val: An instance of type T that stores the encapsulated value.
 *
 * @section Methods
 * - Source(Args... args): A private constructor used internally by the Make
 * factory method to initialize the val member with given arguments.
 * - operator std::string(): Converts the encapsulated T object to a std::string
 * via standard conversion mechanisms.
 * - static Source* Make(Args... args): A static factory method that creates and
 * returns a new instance of Source initialized with provided arguments.
 *
 * @note
 * The class is part of the Spektral::Log namespace, indicating its use within
 * Spektral::Logging system.
 */
template <StrConv T> class Source : public Spektral::Log::ISource {
private:
  /**
   * @brief: val is the encapsulated value that this wrapper will hold onto.
   */
  std::unique_ptr<T> val;

public:
  /**
   * @brief A constructor for use EXCLUSIVELY by Source::Make;
   *
   * @tparam Args The types of the arguments passed to the constructor for T.
   * @param args A parameter pack that will be expanded and converted into type
   * T.
   */
  template <typename... Args> Source(Args... args) {
    val = std::make_unique<T>(args...);
  }

  operator std::string() override { return val->operator std::string(); }
  /**
   * @brief Creates and returns a pointer to a new Source object.
   *
   * This is a templated factory function that allows for the creation of
   * a Source object using variadic template arguments. It forwards these
   * arguments directly to the constructor of Source.
   *
   * @tparam Args The types of the arguments passed to the Source constructor.
   * @param args A variable number of arguments that are forwarded to the
   * Source constructor.
   * @return A unique pointer to a newly created Source object initialized with
   * the provided arguments.
   *
   * @note
   * - This function dynamically allocates memory for a new Source object,
   *   so it is the caller's responsibility to manage (i.e., free) this memory
   * appropriately when it is no longer needed to avoid memory leaks.
   * - The function relies on perfect forwarding of arguments to ensure that
   * lvalues and rvalues are handled correctly in passing them to the Source
   * constructor.
   */
  template <typename... Args>
  static std::unique_ptr<Source> Make(Args... args) {
    return std::make_unique(args...);
  }
};

// TODO: doc
// This needs to be specialized bc std::string doesn't define std::string
// operator but it meets the requirements of StrConv
template <> class Source<std::string> : public Spektral::Log::ISource {
private:
  std::string val;

public:
  Source(std::string &&str) : val(str) {}
  operator std::string() override { return val; }
  static std::unique_ptr<Source> Make(std::string &&value) {
    return std::make_unique<Source>(std::move(value));
  }
};
} // namespace Spektral::Log
