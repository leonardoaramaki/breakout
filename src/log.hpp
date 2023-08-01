#pragma once

#include <cstdint>
#include <iostream>

#define LOGE(...) LogError(__FUNCTION__, __LINE__, __VA_ARGS__);
#define LOG(...) Log(__FUNCTION__, __LINE__, __VA_ARGS__)

namespace {
void PrintError() {}

template <typename T, typename... Args>
void PrintError(T value, Args... args) {
  std::cerr << value;
  PrintError(args...);
}

void Print() {}

template <typename T, typename... Args>
void Print(T value, Args... args) {
  std::cout << value;
  Print(args...);
}

template <typename... Args>
void LogError(const std::string& func, uint line, Args... args) {
  std::cerr << "E:" << func << ":" << line << ": ";
  PrintError(args...);
  std::cerr << std::endl;
}

template <typename... Args>
void Log(const std::string& func, uint line, Args... args) {
  std::cout << "I:" << func << ":" << line << ": ";
  Print(args...);
  std::cout << std::endl;
}
}  // namespace
