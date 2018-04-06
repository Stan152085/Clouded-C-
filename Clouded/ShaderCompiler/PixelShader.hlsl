#define MAX_LIGHTS 8

#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

Texture2D tex : register(t0);
sampler sampl : register(s0);

struct Material
{
  float4 emissive;
  float4 ambient;
  float4 diffuse;
  float4 specular;
  float4 specular_power;
  bool use_texture;
  // 8 bytes of padding will be added automatically
};

struct Light
{
  float4 position;
  float4 direction;
  float4 color;
  float spotangle;
  float const_attenuation;
  float linear_attenuation;
  float quad_attenuation;
  int type;
  bool enabled;
  // 8 bytes of padding will be added automatically
};

cbuffer LightPropertied : register(b1)
{
  float4 eye_position;
  float4 global_ambient;
  Light lights[MAX_LIGHTS];
}

cbuffer MaterialProperties : register(b0)
{
  Material material;
}

struct  PS_INPUT
 {
  float4 positionWS : TEXCOORD1; // vertex position in world space
  float3 normal : TEXCOORD2;     // normal position in world space
  float2 uv : TEXCOORD0;
 };

float4 main(PS_INPUT input) : SV_TARGET
{
  return float4(0.0f, 1.0f, 0.0f, 1.0f);
}