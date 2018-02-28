#pragma once
#include "stdafx.h"
#include <glm/common.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/quaternion.hpp>

namespace math
{
	using Vec2i = glm::i32vec2;
	using Vec3i = glm::i32vec3;
	using Vec4i = glm::i32vec4;

	using Vec2u = glm::u32vec2;
	using Vec3u = glm::u32vec3;
	using Vec4u = glm::u32vec4;

	using Vec2 = glm::f32vec2;
	using Vec3 = glm::f32vec3;
	using Vec4 = glm::f32vec4;

	using Mat22 = glm::mat2x2;
	using Mat23 = glm::mat2x3;
	using Mat24 = glm::mat2x4;
	using Mat32 = glm::mat3x2;
	using Mat33 = glm::mat3x3;
	using Mat34 = glm::mat3x4;
	using Mat42 = glm::mat4x2;
	using Mat43 = glm::mat4x3;
	using Mat44 = glm::mat4x4;
  
  using Quat = glm::quat;
}