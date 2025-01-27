#pragma once

#include <string>

#define LOG(message) Debug::Print(GetPrintMessage("Log: ", message))
#define LOG_WARNING(message) Debug::Print(GetPrintMessage("Warning: ", message, " (in ", __FILE__, " on line ", std::to_string(__LINE__), ")"))
#define LOG_ERROR(message) Debug::Print(GetPrintMessage("Error: ", message, " (in ", __FILE__, " on line ", std::to_string(__LINE__), ")"))
#define ASSERT(value, expected, message) if((value) != (expected)) Debug::Print(GetPrintMessage("Assert: ", message, " (expected ", std::to_string(expected), " got ", std::to_string(value), ") (in ", __FILE__, " on line ", std::to_string(__LINE__), ")"))

namespace Debug
{
	void Initialize();
	void Destroy();

	void Print(std::string);
};

template <typename... Args>
std::string GetPrintMessage(std::string arg)
{
	return arg;
}

template <typename... Args>
std::string GetPrintMessage(std::string arg, Args... args)
{
	return arg + GetPrintMessage(args...);
}