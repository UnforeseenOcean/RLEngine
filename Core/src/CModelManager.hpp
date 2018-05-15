#pragma once

class CModel;
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

	bool Init();
	void Shutdown();

	int AddFromFile(ID3D11Device* device, const char *filename);
	CModel* GetModel(int modelID);
	//ModelClass** GetModels();
	int GetModelCount();

private:
	CModel* m_pModel[32];
	int m_iNumMeshes;
};