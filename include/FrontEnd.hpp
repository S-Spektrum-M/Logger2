/// @file: include/FrontEnd.hpp
/// @brief: This file provides an API for Spektral::Log.
#pragma once
static_assert(__cplusplus >= 202002L,
              "Spektral::Log requires C++20 at minimum");

// TODO: remove when I implement
static_assert(false, "Do not use frontend API yet.");

// TODO: 1.

namespace Spektral::Log {
struct LOGGER_STATE {};
void make_log();
LOGGER_STATE get_log();
} // namespace Spektral::Log
