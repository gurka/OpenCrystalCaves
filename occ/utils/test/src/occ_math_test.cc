#include <gtest/gtest.h>

#include "occ_math.h"

TEST(Math, mod)
{
  const auto a = math::mod(5, 2);
  EXPECT_EQ(1, a);

  const auto b = math::mod(-5, 2);
  EXPECT_EQ(1, b);
}

TEST(Math, abs)
{
  const auto a = math::abs(5);
  EXPECT_EQ(5, a);

  const auto b = math::abs(-5);
  EXPECT_EQ(5, b);
}

TEST(Math, clamp)
{
  const auto a = math::clamp(50, 0, 100);
  EXPECT_EQ(50, a);

  const auto b = math::clamp(-50, 0, 100);
  EXPECT_EQ(0, b);

  const auto c = math::clamp(150, 0, 100);
  EXPECT_EQ(100, c);

  const auto d = math::clamp(0, 0, 100);
  EXPECT_EQ(0, d);

  const auto e = math::clamp(100, 0, 100);
  EXPECT_EQ(100, e);
}
