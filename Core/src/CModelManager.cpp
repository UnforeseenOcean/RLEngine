#include "Core.hpp"
#include "CModelManager.hpp"
#include "CModel.hpp"

#include <d3d11.h>

CModelManager::CModelManager() : m_iNumMeshes(0) {
}

CModelManager::CModelManager(const CModelManager&) {
}

CModelManager::~CModelManager() {
}

bool CModelManager::Init() {
	return true;
}

void CModelManager::Shutdown() {
	for(int i = 0; i < m_iNumMeshes; ++i) {
		SAFE_SHUTDOWN(m_pModel[i]);
	}
}

int CModelManager::AddFromFile(ID3D11Device* device, const char *filename) {
	RL_ASSERT(m_iNumMeshes < MAX_MESHES, "Too many meshes!");
	m_pModel[m_iNumMeshes] = new (nothrow) CModel();
	if(m_pModel[m_iNumMeshes] == nullptr) {
		return -1;
	}
	if(!m_pModel[m_iNumMeshes]->Initialize(device, filename)) {
		return -1;
	}
	
	int result = m_iNumMeshes;
	++m_iNumMeshes;

	return result;
}

CModel* CModelManager::GetModel(int modelID) {
	return m_pModel[modelID];
}

int CModelManager::GetModelCount() {
	return m_iNumMeshes;
}