cbuffer MatrixBuffer
{
  matrix worldMatrix;
  matrix viewMatrix;
  matrix projectionMatrix;
};

struct VertexInputType
{
  float4 position : POSITION;
  float2 texCoord : TEXCOORD;
  float4 normal : NORMAL;
};

struct PixelInputType
{
  float4 position : SV_POSITION;
  float4 worldPosition : POSITION;
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

  // Calculate the position of the vertex against the world, view, and projection matrices.
  output.position = mul(input.position, worldMatrix);
  output.worldPosition = output.position;

  output.normal = mul(input.normal.xyz, (float3x3) worldMatrix);
  output.normal = normalize(output.normal);

  output.position = mul(output.position, viewMatrix);
  output.position = mul(output.position, projectionMatrix);

  // Store the input color for the pixel shader to use.
  output.texCoord = input.texCoord;

  return output;
}