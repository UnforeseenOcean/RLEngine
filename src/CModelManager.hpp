#pragma once

class ModelClass;
struct ID3D11Device;

class CModelManager {
private:
	enum {
		MAX_MESHES = 32,
	};

public:
	CModelManager();
	CModelManager(const CModelManager&);
	~CModelManager();

	bool Init(ID3D11Device* device);
	void Shutdown();

	int AddFromFile(char *filename);
	ModelClass* GetModel(int modelID);

private:
	ID3D11Device* m_pDevice;	
	ModelClass* m_pModel[32];
	int m_iNumMeshes;
};