#pragma once

template <typename T>
class Bound
{
public:
  Bound(T max)
    :
    min(0),
    max(max)
  {}
  Bound(T min, T max)
    :
    min(min),
    max(max)
  {}
  bool InBounds(const T value)
  {
    return (value >= min && value < max);
  }
private:
  T min;
  T max;
};

template <typename T>
class ZeroBound
{
public:
  ZeroBound(T val)
    :
    max(val)
  {}
  bool InBounds(const T value)
  {
    return (value >= 0 && value < max);
  }
private:
  T max;
};


template <template<typename> typename Bounds, typename T>
class Bounds2D
{
public:
  Bounds2D(T x, T y)
    :
    x_bound(x),
    y_bound(y)
  {}
  Bounds2D(T min_x, T max_x, T min_y, T max_y)
    :
    x_bound(min_x, max_x),
    y_bound(min_y, max_y)
  {}
  bool InBounds(const T x, const T y)
  {
    return x_bound.InBounds(x) && y_bound.InBounds(y);
  }
  template<typename T2D>
  bool InBounds(const T2D pos)
  {
    return x_bound.InBounds(static_cast<T>(pos.x)) && y_bound.InBounds(static_cast<T>(pos.y));
  }
private:
  Bounds<T> x_bound;
  Bounds<T> y_bound;
};

template <template<typename> typename Bounds, typename T>
class Bounds3D
{
public:
  Bounds3D(T x, T y, T z)
    :
    x_bound(x),
    y_bound(y),
    z_bound(z)
  {}
  Bounds3D(T min_x, T max_x, T min_y, T max_y, T min_z, T max_z)
    :
    x_bound(min_x, max_x),
    y_bound(min_y, max_y),
    z_bound(min_z, max_z)
  {}
  bool InBounds(const T x, const T y, const T z)
  {
    return x_bound.InBounds(x) && y_bound.InBounds(y)&& z_bound.InBounds(z);
  }
  template<typename T3D>
  bool InBounds(const T3D pos)
  {
    return x_bound.InBounds(static_cast<T>(pos.x)) && 
      y_bound.InBounds(static_cast<T>(pos.y)) &&
      z_bound.InBounds(static_cast<T>(pos.z));
  }
private:
  Bounds<T> x_bound;
  Bounds<T> y_bound;
  Bounds<T> z_bound;
};