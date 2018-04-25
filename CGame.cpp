#include "CGame.h"

CGame::CGame()
{
	m_pPlayerShader = nullptr;
	m_pLandShader = nullptr;
	m_pShadowShader = nullptr;
	m_pCamera = nullptr;
	m_pCameraFromLight = nullptr;

	m_DXGrobj = nullptr;		// DirectX Graphics�I�u�W�F�N�g
	m_pLand = nullptr;
	m_pPlayer = nullptr;
	m_pDebug = nullptr;
    m_pInput = nullptr;

	m_pLight = nullptr;
	g_light_dir = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 0.0f);		// ���̕���
	g_diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);			// ���s�����̐F
	g_ambient = D3DXVECTOR4(0.2f, 0.2f, 0.2f, 0.2f);			// ����
	g_specular = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.2f);

	g_matuv = {
		0.5f,  0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f,  0.0f, 1.0f, 0.0f,
		0.5f,  0.5f, 0.0f, 1.0f
	};

}

CGame::~CGame()
{
	delete m_pPlayerShader;
	delete m_pLandShader;
	delete m_pShadowShader;
	delete m_pCamera;
	delete m_pCameraFromLight;

	delete m_DXGrobj;	
	delete m_pLand;
	delete m_pPlayer;
	delete m_pDebug;
	delete m_pInput;
	delete m_pLight;
}



//==============================================================================
//!	@fn		GameInit
//!	@brief	�Q�[����������
//!	@param	�C���X�^���X�l
//!	@param	�E�C���h�E�n���h���l
//!	@param	�E�C���h�E�T�C�Y���i�␳�ς݂̒l�j
//!	@param	�E�C���h�E�T�C�Y�����i�␳�ς݂̒l�j
//!	@param�@	�t���X�N���[���t���O�@true ; �t���X�N���[���@false : �E�C���h�E
//!	@retval	true �����@false ���s
//==============================================================================
bool CGame::GameInit(HINSTANCE hinst, HWND hwnd, int width, int height, bool fullscreen)
{
	bool sts;
	m_DXGrobj = new CDirectXGraphics();	// DirectX Graphics�I�u�W�F�N�g����
	m_pLand = new CGameObject();
	
	m_pInput = new CDirectInput();
	sts = m_DXGrobj->Init(hwnd, fullscreen, width, height);	// DirectX Graphics�I�u�W�F�N�g������

	m_pPlayerShader = new CShader(m_DXGrobj->GetDXDevice(), "basic.hlsl", "vs_3_0", "VS", "ps_3_0", "PS");
	m_pLandShader = new CShader(m_DXGrobj->GetDXDevice(), "LandShader.hlsl", "vs_3_0", "VS", "ps_3_0", "PS");
	m_pShadowShader = new CShader(m_DXGrobj->GetDXDevice(), "shadow.hlsl", "vs_3_0", "VS", "ps_3_0", "PS");
	m_pDebug = new CDebug(m_DXGrobj->GetDXDevice());

	m_pPlayerShader->InitShader();
	m_pLandShader->InitShader();
	m_pShadowShader->InitShader();

	m_pLight = new CLight(D3DXVECTOR4(0.0f, 1.0f, 0.0f,0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pCamera = new CCamera(D3DX_PI / 2,					// ����p
		(float)width / (float)height,	// �A�X�y�N�g��
		0.1f,						// �j�A�v���[��
		1000.0f);
	m_pCameraFromLight = new CCamera();
	m_pPlayer = new CPlayer(m_pCamera,m_pCameraFromLight,m_pLight);
	if (!sts) {
		MessageBox(hwnd, "ERROR!!", "DirectX �������G���[", MB_OK);
		return false;
	}

	sts = m_pPlayer->LoadXFile("assets/onikiri.x", m_DXGrobj->GetDXDevice());
	if (!sts)
	{
		MessageBox(hwnd, "ERROR!!", "Fail load Xfile", MB_OK);
		return false;
	}

	sts = m_pLand->LoadXFile("yuka2.x", m_DXGrobj->GetDXDevice());
	if (!sts)
	{
		MessageBox(hwnd, "ERROR!!", "Fail load Xfile", MB_OK);
		return false;
	}


	sts = CreateRenderTarget(m_DXGrobj->GetDXDevice(),
		TEXMAP_SIZE,
		TEXMAP_SIZE,
		D3DFMT_R32F,
		m_pPlayer->GetTexture(TEXTURETYPES::SHADOW),
		m_pPlayer->GetSurface(SURFACETYPES::SHADOW),
		m_pPlayer->GetSurface(SURFACETYPES::ZBUFFER));
	if (!sts) {
		MessageBox(nullptr, "ERROR!!", "ShadowTexture �����G���[", MB_OK);
		return false;
	}

	sts = CreateRenderTarget(m_DXGrobj->GetDXDevice(),
		TEXMAP_SIZE,
		TEXMAP_SIZE,
		D3DFMT_R32F,
		m_pLand->GetTexture(TEXTURETYPES::SHADOW),
		m_pLand->GetSurface(SURFACETYPES::SHADOW),
		m_pLand->GetSurface(SURFACETYPES::ZBUFFER));
	if (!sts) {
		MessageBox(nullptr, "ERROR!!", "ShadowTexture �����G���[", MB_OK);
		return false;
	}

	// �J�����ϊ��s��쐬


	// �y�o�b�t�@�L��
	m_DXGrobj->GetDXDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);

	m_DXGrobj->GetDXDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// ���C�g�L��
	m_DXGrobj->GetDXDevice()->SetRenderState(D3DRS_LIGHTING, true);


	m_pCamera->SetCameraView(D3DXVECTOR3(0.0f, 3.0f, -3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pCamera->SetCameraProjection(D3DX_PI / 2,					// ����p
		(float)SCREEN_X / (float)SCREEN_Y,	// �A�X�y�N�g��
		0.1f,						// �j�A�v���[��
		1000.0f);
	m_pCamera->SetCameraMat();
	m_pCamera->SetTransform(m_DXGrobj->GetDXDevice());

	// �C�x���g�n���h������
	m_hEventHandle = CreateEvent(NULL, false, false, NULL);
	if (m_hEventHandle == NULL) {
		MessageBox(hwnd, "ERROR!!", "CreateEvent �G���[", MB_OK);
		return false;
	}
	// �X���b�h����(�Q�[�����C��)

	m_GameMainThread = std::thread(&CGame::GameMain,this);
	
	m_pLand->AddTangentSpace(m_DXGrobj->GetDXDevice());

	m_pInput->InitInput(hinst, hwnd);

	D3DXCreateTextureFromFile(m_DXGrobj->GetDXDevice(), "ToonPaint.png", m_pPlayer->GetTexture(TEXTURETYPES::TOON));
	D3DXCreateTextureFromFile(m_DXGrobj->GetDXDevice(), "yukanormal.tga", m_pLand->GetTexture(TEXTURETYPES::NORMALMAP));
	return	true;
}

//==============================================================================
//!	@fn		GameInput
//!	@brief	�Q�[�����͌��m����
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void CGame::GameInput() {

	if (m_pInput->GetKeyboardPress(DIK_A))
	{
		m_pPlayer->UpdatePos(D3DXVECTOR3(-0.1f, 0.0f, 0.0f));
		m_pLand->UpdatePos(D3DXVECTOR3(-0.1f, 0.0f, 0.0f));
	}
	if (m_pInput->GetKeyboardPress(DIK_D))
	{
		m_pPlayer->UpdatePos(D3DXVECTOR3(+0.1f, 0.0f, 0.0f));
		m_pLand->UpdatePos(D3DXVECTOR3(+0.1f, 0.0f, 0.0f));

	}
	if (m_pInput->GetKeyboardPress(DIK_W))
	{
		m_pPlayer->UpdatePos(D3DXVECTOR3(0.0f, 0.0f, +0.1f));
		m_pLand->UpdatePos(D3DXVECTOR3(0.0f, 0.0f, +0.1f));

	}
	if (m_pInput->GetKeyboardPress(DIK_S))
	{
		m_pPlayer->UpdatePos(D3DXVECTOR3(0.0f, 0.0f, -0.1f));
		m_pLand->UpdatePos(D3DXVECTOR3(0.0f, 0.0f, -0.1f));

	}
	if (m_pInput->GetKeyboardPress(DIK_Z))
	{
		m_pPlayer->UpdatePos(D3DXVECTOR3(0.0f, +0.1f, 0.0f));
		m_pLand->UpdatePos(D3DXVECTOR3(0.0f, +0.1f, 0.0f));

	}
	if (m_pInput->GetKeyboardPress(DIK_X))
	{
		m_pPlayer->UpdatePos(D3DXVECTOR3(0.0f, -0.1f, 0.0f));
		m_pLand->UpdatePos(D3DXVECTOR3(0.0f, -0.1f, 0.0f));

	}
	if (m_pInput->GetKeyboardPress(DIK_RIGHT))
	{
		m_pPlayer->UpdateAngle(D3DXVECTOR3(0.0f, -1.1f, 0.0f));
		m_pLand->UpdateAngle(D3DXVECTOR3(0.0f, -1.1f, 0.0f));

	}
	if (m_pInput->GetKeyboardPress(DIK_LEFT))
	{
		m_pPlayer->UpdateAngle(D3DXVECTOR3(0.0f, +1.1f, 0.0f));
		m_pLand->UpdateAngle(D3DXVECTOR3(0.0f, +1.1f, 0.0f));

	}
	if (m_pInput->GetKeyboardPress(DIK_DOWN))
	{
		m_pPlayer->UpdateAngle(D3DXVECTOR3(+1.1f, 0.0f, 0.0f));
		m_pLand->UpdateAngle(D3DXVECTOR3(+1.1f, 0.0f, 0.0f));

	}
	if (m_pInput->GetKeyboardPress(DIK_UP))
	{
		m_pPlayer->UpdateAngle(D3DXVECTOR3(-1.1f, 0.0f, 0.0f));
		m_pLand->UpdateAngle(D3DXVECTOR3(-1.1f, 0.0f, 0.0f));
	}
}

//==============================================================================
//!	@fn		GameUpdate
//!	@brief	�Q�[���X�V����
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void CGame::GameUpdate() {

	//g_pCamera->UpdateCameraView(D3DXVECTOR3(0.0f, 0.01f, -0.01f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	D3DXVECTOR4 camerapos = D3DXVECTOR4(m_pCamera->GetCameraPos().x,
		m_pCamera->GetCameraPos().y,
		m_pCamera->GetCameraPos().z,
		0.0f);
	
	m_pInput->UpdateInput();

	static int angle = 0;
	m_pLight->SetPos(D3DXVECTOR3(cosf((angle*D3DX_PI) / 180.0f) * 100,15.0f, sinf((angle*D3DX_PI) / 180.0f) * 100));
	m_pLight->SetDirection(D3DXVECTOR4(m_pLight->GetPos(),0.0f));
	angle++;

	D3DXMatrixInverse(&m_pLand->GetInvMat(), nullptr, &m_pLand->GetWorldMatrix());
	
	D3DXVECTOR4 pos(m_pLand->GetWorldMatrix()._41, m_pLand->GetWorldMatrix()._42, m_pLand->GetWorldMatrix()._43, m_pLand->GetWorldMatrix()._44);

	D3DXVec4Transform(&m_pLight->GetInvDirection(), &m_pLight->GetDirection(), &m_pLand->GetInvMat());
	D3DXVec4Transform(&m_pCamera->GetInvCameraPos(), &camerapos, &m_pLand->GetInvMat());
	D3DXVec4Transform(&m_pLand->GetInvPos(), &pos, &m_pLand->GetInvMat());
	/*D3DXVec4Transform(&inv_light_dir, &g_light_dir, &g_InvMatLand);
	D3DXVec4Transform(&inv_camera_pos, &camerapos, &g_InvMatLand);
	D3DXVec4Transform(&inv_pos, &pos, &g_InvMatLand);*/
}

//==============================================================================
//!	@fn		GameRender
//!	@brief	�Q�[���X�V����
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void CGame::GameRender() {

	m_DXGrobj->GetDXDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);
	m_DXGrobj->GetDXDevice()->BeginScene();
	HRESULT  hr;

	LPDIRECT3DSURFACE9 oldsurface;
	LPDIRECT3DSURFACE9 oldzbuffer;
	D3DVIEWPORT9	oldviewport;

	DrawDebug();

	GetCurrentRendertarget(m_DXGrobj->GetDXDevice(), oldsurface, oldzbuffer, oldviewport);
	CreateShadowMap(m_DXGrobj->GetDXDevice());
	SetRenderTarget(m_DXGrobj->GetDXDevice(), oldsurface, oldzbuffer, oldviewport);

	m_DXGrobj->GetDXDevice()->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	m_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_pPlayer->GetWorldMatrix());
	m_pPlayer->Draw(m_DXGrobj->GetDXDevice(),
		m_pPlayerShader->GetVertexShader(),
		m_pPlayerShader->GetVSTable(),
		m_pPlayerShader->GetPixelShader(),
		m_pPlayerShader->GetPSTable());
	
	//DrawPlayer();

	m_DXGrobj->GetDXDevice()->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	m_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &m_pLand->GetWorldMatrix());
	DrawLand();

	m_DXGrobj->GetDXDevice()->EndScene();	// �`��̏I����҂�

	hr = m_DXGrobj->GetDXDevice()->Present(NULL, NULL, NULL, NULL);	// �o�b�N�o�b�t�@����v���C�}���o�b�t�@�֓]��
	if (hr != D3D_OK) {
		m_DXGrobj->GetDXDevice()->Reset(&m_DXGrobj->GetDXD3dpp());
	}

}

//==============================================================================
//!	@fn		GameMain
//!	@brief	�Q�[�����[�v����
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void CGame::GameMain()
{
	while (1) {
		DWORD sts = WaitForSingleObject(m_hEventHandle, 1000);	// �C�x���g�t���O���Z�b�g�����̂�҂i1000ms�Ń^�C���A�E�g�j
		if (sts == WAIT_FAILED) {
			break;
		}
		else if (sts == WAIT_TIMEOUT) {
			if (m_EndFlag) {
				break;
			}
			continue;
		}

		GameInput();					// ����
		GameUpdate();					// �X�V
		GameRender();					// �`��
	}
}

//==============================================================================
//!	@fn		GameExit
//!	@brief	�Q�[���I������
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void CGame::GameExit()
{

	m_GameMainThread.join();					// �Q�[�����C���X���b�h�̏I����҂�

	CloseHandle(m_hEventHandle);				// �C�x���g�n���h���N���[�Y

	if (m_DXGrobj != nullptr) {
		m_DXGrobj->Exit();						// DirectX�̃O���t�B�b�N�I�u�W�F�N�g�̏I������
	}

	if (m_DXGrobj != nullptr) {						// DirectX�̃O���t�B�b�N�I�u�W�F�N�g�̉��
		delete m_DXGrobj;
		m_DXGrobj = nullptr;
	}
	m_pInput->UninitInput();
	delete m_pPlayerShader;
	delete m_pLandShader;
	delete m_pShadowShader;
	delete m_pCamera;
	m_pPlayerShader = nullptr;
	m_pLandShader = nullptr;
	m_pShadowShader = nullptr;
	m_pCamera = nullptr;

}

//==============================================================================
//!	@fn		GameSetEvent
//!	@brief	�Q�[�����[�v���̃C�x���g�I�u�W�F�N�g���Z�b�g����
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void CGame::GameSetEvent()
{
	if (!m_EndFlag) 
	{
		SetEvent(m_hEventHandle);	// �I���t���O���Z�b�g����Ă��Ȃ���΃C�x���g���Z�b�g����
	}
}

//==============================================================================
//!	@fn		GameSetEndFlag
//!	@brief	�Q�[�����[�v�̏I���t���O���Z�b�g����
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void CGame::GameSetEndFlag() {
	m_EndFlag = true;				// �I���t���O���I���ɂ���
}

void CGame::CreateShadowMap(LPDIRECT3DDEVICE9 lpdevice) {

	D3DXVECTOR3 playerpos(m_pPlayer->GetWorldMatrix()._41,
		m_pPlayer->GetWorldMatrix()._42,
		m_pPlayer->GetWorldMatrix()._43);
	D3DXVECTOR3 up(0, 1, 0);
	D3DXVECTOR3 light_dir(m_pLight->GetDirection().x, m_pLight->GetDirection().y, m_pLight->GetDirection().z);
	D3DXVECTOR4 camerapos;

	m_pCameraFromLight->SetCameraView(light_dir, playerpos);
	m_pCameraFromLight->SetCameraProjection(D3DX_PI / 5, 1.0f, 75.0f, 150.0f);
	m_pCameraFromLight->SetCameraMat();

	// ���_�V�F�[�_�[�ƃs�N�Z���V�F�[�_�[���Z�b�g
	lpdevice->SetVertexShader(m_pShadowShader->GetVertexShader());
	lpdevice->SetPixelShader(m_pShadowShader->GetPixelShader());

	m_pShadowShader->GetVSTable()->SetMatrix(lpdevice, "g_world", &m_pPlayer->GetWorldMatrix());
	m_pShadowShader->GetVSTable()->SetMatrix(lpdevice, "g_view", &m_pCameraFromLight->GetViewMatrix());
	m_pShadowShader->GetVSTable()->SetMatrix(lpdevice, "g_projection", &m_pCameraFromLight->GetProjectionMatrix());

	m_pShadowShader->GetPSTable()->SetVector(lpdevice, "g_diffuse", &g_diffuse);
	m_pShadowShader->GetPSTable()->SetVector(lpdevice, "g_ambient", &g_ambient);
	m_pShadowShader->GetPSTable()->SetVector(lpdevice, "g_specular", &g_specular);
	m_pShadowShader->GetPSTable()->SetVector(lpdevice, "g_light_dir", &g_light_dir);


	CStaticMethod::Vec3ToVec4(camerapos, m_pLight->GetPos(), 1.0f);
	
	m_pShadowShader->GetPSTable()->SetVector(lpdevice, "g_camerapos", &camerapos);

	// �r���[�|�[�g
	D3DVIEWPORT9 vp = { 0, 0, TEXMAP_SIZE, TEXMAP_SIZE, 0.0f, 1.0f };

	SetRenderTarget(lpdevice, *m_pPlayer->GetSurface(SURFACETYPES::SHADOW), *m_pPlayer->GetSurface(SURFACETYPES::ZBUFFER), vp);
	lpdevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 0, 0, 0), 1.0f, 0);
	m_pShadowShader->GetVSTable()->SetMatrix(lpdevice, "g_world", &m_pPlayer->GetWorldMatrix());
	m_pPlayer->DrawWithShader(lpdevice, m_pShadowShader->GetVSTable(), m_pShadowShader->GetPSTable());

	// �����_�[�^�[�Q�b�g�ݒ�
	SetRenderTarget(lpdevice, *m_pLand->GetSurface(SURFACETYPES::SHADOW), *m_pLand->GetSurface(SURFACETYPES::ZBUFFER), vp);
	// �^�[�Q�b�g�o�b�t�@�̃N���A�A�y�o�b�t�@�̃N���A
	lpdevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 0, 0, 0), 1.0f, 0);

	m_pShadowShader->GetVSTable()->SetMatrix(lpdevice, "g_world", &m_pLand->GetWorldMatrix());
	m_pLand->DrawWithShader(lpdevice, m_pShadowShader->GetVSTable(), m_pShadowShader->GetPSTable());


	m_pShadowShader->GetVSTable()->SetMatrix(lpdevice, "g_world", &m_pPlayer->GetWorldMatrix());
	m_pPlayer->DrawWithShader(lpdevice, m_pShadowShader->GetVSTable(), m_pShadowShader->GetPSTable());
}

void CGame::DrawPlayer()
{
	
}

void CGame::DrawLand()
{
	LPDIRECT3DDEVICE9 lpdevice = m_DXGrobj->GetDXDevice();
	D3DXVECTOR4 tempVec;
	lpdevice->SetVertexShader(m_pLandShader->GetVertexShader());
	lpdevice->SetPixelShader(m_pLandShader->GetPixelShader());

	int normalindex = m_pLandShader->GetPSTable()->GetSamplerIndex("NormalSampler");
	lpdevice->SetSamplerState(normalindex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	lpdevice->SetTexture(normalindex, *m_pLand->GetTexture(TEXTURETYPES::NORMALMAP));


	m_pLandShader->GetVSTable()->SetMatrix(lpdevice, "g_world", &m_pLand->GetWorldMatrix());
	m_pLandShader->GetVSTable()->SetMatrix(lpdevice, "g_view", &m_pCamera->GetViewMatrix());
	m_pLandShader->GetVSTable()->SetMatrix(lpdevice, "g_projection", &m_pCamera->GetProjectionMatrix());

	m_pLandShader->GetVSTable()->SetVector(m_DXGrobj->GetDXDevice(), "g_inv_pos", &m_pLand->GetInvPos());
	m_pLandShader->GetVSTable()->SetVector(m_DXGrobj->GetDXDevice(), "g_inv_camera_pos", &m_pCamera->GetInvCameraPos());
	m_pLandShader->GetVSTable()->SetVector(m_DXGrobj->GetDXDevice(), "g_inv_light_dir", &m_pLight->GetInvDirection());

	CStaticMethod::Vec3ToVec4(tempVec, m_pCamera->GetCameraPos(), 0.0f);

	m_pLandShader->GetVSTable()->SetVector(lpdevice, "g_camera_pos", &tempVec);
	m_pLandShader->GetPSTable()->SetVector(lpdevice, "g_camera_pos", &tempVec);

	tempVec = m_pLight->GetDirection();
	tempVec.w = 1.0f;

	m_pLandShader->GetVSTable()->SetVector(lpdevice, "g_light_dir", &tempVec);
	m_pLandShader->GetPSTable()->SetVector(lpdevice, "g_light_dir", &tempVec);


	m_pLandShader->GetVSTable()->SetMatrix(lpdevice, "g_lightposcamera", &m_pCameraFromLight->GetViewMatrix());
	m_pLandShader->GetPSTable()->SetMatrix(lpdevice, "g_lightposprojection", &m_pCameraFromLight->GetProjectionMatrix());
	m_pLandShader->GetVSTable()->SetMatrix(lpdevice, "g_matuv", &g_matuv);

	int index = m_pLandShader->GetPSTable()->GetSamplerIndex("ShadowSampler");
	lpdevice->SetTexture(index, *m_pLand->GetTexture(TEXTURETYPES::SHADOW));
	m_pLand->DrawWithShader(lpdevice, m_pLandShader->GetVSTable(), m_pLandShader->GetPSTable());
}

void CGame::DrawDebug()
{
	char	str[128];
	sprintf_s(str, "%f %f %f \0", m_pCamera->GetCameraPos().x, m_pCamera->GetCameraPos().y, m_pCamera->GetCameraPos().z);
	m_pDebug->DrawTextA(10, 50, str);

	sprintf_s(str, "%f %f %f \0", m_pCameraFromLight->GetCameraPos().x, m_pCameraFromLight->GetCameraPos().y, m_pCameraFromLight->GetCameraPos().z);
	m_pDebug->DrawTextA(10, 70, str);


}
//******************************************************************************
//	End of file.
//******************************************************************************
