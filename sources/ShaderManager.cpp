#include "pch.h"
#include "ShaderManager.h"

using namespace DirectX;

ShaderManager::ShaderManager(ID3D11Device* device)
{
  Microsoft::WRL::ComPtr<ID3DBlob> vertexShaderBuffer;
  D3DCompileFromFile(L"shaders/vs.hlsl", NULL, NULL, "ColorVertexShader", "vs_5_0", D3DCOMPILE_DEBUG, 0, &vertexShaderBuffer, NULL);
  device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);

  Microsoft::WRL::ComPtr<ID3DBlob> pixelShaderBuffer;
  D3DCompileFromFile(L"shaders/ps.hlsl", NULL, NULL, "ColorPixelShader", "ps_5_0", D3DCOMPILE_DEBUG, 0, &pixelShaderBuffer, NULL);
  device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);

  D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
  polygonLayout[0].SemanticName = "POSITION";
  polygonLayout[0].SemanticIndex = 0;
  polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
  polygonLayout[0].InputSlot = 0;
  polygonLayout[0].AlignedByteOffset = 0;
  polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  polygonLayout[0].InstanceDataStepRate = 0;

  polygonLayout[1].SemanticName = "TEXCOORD";
  polygonLayout[1].SemanticIndex = 0;
  polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
  polygonLayout[1].InputSlot = 0;
  polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
  polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  polygonLayout[1].InstanceDataStepRate = 0;

  polygonLayout[2].SemanticName = "NORMAL";
  polygonLayout[2].SemanticIndex = 0;
  polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
  polygonLayout[2].InputSlot = 0;
  polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
  polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  polygonLayout[2].InstanceDataStepRate = 0;

  // Get a count of the elements in the layout.
  unsigned int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

  // Create the vertex input layout.
  device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout);

  D3D11_BUFFER_DESC matrixBufferDesc = {};
  matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
  matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  matrixBufferDesc.MiscFlags = 0;
  matrixBufferDesc.StructureByteStride = 0;

  // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
  device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
}

void ShaderManager::ApplyColorShader(ID3D11DeviceContext* context, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
  // Transpose the matrices to prepare them for the shader.
  /* TUCNA
  worldMatrix = XMMatrixTranspose(worldMatrix);
  viewMatrix = XMMatrixTranspose(viewMatrix);
  projectionMatrix = XMMatrixTranspose(projectionMatrix);
  */

  // Lock the matrix constant buffer so it can be written to.
  D3D11_MAPPED_SUBRESOURCE mappedResource = {};
  context->Map(m_matrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource); // TUCNA HR

  // Get a pointer to the data in the matrix constant buffer.
  MatrixBufferType* dataPtr = (MatrixBufferType*)mappedResource.pData;

  // Copy the matrices into the matrix constant buffer.
  dataPtr->world = worldMatrix;
  dataPtr->view = viewMatrix;
  dataPtr->projection = projectionMatrix;

  // Unlock the matrix constant buffer.
  context->Unmap(m_matrixBuffer.Get(), 0);

  // Finanly set the matrix constant buffer in the vertex shader with the updated values.
  context->VSSetConstantBuffers(0, 1, m_matrixBuffer.GetAddressOf());

  // Set the vertex input layout.
  context->IASetInputLayout(m_layout.Get());

  // Set the vertex and pixel shaders that will be used to do the rendering.
  context->VSSetShader(m_vertexShader.Get(), NULL, 0);
  context->PSSetShader(m_pixelShader.Get(), NULL, 0);

  // Render the data.
  context->DrawIndexed(indexCount, 0, 0);
}
