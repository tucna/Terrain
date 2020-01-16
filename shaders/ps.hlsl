////////////////////////////////////////////////////////////////////////////////
// Filename: color.ps
////////////////////////////////////////////////////////////////////////////////


Texture2D     tTexture    : register(t0);
SamplerState  samplerState  : register(s0);

//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
  float4 position : SV_POSITION;
  float2 texCoord : TEXCOORD;
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 ColorPixelShader(PixelInputType input) : SV_TARGET
{
  return tTexture.Sample(samplerState, input.texCoord);
}
