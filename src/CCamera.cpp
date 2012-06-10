#include "PCH.hpp"
#include "CCamera.hpp"

using namespace DirectX;

CCamera::CCamera() {
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;
}

CCamera::CCamera(const CCamera&) {
}

CCamera::~CCamera() {
}

void CCamera::SetPosition(float x, float y, float z) {
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}

void CCamera::SetRotation(float x, float y, float z) {
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}

XMVECTOR CCamera::GetPosition() {
	return XMLoadFloat3(&XMFLOAT3(m_positionX, m_positionY, m_positionZ));
}

XMVECTOR CCamera::GetRotation() {
	return XMLoadFloat3(&XMFLOAT3(m_rotationX, m_rotationY, m_rotationZ));
}

void CCamera::Render() {
	XMFLOAT3 up;
	XMFLOAT3 position;
	XMFLOAT3 lookAt;
	float pitch;
	float yaw;
	float roll;
	XMMATRIX rotationMatrix;

	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;

	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	XMVECTOR lookAt2 = XMVector3TransformCoord(XMLoadFloat3(&lookAt), rotationMatrix);
	XMVECTOR up2 = XMVector3TransformCoord(XMLoadFloat3(&up), rotationMatrix);

	lookAt2 = XMLoadFloat3(&position) + lookAt2;



	m_viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&position), lookAt2, up2);

	return;
}

void CCamera::GetViewMatrix(XMMATRIX& viewMatrix) {
	viewMatrix = m_viewMatrix;
	return;
}
/*
void CCamera::RenderBaseViewMatrix() {
	D3DXVECTOR3 up;
	D3DXVECTOR3 position;
	D3DXVECTOR3 lookAt;
	float radians;

	up.x = 0.0;
	up.y = 1.0f;
	up.z = 0.0f;

	position.x = m_PositionX;
	position.y = m_PositionY;
	position.z = m_PositionZ;

	radians = m_RotationY * 0.0174532925f;

	lookAt.x = sinf(radians) + m_PositionX;
	lookAt.y = m_PositionY;
	lookAt.z = cosf(radians) + m_PositionZ;

	D3DXMatrixLookAtLH(&m_baseViewMatrix, &position, &lookAt, &up);

	return;
}

void CCamera::GetBaseViewMatrix(D3DXMATRIX& viewMatrix) {
	viewMatrix = m_baseViewMatrix;
}*/