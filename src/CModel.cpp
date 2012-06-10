#include "PCH.hpp"
#include "CModel.hpp"
#include "Lexer.hpp"

#include <vector>
#include <fstream>
#include <iostream>
using std::ifstream;
using namespace std;

using namespace DirectX;

ModelClass::ModelClass() {
	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;
	m_Mesh = nullptr;
}

ModelClass::ModelClass(const ModelClass& other) {
}

ModelClass::~ModelClass() {
}

bool ModelClass::Initialize(ID3D11Device* device, const char* modelFileName) {
	bool result;

	result = LoadObj(modelFileName);
	if(!result) {
		return false;
	}

	result = InitializeBuffers(device);
	if(!result) {
		return false;
	}

	return true;
}

void ModelClass::Shutdown() {
	ShutdownBuffers();
	
	ReleaseModel();

	return;
}

void ModelClass::Render(ID3D11DeviceContext* deviceContext) {
	RenderBuffers(deviceContext);

	return;
}

int ModelClass::GetIndexCount() {
	return m_indexCount;
}

bool ModelClass::InitializeBuffers(ID3D11Device* device) {	
	VertexType* vertices = new (nothrow) VertexType[m_vertexCount];
	if(vertices == nullptr) {
		return false;
	}

	unsigned long* indices = new (nothrow) unsigned long[m_indexCount];
	if(indices == nullptr) {
		return false;
	}

	for(int i = 0; i < m_vertexCount; ++i) {
		vertices[i].position = XMFLOAT3(m_Mesh[i].x, m_Mesh[i].y, m_Mesh[i].z);
		vertices[i].color = XMFLOAT4(m_Mesh[i].nx, m_Mesh[i].ny, m_Mesh[i].nz, 1.0f);

		indices[i] = i;
	}


	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

    vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

    HRESULT result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result)) {
		return false;
	}

    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

    indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result)) {
		return false;
	}

	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);

	return true;
}

void ModelClass::ShutdownBuffers() {
	SAFE_RELEASE(m_indexBuffer);
	SAFE_RELEASE(m_vertexBuffer);

	return;
}

void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext) {
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;
    
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool ModelClass::LoadObj(const char* filename) {
	typedef struct {
		float x, y, z;
	}VertType;

	struct FaceType {
		int vIndex1, vIndex2, vIndex3;
		int tIndex1, tIndex2, tIndex3;
		int nIndex1, nIndex2, nIndex3;
	};
	
	int vertexCount = 0;
	int textureCount = 0;
	int normalCount = 0;
	int faceCount = 0;
	
	vector<VertType> vertices;
	vector<VertType> texcoords;
	vector<VertType> normals;
	vector<FaceType> faces;

	Lexer lexer;
	bool result = lexer.LoadFile(filename);
	if(!result) {
		return false;
	}
	else {
		lexer.SetCommentStrings("#");
		lexer.SetSingleTokenChars("/");

		const Token* token = nullptr;
		const char* string = nullptr;

		while((token = lexer.ReadToken()) != nullptr) {
			string = token->GetString();
			if(strncmp(string, "#", 1) == 0) {
				Console::Print(string);
			}

			if(strcmp(string, "v") == 0) {
				VertType verticie;
				verticie.x = lexer.ReadFloat();
				verticie.y = lexer.ReadFloat();
				verticie.z = lexer.ReadFloat();
				verticie.z = verticie.z * -1.0f;
				vertices.push_back(verticie);

				vertexCount++;
			}
			else if(strcmp(string, "vt") == 0) {
				VertType texcoord;
				texcoord.x = lexer.ReadFloat();
				texcoord.y = lexer.ReadFloat();
				texcoord.y = 1.0f - texcoord.y;
				texcoords.push_back(texcoord);

				textureCount++;
			}
			else if(strcmp(string, "vn") == 0) {				
				VertType normal;
				normal.x = lexer.ReadFloat();
				normal.y = lexer.ReadFloat();
				normal.z = lexer.ReadFloat();
				normal.z = normal.z * -1.0f;
				normals.push_back(normal);

				normalCount++;
			}
			else if(strcmp(string, "f") == 0) {
				FaceType face;
				face.vIndex3 = lexer.ReadInt();
				if(!lexer.CheckToken("/")) {
						return false;
				}
				face.tIndex3 = lexer.ReadInt();
				if(!lexer.CheckToken("/")) {
						return false;
				}
				face.nIndex3 = lexer.ReadInt();

				face.vIndex2 = lexer.ReadInt();
				if(!lexer.CheckToken("/")) {
						return false;
				}
				face.tIndex2 = lexer.ReadInt();
				if(!lexer.CheckToken("/")) {
						return false;
				}
				face.nIndex2 = lexer.ReadInt();
				
				face.vIndex1 = lexer.ReadInt();
				if(!lexer.CheckToken("/")) {
						return false;
				}
				face.tIndex1 = lexer.ReadInt();
				if(!lexer.CheckToken("/")) {
						return false;
				}
				face.nIndex1 = lexer.ReadInt();

				faces.push_back(face);
				faceCount++;
			}
		}
	}

	m_indexCount = m_vertexCount = faceCount * 3;

	m_Mesh = new MeshType[m_vertexCount];
	if(m_Mesh == nullptr) {
		return false;
	}
	
	int vIndex, tIndex, nIndex;
	int tmp = 0;
	for(int i = 0; i < faceCount; ++i)
	{
		vIndex = faces[i].vIndex1 - 1;
		tIndex = faces[i].tIndex1 - 1;
		nIndex = faces[i].nIndex1 - 1;
				
		m_Mesh[tmp].x = vertices[vIndex].x;
		m_Mesh[tmp].y = vertices[vIndex].y;
		m_Mesh[tmp].z = vertices[vIndex].z;
		m_Mesh[tmp].tu = texcoords[tIndex].x;
		m_Mesh[tmp].tv = texcoords[tIndex].y;
		m_Mesh[tmp].nx = normals[nIndex].x;
		m_Mesh[tmp].ny = normals[nIndex].y;
		m_Mesh[tmp].nz = normals[nIndex].z;
		++tmp;


		vIndex = faces[i].vIndex2 - 1;
		tIndex = faces[i].tIndex2 - 1;
		nIndex = faces[i].nIndex2 - 1;
				
		m_Mesh[tmp].x = vertices[vIndex].x;
		m_Mesh[tmp].y = vertices[vIndex].y;
		m_Mesh[tmp].z = vertices[vIndex].z;
		m_Mesh[tmp].tu = texcoords[tIndex].x;
		m_Mesh[tmp].tv = texcoords[tIndex].y;
		m_Mesh[tmp].nx = normals[nIndex].x;
		m_Mesh[tmp].ny = normals[nIndex].y;
		m_Mesh[tmp].nz = normals[nIndex].z;
		++tmp;


		vIndex = faces[i].vIndex3 - 1;
		tIndex = faces[i].tIndex3 - 1;
		nIndex = faces[i].nIndex3 - 1;

		m_Mesh[tmp].x = vertices[vIndex].x;
		m_Mesh[tmp].y = vertices[vIndex].y;
		m_Mesh[tmp].z = vertices[vIndex].z;
		m_Mesh[tmp].tu = texcoords[tIndex].x;
		m_Mesh[tmp].tv = texcoords[tIndex].y;
		m_Mesh[tmp].nx = normals[nIndex].x;
		m_Mesh[tmp].ny = normals[nIndex].y;
		m_Mesh[tmp].nz = normals[nIndex].z;
		++tmp;
	}

	return true;
}

void ModelClass::ReleaseModel() {
	SAFE_DELETE(m_Mesh);

	return;
}