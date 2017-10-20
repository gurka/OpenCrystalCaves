#ifndef MATH_H_
#define MATH_H_

namespace math
{

// euclidean modulus
constexpr int mod(int a, int b)
{
  return a % b >= 0 ? a % b : (a % b) + b;
}

// constexpr abs
constexpr int abs(int a)
{
  return a < 0 ? -a : a;
}

// constexpr clamp
constexpr int clamp(int v, int min, int max)
{
  return v >= min ? (v <= max ? v : max) : min;
}

}

#endif  // MATH_H_
