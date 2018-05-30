#pragma once
#include"stdafx.h"
#include "Transform.h"

class Camera
{
public:
  Camera() = default;

  Camera(float width, float height, float fov);
  
  void set_fov(float degrees);
  
  Mat44 view() const;
  Mat44 perspective() const;
  Mat44 hmd_pose_;

  Transform transform;
private:
  Mat44 persp_;
  float fov_;
  Vec2 persp_size_;
};