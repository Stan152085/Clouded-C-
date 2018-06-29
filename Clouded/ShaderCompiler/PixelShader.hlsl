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
  float specular_power;
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
  // 12 bytes of padding will be added automatically
};

struct LightingResult
{
  float4 diffuse;
  float4 specular;
};

cbuffer LightProperties : register(b1)
{
  float4 eye_position;
  float4 global_ambient;
  int num_lights;
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

float4 ShadeDiffuse(Light light, float3 L, float3 N)
{
  float NdotL = max(0, dot(N, L));
  return light.color * NdotL;
}

float4 ShadeSpecular(Light light, float3 V, float3 L, float3 N)
{
  float3 R = normalize(reflect(-L, N));
  float RdotV = max(0, dot(R, V));
  return light.color * pow(RdotV, material.specular_power);
}

float CalcAttenuation(Light light, float d)
{
  return 1.0f / (light.const_attenuation + light.linear_attenuation * d + light.quad_attenuation * d * d);
}

LightingResult ApplyPointLight(Light light, float3 V, float4 P, float3 N)
{
  LightingResult result;
  float3 L = (light.position - P).xyz;
  float distance = length(L);
  L = L / distance;

  float attenuation = CalcAttenuation(light, distance);

  result.diffuse = ShadeDiffuse(light, L, N) * attenuation;
  result.specular = ShadeSpecular(light, V, L, N) * attenuation;

  return result;
}

LightingResult ApplyDirectionalLight(Light light, float3 V, float4 P, float3 N)
{
  LightingResult result;

  float3 L = -light.direction.xyz;

  result.diffuse = ShadeDiffuse(light, L, N);
  result.specular = ShadeSpecular(light, V, L, N);

  return result;
}

float ApplySpotCone(Light light, float3 L)
{
  float min = cos(light.spotangle);
  float max = (min + 1.0f) / 2.0f;
  float cos_angle = dot(light.direction.xyz, -L);
  return smoothstep(min, max, cos_angle);
}

LightingResult ApplySpotLight(Light light, float3 V, float4 P, float3 N)
{
  LightingResult result;

  float3 L = (light.position - P).xyz;
  float distance = length(L);
  L = L / distance;

  float attenuation = CalcAttenuation(light, distance);
  float intensity = ApplySpotCone(light, L);

  result.diffuse = ShadeDiffuse(light, L, N) * attenuation * intensity;
  result.specular = ShadeSpecular(light, V, L, N) * attenuation * intensity;

  return result;
}

LightingResult ComputeLighting(float4 P, float3 N)
{
  float3 V = normalize(eye_position - P).xyz;

  LightingResult total = { { 0, 0, 0, 0 },{ 0, 0, 0, 0 } };

  [unroll]
  for (int i = 0; i < num_lights; ++i)
  {
    LightingResult result = { { 0, 0, 0, 0 },{ 0, 0, 0, 0 } };

    switch (lights[i].type)
    {
    case DIRECTIONAL_LIGHT:
    {
      result = ApplyDirectionalLight(lights[i], V, P, N);
    }
    break;
    case POINT_LIGHT:
    {
      result = ApplyPointLight(lights[i], V, P, N);
    }
    break;
    case SPOT_LIGHT:
    {
      result = ApplySpotLight(lights[i], V, P, N);
    }
    break;
    }
    total.diffuse += result.diffuse;
    total.specular += result.specular;
  }

  total.diffuse = saturate(total.diffuse);
  total.specular = saturate(total.specular);

  return total;
}

float4 main(PS_INPUT input) : SV_TARGET
{
  LightingResult lighting = ComputeLighting(input.positionWS, normalize(input.normal));

  float4 emissive = material.emissive;
  float4 ambient = material.ambient * global_ambient;
  float4 diffuse = material.diffuse * lighting.diffuse;
  float4 specular = material.specular * lighting.specular;
  
  float4 color = { 1, 1, 1, 1 };
  color = tex.Sample(sampl, input.uv);
 
  return (emissive + ambient + diffuse + specular) * color;
}