#include "Common.h"

Texture2D tTexture : register(t0);
SamplerState samplerState : register(s0);

float4 PS(PixelIn input) : SV_TARGET
{
  /* TUCNA - valid code
  input.texCoord = input.worldPosition.xz / 6.0f;

  float3 lightDirection = float3(-0.5f, -1.0f, -0.5f);
  float3 lightDir = -lightDirection;

  float4 diffuseColor = float4(1,1,1,1);

  float lightIntensity = saturate(dot(input.normal, lightDir));

  float4 lightColor = saturate(diffuseColor * lightIntensity);
  float4 textureColor = tTexture.Sample(samplerState, input.texCoord);

  //float4 normalF = float4(input.normal, 1.0f);
  //normalF = normalF * 0.5f + 0.5f;

  textureColor = float4(1,1,1,1);

  //return lightColor * textureColor;
  */

  return float4(input.texCoord.r, input.texCoord.g, 0, 1);
}
