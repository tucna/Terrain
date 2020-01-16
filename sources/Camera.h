#pragma once

class Camera
{
public:
  explicit Camera();
  ~Camera();

  void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);
  void SetOrthographicValues(float width, float height);
  void SetPosition(float x, float y, float z);
  void SetRotation(float x, float y, float z);
  void SetLookAtPos(float x, float y, float z);

  void AdjustPosition(float x, float y, float z);

  void MoveForward();
  void TurnLeft();
  void TurnRight();

  const DirectX::XMMATRIX GetViewMatrix() const { return XMLoadFloat4x4(&m_viewMatrix); }
  const DirectX::XMMATRIX GetProjectionMatrix() const { return XMLoadFloat4x4(&m_projectionMatrix); }

private:
  void UpdateViewMatrix();

  const DirectX::XMVECTOR forwardVector = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
  const DirectX::XMVECTOR upVector = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

  DirectX::XMFLOAT3 m_position;
  DirectX::XMFLOAT3 m_rotation;
  DirectX::XMFLOAT3 m_lookAt; // TUCNA - not needed

  DirectX::XMFLOAT4X4 m_viewMatrix;
  DirectX::XMFLOAT4X4 m_projectionMatrix;
  DirectX::XMFLOAT4X4 m_orthoMatrix;
};
