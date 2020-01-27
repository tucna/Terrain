////////////////////////////////////////////////////////////////////////////////
// Filename: color.ps
////////////////////////////////////////////////////////////////////////////////


Texture2D tTexture : register(t0);
SamplerState samplerState : register(s0);

//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
  float4 position : SV_POSITION;
  float2 texCoord : TEXCOORD;
  float3 normal : NORMAL;
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 ColorPixelShader(PixelInputType input) : SV_TARGET
{
  // Compute normals
  //float3 normal = normalize(cross(ddx_fine(input.pos), ddy_fine(input.pos)));
  /*
  float3 normal = normalize(input.normal);

  normal.x = normal.x * 2.0f - 1.0f;
  normal.y = normal.y * 2.0f - 1.0f;
  normal.z = -normal.z;
  */
  float3 lightDirection = float3(-0.5f, -1.0f, -0.5f);
  float3 lightDir = -lightDirection;

  float4 diffuseColor = float4(1,1,1,1);

  float lightIntensity = saturate(dot(input.normal, lightDir));

  float4 lightColor = saturate(diffuseColor * lightIntensity);
  float4 textureColor = tTexture.Sample(samplerState, input.texCoord);

  //float4 normalF = float4(input.normal, 1.0f);
  //normalF = normalF * 0.5f + 0.5f;

  textureColor = float4(1,1,1,1);

  return lightColor * textureColor;
}
