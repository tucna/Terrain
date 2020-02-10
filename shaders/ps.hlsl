Texture2D tTexture : register(t0);
SamplerState samplerState : register(s0);

struct PixelInputType
{
  float4 position : SV_POSITION;
  float4 worldPosition : POSITION;
  float2 texCoord : TEXCOORD;
  float3 normal : NORMAL;
};

struct VertexOut
{
  float3 PosW : POSITION;
  float2 tex : TEXCOORD0;
};

struct DomainOut
{
  float4 PosH : SV_POSITION;
  float3 PosW : POSITION;
  float2 Tex : TEXCOORD0;
  float2 TiledTex : TEXCOORD1;
};

//float4 ColorPixelShader(PixelInputType input) : SV_TARGET
float4 ColorPixelShader(DomainOut input) : SV_TARGET
{
  /*
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
  */
  //return /*lightColor **/ textureColor;
  
  float4 color = float4(input.Tex.r, input.Tex.g, 0, 1);
  
  return color;

}
