#pragma once

class World
{
public:
  World(ID3D11Device* device, ID3D11DeviceContext* context);
  ~World() {}

  void Draw(ID3D11DeviceContext* context);

  int GetIndexCount() { return m_indexCount; }

private:
  struct VertexType
  {
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT2 texCoord;
  };

  Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
  Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;

  Microsoft::WRL::ComPtr<ID3D11Resource>           m_resource; // TUCNA - this is probably not necessary to keep
  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;
  Microsoft::WRL::ComPtr<ID3D11SamplerState>       m_samplerState;


  uint32_t m_vertexCount;
  uint32_t m_indexCount;
};
