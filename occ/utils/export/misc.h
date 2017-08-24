#ifndef MISC_H_
#define MISC_H_

#include <array>
#include <type_traits>
#include <utility>

namespace misc
{

template <typename... T>
constexpr auto make_array(T&&... values) -> std::array<typename std::decay<typename std::common_type<T...>::type>::type, sizeof...(T)>
{
  return std::array<typename std::decay<typename std::common_type<T...>::type>::type, sizeof...(T)>{std::forward<T>(values)...};
}

}

#endif  // MISC_H_
