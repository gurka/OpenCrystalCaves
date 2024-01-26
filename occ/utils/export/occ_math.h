#ifndef MATH_H_
#define MATH_H_

namespace math
{

// constexpr clamp
constexpr int clamp(int v, int min, int max)
{
	return v >= min ? (v <= max ? v : max) : min;
}

constexpr int round_up(const int n, const int m)
{
	const int remainder = n % m;
	return remainder == 0 ? n : n + m - remainder;
}

}
#endif  // MATH_H_
