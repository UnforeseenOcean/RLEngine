#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

class CModel {
private:
	struct VertexType {
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};

	struct MeshType {
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	CModel();
	CModel(const CModel&);
	~CModel();

	bool Initialize(ID3D11Device* device, const char* modelFileName);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount();

	void SetPosition(DirectX::XMFLOAT3 pos);
	void SetRotation(DirectX::XMFLOAT3 rot);
	void SetScale(DirectX::XMFLOAT3 scale);

	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetRotation();
	DirectX::XMFLOAT3 GetScale();

private:
	bool InitializeBuffers(ID3D11Device* device);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* deviceContext);
	bool LoadObj(const char* filename);
	void ReleaseModel();

	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;
	MeshType* m_Mesh;
	int m_vertexCount, m_indexCount;
	DirectX::XMFLOAT3 m_Position;
	DirectX::XMFLOAT3 m_Rotation;
	DirectX::XMFLOAT3 m_Scale;
};