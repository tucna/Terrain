#pragma once

class Camera
{
public:
  explicit Camera();
  ~Camera();

  void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);
  void SetOrthographicValues(float width, float height);
  void SetPosition(float x, float y, float z);

  // Movement
  void MoveForward();
  void MoveBackward();

  // Rotation
  void TurnLeft();
  void TurnRight();
  void TurnUp();
  void TurnDown();

  const DirectX::XMMATRIX GetViewMatrix() const { return XMLoadFloat4x4(&m_viewMatrix); }
  const DirectX::XMMATRIX GetProjectionMatrix() const { return XMLoadFloat4x4(&m_projectionMatrix); }

private:
  void UpdateViewMatrix();

  const DirectX::XMVECTOR defaultForwardVector = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
  const DirectX::XMVECTOR defaultRightVector = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
  const DirectX::XMVECTOR defaultUpVector = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

  DirectX::XMFLOAT3 m_forwardVector;

  DirectX::XMFLOAT3 m_position;
  DirectX::XMFLOAT3 m_rotation;

  DirectX::XMFLOAT4X4 m_viewMatrix;
  DirectX::XMFLOAT4X4 m_projectionMatrix;
  DirectX::XMFLOAT4X4 m_orthoMatrix;

  //float m_pitch;
  //float m_yaw;
};
