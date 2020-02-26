#include "pch.h"

#include <array>
#include <fstream>
#include <WICTextureLoader.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "World.h"

// TUCNA DELETE
#include <iostream>

using namespace DirectX;
using namespace std;

World::World(ID3D11Device* device, ID3D11DeviceContext* context) :
  m_indexCount(0),
  m_numPatchesCols(0),
  m_numPatchesRows(0)
{
  // TUCNA - this is probably wrong - must be handled in the shader
  Load16bHeightmap(device);

  // Texture
  CreateWICTextureFromFile(device, context, L"resources/grass.png", nullptr, m_shaderResourceView.GetAddressOf());

  // Sampler
  D3D11_SAMPLER_DESC sampDesc = {};
  sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
  sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
  sampDesc.MinLOD = 0;
  sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

  //Create the sample state
  device->CreateSamplerState(&sampDesc, m_samplerState.GetAddressOf());

  // TUCNA - wireframe
  D3D11_RASTERIZER_DESC wfdesc = {};
  wfdesc.FillMode = D3D11_FILL_WIREFRAME;
  wfdesc.CullMode = D3D11_CULL_NONE;
  device->CreateRasterizerState(&wfdesc, m_wireFrame.GetAddressOf());

  context->RSSetState(m_wireFrame.Get());
}

void World::Draw(ID3D11DeviceContext* context)
{
  unsigned int stride;
  unsigned int offset;

  // Set vertex buffer stride and offset.
  stride = sizeof(TerrainPatch);
  offset = 0;

  context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
  context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
  context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);

  context->PSSetShaderResources(0, 1, m_shaderResourceView.GetAddressOf());
  context->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());
}

void World::Load16bHeightmap(ID3D11Device* device)
{
  // 1025 x 1025
  ifstream dataFile(L"resources/heightmap.r16", ios::binary);

  vector<uint16_t> heights(m_worldWidth * m_worldHeight);
  dataFile.read((char*)heights.data(), sizeof(uint16_t) * m_worldWidth * m_worldHeight);

  dataFile.close();

  m_numPatchesRows = ((m_worldHeight - 1) / m_cellsPerPatch) + 1;
  m_numPatchesCols = ((m_worldWidth - 1) / m_cellsPerPatch) + 1;

  vector<TerrainPatch> patchVertices(m_numPatchesRows * m_numPatchesCols);

  // TUCNA - I really think I should take m_worldWidth - 1
  float halfWidth = m_worldWidth / 2.0f;
  float halfHeight = m_worldHeight/ 2.0f;

  float patchWidth = (float)m_worldWidth / (m_numPatchesCols - 1);
  float patchHeight = (float)m_worldHeight / (m_numPatchesRows - 1);

  float du = 1.0f / (m_numPatchesCols - 1);
  float dv = 1.0f / (m_numPatchesRows - 1);

  for(uint16_t i = 0; i < m_numPatchesRows; i++)
  {
    float z = halfHeight - i * patchHeight;

    for(uint16_t j = 0; j < m_numPatchesCols; j++)
    {
      float x = -halfWidth + j * patchWidth;

      patchVertices[i * m_numPatchesCols + j].position = XMFLOAT3(x, 0.0f, z);

      // Texture
      patchVertices[i * m_numPatchesCols + j].texCoord = XMFLOAT2(j * du, i * dv);
    }
  }

  D3D11_BUFFER_DESC vertexBufferDesc = {};
  vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
  vertexBufferDesc.ByteWidth = (UINT)(sizeof(TerrainPatch) * patchVertices.size());
  vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  vertexBufferDesc.CPUAccessFlags = 0;
  vertexBufferDesc.MiscFlags = 0;
  vertexBufferDesc.StructureByteStride = 0;

  D3D11_SUBRESOURCE_DATA vertexData = {};
  vertexData.pSysMem = patchVertices.data();

  DX::ThrowIfFailed(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer));

  // Index buffer
  uint32_t numPatchQuadFaces = (m_numPatchesRows - 1) * (m_numPatchesCols - 1);
  vector<uint16_t> indices(numPatchQuadFaces * 4);

  uint32_t k = 0;

  for(uint16_t i = 0; i < m_numPatchesRows - 1; i++)
  {
    for(uint16_t j = 0; j < m_numPatchesCols - 1; j++)
    {
      indices[k + 0] = i * m_numPatchesCols + j;
      indices[k + 1] = i * m_numPatchesCols + j + 1;

      indices[k + 2] = (i + 1) * m_numPatchesCols + j;
      indices[k + 3] = (i + 1) * m_numPatchesCols + j + 1;

      k += 4;
    }
  }

  D3D11_BUFFER_DESC indexBufferDesc = {};
  indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
  indexBufferDesc.ByteWidth = (UINT)(sizeof(uint16_t) * indices.size());
  indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  indexBufferDesc.CPUAccessFlags = 0;
  indexBufferDesc.MiscFlags = 0;
  indexBufferDesc.StructureByteStride = 0;

  D3D11_SUBRESOURCE_DATA indexData = {};
  indexData.pSysMem = indices.data();

  DX::ThrowIfFailed(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer));

  m_indexCount = indices.size();

  /*
  vector<VertexType> vertices;
  vector<uint32_t> indices;

  float h, T, B, L, R;

  for (uint32_t z = 1; z < m_worldHeight - 2; z++)
    for (uint32_t x = 1; x < m_worldWidth - 2; x++)
    {
      h = heights[((z + 0) * m_worldHeight) + (x + 0)] / 300.0f;
      T = heights[((z + 1) * m_worldHeight) + (x + 0)] / 300.0f;
      B = heights[((z - 1) * m_worldHeight) + (x + 0)] / 300.0f;
      L = heights[((z + 0) * m_worldHeight) + (x - 1)] / 300.0f;
      R = heights[((z + 0) * m_worldHeight) + (x + 1)] / 300.0f;
      vertices.push_back({
        {(float)x * m_samplingStep, h, (float)z * m_samplingStep},
        {0, 1},
        {L - R, 2.0f, T - B}
        });

      h = heights[((z + 1 + 0) * m_worldHeight) + (x + 0)] / 300.0f;
      T = heights[((z + 1 + 1) * m_worldHeight) + (x + 0)] / 300.0f;
      B = heights[((z - 1 + 1) * m_worldHeight) + (x + 0)] / 300.0f;
      L = heights[((z + 0 + 1) * m_worldHeight) + (x - 1)] / 300.0f;
      R = heights[((z + 0 + 1) * m_worldHeight) + (x + 1)] / 300.0f;
      vertices.push_back({
        {(float)x * m_samplingStep, h, (float)z * m_samplingStep + m_samplingStep},
        {0, 0},
        {L - R, 2.0f, T - B}
        });

      h = heights[((z + 1 + 0) * m_worldHeight) + (x + 1 + 0)] / 300.0f;
      T = heights[((z + 1 + 1) * m_worldHeight) + (x + 0 + 1)] / 300.0f;
      B = heights[((z - 1 + 1) * m_worldHeight) + (x + 0 + 1)] / 300.0f;
      L = heights[((z + 0 + 1) * m_worldHeight) + (x - 1 + 1)] / 300.0f;
      R = heights[((z + 0 + 1) * m_worldHeight) + (x + 1 + 1)] / 300.0f;
      vertices.push_back({
        {(float)x * m_samplingStep + m_samplingStep, h, (float)z * m_samplingStep + m_samplingStep},
        {1, 0},
        {L - R, 2.0f, T - B}
        });

      h = heights[((z + 0) * m_worldHeight) + (x + 1 + 0)] / 300.0f;
      T = heights[((z + 1) * m_worldHeight) + (x + 0 + 1)] / 300.0f;
      B = heights[((z - 1) * m_worldHeight) + (x + 0 + 1)] / 300.0f;
      L = heights[((z + 0) * m_worldHeight) + (x - 1 + 1)] / 300.0f;
      R = heights[((z + 0) * m_worldHeight) + (x + 1 + 1)] / 300.0f;
      vertices.push_back({ {(float)x * m_samplingStep + m_samplingStep, h, (float)z * m_samplingStep},
        {1, 1},
        {L - R, 2.0f, T - B}
        });

      uint32_t lastIndex = indices.empty() ? 0 : indices.back() + 1;
      indices.push_back(lastIndex + 0);
      indices.push_back(lastIndex + 1);
      indices.push_back(lastIndex + 2);
      indices.push_back(lastIndex + 0);
      indices.push_back(lastIndex + 2);
      indices.push_back(lastIndex + 3);
    }
  */
}
