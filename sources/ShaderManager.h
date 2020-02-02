#pragma once

class ShaderManager
{
public:
  ShaderManager(ID3D11Device* device);
  ~ShaderManager() {}

  void ApplyColorShader(ID3D11DeviceContext* context, size_t indexCount, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix);

private:
  struct MatrixBufferType // TUCNA rename to perFrame
  {
    DirectX::XMMATRIX world;
    DirectX::XMMATRIX view;
    DirectX::XMMATRIX projection;
  };

  Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
  Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
  Microsoft::WRL::ComPtr<ID3D11InputLayout> m_layout;
  Microsoft::WRL::ComPtr<ID3D11Buffer> m_matrixBuffer;
};

