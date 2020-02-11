#include "Common.h"

cbuffer MatrixBuffer : register(b0)
{
  matrix worldMatrix;
  matrix viewMatrix;
  matrix projectionMatrix;
};

[domain("quad")]
PixelIn DS(PatchTess patchTess, float2 uv : SV_DomainLocation, const OutputPatch<DomainIn, 4> quad)
{
  PixelIn output;

	// Bilinear interpolation.
  output.positionWorld = lerp(
		lerp(quad[0].positionWorld, quad[1].positionWorld, uv.x),
		lerp(quad[2].positionWorld, quad[3].positionWorld, uv.x),
		uv.y);

  output.texCoord = lerp(
		lerp(quad[0].texCoord, quad[1].texCoord, uv.x),
		lerp(quad[2].texCoord, quad[3].texCoord, uv.x),
		uv.y);

  output.positionProjection = mul(float4(output.positionWorld, 1.0f), viewMatrix);
  output.positionProjection = mul(output.positionProjection, projectionMatrix);

  return output;
}