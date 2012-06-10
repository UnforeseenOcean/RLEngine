#pragma once

#include "CDirect3D11.hpp"
#include "CCamera.hpp"
#include "CModelManager.hpp"
#include "Shader.hpp"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 100.0f;
const float SCREEN_NEAR = 1.0f;

class GraphicsClass {
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(HWND hwnd, int screenWidth, int screenHeight);
	void Shutdown();
	bool Frame(int mx, int my, bool leftMouseDown, bool wheelForward, bool wheelBackward);
	
private:
	bool Render();
	
	D3DClass* m_D3D;
	CCamera* m_Camera;
	CModelManager* m_ModelManager;
	ColorShaderClass* m_ColorShader;
};