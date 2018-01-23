#include <gtest/gtest.h>

#include "misc.h"

TEST(Misc, make_array)
{
  const auto array = misc::make_array(1, 2, 3);
  ASSERT_EQ(3u, array.size());
  EXPECT_EQ(1, array[0]);
  EXPECT_EQ(2, array[1]);
  EXPECT_EQ(3, array[2]);
}

TEST(Misc, random)
{
  // Yeah, what can we test really..?
  const auto a = misc::random<int>(0, 10);
}
