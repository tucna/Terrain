#pragma once

class World
{
public:
  World(ID3D11Device* device, ID3D11DeviceContext* context);
  ~World() {}

  void Draw(ID3D11DeviceContext* context);

  void Load8bHeightmap(ID3D11Device* device, ID3D11DeviceContext* context);
  void Load16bHeightmap(ID3D11Device* device, ID3D11DeviceContext* context);

  int GetIndexCount() { return m_indexCount; }

private:
  struct VertexType
  {
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT2 texCoord;
    DirectX::XMFLOAT3 normal;
  };

  Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
  Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;

  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;
  Microsoft::WRL::ComPtr<ID3D11SamplerState>       m_samplerState;

  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_heightMapResourceView;
  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_normalMapResourceView;

  uint32_t m_vertexCount;
  uint32_t m_indexCount;

  uint16_t m_worldWidth;
  uint16_t m_worldHeight;
};

