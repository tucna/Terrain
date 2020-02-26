#pragma once

class World
{
public:
  World(ID3D11Device* device, ID3D11DeviceContext* context);
  ~World() {}

  void Draw(ID3D11DeviceContext* context);

  void Load16bHeightmap(ID3D11Device* device);

  size_t GetIndexCount() { return m_indexCount; }

private:
  struct TerrainPatch
  {
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT2 texCoord;
  };

  static constexpr uint16_t m_worldWidth = 1025;
  static constexpr uint16_t m_worldHeight = 1025;
  static constexpr uint8_t m_cellsPerPatch = 64;
  static constexpr float m_samplingStep = 1.0f;

  Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
  Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;

  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;
  Microsoft::WRL::ComPtr<ID3D11SamplerState>       m_samplerState;

  Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_wireFrame;

  size_t m_indexCount;

  uint16_t m_numPatchesRows;
  uint16_t m_numPatchesCols;
};

