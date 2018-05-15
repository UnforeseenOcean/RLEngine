#pragma once

#include "CDirect3D11.hpp"
#include "CCamera.hpp"
#include "Shader.hpp"
#include "CModelManager.hpp"

const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 100.0f;
const float SCREEN_NEAR = 1.0f;

class CRenderer {
public:
	CRenderer();
	CRenderer(const CRenderer&);
	~CRenderer();

	bool Initialize(HWND hwnd, int screenWidth, int screenHeight, bool fullscreen);
	void Shutdown();
	bool Frame(int mX, int mY, bool leftMouseDown, bool wheelForward, bool wheelBackward);

	int AddModel(const char* filename);
	CModelManager* GetModelManager();
	
private:
	bool Render();
	
	CDirect3D11* m_D3D;
	CModelManager* m_ModelManager;
	CCamera* m_Camera;
	ModelShaderClass* m_ModelShader;
};