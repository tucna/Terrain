#include "pch.h"

#include <WICTextureLoader.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "World.h"

// TUCNA DELETE
#include <iostream>

using namespace DirectX;
using namespace std;

World::World(ID3D11Device* device, ID3D11DeviceContext* context) :
  m_vertexCount(0),
  m_indexCount(0),
  m_worldWidth(256),
  m_worldHeight(256)
{
  int width = -1;
  int height = -1;
  int channels = -1;

  uint8_t* image = stbi_load("resources/hm.png", &width, &height, &channels, STBI_grey);

  vector<VertexType> vertices;
  vector<uint32_t> indices;

  float scaleX = (float)width / (float)m_worldWidth;
  float scaleZ = (float)height / (float)m_worldHeight;

  // Sample height map
  vector<float> heights;
  float HEIGHT_STEP = 3.0f;

  for (uint32_t z = 0; z < m_worldHeight; z++)
    for (uint32_t x = 0; x < m_worldWidth; x++)
    {
      float h = image[(((int)truncf((z + 0) * scaleZ)) * height) + ((int)truncf((x + 0) * scaleX))] / 255.0f;
      heights.push_back(h * HEIGHT_STEP);
    }

  // Vertices
  float STEP = 0.1f;

  float h;
  float T;
  float B;
  float L;
  float R;

  // TUCNA - we should start from 0 till width -1
  for (uint32_t z = 1; z < m_worldHeight - 2; z++)
    for (uint32_t x = 1; x < m_worldWidth - 2; x++)
    {
      h = heights[((z + 0) * height) + (x + 0)];
      T = heights[((z + 1) * height) + (x + 0)];
      B = heights[((z - 1) * height) + (x + 0)];
      L = heights[((z + 0) * height) + (x - 1)];
      R = heights[((z + 0) * height) + (x + 1)];
      vertices.push_back({ {(float)x * STEP, h, (float)z * STEP}, {0, 1}, {L - R, 2.0f, T - B} });

      h = heights[((z + 1) * height) + (x + 0)];
      T = heights[((z + 1 + 1) * height) + (x + 0)];
      B = heights[((z - 1 + 1) * height) + (x + 0)];
      L = heights[((z + 0 + 1) * height) + (x - 1)];
      R = heights[((z + 0 + 1) * height) + (x + 1)];
      vertices.push_back({ {(float)x * STEP, h, (float)z * STEP + STEP}, {0, 0}, {L - R, 2.0f, T - B} });

      h = heights[((z + 1) * height) + (x + 1)];
      T = heights[((z + 1 + 1) * height) + (x + 0 + 1)];
      B = heights[((z - 1 + 1) * height) + (x + 0 + 1)];
      L = heights[((z + 0 + 1) * height) + (x - 1 + 1)];
      R = heights[((z + 0 + 1) * height) + (x + 1 + 1)];
      vertices.push_back({ {(float)x * STEP + STEP, h, (float)z * STEP + STEP}, {1, 0}, {L - R, 2.0f, T - B} });

      h = heights[((z + 0) * height) + (x + 1)];
      T = heights[((z + 1) * height) + (x + 0 + 1)];
      B = heights[((z - 1) * height) + (x + 0 + 1)];
      L = heights[((z + 0) * height) + (x - 1 + 1)];
      R = heights[((z + 0) * height) + (x + 1 + 1)];
      vertices.push_back({ {(float)x * STEP + STEP, h, (float)z * STEP}, {1, 1}, {L - R, 2.0f, T - B} });

      uint32_t lastIndex = indices.empty() ? 0 : indices.back() + 1;
      indices.push_back(lastIndex + 0);
      indices.push_back(lastIndex + 1);
      indices.push_back(lastIndex + 2);
      indices.push_back(lastIndex + 0);
      indices.push_back(lastIndex + 2);
      indices.push_back(lastIndex + 3);
    }

  stbi_image_free(image);

  // Computation of the normals
  /*
      T
    L O R
      B
  */
  /*
  for (uint32_t z = 1; z < m_worldHeight - 3; z++)
    for (uint32_t x = 1; x < m_worldWidth - 3; x++)
    {
      float T = vertices[((z + 1) * m_worldHeight) + (x + 0)].position.y;
      float B = vertices[((z - 1) * m_worldHeight) + (x + 0)].position.y;
      float L = vertices[((z + 0) * m_worldHeight) + (x - 1)].position.y;
      float R = vertices[((z + 0) * m_worldHeight) + (x + 1)].position.y;

      vertices[(z * m_worldHeight) + x].normal.x = L - R;
      vertices[(z * m_worldHeight) + x].normal.y = 2.0f;
      vertices[(z * m_worldHeight) + x].normal.z = T - B;

      // TODO: Normalization can be added
    }
    */
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
  CreateWICTextureFromFile(device, context, L"resources/test.dds", nullptr, m_shaderResourceView.GetAddressOf());
  CreateWICTextureFromFile(device, nullptr, L"resources/hm.dds", nullptr, m_heightMapResourceView.GetAddressOf());
  CreateWICTextureFromFile(device, nullptr, L"resources/hm_n.dds", nullptr, m_normalMapResourceView.GetAddressOf());

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

  context->VSSetShaderResources(0, 1, m_heightMapResourceView.GetAddressOf());
  context->VSSetShaderResources(1, 1, m_normalMapResourceView.GetAddressOf());

  context->VSSetSamplers(0, 1, m_samplerState.GetAddressOf());
}
