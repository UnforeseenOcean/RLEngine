#include "PCH.hpp"
#include "CRenderer.hpp"
#include "CModel.hpp"

CRenderer::CRenderer() {
	m_D3D = nullptr;
	m_ModelManager = nullptr;
	m_Camera = nullptr;
	m_ModelShader = 0;
}

CRenderer::CRenderer(const CRenderer&) {
}

CRenderer::~CRenderer() {
}

bool CRenderer::Initialize(HWND hwnd, int screenWidth, int screenHeight, bool fullscreen) {
	m_D3D = new CDirect3D11;
	if(m_D3D == nullptr) {
		return false;
	}
	bool result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, fullscreen, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result) {
		MessageBox(hwnd, L"Could not Initialize D3D", L"Error", MB_OK);
		return false;
	}
	
	m_ModelManager = new (nothrow) CModelManager;
	if(m_ModelManager == nullptr) {
		return false;
	}
	result = m_ModelManager->Init();
	if(!result) {
		MessageBox(hwnd, L"Could not initialize the ModelManager object.", L"Error", MB_OK);
		return false;
	}

	m_Camera = new (nothrow) CCamera;
	if(m_Camera == nullptr) {
		return false;
	}
	m_Camera->SetPosition(XMFLOAT3(0.0f, 5.0f, -40.0f));
	
	m_ModelShader = new ModelShaderClass;
	if(!m_ModelShader) {
		return false;
	}
	result = m_ModelShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void CRenderer::Shutdown() {
	SAFE_SHUTDOWN(m_ModelShader);
	SAFE_SHUTDOWN(m_ModelManager);
	SAFE_DELETE(m_Camera);
	SAFE_SHUTDOWN(m_D3D);
}

CModelManager* CRenderer::GetModelManager() {
	return m_ModelManager;
}

int CRenderer::AddModel(const char* filename) {
	return m_ModelManager->AddFromFile(m_D3D->GetDevice(), filename);
}

bool CRenderer::Frame(int mX, int mY, bool leftMouseDown, bool wheelForward, bool wheelBackward) {
	static float camdist = -40.0f;
	if(wheelForward) {
		camdist += 0.5f;
	}
	else if(wheelBackward) {
		camdist -= 0.5f;
	}
	
	/*
	float a1 = -(float)x * 0.005;
	float a2 = (float)y * 0.005;

	cam->SetPosition(m33_mul_v3(m33_rotation_y( a1 ), cam->position ));

	vector3 rotation_axis = v3_normalize(v3_cross( v3_sub(cam->position, cam->target) , v3(0,1,0) ));

	cam->position = m33_mul_v3(m33_rotation_axis_angle(rotation_axis, a2 ), cam->position );*/
	static int lastmx = 0;
	static int lastmy = 0;
	int deltamx = lastmx - mX;
	int deltamy = lastmy - mY;
	lastmx = mX;
	lastmy = mY;

	XMFLOAT3 pos = m_Camera->GetPosition();
	XMFLOAT3 rot = m_Camera->GetRotation();
	pos.z = camdist;
	if(leftMouseDown) {
		pos.x += (deltamx * 0.025f);
		pos.y -= (deltamy * 0.025f);
	}
	
	m_Camera->SetPosition(pos);
	m_Camera->SetRotation(rot);

	static float i = 0.0f;
	i += 0.01f;
	m_ModelManager->GetModel(1)->SetRotation(DirectX::XMFLOAT3(0.0f, i, 0.0f));

	bool result = Render();
	if(!result) {
		return false;
	}

	return true;
}

bool CRenderer::Render() {
	m_D3D->BeginScene(0.3f, 0.3f, 0.3f, 1.0f);

	m_Camera->Render();

	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;
	XMMATRIX worldMatrix;

	int numModels = m_ModelManager->GetModelCount();
	CModel* model = nullptr;
	XMFLOAT3 rotation;
	for(int i = 0; i < numModels; ++i) {
		model = m_ModelManager->GetModel(i);
		rotation = model->GetRotation();

		viewMatrix = m_Camera->GetViewMatrix();
		m_D3D->GetProjectionMatrix(projectionMatrix);
		m_D3D->GetWorldMatrix(worldMatrix);
		
		worldMatrix = XMMatrixScalingFromVector(XMLoadFloat3(&model->GetScale()));
		worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixTranslationFromVector(XMLoadFloat3(&model->GetPosition())));				
		worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationX(rotation.x));
		worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationY(rotation.y));
		worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationZ(rotation.z));
		
		model->Render(m_D3D->GetDeviceContext());
		if(!m_ModelShader->Render(m_D3D->GetDeviceContext(), model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix)) {
			return false;
		}
	}

	m_D3D->EndScene();

	return true;
}