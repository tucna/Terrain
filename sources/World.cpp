#include "pch.h"

#include <WICTextureLoader.h>

#include "World.h"

using namespace DirectX;
using namespace std;

World::World(ID3D11Device* device, ID3D11DeviceContext* context) :
  m_vertexCount(0),
  m_indexCount(0)
{
  uint16_t terrainHeight = 256;
  uint16_t terrainWidth = 256;

  XMFLOAT4 color(1.0f, 1.0f, 1.0f, 1.0f);

  vector<VertexType> vertices;
  vector<uint32_t> indices;

  for (uint16_t z = 0; z != 100; z++)
    for (uint16_t x = 0; x != 100; x++)
    {
      vertices.push_back({ {(float)x    , 0, (float)z}, {0, 1} }); // 0 0
      vertices.push_back({ {(float)x    , 0, (float)z + 1}, {0, 0} }); // 0 1
      vertices.push_back({ {(float)x + 1, 0, (float)z + 1}, {1, 0} }); // 1 1
      vertices.push_back({ {(float)x + 1, 0, (float)z}, {1, 1} }); // 1 0

      uint16_t lastIndex = indices.empty() ? 0 : indices.back() + 1;
      indices.push_back(lastIndex + 0);
      indices.push_back(lastIndex + 1);
      indices.push_back(lastIndex + 2);
      indices.push_back(lastIndex + 0);
      indices.push_back(lastIndex + 2);
      indices.push_back(lastIndex + 3);
    }

  m_vertexCount = vertices.size();
  m_indexCount = indices.size();

  D3D11_BUFFER_DESC vertexBufferDesc = {};
  vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
  vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  vertexBufferDesc.CPUAccessFlags = 0;
  vertexBufferDesc.MiscFlags = 0;
  vertexBufferDesc.StructureByteStride = 0;

  D3D11_SUBRESOURCE_DATA vertexData = {};
  vertexData.pSysMem = vertices.data();
  vertexData.SysMemPitch = 0;
  vertexData.SysMemSlicePitch = 0;

  device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer); // TUCNA - HR handling

  D3D11_BUFFER_DESC indexBufferDesc = {};
  indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  indexBufferDesc.ByteWidth = sizeof(uint32_t) * m_indexCount;
  indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  indexBufferDesc.CPUAccessFlags = 0;
  indexBufferDesc.MiscFlags = 0;
  indexBufferDesc.StructureByteStride = 0;

  D3D11_SUBRESOURCE_DATA indexData = {};
  indexData.pSysMem = indices.data();
  indexData.SysMemPitch = 0;
  indexData.SysMemSlicePitch = 0;

  device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer); // TUCNA HR

  // Texture
  // TUCNA m_resource.GetAddressOf() is not needed can be nullpt
  CreateWICTextureFromFileEx(device, context, L"resources/test.png", 0, D3D11_USAGE_DEFAULT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, 0, D3D11_RESOURCE_MISC_GENERATE_MIPS, WIC_LOADER_IGNORE_SRGB, m_resource.GetAddressOf(), m_shaderResourceView.GetAddressOf());


  //CreateWICTextureFromFile(device, context, L"resources/test.png", m_resource.GetAddressOf(), m_shaderResourceView.GetAddressOf());

  // Sampler
  D3D11_SAMPLER_DESC sampDesc = {};
  sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
  sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
  sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
  sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
  sampDesc.MinLOD = 0;
  sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

  //Create the sample state
  device->CreateSamplerState(&sampDesc, m_samplerState.GetAddressOf());
}

void World::Draw(ID3D11DeviceContext* context)
{
  unsigned int stride;
  unsigned int offset;

  // Set vertex buffer stride and offset.
  stride = sizeof(VertexType);
  offset = 0;

  context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
  context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
  context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // TUCNA - triangle list here is probably better

  context->PSSetShaderResources(0, 1, m_shaderResourceView.GetAddressOf());
  context->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());
}
