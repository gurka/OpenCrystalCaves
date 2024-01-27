#pragma once

#include <array>
#include <memory>
#include <random>
#include <type_traits>
#include <utility>

namespace misc
{

template<typename... T>
constexpr auto make_array(T&&... values) -> std::array<typename std::decay<typename std::common_type<T...>::type>::type, sizeof...(T)>
{
  return std::array<typename std::decay<typename std::common_type<T...>::type>::type, sizeof...(T)>{std::forward<T>(values)...};
}

template<typename T>
T random(T min, T max)
{
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_int_distribution<T> dis(min, max);
  return dis(gen);
}

// TODO: use C++20 format
template<typename... Args>
std::string string_format(const std::string& format, Args... args)
{
  int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;  // Extra space for '\0'
  if (size_s <= 0)
  {
    return "";
  }
  auto size = static_cast<size_t>(size_s);
  std::unique_ptr<char[]> buf(new char[size]);
  std::snprintf(buf.get(), size, format.c_str(), args...);
  return std::string(buf.get(), buf.get() + size - 1);  // We don't want the '\0' inside
}

}
