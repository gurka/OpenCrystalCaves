#include <gtest/gtest.h>

#include "geometry.h"

TEST(Rectangle, Constructor)
{
  // Default constructor
  const geometry::Rectangle a;
  EXPECT_EQ(0, a.position.x());
  EXPECT_EQ(0, a.position.y());
  EXPECT_EQ(0, a.size.x());
  EXPECT_EQ(0, a.size.y());

  // Position + Size
  const geometry::Rectangle b(geometry::Position(1, 2), geometry::Size(3, 4));
  EXPECT_EQ(1, b.position.x());
  EXPECT_EQ(2, b.position.y());
  EXPECT_EQ(3, b.size.x());
  EXPECT_EQ(4, b.size.y());

  // Position + w,h
  const geometry::Rectangle c(geometry::Position(5, 6), 7, 8);
  EXPECT_EQ(5, c.position.x());
  EXPECT_EQ(6, c.position.y());
  EXPECT_EQ(7, c.size.x());
  EXPECT_EQ(8, c.size.y());

  // x,y + Size
  const geometry::Rectangle d(9, 10, geometry::Size(11, 12));
  EXPECT_EQ(9, d.position.x());
  EXPECT_EQ(10, d.position.y());
  EXPECT_EQ(11, d.size.x());
  EXPECT_EQ(12, d.size.y());

  // x,y + w,h
  const geometry::Rectangle e(13, 14, 15, 16);
  EXPECT_EQ(13, e.position.x());
  EXPECT_EQ(14, e.position.y());
  EXPECT_EQ(15, e.size.x());
  EXPECT_EQ(16, e.size.y());
}

TEST(Rectangle, Colliding)
{
  const geometry::Rectangle a(0, 0, 16, 16);
  const geometry::Rectangle b(2, 2, 4, 4);
  const geometry::Rectangle c(8, 8, 2, 2);

  // Both b and c should collide with a because they are inside a
  EXPECT_TRUE(geometry::isColliding(a, b));
  EXPECT_TRUE(geometry::isColliding(b, a));
  EXPECT_TRUE(geometry::isColliding(a, c));
  EXPECT_TRUE(geometry::isColliding(c, a));

  // b and c should not collide
  EXPECT_FALSE(geometry::isColliding(b, c));
  EXPECT_FALSE(geometry::isColliding(c, b));
}
