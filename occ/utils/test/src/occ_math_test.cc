#include <gtest/gtest.h>

#include "occ_math.h"

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
