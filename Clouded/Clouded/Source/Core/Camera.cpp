#include "stdafx.h"
#include "Camera.h"

Camera::Camera(float width, float height, float fov)
{
  //view_ = glm::lookAtLH(pos_, target_, up_); // left handed winding order
  fov_ = (float)glm::radians(fov);
  persp_size_.x = width;
  persp_size_.y = height;
  persp_ = glm::perspectiveFovLH_ZO(fov_, persp_size_.x, persp_size_.y, 1.0f, 1000.0f);
}


Mat44 Camera::view() const
{
  return transform.GetInversedMatrix();
}

Mat44 Camera::perspective() const
{
  return persp_;
}
