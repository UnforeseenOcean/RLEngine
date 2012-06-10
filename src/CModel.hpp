#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

class ModelClass {
private:
	struct VertexType {
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};

	struct MeshType {
		float x;
		float y;
		float z;
		float tu;
		float tv;
		float nx;
		float ny;
		float nz;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device* device, const char* modelFileName);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount();

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
};