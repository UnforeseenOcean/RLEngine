#pragma once

#include <DirectXMath.h>

class CCamera {
public:
	CCamera();
	CCamera(const CCamera&);
	~CCamera();

	void SetPosition(DirectX::XMFLOAT3 pos);
	void SetRotation(DirectX::XMFLOAT3 rot);

	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetRotation();	
	DirectX::XMMATRIX GetViewMatrix();

	void Render();
private:
	DirectX::XMFLOAT3 m_Position;
	DirectX::XMFLOAT3 m_Rotation;
	DirectX::XMMATRIX m_viewMatrix;
	// AR, FoV, zNear, zFar
};