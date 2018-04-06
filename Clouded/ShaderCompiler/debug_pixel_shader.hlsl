struct  PS_INPUT
{
  float4 pos : SV_POSITION;
  float4 color : COLOR;
};

float4 main(PS_INPUT input) : SV_TARGET
{
  return float4 (input.color.x, input.color.y, input.color.z, 1.0f);
}