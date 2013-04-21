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
	const XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_Rotation.x), XMConvertToRadians(m_Rotation.y), XMConvertToRadians(m_Rotation.z));
	const XMVECTOR position = XMLoadFloat3(&m_Position);
	
	const XMFLOAT3 lookAtTemp(0.0f, 0.0f, 1.0f);
	XMVECTOR lookAt = XMLoadFloat3(&lookAtTemp);
	lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);
	lookAt = position + lookAt;
	
	const XMFLOAT3 upTemp(0.0f, 1.0f, 0.0f);
	XMVECTOR up = XMLoadFloat3(&upTemp);
	up = XMVector3TransformCoord(up, rotationMatrix);

	XMStoreFloat4x4(&m_viewMatrix, XMMatrixLookAtLH(position, lookAt, up));
}

XMMATRIX CCamera::GetViewMatrix() {
	return XMLoadFloat4x4(&m_viewMatrix);
}