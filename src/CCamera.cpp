#include "PCH.hpp"
#include "CCamera.hpp"

using namespace DirectX;

CCamera::CCamera() : m_Position(0.0f, 0.0f, 0.0f), m_Rotation(0.0f, 0.0f, 0.0f) {
}

CCamera::CCamera(const CCamera&) {
}

CCamera::~CCamera() {
}

void CCamera::SetPosition(XMFLOAT3 pos) {
	m_Position = pos;
}

void CCamera::SetRotation(XMFLOAT3 rot) {
	m_Rotation = rot;
}

XMFLOAT3 CCamera::GetPosition() {
	return m_Position;
}

XMFLOAT3 CCamera::GetRotation() {
	return m_Rotation;
}

void CCamera::Render() {
	const float pitch = XMConvertToRadians(m_Rotation.x);
	const float yaw = XMConvertToRadians(m_Rotation.y);
	const float roll = XMConvertToRadians(m_Rotation.z);
	const XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
	const XMVECTOR position = XMLoadFloat3(&m_Position);
	
	XMVECTOR lookAt = XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 1.0f));
	lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);
	lookAt = position + lookAt;
	
	XMVECTOR up = XMLoadFloat3(&XMFLOAT3(0.0f, 1.0f, 0.0f));
	up = XMVector3TransformCoord(up, rotationMatrix);

	
	m_viewMatrix = XMMatrixLookAtLH(position, lookAt, up);
}

XMMATRIX CCamera::GetViewMatrix() {
	return m_viewMatrix;
}