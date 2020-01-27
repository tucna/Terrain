#include "pch.h"

#include "Camera.h"

using namespace DirectX;

Camera::Camera() :
  m_position(0.0f, 0.0f, 0.0f),
  m_rotation(0, 0, 0)
{
  //SetPosition(50, 5, 10);
  SetPosition(0, 5, 0);

  m_forwardVector.x = 0;
  m_forwardVector.y = 0;
  m_forwardVector.z = 1;
}

Camera::~Camera()
{
}

void Camera::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
  float fovRadians = (fovDegrees / 360.0f) * XM_2PI;

  XMStoreFloat4x4(&m_projectionMatrix, XMMatrixTranspose(XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ)));
}

void Camera::SetOrthographicValues(float width, float height)
{
  XMStoreFloat4x4(&m_orthoMatrix, XMMatrixTranspose(XMMatrixOrthographicOffCenterLH(0, width, height, 0, 0.0f, 100.0f)));
}

void Camera::SetPosition(float x, float y, float z)
{
  m_position.x = x;
  m_position.y = y;
  m_position.z = z;

  UpdateViewMatrix();
}

void Camera::MoveForward()
{
  float forwardSpeed = 0.05f; //m_frameTime * 50.0f; TUCNA handle m_frameTime

  XMStoreFloat3(&m_position, XMVector3Transform(XMLoadFloat3(&m_position), XMMatrixTranslation(m_forwardVector.x * forwardSpeed, m_forwardVector.y * forwardSpeed, m_forwardVector.z * forwardSpeed)));

  UpdateViewMatrix();
}

void Camera::MoveBackward()
{
  float forwardSpeed = 0.05f; //m_frameTime * 50.0f; TUCNA handle m_frameTime

  XMStoreFloat3(&m_position, XMVector3Transform(XMLoadFloat3(&m_position), XMMatrixTranslation(-m_forwardVector.x * forwardSpeed, -m_forwardVector.y * forwardSpeed, -m_forwardVector.z * forwardSpeed)));

  UpdateViewMatrix();
}

void Camera::TurnLeft()
{
  float leftTurnSpeed = 0.9f;

  // Update the rotation.
  m_rotation.y -= leftTurnSpeed;

  // Keep the rotation in the 0 to 360 range.
  if (m_rotation.y < 0.0f)
  {
    m_rotation.y += 360.0f;
  }

  UpdateViewMatrix();
}

void Camera::TurnRight()
{
  float rightTurnSpeed = 0.9f;

  // Update the rotation.
  m_rotation.y += rightTurnSpeed;

  // Keep the rotation in the 0 to 360 range.
  if (m_rotation.y > 360.0f)
  {
    m_rotation.y -= 360.0f;
  }

  UpdateViewMatrix();
}

void Camera::TurnUp()
{
  float upTurnSpeed = 0.9f;

  // Update the rotation.
  m_rotation.x -= upTurnSpeed;

  // Keep the rotation in the 0 to 360 range.
  if (m_rotation.x < 0.0f)
  {
    m_rotation.x += 360.0f;
  }

  UpdateViewMatrix();
}

void Camera::TurnDown()
{
  float downTurnSpeed = 0.9f;

  // Update the rotation.
  m_rotation.x += downTurnSpeed;

  // Keep the rotation in the 0 to 360 range.
  if (m_rotation.x > 360.0f)
  {
    m_rotation.x -= 360.0f;
  }

  UpdateViewMatrix();
}

void Camera::UpdateViewMatrix()
{
  float pitch = XMConvertToRadians(m_rotation.x);
  float yaw = XMConvertToRadians(m_rotation.y);

  XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, 0); // I do not need roll

  XMVECTOR upDir = XMVector3TransformCoord(defaultUpVector, camRotationMatrix);
  XMVECTOR forwardDir = XMVector3TransformCoord(defaultForwardVector, camRotationMatrix);
  XMVECTOR rightDir = XMVector3TransformCoord(defaultRightVector, camRotationMatrix);

  XMStoreFloat3(&m_forwardVector, forwardDir);

  XMVECTOR positionVector = XMLoadFloat3(&m_position);
  forwardDir += positionVector;

  XMStoreFloat4x4(&m_viewMatrix, XMMatrixTranspose(XMMatrixLookAtLH(positionVector, forwardDir, upDir)));
}
