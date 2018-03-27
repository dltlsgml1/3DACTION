//*****************************************************************************
//!	@file	game.cpp
//!	@brief	
//!	@note	�Q�[������
//!	@author	
//*****************************************************************************
#define TEXMAP_SIZE 4096*2


//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------

#include "game.h"

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
HANDLE				g_hEventHandle;		// �C�x���g�n���h��
bool				g_EndFlag = false;	// �I���t���O
std::thread			g_gamemainthread;	// �Q�[�����C���X���b�h

CShader				*g_pPlayerShader = nullptr;
CShader				*g_pLandShader = nullptr;
CShader				*g_pShadowShader = nullptr;

CCamera				*g_pCamera = nullptr;
CCamera				*g_pCameraFromLight = nullptr;

CDirectXGraphics	*g_DXGrobj = nullptr;		// DirectX Graphics�I�u�W�F�N�g
CGameObject			*g_pLand = nullptr;

D3DXMATRIX			g_MatLand;			//�n�`�̍s��
D3DXMATRIX			g_Scale2;			//�P�O�{�ɂ���s��
D3DXMATRIX			g_InvMatLand;		//�n�`�̋t�s��
D3DXMATRIX  g_matuv = {
	0.5f,  0.0f, 0.0f, 0.0f,
	0.0f, -0.5f, 0.0f, 0.0f,
	0.0f,  0.0f, 1.0f, 0.0f,
	0.5f,  0.5f, 0.0f, 1.0f
};


D3DXVECTOR4			inv_light_dir;
D3DXVECTOR4			inv_camera_pos;
D3DXVECTOR4			inv_pos;

// ���̐ݒ���
D3DXVECTOR4		g_light_pos;
D3DXVECTOR4		g_light_dir(0.0f, 1.0f, 0.0f, 0.0f);		// ���̕���
D3DXVECTOR4		g_diffuse(1.0f, 1.0f, 1.0f, 1.0f);			// ���s�����̐F
D3DXVECTOR4		g_ambient(0.2f, 0.2f, 0.2f, 0.2f);			// ����
D3DXVECTOR4		g_specular(1.0f, 1.0f, 1.0f, 0.2f);			// �X�y�L������

D3DXMATRIX			g_lightcameramat;
D3DXMATRIX			g_lightprojectionmat;

LPDIRECT3DTEXTURE9			g_toontexture;


CPlayer			*g_pPlayer = nullptr;
CDebug			*g_pDebug = nullptr;
CDirectInput	*g_pInput = nullptr;



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
bool GameInit(HINSTANCE hinst, HWND hwnd, int width, int height,bool fullscreen)
{
	bool sts;

	g_DXGrobj = new CDirectXGraphics();	// DirectX Graphics�I�u�W�F�N�g����
	g_pLand = new CGameObject();
	g_pPlayer = new CPlayer();
	g_pInput = new CDirectInput();
	


	sts = g_DXGrobj->Init(hwnd, fullscreen, width, height);	// DirectX Graphics�I�u�W�F�N�g������

	g_pPlayerShader = new CShader(g_DXGrobj->GetDXDevice(), "basic.hlsl", "vs_3_0", "VS", "ps_3_0", "PS");
	g_pLandShader = new CShader(g_DXGrobj->GetDXDevice(), "LandShader.hlsl", "vs_3_0", "VS", "ps_3_0", "PS");
	g_pShadowShader = new CShader(g_DXGrobj->GetDXDevice(), "shadow.hlsl", "vs_3_0", "VS", "ps_3_0", "PS");
	g_pDebug = new CDebug(g_DXGrobj->GetDXDevice());

	g_pPlayerShader->InitShader();
	g_pLandShader->InitShader();
	g_pShadowShader->InitShader();
	
	g_pCamera = new CCamera(D3DX_PI / 2,					// ����p
		(float)width / (float)height,	// �A�X�y�N�g��
		0.1f,						// �j�A�v���[��
		1000.0f);
	g_pCameraFromLight = new CCamera();

	if (!sts){
		MessageBox(hwnd, "ERROR!!", "DirectX �������G���[", MB_OK);
		return false;
	}

	sts = g_pPlayer->LoadXFile("assets/onikiri.x", g_DXGrobj->GetDXDevice());
	if (!sts)
	{
		MessageBox(hwnd, "ERROR!!", "Fail load Xfile", MB_OK);
		return false;
	}

	sts = g_pLand->LoadXFile("yuka2.x", g_DXGrobj->GetDXDevice());
	if (!sts)
	{
		MessageBox(hwnd, "ERROR!!", "Fail load Xfile", MB_OK);
		return false;
	}


	sts = CreateRenderTarget(g_DXGrobj->GetDXDevice(),
		TEXMAP_SIZE,
		TEXMAP_SIZE,
		D3DFMT_R32F,
		g_pPlayer->GetTexture(TEXTURETYPES::SHADOW),
		g_pPlayer->GetSurface(SURFACETYPES::SHADOW),
		g_pPlayer->GetSurface(SURFACETYPES::ZBUFFER));
	if (!sts) {
		MessageBox(nullptr, "ERROR!!", "ShadowTexture �����G���[", MB_OK);
		return false;
	}

	sts = CreateRenderTarget(g_DXGrobj->GetDXDevice(),
		TEXMAP_SIZE,
		TEXMAP_SIZE,
		D3DFMT_R32F,
		g_pLand->GetTexture(TEXTURETYPES::SHADOW),
		g_pLand->GetSurface(SURFACETYPES::SHADOW),
		g_pLand->GetSurface(SURFACETYPES::ZBUFFER));
	if (!sts) {
		MessageBox(nullptr, "ERROR!!", "ShadowTexture �����G���[", MB_OK);
		return false;
	}

	// �J�����ϊ��s��쐬
	
	
	// �y�o�b�t�@�L��
	g_DXGrobj->GetDXDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);

	g_DXGrobj->GetDXDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// ���C�g�L��
	g_DXGrobj->GetDXDevice()->SetRenderState(D3DRS_LIGHTING, true);


	g_pCamera->SetCameraView(D3DXVECTOR3(0.0f, 5.0f, -5.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	g_pCamera->SetCameraProjection(D3DX_PI / 2,					// ����p
		(float)SCREEN_X / (float)SCREEN_Y,	// �A�X�y�N�g��
		0.1f,						// �j�A�v���[��
		1000.0f);
	g_pCamera->SetCameraMat();
	g_pCamera->SetTransform(g_DXGrobj->GetDXDevice());
	
	// �C�x���g�n���h������
	g_hEventHandle = CreateEvent(NULL, false, false, NULL);
	if (g_hEventHandle == NULL){
		MessageBox(hwnd, "ERROR!!", "CreateEvent �G���[", MB_OK);
		return false;
	}
	// �X���b�h����(�Q�[�����C��)
	g_gamemainthread = std::thread(GameMain);

	g_pLand->AddTangentSpace(g_DXGrobj->GetDXDevice());


	g_pInput->InitInput(hinst, hwnd);


	


	D3DXMatrixScaling(&g_Scale2, 10.0f, 2.0f, 10.0f);
	
	D3DXCreateTextureFromFile(g_DXGrobj->GetDXDevice(), "ToonPaint.png", &g_toontexture);
	D3DXCreateTextureFromFile(g_DXGrobj->GetDXDevice(), "yukanormal.tga", g_pLand->GetTexture(TEXTURETYPES::NORMALMAP));
	
	return	true;
}

//==============================================================================
//!	@fn		GameInput
//!	@brief	�Q�[�����͌��m����
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void GameInput(){
	
	if (g_pInput->GetKeyboardPress(DIK_A))
	{
		g_pPlayer->UpdatePos(D3DXVECTOR3(-0.1f, 0.0f, 0.0f));
		g_pLand->UpdatePos(D3DXVECTOR3(-0.1f, 0.0f, 0.0f));
	}
	if (g_pInput->GetKeyboardPress(DIK_D))
	{
		g_pPlayer->UpdatePos(D3DXVECTOR3(+0.1f, 0.0f, 0.0f));
		g_pLand->UpdatePos(D3DXVECTOR3(+0.1f, 0.0f, 0.0f));

	}
	if (g_pInput->GetKeyboardPress(DIK_W))
	{
		g_pPlayer->UpdatePos(D3DXVECTOR3(0.0f, 0.0f, +0.1f));
		g_pLand->UpdatePos(D3DXVECTOR3(0.0f, 0.0f, +0.1f));

	}
	if (g_pInput->GetKeyboardPress(DIK_S))
	{
		g_pPlayer->UpdatePos(D3DXVECTOR3(0.0f, 0.0f, -0.1f));
		g_pLand->UpdatePos(D3DXVECTOR3(0.0f, 0.0f, -0.1f));

	}
	if (g_pInput->GetKeyboardPress(DIK_Z))
	{
		g_pPlayer->UpdatePos(D3DXVECTOR3(0.0f, +0.1f, 0.0f));
		g_pLand->UpdatePos(D3DXVECTOR3(0.0f, +0.1f, 0.0f));

	}
	if (g_pInput->GetKeyboardPress(DIK_X))
	{
		g_pPlayer->UpdatePos(D3DXVECTOR3(0.0f, -0.1f, 0.0f));
		g_pLand->UpdatePos(D3DXVECTOR3(0.0f, -0.1f, 0.0f));

	}
	if (g_pInput->GetKeyboardPress(DIK_RIGHT))
	{
		g_pPlayer->UpdateAngle(D3DXVECTOR3(0.0f, -1.1f, 0.0f));
		g_pLand->UpdateAngle(D3DXVECTOR3(0.0f, -1.1f, 0.0f));

	}
	if (g_pInput->GetKeyboardPress(DIK_LEFT))
	{
		g_pPlayer->UpdateAngle(D3DXVECTOR3(0.0f, +1.1f, 0.0f));
		g_pLand->UpdateAngle(D3DXVECTOR3(0.0f, +1.1f, 0.0f));

	}
	if (g_pInput->GetKeyboardPress(DIK_DOWN))
	{
		g_pPlayer->UpdateAngle(D3DXVECTOR3(+1.1f, 0.0f, 0.0f));
		g_pLand->UpdateAngle(D3DXVECTOR3(+1.1f, 0.0f, 0.0f));

	}
	if (g_pInput->GetKeyboardPress(DIK_UP))
	{
		g_pPlayer->UpdateAngle(D3DXVECTOR3(-1.1f, 0.0f, 0.0f));
		g_pLand->UpdateAngle(D3DXVECTOR3(-1.1f, 0.0f, 0.0f));
	}


}

//==============================================================================
//!	@fn		GameUpdate
//!	@brief	�Q�[���X�V����
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void GameUpdate(){

	//g_pCamera->UpdateCameraView(D3DXVECTOR3(0.0f, 0.01f, -0.01f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	
	D3DXVECTOR4 camerapos = D3DXVECTOR4(g_pCamera->GetCameraPos().x,
										g_pCamera->GetCameraPos().y,
										g_pCamera->GetCameraPos().z,
										0.0f);
	static int angle = 0;

	

	g_pInput->UpdateInput();



	g_light_pos.x = cosf((angle*D3DX_PI) / 180.0f) * 100;
	g_light_pos.y = 15.0f;
	g_light_pos.z = sinf((angle*D3DX_PI) / 180.0f) * 100;
	angle++;

	g_light_dir.x = (g_light_pos.x);
	g_light_dir.y = (g_light_pos.y);
	g_light_dir.z = (g_light_pos.z);


	D3DXMatrixInverse(&g_InvMatLand, nullptr, &g_pLand->GetWorldMatrix());

	D3DXVECTOR4 pos(g_pLand->GetWorldMatrix()._41, g_pLand->GetWorldMatrix()._42, g_pLand->GetWorldMatrix()._43, g_pLand->GetWorldMatrix()._44);
	
	D3DXVec4Transform(&inv_light_dir, &g_light_dir, &g_InvMatLand);
	D3DXVec4Transform(&inv_camera_pos, &camerapos, &g_InvMatLand);
	D3DXVec4Transform(&inv_pos, &pos, &g_InvMatLand);
}

//==============================================================================
//!	@fn		GameRender
//!	@brief	�Q�[���X�V����
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void GameRender(){

	g_DXGrobj->GetDXDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);
	g_DXGrobj->GetDXDevice()->BeginScene();
	HRESULT  hr;
	
	LPDIRECT3DSURFACE9 oldsurface;
	LPDIRECT3DSURFACE9 oldzbuffer;
	D3DVIEWPORT9	oldviewport;

	DrawDebug();

	GetCurrentRendertarget(g_DXGrobj->GetDXDevice(), oldsurface, oldzbuffer, oldviewport);
	CreateShadowMap(g_DXGrobj->GetDXDevice());
	SetRenderTarget(g_DXGrobj->GetDXDevice(), oldsurface, oldzbuffer, oldviewport);

	g_DXGrobj->GetDXDevice()->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &g_pPlayer->GetWorldMatrix());
	DrawPlayer();

	g_DXGrobj->GetDXDevice()->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &g_pLand->GetWorldMatrix());
	DrawLand();

	g_DXGrobj->GetDXDevice()->EndScene();	// �`��̏I����҂�

	hr = g_DXGrobj->GetDXDevice()->Present(NULL, NULL, NULL, NULL);	// �o�b�N�o�b�t�@����v���C�}���o�b�t�@�֓]��
	if (hr != D3D_OK){
		g_DXGrobj->GetDXDevice()->Reset(&g_DXGrobj->GetDXD3dpp());
	}

}

//==============================================================================
//!	@fn		GameMain
//!	@brief	�Q�[�����[�v����
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void GameMain()
{
	while (1){
		DWORD sts = WaitForSingleObject(g_hEventHandle, 1000);	// �C�x���g�t���O���Z�b�g�����̂�҂i1000ms�Ń^�C���A�E�g�j
		if (sts == WAIT_FAILED){
			break;
		}
		else if (sts == WAIT_TIMEOUT){
			if (g_EndFlag){
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
void GameExit()
{

	g_gamemainthread.join();					// �Q�[�����C���X���b�h�̏I����҂�

	CloseHandle(g_hEventHandle);				// �C�x���g�n���h���N���[�Y

	if (g_DXGrobj != nullptr){
		g_DXGrobj->Exit();						// DirectX�̃O���t�B�b�N�I�u�W�F�N�g�̏I������
	}

	if (g_DXGrobj != nullptr){						// DirectX�̃O���t�B�b�N�I�u�W�F�N�g�̉��
		delete g_DXGrobj;
		g_DXGrobj = nullptr;
	}
	g_pInput->UninitInput();
	delete g_pPlayerShader;
	delete g_pLandShader;
	delete g_pShadowShader;
	delete g_pCamera;
	g_pPlayerShader = nullptr;
	g_pLandShader = nullptr;
	g_pShadowShader = nullptr;
	g_pCamera = nullptr;
	
}

//==============================================================================
//!	@fn		GameSetEvent
//!	@brief	�Q�[�����[�v���̃C�x���g�I�u�W�F�N�g���Z�b�g����
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void GameSetEvent(){
	if (!g_EndFlag){
		SetEvent(g_hEventHandle);	// �I���t���O���Z�b�g����Ă��Ȃ���΃C�x���g���Z�b�g����
	}
}

//==============================================================================
//!	@fn		GameSetEndFlag
//!	@brief	�Q�[�����[�v�̏I���t���O���Z�b�g����
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void GameSetEndFlag(){
	g_EndFlag = true;				// �I���t���O���I���ɂ���
}



void CreateShadowMap(LPDIRECT3DDEVICE9 lpdevice) {
	
	D3DXVECTOR3 playerpos(g_pPlayer->GetWorldMatrix()._41,
						  g_pPlayer->GetWorldMatrix()._42,
						  g_pPlayer->GetWorldMatrix()._43);
	D3DXVECTOR3 up(0, 1, 0);
	D3DXVECTOR3 light_dir(g_light_dir.x, g_light_dir.y, g_light_dir.z);
	D3DXVECTOR4 camerapos;

	g_pCameraFromLight->SetCameraView(light_dir, playerpos);
	g_pCameraFromLight->SetCameraProjection(D3DX_PI / 5, 1.0f, 75.0f, 150.0f);
	g_pCameraFromLight->SetCameraMat();

	// ���_�V�F�[�_�[�ƃs�N�Z���V�F�[�_�[���Z�b�g
	lpdevice->SetVertexShader(g_pShadowShader->GetVertexShader());
	lpdevice->SetPixelShader(g_pShadowShader->GetPixelShader());

	g_pShadowShader->GetVSTable()->SetMatrix(lpdevice, "g_world", &g_pPlayer->GetWorldMatrix());
	g_pShadowShader->GetVSTable()->SetMatrix(lpdevice, "g_view", &g_pCameraFromLight->GetViewMatrix());
	g_pShadowShader->GetVSTable()->SetMatrix(lpdevice, "g_projection", &g_pCameraFromLight->GetProjectionMatrix());

	g_pShadowShader->GetPSTable()->SetVector(lpdevice, "g_diffuse", &g_diffuse);
	g_pShadowShader->GetPSTable()->SetVector(lpdevice, "g_ambient", &g_ambient);
	g_pShadowShader->GetPSTable()->SetVector(lpdevice, "g_specular", &g_specular);
	g_pShadowShader->GetPSTable()->SetVector(lpdevice, "g_light_dir", &g_light_dir);

	camerapos.x = g_light_pos.x;
	camerapos.y = g_light_pos.y;
	camerapos.z = g_light_pos.z;
	camerapos.w = 1.0f;

	g_pShadowShader->GetPSTable()->SetVector(lpdevice, "g_camerapos", &camerapos);

	// �r���[�|�[�g
	D3DVIEWPORT9 vp = { 0, 0, TEXMAP_SIZE, TEXMAP_SIZE, 0.0f, 1.0f };

	SetRenderTarget(lpdevice, *g_pPlayer->GetSurface(SURFACETYPES::SHADOW), *g_pPlayer->GetSurface(SURFACETYPES::ZBUFFER), vp);
	lpdevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 0, 0, 0), 1.0f, 0);
	g_pShadowShader->GetVSTable()->SetMatrix(lpdevice, "g_world", &g_pPlayer->GetWorldMatrix());
	g_pPlayer->Draw(lpdevice, g_pShadowShader->GetVSTable(), g_pShadowShader->GetPSTable());

	// �����_�[�^�[�Q�b�g�ݒ�
	SetRenderTarget(lpdevice, *g_pLand->GetSurface(SURFACETYPES::SHADOW), *g_pLand->GetSurface(SURFACETYPES::ZBUFFER), vp);
	// �^�[�Q�b�g�o�b�t�@�̃N���A�A�y�o�b�t�@�̃N���A
	lpdevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 0, 0, 0), 1.0f, 0);

	g_pShadowShader->GetVSTable()->SetMatrix(lpdevice, "g_world", &g_pLand->GetWorldMatrix());
	g_pLand->Draw(lpdevice, g_pShadowShader->GetVSTable(), g_pShadowShader->GetPSTable());


	g_pShadowShader->GetVSTable()->SetMatrix(lpdevice, "g_world", &g_pPlayer->GetWorldMatrix());
	g_pPlayer->Draw(lpdevice, g_pShadowShader->GetVSTable(), g_pShadowShader->GetPSTable());
}

void DrawPlayer()
{
	LPDIRECT3DDEVICE9 lpdevice = g_DXGrobj->GetDXDevice();
	D3DXVECTOR4 tempVec;
	lpdevice->SetVertexShader(g_pPlayerShader->GetVertexShader());
	lpdevice->SetPixelShader(g_pPlayerShader->GetPixelShader());

	
	g_pPlayerShader->GetVSTable()->SetMatrix(lpdevice, "g_world", &g_pPlayer->GetWorldMatrix());
	g_pPlayerShader->GetVSTable()->SetMatrix(lpdevice, "g_view", &g_pCamera->GetViewMatrix());
	g_pPlayerShader->GetVSTable()->SetMatrix(lpdevice, "g_projection", &g_pCamera->GetProjectionMatrix());

	
	tempVec.x = g_pCamera->GetCameraPos().x;
	tempVec.y = g_pCamera->GetCameraPos().y;
	tempVec.z = g_pCamera->GetCameraPos().z;
	tempVec.w = 0;

	g_pPlayerShader->GetVSTable()->SetVector(lpdevice, "g_camera_pos", &tempVec);
	g_pPlayerShader->GetPSTable()->SetVector(lpdevice, "g_camera_pos", &tempVec);

	tempVec.x = g_light_dir.x;
	tempVec.y = g_light_dir.y;
	tempVec.z = g_light_dir.z;
	tempVec.w = 1;
	g_pPlayerShader->GetVSTable()->SetVector(lpdevice, "g_light_dir", &tempVec);
	g_pPlayerShader->GetPSTable()->SetVector(lpdevice, "g_light_dir", &tempVec);
	g_pPlayerShader->GetVSTable()->SetBool(lpdevice, "drawguideline", false);
	g_pPlayerShader->GetPSTable()->SetBool(lpdevice, "drawguideline", false);

	g_pPlayerShader->GetVSTable()->SetMatrix(lpdevice, "g_lightposcamera", &g_pCameraFromLight->GetViewMatrix());
	g_pPlayerShader->GetVSTable()->SetMatrix(lpdevice, "g_lightposprojection", &g_pCameraFromLight->GetProjectionMatrix());
	g_pPlayerShader->GetVSTable()->SetMatrix(lpdevice, "g_matuv", &g_matuv);

	
	int toonindex = g_pPlayerShader->GetPSTable()->GetSamplerIndex("ToonSampler1");
	lpdevice->SetSamplerState(toonindex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	lpdevice->SetTexture(toonindex, g_toontexture);
	int index = g_pPlayerShader->GetPSTable()->GetSamplerIndex("ShadowSampler");
	lpdevice->SetTexture(index, *g_pPlayer->GetTexture(TEXTURETYPES::SHADOW));
	
	g_pPlayer->Draw(lpdevice, g_pPlayerShader->GetVSTable(), g_pPlayerShader->GetPSTable());
}

void DrawLand()
{
	LPDIRECT3DDEVICE9 lpdevice = g_DXGrobj->GetDXDevice();
	D3DXVECTOR4 tempVec;
	lpdevice->SetVertexShader(g_pLandShader->GetVertexShader());
	lpdevice->SetPixelShader(g_pLandShader->GetPixelShader());

	int normalindex = g_pLandShader->GetPSTable()->GetSamplerIndex("NormalSampler");
	lpdevice->SetSamplerState(normalindex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	lpdevice->SetTexture(normalindex, *g_pLand->GetTexture(TEXTURETYPES::NORMALMAP));


	g_pLandShader->GetVSTable()->SetMatrix(lpdevice, "g_world", &g_pLand->GetWorldMatrix());
	g_pLandShader->GetVSTable()->SetMatrix(lpdevice, "g_view", &g_pCamera->GetViewMatrix());
	g_pLandShader->GetVSTable()->SetMatrix(lpdevice, "g_projection", &g_pCamera->GetProjectionMatrix());

	g_pLandShader->GetVSTable()->SetVector(g_DXGrobj->GetDXDevice(), "g_inv_pos", &inv_pos);
	g_pLandShader->GetVSTable()->SetVector(g_DXGrobj->GetDXDevice(), "g_inv_camera_pos", &inv_camera_pos);
	g_pLandShader->GetVSTable()->SetVector(g_DXGrobj->GetDXDevice(), "g_inv_light_dir", &inv_light_dir);

	tempVec.x = g_pCamera->GetCameraPos().x;
	tempVec.y = g_pCamera->GetCameraPos().y;
	tempVec.z = g_pCamera->GetCameraPos().z;
	tempVec.w = 0;
	g_pLandShader->GetVSTable()->SetVector(lpdevice, "g_camera_pos", &tempVec);
	g_pLandShader->GetPSTable()->SetVector(lpdevice, "g_camera_pos", &tempVec);

	tempVec.x = g_light_dir.x;
	tempVec.y = g_light_dir.y;
	tempVec.z = g_light_dir.z;
	tempVec.w = 0;
	g_pLandShader->GetVSTable()->SetVector(lpdevice, "g_light_dir", &tempVec);
	g_pLandShader->GetPSTable()->SetVector(lpdevice, "g_light_dir", &tempVec);
	

	g_pLandShader->GetVSTable()->SetMatrix(lpdevice, "g_lightposcamera", &g_pCameraFromLight->GetViewMatrix());
	g_pLandShader->GetPSTable()->SetMatrix(lpdevice, "g_lightposprojection", &g_pCameraFromLight->GetProjectionMatrix());
	g_pLandShader->GetVSTable()->SetMatrix(lpdevice, "g_matuv", &g_matuv);

	int index = g_pLandShader->GetPSTable()->GetSamplerIndex("ShadowSampler");
	lpdevice->SetTexture(index, *g_pLand->GetTexture(TEXTURETYPES::SHADOW));
	g_pLand->Draw(lpdevice, g_pLandShader->GetVSTable(), g_pLandShader->GetPSTable());
}

void DrawDebug()
{
	char	str[128];
	sprintf_s(str, "%f %f %f \0", g_pPlayer->GetPos().x, g_pPlayer->GetPos().y, g_pPlayer->GetPos().z);
	g_pDebug->DrawTextA(10, 10, str);

	sprintf_s(str, "%f %f %f \0", g_pPlayer->GetAngle().x, g_pPlayer->GetAngle().y, g_pPlayer->GetAngle().z);
	g_pDebug->DrawTextA(10, 30, str);

	sprintf_s(str, "%f %f %f \0", g_pPlayer->GetWorldMatrix()._41, g_pPlayer->GetWorldMatrix()._42, g_pPlayer->GetWorldMatrix()._43);
	g_pDebug->DrawTextA(10, 50, str);
}
//******************************************************************************
//	End of file.
//******************************************************************************
