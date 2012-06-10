#pragma once

#include <DirectXMath.h>

class CCamera {
public:
	CCamera();
	CCamera(const CCamera&);
	~CCamera();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	DirectX::XMVECTOR GetPosition();
	DirectX::XMVECTOR GetRotation();

	void Render();
	void GetViewMatrix(DirectX::XMMATRIX&);

	//void RenderBaseViewMatrix();
	//void GetBaseViewMatrix(DirectX::XMMATRIX&);

private:
	float m_positionX;
	float m_positionY;
	float m_positionZ;

	float m_rotationX;
	float m_rotationY;
	float m_rotationZ;
	DirectX::XMMATRIX m_viewMatrix;
	//DirectX::XMMATRIX m_baseViewMatrix;
	// AR, FoV, zNear, zFar
};