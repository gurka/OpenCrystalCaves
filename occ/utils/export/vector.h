#ifndef VECTOR_H_
#define VECTOR_H_

#include <cmath>
#include <vector>

template <typename T>
class Vector
{
 public:
  // Constructors
  constexpr Vector()
    : x_(0), y_(0)
  {
  }

  constexpr Vector(T x, T y)
    : x_(x), y_(y)
  {
  }

  Vector(const Vector&) = default;
  Vector& operator=(const Vector&) = default;

  Vector(Vector&&) = default;
  Vector& operator=(Vector&&) = default;

  ~Vector() = default;

  // Constructors for other templated type
  template <typename Y>
  Vector(const Vector<Y>& other)
    : x_(other.x_), y_(other.y_)
  {
  }

  template <typename Y>
  Vector& operator=(const Vector<Y>& other)
  {
    x_ = other.x_;
    y_ = other.y_;
    return *this;
  }

  template <typename Y>
  Vector(Vector<Y>&& other)
    : x_(other.x_), y_(other.y_)
  {
  }

  template <typename Y>
  Vector& operator=(Vector<Y>&& other)
  {
    if (this != &other)
    {
      x_ = other.x_;
      y_ = other.y_;
    }
    return *this;
  }

  constexpr T getX() const { return x_; }
  constexpr T getY() const { return y_; }

  // Operators
  Vector operator+(const Vector& other) const
  {
    return Vector(x_ + other.x_, y_ + other.y_);
  }

  Vector operator-(const Vector& other) const
  {
    return Vector(x_ - other.x_, y_ - other.y_);
  }

  Vector operator+() const
  {
    return Vector(x_, y_);
  }

  Vector operator-() const
  {
    return Vector(-x_, -y_);
  }

  Vector operator*(const Vector& other) const
  {
    return Vector(x_ * other.x_, y_ * other.y_);
  }

  Vector operator/(const Vector& other) const
  {
    return Vector(x_ / other.x_, y_ / other.y_);
  }

  Vector operator*(double d) const
  {
    return Vector(x_ * d, y_ * d);
  }

  Vector operator/(double d) const
  {
    return Vector(x_ / d, y_ / d);
  }

  bool operator==(const Vector& other) const
  {
    return x_ == other.x_ && y_ == other.y_;
  }

  bool operator!=(const Vector& other) const
  {
    return *this != other;
  }

  Vector& operator+=(const Vector& other)
  {
    x_ += other.x_;
    y_ += other.y_;
    return *this;
  }

  Vector& operator-=(const Vector& other)
  {
    x_ -= other.x_;
    y_ -= other.y_;
    return *this;
  }

  Vector& operator*=(const Vector& other)
  {
    x_ *= other.x_;
    y_ *= other.y_;
    return *this;
  }

  Vector& operator/=(const Vector& other)
  {
    x_ /= other.x_;
    y_ /= other.y_;
    return *this;
  }

  Vector& operator*=(double d)
  {
    x_ *= d;
    y_ *= d;
    return *this;
  }

  Vector& operator/=(double d)
  {
    x_ /= d;
    y_ /= d;
    return *this;
  }

 private:
  T x_;
  T y_;

  template <typename Y>
  friend class Vector;
};

#endif  // VECTOR_H_