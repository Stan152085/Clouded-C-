#pragma once
#include"stdafx.h"

class Camera
{
public:
  Camera() = default;

  Camera(float width, float height, float fov);
  void Move(Vec3 Direction, float value);
  void set_fov(float degrees);
  void set_position(Vec3 pos);
  void set_target(Vec3 pos);

  Vec3 pos() const;
  Vec3 target() const;
  Vec3 up() const;

  Mat44 view() const;
  Mat44 perspective() const;
private:
  Vec3 pos_ = Vec3(0.0f, 3.0f, -8.0f);
  Vec3 target_ = Vec3(0.0f, 0.0f, 0.0f);
  Vec3 up_ = Vec3(0.0f, 1.0f, 0.0f);

  Mat44 view_;
  Mat44 persp_;
  float fov_;
  Vec2 persp_size_;
};