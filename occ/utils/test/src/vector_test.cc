#include <gtest/gtest.h>

#include "vector.h"

TEST(Vector, Constructor)
{
  const Vector<int> a;
  EXPECT_EQ(0, a.x());
  EXPECT_EQ(0, a.y());

  const Vector<int> b(13, 37);
  EXPECT_EQ(13, b.x());
  EXPECT_EQ(37, b.y());

  const Vector<double> c;
  EXPECT_FLOAT_EQ(0.0, c.x());
  EXPECT_FLOAT_EQ(0.0, c.y());

  const Vector<double> d(13.0, 37.0);
  EXPECT_FLOAT_EQ(13.0, d.x());
  EXPECT_FLOAT_EQ(37.0, d.y());
}

TEST(Vector, ConstructorOtherType)
{
  const Vector<double> a(1.3, 3.7);

  const Vector<int> b(a);
  EXPECT_EQ(1, b.x());
  EXPECT_EQ(3, b.y());

  const Vector<int> c = a;
  EXPECT_EQ(1, c.x());
  EXPECT_EQ(3, c.y());

  const Vector<int> d(std::move(a));
  EXPECT_EQ(1, d.x());
  EXPECT_EQ(3, d.y());

  const Vector<double> aa = Vector<double>(1.3, 3.7);

  const Vector<int> e = std::move(aa);
  EXPECT_EQ(1, e.x());
  EXPECT_EQ(3, e.y());
}

TEST(Vector, operatorPlus)
{
  const auto a = Vector<int>(1, 2) + Vector<int>(3, 4);
  EXPECT_EQ(4, a.x());
  EXPECT_EQ(6, a.y());

  const auto b = Vector<int>(5, 6);
  const auto c = b + Vector<int>(7, 8);
  EXPECT_EQ(12, c.x());
  EXPECT_EQ(14, c.y());

  auto d = Vector<int>(9, 10);
  d += Vector<int>(11, 12);
  EXPECT_EQ(20, d.x());
  EXPECT_EQ(22, d.y());

  const auto e = Vector<int>(-13, 14);
  EXPECT_EQ(-13, (+e).x());
  EXPECT_EQ(14, (+e).y());
}

TEST(Vector, operatorMinus)
{
  const auto a = Vector<int>(1, 2) - Vector<int>(3, 4);
  EXPECT_EQ(-2, a.x());
  EXPECT_EQ(-2, a.y());

  const auto b = Vector<int>(5, 6);
  const auto c = b - Vector<int>(7, 8);
  EXPECT_EQ(-2, c.x());
  EXPECT_EQ(-2, c.y());

  auto d = Vector<int>(9, 10);
  d -= Vector<int>(11, 12);
  EXPECT_EQ(-2, d.x());
  EXPECT_EQ(-2, d.y());

  const auto e = Vector<int>(-13, 14);
  EXPECT_EQ(13, (-e).x());
  EXPECT_EQ(-14, (-e).y());
}

TEST(Vector, operatorMultiplication)
{
  const auto a = Vector<int>(1, 2) * Vector<int>(3, 4);
  EXPECT_EQ(3, a.x());
  EXPECT_EQ(8, a.y());

  const auto b = Vector<int>(5, 6);
  const auto c = b * Vector<int>(7, 8);
  EXPECT_EQ(35, c.x());
  EXPECT_EQ(48, c.y());

  auto d = Vector<int>(9, 10);
  d *= Vector<int>(11, 12);
  EXPECT_EQ(99, d.x());
  EXPECT_EQ(120, d.y());

  const auto e = Vector<int>(1, 2) * 2.0;
  EXPECT_EQ(2, e.x());
  EXPECT_EQ(4, e.y());

  const auto f = Vector<int>(5, 6);
  const auto g = f * 3.0;
  EXPECT_EQ(15, g.x());
  EXPECT_EQ(18, g.y());

  auto h = Vector<int>(9, 10);
  h *= 4.0;
  EXPECT_EQ(36, h.x());
  EXPECT_EQ(40, h.y());
}

TEST(Vector, operatorDivision)
{
  const auto a = Vector<int>(8, 4) / Vector<int>(2, 2);
  EXPECT_EQ(4, a.x());
  EXPECT_EQ(2, a.y());

  const auto b = Vector<int>(32, 16);
  const auto c = b / Vector<int>(4, 2);
  EXPECT_EQ(8, c.x());
  EXPECT_EQ(8, c.y());

  auto d = Vector<int>(4, 2);
  d /= Vector<int>(4, 1);
  EXPECT_EQ(1, d.x());
  EXPECT_EQ(2, d.y());

  const auto e = Vector<int>(10, 4) / 2.0;
  EXPECT_EQ(5, e.x());
  EXPECT_EQ(2, e.y());

  const auto f = Vector<int>(18, 12);
  const auto g = f / 3.0;
  EXPECT_EQ(6, g.x());
  EXPECT_EQ(4, g.y());

  auto h = Vector<int>(24, 12);
  h /= 4.0;
  EXPECT_EQ(6, h.x());
  EXPECT_EQ(3, h.y());
}

TEST(Vector, Comparison)
{
  const auto a = Vector<int>(13, 37);
  const auto b = Vector<int>(13, 37);
  const auto c = Vector<int>(13, 47);
  const auto d = Vector<double>(13.0, 47.0);

  EXPECT_TRUE(a == a);
  EXPECT_TRUE(a == b);
  EXPECT_FALSE(a == c);
  EXPECT_FALSE(a == d);

  EXPECT_TRUE(b == a);
  EXPECT_TRUE(b == b);
  EXPECT_FALSE(b == c);
  EXPECT_FALSE(b == d);

  EXPECT_FALSE(c == a);
  EXPECT_FALSE(c == b);
  EXPECT_TRUE(c == c);
  EXPECT_TRUE(c == d);

  EXPECT_FALSE(d == a);
  EXPECT_FALSE(d == b);
  EXPECT_TRUE(d == c);
  EXPECT_TRUE(d == d);

  EXPECT_FALSE(a != a);
  EXPECT_FALSE(a != b);
  EXPECT_TRUE(a != c);
  EXPECT_TRUE(a != d);

  EXPECT_FALSE(b != a);
  EXPECT_FALSE(b != b);
  EXPECT_TRUE(b != c);
  EXPECT_TRUE(b != d);

  EXPECT_TRUE(c != a);
  EXPECT_TRUE(c != b);
  EXPECT_FALSE(c != c);
  EXPECT_FALSE(c != d);

  EXPECT_TRUE(d != a);
  EXPECT_TRUE(d != b);
  EXPECT_FALSE(d != c);
  EXPECT_FALSE(d != d);
}
