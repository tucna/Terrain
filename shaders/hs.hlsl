#include "Common.h"

static float3 gEyePosW = float3(0, 0, 0);

static float gMinDist = 10;
static float gMaxDist = 300;

static float gMinTess = 1;
static float gMaxTess = 6;

float CalcTessFactor(float3 p)
{
  float d = distance(p, gEyePosW);
  float s = saturate((d - gMinDist) / (gMaxDist - gMinDist));

  return pow(2, (lerp(gMaxTess, gMinTess, s)));
}

PatchTess ConstantHS(InputPatch<HullIn, 4> patch, uint patchID : SV_PrimitiveID)
{
  PatchTess pt;

  // Frustum cull can be here
  // ...

  // Yellow patch
  float3 e0 = 0.5f * (patch[0].positionWorld + patch[2].positionWorld); // Left
  float3 e1 = 0.5f * (patch[0].positionWorld + patch[1].positionWorld); // Down
  float3 e2 = 0.5f * (patch[1].positionWorld + patch[3].positionWorld); // Right
  float3 e3 = 0.5f * (patch[2].positionWorld + patch[3].positionWorld); // Up
  float3 c = 0.25f * (patch[0].positionWorld + patch[1].positionWorld + patch[2].positionWorld + patch[3].positionWorld);

  pt.edgeTess[0] = CalcTessFactor(e0);
  pt.edgeTess[1] = CalcTessFactor(e1);
  pt.edgeTess[2] = CalcTessFactor(e2);
  pt.edgeTess[3] = CalcTessFactor(e3);

  pt.insideTess[0] = CalcTessFactor(c);
  pt.insideTess[1] = pt.insideTess[0];

  return pt;
}

[domain("quad")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("ConstantHS")]
[maxtessfactor(64.0f)]
DomainIn HS(InputPatch<HullIn, 4> p, uint i : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
  DomainIn output;

  output.positionWorld = p[i].positionWorld;
  //hout.Tex = p[i].tex;

  // TUCNA debug draw
  output.texCoord.r = 1;
  output.texCoord.g = 0;

  if (patchId == 120)
    output.texCoord.g = 1;
  //-----------------

  return output;
}
