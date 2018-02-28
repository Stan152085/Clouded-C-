#pragma once
#include "Math/math_defines.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

  class Transform
  {
  public:
    Transform()
      :
      scale(1, 1, 1),
      rotation(1.0f, 0.0f, 0.0f, 0.0f),
      translation(0, 0, 0)
    {}

    Transform(Vec3 scale, Quat rot, Vec3 translation)
      :
      scale(scale),
      rotation(rot),
      translation(translation)
    {}

    inline void Translate(const Vec3& trans)
    {
      translation += trans;
    }
    inline void Translate(float x, float y, float z)
    {
      Translate(Vec3(x, y, z));
    }
    inline void SetPosition(const Vec3& pos)
    {
      translation = pos;
    }
    inline void Translate(float x, float y, float z)
    {
      translation = Vec3(x, y, z);
    }

    inline Transform& Rotate(const Quat& rot)
    {
      rotation = glm::normalize(rot * rotation);
      return *this;
    }
    inline Transform& Rotate(const Vec3& rot)
    {
      Rotate(Quat(rot));
      return *this;
    }
    inline Transform& RotateDegrees(const Vec3& rot)
    {
      Rotate(Quat(glm::radians(rot)));
      return *this;
    }

    inline void SetScale(const Vec3& scale)
    {
      assert(scale.x != 0 || scale.y != 0 || scale.z != 0);
      this->scale = scale;
    }
    inline void SetScale(float x, float y, float z)
    {
      SetScale(Vec3(x, y, z));
    }

    inline void SetScale(float new_scale)
    {
      SetScale(Vec3(new_scale, new_scale, new_scale));
    }

    inline void SetRotation(const Quat& new_rotation)
    {
      rotation = new_rotation;
    }
    inline void SetRotation(float x, float y, float z, float w)
    {
      rotation = Quat(w,x,y,z);
    }
    inline void SetRotation(Vec3 euler_angles)
    {
      rotation = Quat(euler_angles);
    }
    inline void SetRotationDegrees(Vec3 euler_angles)
    {
      rotation = Quat(glm::radians(euler_angles));
    }
    inline Vec3 GetRotation()
    {
      return glm::eulerAngles(rotation);
    }
    inline Vec3 GetRotationDegrees()
    {
      return glm::radians(glm::eulerAngles(rotation));
    }

    inline Transform operator*(const Transform& lhs)
    {
      return Transform(
        Vec3(scale.x * lhs.scale.x, scale.y * lhs.scale.y, scale.z*lhs.scale.z),
        rotation * lhs.rotation, 
        glm::rotate(lhs.rotation, lhs.translation) + lhs.translation);
    }

    inline Mat44 GetMatrix()
    {
      return glm::scale(scale) *
        glm::toMat4(rotation)*
        glm::translate(Vec3(translation.x, translation.y, translation.z));
    }
    inline Mat44 GetInversedMatrix()
    {
      return glm::translate(Vec3(-translation.x, -translation.y, -translation.z))*
        glm::toMat4(glm::conjugate(rotation))*
        glm::scale(Vec3(1.0f / scale.x, 1.0f / scale.y, 1.0f / scale.z));
    }
  private:
    Vec3 scale;
    Quat rotation;
    Vec3 translation;
  };