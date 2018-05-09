#include "stdafx.h"
#include "Camera.h"

Camera::Camera(float width, float height, float fov)
{
  view_ = glm::lookAtLH(pos_, target_, up_); // left handed winding order
  fov_ = (float)glm::radians(fov);
  persp_size_.x = width;
  persp_size_.y = height;
  persp_ = glm::perspectiveFovLH_ZO(fov_, persp_size_.x, persp_size_.y, 1.0f, 1000.0f);
}

void Camera::Move(Vec3 direction, float value)
{
  pos_ += glm::normalize(direction) * value;
  view_ = glm::translate(view_, glm::normalize(direction) * value);
}

void Camera::set_fov(float degrees)
{
  fov_ = glm::radians(degrees);
  persp_ = glm::perspectiveFovLH_ZO(fov_, persp_size_.x, persp_size_.y, 1.0f, 1000.0f);
}

void Camera::set_position(Vec3 pos)
{
  pos_ = pos;
  view_ = glm::lookAtLH(pos_, target_, up_);
}

void Camera::set_target(Vec3 pos)
{
  target_ = pos;
  view_ = glm::lookAtLH(pos_, target_, up_);
}

Vec3 Camera::pos() const
{
  return pos_;
}

Vec3 Camera::target() const
{
  return target_;
}

Vec3 Camera::up() const
{
  return up_;
}

Mat44 Camera::view() const
{
  return view_;
}

Mat44 Camera::perspective() const
{
  return persp_;
}
