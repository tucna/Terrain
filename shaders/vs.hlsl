////////////////////////////////////////////////////////////////////////////////
// Filename: color.vs
////////////////////////////////////////////////////////////////////////////////

Texture2D hmTexture : register(t0);
Texture2D nTexture : register(t1);
SamplerState samplerState : register(s0);

/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer
{
  matrix worldMatrix;
  matrix viewMatrix;
  matrix projectionMatrix;
};


//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
  float4 position : POSITION;
  float2 texCoord : TEXCOORD;
  float4 normal : NORMAL;
};

struct PixelInputType
{
  float4 position : SV_POSITION;
  float2 texCoord : TEXCOORD;
  float3 normal : NORMAL;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType ColorVertexShader(VertexInputType input)
{
  PixelInputType output;

  // Change the position vector to be 4 units for proper matrix calculations.
  input.position.w = 1.0f;

  // Sample from chosen LOD level - 0
  //float4 height = hmTexture.SampleLevel(samplerState, float2(input.position.x / 257.0f, input.position.z / 257.0f), 0);
  //float3 normal = nTexture.SampleLevel(samplerState, float2(input.position.x / 257.0f, input.position.z / 257.0f), 0).xyz;

  //input.position.y = height.r * 12;

  // Calculate the position of the vertex against the world, view, and projection matrices.
  output.position = mul(input.position, worldMatrix);

  output.normal = mul(input.normal.xyz, (float3x3) worldMatrix);
  output.normal = normalize(output.normal);

  output.position = mul(output.position, viewMatrix);
  output.position = mul(output.position, projectionMatrix);

  // Store the input color for the pixel shader to use.
  output.texCoord = input.texCoord;

  return output;
}