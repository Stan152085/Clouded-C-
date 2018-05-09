cbuffer cb_per_object
{
  // float4x4 inverse_transpose_world;
  float4x4 world;
  float4x4 view;
  float4x4 persp;
};

struct VS_Input
{
  float4 position : POSITION;
  // float4 normal : NORMAL;
  // float2 uv : TEXCOORD;
};

struct  VS_OUTPUT
{
  float4 positionWS : TEXCOORD1; // vertex position in world space
  float3 normal : TEXCOORD2;     // normal position in world space
  float2 uv : TEXCOORD0;
  float4 position : SV_POSITION; // vertex position in clip space
};

VS_OUTPUT main(VS_Input input)
{
  VS_OUTPUT output;
  output.position = mul(mul(input.position, mul(world, view)),persp);
  // output.positionWS = mul(world, float4(input.position)); // local space point to world space
  // output.normal = mul((float3x3)inverse_transpose_world, input.normal);
  // output.uv = input.uv;
  return output;
}