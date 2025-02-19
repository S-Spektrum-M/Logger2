/// @file: include/Messages.hpp
///
/// 1. defines the StrConv<T> concept, which checks to make sure that a type
/// defines a string conversion operator.
/// 2. provides class Message<T>, a encapsulation around a value of type T,
/// which implements IMessage.

#pragma once
#include "LogEvent.hpp"

namespace Spektral::Log {

#ifndef Spektral__Log__STRCONV
#define Spektral__Log__STRCONV
template <typename T>
concept StrConv = std::convertible_to<T, std::string>;
#endif

/**
 * @class Message
 * @brief A templated source class that implements the IMessage interface from
 * the Spektral::Log namespace.
 *
 * The Message class provides a mechanism to encapsulate any value of type T,
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
 * - Message(Args... args): A private constructor used internally by the Make
 * factory method to initialize the val member with given arguments.
 * - operator std::string(): Converts the encapsulated T object to a std::string
 * via standard conversion mechanisms.
 * - static Message* Make(Args... args): A static factory method that creates
 * and returns a new instance of Message initialized with provided arguments.
 *
 * @note
 * The class is part of the Spektral::Log namespace, indicating its use within
 * Spektral::Logging system.
 */
template <StrConv T> class Message : public Spektral::Log::IMessage {
private:
  /**
   * @brief: val is the encapsulated value that this wrapper will hold onto.
   */
  std::unique_ptr<T> val;

public:
  /**
   * @brief A constructor for use EXCLUSIVELY by Message::Make;
   *
   * @tparam Args The types of the arguments passed to the constructor for T.
   * @param args A parameter pack that will be expanded and converted into type
   * T.
   */
  template <typename... Args> Message(Args... args) {
    val = std::make_unique<T>(args...);
  }

  operator std::string() override { return val->operator std::string(); }
  /**
   * @brief Creates and returns a pointer to a new Message object.
   *
   * This is a templated factory function that allows for the creation of
   * a Message object using variadic template arguments. It forwards these
   * arguments directly to the constructor of Message.
   *
   * @tparam Args The types of the arguments passed to the Message constructor.
   * @param args A variable number of arguments that are forwarded to the
   * Message constructor.
   * @return A unique pointer to a newly created Message object initialized with
   * the provided arguments.
   *
   * @note
   * - This function dynamically allocates memory for a new Message object,
   *   so it is the caller's responsibility to manage (i.e., free) this memory
   * appropriately when it is no longer needed to avoid memory leaks.
   * - The function relies on perfect forwarding of arguments to ensure that
   * lvalues and rvalues are handled correctly in passing them to the Message
   * constructor.
   */
  template <typename... Args>
  static std::unique_ptr<Message> Make(Args... args) {
    Message *ptr = new Message(args...);
    return std::unique_ptr<Message>{ptr};
  }
};

// TODO: doc
// This needs to be specialized bc std::string doesn't define std::string
// operator but it meets the requirements of StrConv
template <> class Message<std::string> : public Spektral::Log::IMessage {
private:
  std::unique_ptr<std::string> val;

public:
  Message(const std::string &str) { val = std::make_unique<std::string>(str); }
  operator std::string() override { return *val; }
  static std::unique_ptr<Message<std::string>> Make(const std::string &value) {
    Message *ptr = new Message(value);
    return std::unique_ptr<Message>{ptr};
  }
};
} // namespace Spektral::Log
