#include "Common.h"

HullIn VS(VertexIn input)
{
  HullIn output;

  output.positionWorld = input.positionWorld.xyz;
  output.texCoord = float2(0, 0);

  return output;
}