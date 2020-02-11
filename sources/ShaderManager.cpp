#include "pch.h"

#include <string>

#include "ShaderManager.h"

using namespace DirectX;
using namespace std;
using namespace Microsoft::WRL;

ShaderManager::ShaderManager(ID3D11Device* device)
{
  auto loadShader = [&](ShaderType type, const wstring& path, const string& entryPoint, const string& target)
  {
    ComPtr<ID3DBlob> shaderBuffer;
    ComPtr<ID3DBlob> errorBlob;

    UINT flags = 0;

#ifdef _DEBUG
    flags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    HRESULT hr = D3DCompileFromFile(path.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint.c_str(), target.c_str(), flags, 0, &shaderBuffer, &errorBlob);

    if (FAILED(hr) && errorBlob)
      OutputDebugStringA((char*)errorBlob->GetBufferPointer());

    switch (type)
    {
    case ShaderType::Vertex:
    {
      DX::ThrowIfFailed(device->CreateVertexShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), NULL, &m_vertexShader));

      const D3D11_INPUT_ELEMENT_DESC polygonLayout[] =
      {
        { "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
        { "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT   ,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
        { "NORMAL"  ,0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 }
      };

      // Create the vertex input layout.
      DX::ThrowIfFailed(device->CreateInputLayout(polygonLayout, (UINT)size(polygonLayout), shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), &m_layout));

      break;
    }
    case ShaderType::Hull:
      DX::ThrowIfFailed(device->CreateHullShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), NULL, &m_hullShader));
      break;
    case ShaderType::Domain:
      DX::ThrowIfFailed(device->CreateDomainShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), NULL, &m_domainShader));
      break;
    case ShaderType::Pixel:
      DX::ThrowIfFailed(device->CreatePixelShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), NULL, &m_pixelShader));
      break;
    default:
      OutputDebugStringA("Unsupported shader!");
      break;
    };
  };

  loadShader(ShaderType::Vertex, L"shaders/vs.hlsl", "VS", "vs_5_0");
  loadShader(ShaderType::Hull, L"shaders/hs.hlsl", "HS", "hs_5_0");
  loadShader(ShaderType::Domain, L"shaders/ds.hlsl", "DS", "ds_5_0");
  loadShader(ShaderType::Pixel, L"shaders/ps.hlsl", "PS", "ps_5_0");

  D3D11_BUFFER_DESC matrixBufferDesc = {};
  matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  matrixBufferDesc.ByteWidth = sizeof(MatrixBuffer);
  matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  matrixBufferDesc.MiscFlags = 0;
  matrixBufferDesc.StructureByteStride = 0;

  // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
  DX::ThrowIfFailed(device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer));
}

void ShaderManager::ApplyColorShader(ID3D11DeviceContext* context, size_t indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
  D3D11_MAPPED_SUBRESOURCE mappedResource = {};

  context->Map(m_matrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource); // TUCNA HR
  MatrixBuffer* dataPtr = (MatrixBuffer*)mappedResource.pData;

  // Copy the matrices into the matrix constant buffer.
  dataPtr->world = worldMatrix;
  dataPtr->view = viewMatrix;
  dataPtr->projection = projectionMatrix;

  context->Unmap(m_matrixBuffer.Get(), 0);

  context->DSSetConstantBuffers(0, 1, m_matrixBuffer.GetAddressOf());
  context->IASetInputLayout(m_layout.Get());
  context->VSSetShader(m_vertexShader.Get(), NULL, 0);
  context->HSSetShader(m_hullShader.Get(), NULL, 0);
  context->DSSetShader(m_domainShader.Get(), NULL, 0);
  context->PSSetShader(m_pixelShader.Get(), NULL, 0);

  // Render the data
  context->DrawIndexed((UINT)indexCount, 0, 0);
}
