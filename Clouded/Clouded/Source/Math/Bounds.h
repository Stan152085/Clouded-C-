#pragma once

template <typename T>
class Bound
{
public:
  Bound(T max)
    :
    min_(0),
    max_(max)
  {}
  Bound(T min, T max)
    :
    min_(min),
    max_(max)
  {}
  bool InBounds(const T value)
  {
    return (value >= min_ && value < max_);
  }
  T min()
  {
    return min_;
  }
  T max()
  {
    return max_;
  }
private:
  T min_;
  T max_;
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
  T min()
  {
    return 0;
  }
  T max()
  {
    return max_;
  }
private:
  T max_;
};


template <template<typename> typename Bounds, typename T>
class Bounds2D
{
public:
  Bounds2D(T x, T y)
    :
    x_bound_(x),
    y_bound_(y)
  {}
  Bounds2D(T min_x, T max_x, T min_y, T max_y)
    :
    x_bound_(min_x, max_x),
    y_bound_(min_y, max_y)
  {}
  bool InBounds(const T x, const T y)
  {
    return x_bound_.InBounds(x) && y_bound_.InBounds(y);
  }
  template<typename T2D>
  bool InBounds(const T2D pos)
  {
    return x_bound_.InBounds(static_cast<T>(pos.x)) && y_bound_.InBounds(static_cast<T>(pos.y));
  }
  T GetMinX()
  {
    return x_bound_.min();
  }
  T GetMinY()
  {
    return y_bound_.min();
  }
  T GetMaxX()
  {
    return x_bound_.max();
  }
  T GetMaxY()
  {
    return y_bound_.max();
  }
private:
  Bounds<T> x_bound_;
  Bounds<T> y_bound_;
};

template <template<typename> typename Bounds, typename T>
class Bounds3D
{
public:
  Bounds3D(T x, T y, T z)
    :
    x_bound_(x),
    y_bound_(y),
    z_bound_(z)
  {}
  Bounds3D(T min_x, T max_x, T min_y, T max_y, T min_z, T max_z)
    :
    x_bound_(min_x, max_x),
    y_bound_(min_y, max_y),
    z_bound_(min_z, max_z)
  {}
  bool InBounds(const T x, const T y, const T z)
  {
    return x_bound_.InBounds(x) && y_bound_.InBounds(y)&& z_bound_.InBounds(z);
  }
  template<typename T3D>
  bool InBounds(const T3D pos)
  {
    return x_bound_.InBounds(static_cast<T>(pos.x)) && 
      y_bound_.InBounds(static_cast<T>(pos.y)) &&
      z_bound_.InBounds(static_cast<T>(pos.z));
  }
  T GetMinX()
  {
    return x_bound_.min();
  }
  T GetMinY()
  {
    return y_bound_.min();
  }
  T GetMinZ()
  {
    return z_bound_.min();
  }
  T GetMaxX()
  {
    return x_bound_.max();
  }
  T GetMaxY()
  {
    return y_bound_.max();
  }
  T GetMaxZ()
  {
    return z_bound_.max();
  }
private:
  Bounds<T> x_bound_;
  Bounds<T> y_bound_;
  Bounds<T> z_bound_;
};