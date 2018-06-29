#pragma once
#include "Lights.h"
#include <stdafx.h>
#define MAX_LIGHTS 8

// per object buffers
struct cb_per_object
{
  Mat44 invers_transpose_world;
  Mat44 world;
  Mat44 view;
  Mat44 persp;
};

struct cb_material_per_mesh
{
  cb_material_per_mesh()
    : emissive(0.0f, 0.0f, 0.0f, 1.0f),
    ambient(0.1f, 0.1f, 0.1f, 1.0f),
    diffuse(1.0f, 1.0f, 1.0f, 1.0f),
    specular(1.0f, 1.0f, 1.0f, 1.0f),
    specular_power(8.0f)
  {}

  Vec4 emissive;
  Vec4 ambient;
  Vec4 diffuse;
  Vec4 specular;
  float specular_power;
private:
  int pad[3];
};

struct cb_lights_per_frame
{
  Vec4 eye_position;
  Vec4 global_ambient;
  int num_lights;
private:
  int pad_[3];
public:
  Light lights[MAX_LIGHTS];
};

// per drawcall buffer.