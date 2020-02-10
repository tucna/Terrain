#pragma once

class ShaderManager
{
public:
  enum class ShaderType
  {
    Vertex,
    Hull,
    Domain,
    Pixel
  };

  ShaderManager(ID3D11Device* device);
  ~ShaderManager() {}

  void ApplyColorShader(ID3D11DeviceContext* context, size_t indexCount, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix);

private:
  struct MatrixBuffer
  {
    DirectX::XMMATRIX world;
    DirectX::XMMATRIX view;
    DirectX::XMMATRIX projection;
  };

  Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
  Microsoft::WRL::ComPtr<ID3D11HullShader> m_hullShader;
  Microsoft::WRL::ComPtr<ID3D11DomainShader> m_domainShader;
  Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

  Microsoft::WRL::ComPtr<ID3D11InputLayout> m_layout;
  Microsoft::WRL::ComPtr<ID3D11Buffer> m_matrixBuffer;
};

