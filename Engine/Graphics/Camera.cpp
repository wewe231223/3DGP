#include "pch.h"
#include "Graphics/Camera.h"
#include "System/Input.h"


Camera::Camera()
    : m_position(DirectX::XMVectorSet(0.0f, 0.0f, -20.0f, 1.0f)),
    m_target(DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)),
    m_up(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)),
    m_pitch(0.0f),
    m_yaw(0.0f)
{
	




    FocusUpdate(0.f, 0.f);
    UpdateViewMatrix();
}

void Camera::FocusUpdate(float dx, float dy)
{
    const float rotationSpeed = 0.001f;
    
    m_yaw += dx * rotationSpeed;
    m_pitch += dy * rotationSpeed;

    

    DirectX::XMVECTOR quaternionRotation = DirectX::XMQuaternionRotationRollPitchYaw(m_pitch, m_yaw, 0.0f);
    m_forward = DirectX::XMVector3Rotate(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), quaternionRotation);
    m_up = DirectX::XMVector3Rotate(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), quaternionRotation);
    m_target = DirectX::XMVectorAdd(m_position, m_forward);
	m_right = DirectX::XMVector3Cross(m_up, m_forward);

}

void Camera::PositionUpdate(Input* input){
    if (input->GetKeyState(DIK_W) == KEY_STATE::PRESS) {
		m_position = DirectX::XMVectorAdd(m_position, DirectX::XMVectorScale(m_forward, 0.1f));
	}

    if (input->GetKeyState(DIK_S) == KEY_STATE::PRESS) {
        m_position = DirectX::XMVectorSubtract(m_position, DirectX::XMVectorScale(m_forward, 0.1f));
    }

    if (input->GetKeyState(DIK_D) == KEY_STATE::PRESS) {
		m_position = DirectX::XMVectorAdd(m_position, DirectX::XMVectorScale(m_right, 0.1f));
    }

    if (input->GetKeyState(DIK_A) == KEY_STATE::PRESS) {
        m_position = DirectX::XMVectorSubtract(m_position, DirectX::XMVectorScale(m_right, 0.1f));
    }

    m_target = DirectX::XMVectorAdd(m_position, m_forward);
}

void Camera::UpdateViewMatrix()
{
    DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(m_position, m_target, m_up);
    DirectX::XMStoreFloat4x4(&m_viewMatrix, viewMatrix);
}

DirectX::XMMATRIX Camera::GetViewMatrix() const {
    return DirectX::XMLoadFloat4x4(&m_viewMatrix);
}