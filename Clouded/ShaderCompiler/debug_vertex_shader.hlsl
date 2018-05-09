cbuffer cb_per_object
{
  float4x4 world;
  float4x4 view;
  float4x4 persp;
};

struct  VS_OUTPUT
{
  float4 pos : SV_POSITION;
  float4 color : COLOR;
};

VS_OUTPUT main(float4 inPos : POSITION, float4 inColor : COLOR)
{
  VS_OUTPUT output;
  output.pos = mul(mul(inPos, mul(world, view)), persp);
  output.color = inColor;
  return output;
}