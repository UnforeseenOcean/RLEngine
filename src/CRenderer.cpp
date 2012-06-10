#include "PCH.hpp"
#include "CRenderer.hpp"
#include "CModel.hpp"

GraphicsClass::GraphicsClass() {
	m_D3D = nullptr;
	m_Camera = nullptr;
	m_ModelManager = nullptr;
	m_ColorShader = 0;
}

GraphicsClass::GraphicsClass(const GraphicsClass&) {
}

GraphicsClass::~GraphicsClass() {
}

bool GraphicsClass::Initialize(HWND hwnd, int screenWidth, int screenHeight) {
	m_D3D = new D3DClass;
	if(m_D3D == nullptr) {
		return false;
	}

	bool result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result) {
		MessageBox(hwnd, L"Could not Initialize D3D", L"Error", MB_OK);
		return false;
	}

	m_Camera = new (nothrow) CCamera;
	if(m_Camera == nullptr) {
		return false;
	}
	m_Camera->SetPosition(0.0f, 0.0f, -3.0f);


	m_ModelManager = new (nothrow) CModelManager;
	if(m_ModelManager == nullptr) {
		return false;
	}

	m_ModelManager->Init(m_D3D->GetDevice());
	
	m_ModelManager->AddFromFile("data/models/podium.obj");
	m_ModelManager->AddFromFile("data/models/piano.obj");


	// Create the color shader object.
	m_ColorShader = new ColorShaderClass;
	if(!m_ColorShader)
	{
		return false;
	}

	// Initialize the color shader object.
	result = m_ColorShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}
	/////////////

	return true;
}

void GraphicsClass::Shutdown() {
	if(m_Camera != nullptr) {
		delete m_Camera;
		m_Camera = nullptr;
	}

	if(m_D3D != nullptr) {
		m_D3D->Shutdown();
		delete m_D3D;
	}

	return;
}

bool GraphicsClass::Frame(int mx, int my, bool leftMouseDown, bool wheelForward, bool wheelBackward) {
	static float campos = -15.0f;
	if(wheelForward) {
		campos += 0.5f;
	}
	else if(wheelBackward) {
		campos -= 0.5f;
	}


	/*
	float a1 = -(float)x * 0.005;
	float a2 = (float)y * 0.005;

	cam->SetPosition(m33_mul_v3(m33_rotation_y( a1 ), cam->position ));

	vector3 rotation_axis = v3_normalize(v3_cross( v3_sub(cam->position, cam->target) , v3(0,1,0) ));

	cam->position = m33_mul_v3(m33_rotation_axis_angle(rotation_axis, a2 ), cam->position );*/
	XMVECTOR pos = m_Camera->GetPosition();
	float x = XMVectorGetX(pos);
	float y = XMVectorGetY(pos);
	if(leftMouseDown) {
		x += (mx * 0.001f);
		y -= (my * 0.001f);
	}
	
	m_Camera->SetPosition(x, y, campos);

	bool result = Render();
	if(!result) {
		return false;
	}

	return true;
}

bool GraphicsClass::Render() {
	m_D3D->BeginScene(0.3f, 0.3f, 0.3f, 1.0f);

	m_Camera->Render();

	XMMATRIX viewMatrix;
	m_Camera->GetViewMatrix(viewMatrix);
	XMMATRIX projectionMatrix;
	m_D3D->GetProjectionMatrix(projectionMatrix);
	XMMATRIX worldMatrix;
	m_D3D->GetWorldMatrix(worldMatrix);
	XMMATRIX translateMatrix;

	ModelClass* Model = m_ModelManager->GetModel(0);
	Model->Render(m_D3D->GetDeviceContext());
	if(!m_ColorShader->Render(m_D3D->GetDeviceContext(), Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix)) {
		return false;
	}

	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	translateMatrix = XMMatrixTranslation(0.0f, 5.0f, 0.0f);
	worldMatrix = XMMatrixMultiply(worldMatrix, translateMatrix);

	Model = m_ModelManager->GetModel(1);
	Model->Render(m_D3D->GetDeviceContext());
	if(!m_ColorShader->Render(m_D3D->GetDeviceContext(), Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix)) {
		return false;
	}

	m_D3D->EndScene();

	return true;
}