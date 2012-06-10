#include "PCH.hpp"
#include "CModelManager.hpp"
#include "CModel.hpp"

#include <d3d11.h>

CModelManager::CModelManager() {
	m_iNumMeshes = 0;
}

CModelManager::CModelManager(const CModelManager&) {
}

CModelManager::~CModelManager() {
}

bool CModelManager::Init(ID3D11Device* device) {
	m_pDevice = device;

	return true;
}

void CModelManager::Shutdown() {
	for(int i = 0; i < m_iNumMeshes; ++i) {
		SAFE_SHUTDOWN(m_pModel[i]);
	}
}

int CModelManager::AddFromFile(char *filename) {
	RL_ASSERT(m_iNumMeshes < MAX_MESHES, "Too many meshes!");
	m_pModel[m_iNumMeshes] = new (nothrow) ModelClass();
	RL_ASSERT(m_pModel[m_iNumMeshes] != nullptr, "Ugh new[] failed");

	m_pModel[m_iNumMeshes]->Initialize(m_pDevice, filename);
	
	int result = m_iNumMeshes;
	++m_iNumMeshes;

	return result;
}

ModelClass* CModelManager::GetModel(int modelID) {
	return m_pModel[modelID];
}