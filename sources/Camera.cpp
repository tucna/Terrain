#include "pch.h"

#include "Camera.h"

using namespace DirectX;

Camera::Camera() :
  m_position(0.0f, 0.0f, 0.0f),
  m_rotation(0, 0, 0),
  m_lookAt(0,0,0)
{
  SetPosition(50, 5, 10);
  SetLookAtPos(0, 0, 1);
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

void Camera::SetRotation(float x, float y, float z)
{
  m_rotation.x = x;
  m_rotation.y = y;
  m_rotation.z = z;

  UpdateViewMatrix();
}

// TUCNA - whole method is not necessary?
void Camera::SetLookAtPos(float x, float y, float z)
{
  XMFLOAT3 lookAtPos = {};

  lookAtPos.x = m_position.x - x;
  lookAtPos.y = m_position.y - y;
  lookAtPos.z = m_position.z - z;

  float pitch = 0.0f;
  if (lookAtPos.y != 0.0f)
  {
    const float distance = sqrt(lookAtPos.x * lookAtPos.x + lookAtPos.z * lookAtPos.z);

    pitch = atan(lookAtPos.y / distance);
  }

  float yaw = 0.0f;
  if (lookAtPos.x != 0.0f)
    yaw = atan(lookAtPos.x / lookAtPos.z);

  if (lookAtPos.z > 0)
    yaw += XM_PI;

  SetRotation(pitch, yaw, 0.0f);
}

void Camera::AdjustPosition(float x, float y, float z)
{
  m_position.x += x;
  m_position.y += y;
  m_position.z += z;

  UpdateViewMatrix();
}

void Camera::MoveForward()
{
  float forwardSpeed = 0.05f; //m_frameTime * 50.0f; TUCNA handle m_frameTime

    // Convert degrees to radians.
  float radians = m_rotation.y * 0.0174532925f; // TUCNA what is this number?

  // Update the position.
  m_position.x += sinf(radians) * forwardSpeed;
  m_position.z += cosf(radians) * forwardSpeed;

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

void Camera::UpdateViewMatrix()
{
  // Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
  float pitch = m_rotation.x * 0.0174532925f;
  float yaw = m_rotation.y * 0.0174532925f;
  float roll = m_rotation.z * 0.0174532925f;

  XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
  XMVECTOR camTarget = XMVector3TransformCoord(forwardVector, camRotationMatrix);
  XMVECTOR positionVector = XMLoadFloat3(&m_position);
  XMVECTOR upDir = XMVector3TransformCoord(upVector, camRotationMatrix);

  camTarget += positionVector;

  XMStoreFloat4x4(&m_viewMatrix, XMMatrixTranspose(XMMatrixLookAtLH(positionVector, camTarget, upDir)));
}
