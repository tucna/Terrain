#pragma once

struct VertexIn
{
  float4 positionWorld : POSITION;
  float2 texCoord : TEXCOORD;
  float4 normal : NORMAL;
};


struct HullIn
{
  float3 positionWorld : POSITION;
  float2 texCoord : TEXCOORD;
};

struct DomainIn
{
  float3 positionWorld : POSITION;
  float2 texCoord : TEXCOORD;
};

struct PixelIn
{
  float4 positionProjection : SV_POSITION;
  float3 positionWorld : POSITION;
  float2 texCoord : TEXCOORD;
};

struct PatchTess
{
  float edgeTess[4] : SV_TessFactor;
  float insideTess[2] : SV_InsideTessFactor;
};
