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
#include <Windows.h>
#include <thread>
#include "CDirectXGraphics.h"
#include "CDirect3DXFile.h"
#include "game.h"
#include "shader.h"
#include "mathutil.h"
#include "input.h"
//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
HANDLE				g_hEventHandle;		// �C�x���g�n���h��
bool				g_EndFlag = false;	// �I���t���O
std::thread			g_gamemainthread;	// �Q�[�����C���X���b�h

CShader				*g_pShader = nullptr;

CDirectXGraphics	*g_DXGrobj = nullptr;		// DirectX Graphics�I�u�W�F�N�g
CDirect3DXFile		*g_land = nullptr;	// �w�t�@�C���I�u�W�F�N�g
CDirect3DXFile		*g_pPlayer = nullptr;

D3DXMATRIX			g_MatView;			// �J�����s��
D3DXMATRIX			g_MatProjection;	// �v���W�F�N�V�����ϊ��s��
D3DXMATRIX			g_MatPlayer;			// ���[���h�ϊ��s��
D3DXMATRIX			g_MatLand;			//�n�`�̍s��
D3DXMATRIX			g_Scale;			//�Q�{�ɂ���s��
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

LPDIRECT3DVERTEXSHADER9 g_pPlayerVS = nullptr;
LPDIRECT3DPIXELSHADER9  g_pPlayerPS = nullptr;
LPDIRECT3DVERTEXSHADER9 g_pLandVS = nullptr;
LPDIRECT3DPIXELSHADER9  g_pLandPS = nullptr;
LPDIRECT3DVERTEXSHADER9 g_pShadowVS = nullptr;
LPDIRECT3DPIXELSHADER9  g_pShadowPS = nullptr;

LPD3DXCONSTANTTABLE		g_pVSTablePlayer = nullptr;
LPD3DXCONSTANTTABLE		g_pVSTableLand = nullptr;
LPD3DXCONSTANTTABLE		g_pVSTableShadow = nullptr;
LPD3DXCONSTANTTABLE		g_pPSTablePlayer = nullptr;
LPD3DXCONSTANTTABLE		g_pPSTableLand = nullptr;
LPD3DXCONSTANTTABLE		g_pPSTableShadow = nullptr;


LPDIRECT3DTEXTURE9		g_ShadowTex = nullptr;				// ���e�V���h�E�e�N�X�`��
LPDIRECT3DSURFACE9		g_ShadowSurface = nullptr;			// ���e�V���h�E�e�N�X�`���T�[�t�F�X	
LPDIRECT3DSURFACE9		g_ShadowZbufferSurface = nullptr;		// ���e�e�N�X�`���փ����_�����O����ۂ̂y�o�b�t�@�i�[�x�o�b�t�@�j

LPDIRECT3DTEXTURE9		g_ShadowTex2 = nullptr;				// ���e�V���h�E�e�N�X�`��
LPDIRECT3DSURFACE9		g_ShadowSurface2 = nullptr;			// ���e�V���h�E�e�N�X�`���T�[�t�F�X	
LPDIRECT3DSURFACE9		g_ShadowZbufferSurface2 = nullptr;		// ���e�e�N�X�`���փ����_�����O����ۂ̂y�o�b�t�@�i�[�x�o�b�t�@�j



D3DXVECTOR3			g_angle = { 0.0f,0.0f,0.0f };
D3DXVECTOR3			g_trans = { 0.0f,0.0f,0.0f };
D3DXVECTOR4			g_camera = { 0.0f,5.0f,-5.0f,0.0f };

// ���̐ݒ���
D3DXVECTOR4		g_light_pos;
D3DXVECTOR4		g_light_dir(0.0f, 1.0f, 0.0f, 0.0f);		// ���̕���
D3DXVECTOR4		g_diffuse(1.0f, 1.0f, 1.0f, 1.0f);			// ���s�����̐F
D3DXVECTOR4		g_ambient(0.2f, 0.2f, 0.2f, 0.2f);			// ����
D3DXVECTOR4		g_specular(1.0f, 1.0f, 1.0f, 0.2f);			// �X�y�L������

D3DXMATRIX			g_lightcameramat;
D3DXMATRIX			g_lightprojectionmat;
D3DXVECTOR4			g_camerapos;

LPDIRECT3DTEXTURE9			g_toontexture;
LPDIRECT3DTEXTURE9			g_normaltexture;
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
	g_pPlayer = new CDirect3DXFile();
	g_land = new CDirect3DXFile();


	g_pShader = new CShader(g_DXGrobj->GetDXDevice(), "basic.hlsl", "vs_3_0", "VS", "ps_3_0", "PS");

	sts = g_DXGrobj->Init(hwnd, fullscreen, width, height);	// DirectX Graphics�I�u�W�F�N�g������

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
	sts = g_land->LoadXFile("yuka2.x", g_DXGrobj->GetDXDevice());
	if (!sts)
	{
		MessageBox(hwnd, "ERROR!!", "Fail load Xfile", MB_OK);
		return false;
	}
	sts = CreateRenderTarget(g_DXGrobj->GetDXDevice(), TEXMAP_SIZE, TEXMAP_SIZE, D3DFMT_R32F, &g_ShadowTex, &g_ShadowSurface, &g_ShadowZbufferSurface);
	if (!sts) {
		MessageBox(nullptr, "ERROR!!", "ShadowTexture �����G���[", MB_OK);
		return false;
	}
	sts = CreateRenderTarget(g_DXGrobj->GetDXDevice(), TEXMAP_SIZE, TEXMAP_SIZE, D3DFMT_R32F, &g_ShadowTex2, &g_ShadowSurface2, &g_ShadowZbufferSurface2);
	if (!sts) {
		MessageBox(nullptr, "ERROR!!", "ShadowTexture �����G���[", MB_OK);
		return false;
	}

	// �J�����ϊ��s��쐬
	D3DXMatrixLookAtLH(&g_MatView,
		&D3DXVECTOR3(g_camera.x,g_camera.y,g_camera.z),		// ���_
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// �����_
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));		// �����

	// �J�����s����Œ�p�C�v���C���փZ�b�g
	g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_VIEW, &g_MatView);

	// �v���W�F�N�V�����ϊ��s��쐬
	D3DXMatrixPerspectiveFovLH(&g_MatProjection,
		D3DX_PI / 2,					// ����p
		(float)width / (float)height,	// �A�X�y�N�g��
		0.1f,						// �j�A�v���[��
		1000.0f);					// �t�@�[�v���[��

	// �ˉe�ϊ��s����Œ�p�C�v���C���փZ�b�g
	g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_PROJECTION, &g_MatProjection);
	// �y�o�b�t�@�L��
	g_DXGrobj->GetDXDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);

	g_DXGrobj->GetDXDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// ���C�g�L��
	g_DXGrobj->GetDXDevice()->SetRenderState(D3DRS_LIGHTING, true);

	InitShader(g_DXGrobj->GetDXDevice(),"basic.hlsl", &g_pPlayerVS, &g_pVSTablePlayer, &g_pPlayerPS, &g_pPSTablePlayer);
	InitShader(g_DXGrobj->GetDXDevice(), "LandShader.hlsl", &g_pLandVS, &g_pVSTableLand, &g_pLandPS ,&g_pPSTableLand);
	InitShader(g_DXGrobj->GetDXDevice(), "shadow.hlsl", &g_pShadowVS,&g_pVSTableShadow, &g_pShadowPS,&g_pPSTableShadow);

	
	// �C�x���g�n���h������
	g_hEventHandle = CreateEvent(NULL, false, false, NULL);
	if (g_hEventHandle == NULL){
		MessageBox(hwnd, "ERROR!!", "CreateEvent �G���[", MB_OK);
		return false;
	}
	// �X���b�h����(�Q�[�����C��)
	g_gamemainthread = std::thread(GameMain);

	g_land->AddTangentSpace(g_DXGrobj->GetDXDevice());

	InitInput(hinst,hwnd);
	D3DXMatrixIdentity(&g_MatPlayer);
	D3DXMatrixIdentity(&g_MatLand);
	D3DXMatrixScaling(&g_MatLand, 10.0f, 1.0f, 10.0f);
	D3DXMatrixScaling(&g_Scale, 2.0f, 2.0f, 2.0f);
	D3DXMatrixScaling(&g_Scale2, 10.0f, 2.0f, 10.0f);
	
	D3DXCreateTextureFromFile(g_DXGrobj->GetDXDevice(), "ToonPaint.png", &g_toontexture);
	D3DXCreateTextureFromFile(g_DXGrobj->GetDXDevice(), "yukanormal.tga", &g_normaltexture);
	return	true;
}

//==============================================================================
//!	@fn		GameInput
//!	@brief	�Q�[�����͌��m����
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void GameInput(){
	if (GetKeyboardPress(DIK_A))
	{
		g_trans.x -= 0.1f;
	}
	if (GetKeyboardPress(DIK_D))
	{
		g_trans.x += 0.1f;
	}
	if (GetKeyboardPress(DIK_W))
	{
		g_trans.z += 0.1f;
	}
	if (GetKeyboardPress(DIK_S))
	{
		g_trans.z -= 0.1f;
	}
	if (GetKeyboardPress(DIK_Z))
	{
		g_trans.y += 0.1f;
	}
	if (GetKeyboardPress(DIK_X))
	{
		g_trans.y -= 0.1f;
	}
	if (GetKeyboardPress(DIK_RIGHT))
	{
		g_angle.y -= 1.1f;
	}
	if (GetKeyboardPress(DIK_LEFT))
	{
		g_angle.y += 1.1f;
	}
	if (GetKeyboardPress(DIK_DOWN))
	{
		g_angle.x += 1.1f;
	}
	if (GetKeyboardPress(DIK_UP))
	{
		g_angle.x -= 1.1f;
	}


}

//==============================================================================
//!	@fn		GameUpdate
//!	@brief	�Q�[���X�V����
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void GameUpdate(){
	static int angle = 0;

	UpdateInput();
	MakeWorldMatrix(g_MatPlayer, g_angle, g_trans);
	MakeWorldMatrix(g_MatLand, g_angle, g_trans);
	
	g_MatPlayer *= g_Scale;		//�T�C�Y�Q�{
	g_MatLand *= g_Scale2;		//�T�C�Y�P�O�{

	g_light_pos.x = cosf((angle*D3DX_PI) / 180.0f) * 100;
	g_light_pos.y = 15.0f;
	g_light_pos.z = sinf((angle*D3DX_PI) / 180.0f) * 100;
	angle++;

	g_light_dir.x = (g_light_pos.x);
	g_light_dir.y = (g_light_pos.y);
	g_light_dir.z = (g_light_pos.z);


	D3DXMatrixInverse(&g_InvMatLand, nullptr, &g_MatLand);

	D3DXVECTOR4 pos(g_MatLand._41, g_MatLand._42, g_MatLand._43, g_MatLand._44);

	D3DXVec4Transform(&inv_light_dir, &g_light_dir, &g_InvMatLand);
	D3DXVec4Transform(&inv_camera_pos, &g_camera, &g_InvMatLand);
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


	GetCurrentRendertarget(g_DXGrobj->GetDXDevice(), oldsurface, oldzbuffer, oldviewport);
	CreateShadowMap(g_DXGrobj->GetDXDevice());
	SetRenderTarget(g_DXGrobj->GetDXDevice(), oldsurface, oldzbuffer, oldviewport);

	g_DXGrobj->GetDXDevice()->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &g_MatPlayer);
	DrawPlayer();

	g_DXGrobj->GetDXDevice()->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_WORLD, &g_MatLand);
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
	UninitInput();
	ExitShader(&g_pPlayerVS,&g_pVSTablePlayer,&g_pPlayerPS,&g_pPSTablePlayer);
	ExitShader(&g_pLandVS, &g_pVSTableLand, &g_pLandPS, &g_pPSTableLand);
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
	
	D3DXVECTOR3 playerpos(g_MatPlayer._41, g_MatPlayer._42, g_MatPlayer._43);
	D3DXVECTOR3 up(0, 1, 0);
	D3DXVECTOR3 light_dir(g_light_dir.x, g_light_dir.y, g_light_dir.z);

	D3DXMatrixLookAtLH(&g_lightcameramat, &light_dir, &playerpos, &up);

	D3DXMatrixPerspectiveFovLH(&g_lightprojectionmat, D3DX_PI / 5, 1.0f, 75.0f, 150.0f);

	// ���_�V�F�[�_�[�ƃs�N�Z���V�F�[�_�[���Z�b�g
	lpdevice->SetVertexShader(g_pShadowVS);
	lpdevice->SetPixelShader(g_pShadowPS);

	// �萔���Z�b�g(���_�V�F�[�_�[)
	g_pVSTableShadow->SetMatrix(lpdevice, "g_world", &g_MatPlayer);
	g_pVSTableShadow->SetMatrix(lpdevice, "g_view", &g_lightcameramat);
	g_pVSTableShadow->SetMatrix(lpdevice, "g_projection", &g_lightprojectionmat);

	g_pPSTableShadow->SetVector(lpdevice, "g_diffuse", &g_diffuse);
	g_pPSTableShadow->SetVector(lpdevice, "g_ambient", &g_ambient);
	g_pPSTableShadow->SetVector(lpdevice, "g_specular", &g_specular);
	g_pPSTableShadow->SetVector(lpdevice, "g_light_dir", &g_light_dir);


	g_camera.x = g_light_pos.x;
	g_camera.y = g_light_pos.y;
	g_camera.z = g_light_pos.z;
	g_camera.w = 1.0f;

	g_pPSTableShadow->SetVector(lpdevice, "g_camerapos", &g_camera);

	// �r���[�|�[�g
	D3DVIEWPORT9 vp = { 0, 0, TEXMAP_SIZE, TEXMAP_SIZE, 0.0f, 1.0f };

	SetRenderTarget(lpdevice, g_ShadowSurface2, g_ShadowZbufferSurface2, vp);
	lpdevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 0, 0, 0), 1.0f, 0);
	g_pVSTableShadow->SetMatrix(lpdevice, "g_world", &g_MatPlayer);
	g_pPlayer->Draw(lpdevice, g_pVSTableShadow, g_pPSTableShadow);

	// �����_�[�^�[�Q�b�g�ݒ�
	SetRenderTarget(lpdevice, g_ShadowSurface, g_ShadowZbufferSurface, vp);
	// �^�[�Q�b�g�o�b�t�@�̃N���A�A�y�o�b�t�@�̃N���A
	lpdevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 0, 0, 0), 1.0f, 0);

	g_pVSTableShadow->SetMatrix(lpdevice, "g_world", &g_MatLand);
	g_land->Draw(lpdevice, g_pVSTableShadow, g_pPSTableShadow);


	g_pVSTableShadow->SetMatrix(lpdevice, "g_world", &g_MatPlayer);
	g_pPlayer->Draw(lpdevice, g_pVSTableShadow, g_pPSTableShadow);

	// �n�`�`��
}

void DrawPlayer()
{
	LPDIRECT3DDEVICE9 lpdevice = g_DXGrobj->GetDXDevice();
	D3DXVECTOR4 tempVec;
	lpdevice->SetVertexShader(g_pPlayerVS);
	lpdevice->SetPixelShader(g_pPlayerPS);

	
	g_pVSTablePlayer->SetMatrix(lpdevice, "g_world", &g_MatPlayer);
	g_pVSTablePlayer->SetMatrix(lpdevice, "g_view", &g_MatView);
	g_pVSTablePlayer->SetMatrix(lpdevice, "g_projection", &g_MatProjection);

	tempVec.x = g_camera.x;
	tempVec.y = g_camera.y;
	tempVec.z = g_camera.z;
	tempVec.w = 0;
	g_pVSTablePlayer->SetVector(lpdevice, "g_camera_pos", &tempVec);
	g_pPSTablePlayer->SetVector(lpdevice, "g_camera_pos", &tempVec);

	tempVec.x = g_light_dir.x;
	tempVec.y = g_light_dir.y;
	tempVec.z = g_light_dir.z;
	tempVec.w = 1;
	g_pVSTablePlayer->SetVector(lpdevice, "g_light_dir", &tempVec);
	g_pPSTablePlayer->SetVector(lpdevice, "g_light_dir", &tempVec);
	g_pVSTablePlayer->SetBool(lpdevice, "drawguideline", false);
	g_pPSTablePlayer->SetBool(lpdevice, "drawguideline", false);

	g_pVSTablePlayer->SetMatrix(lpdevice, "g_lightposcamera", &g_lightcameramat);
	g_pVSTablePlayer->SetMatrix(lpdevice, "g_lightposprojection", &g_lightprojectionmat);
	g_pVSTablePlayer->SetMatrix(lpdevice, "g_matuv", &g_matuv);

	
	int toonindex = g_pPSTablePlayer->GetSamplerIndex("ToonSampler1");
	lpdevice->SetSamplerState(toonindex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	lpdevice->SetTexture(toonindex, g_toontexture);
	int index = g_pPSTablePlayer->GetSamplerIndex("ShadowSampler");
	lpdevice->SetTexture(index, g_ShadowTex2);
	g_pPlayer->Draw(lpdevice, g_pVSTablePlayer, g_pPSTablePlayer);
}

void DrawLand()
{
	LPDIRECT3DDEVICE9 lpdevice = g_DXGrobj->GetDXDevice();
	D3DXVECTOR4 tempVec;
	lpdevice->SetVertexShader(g_pLandVS);
	lpdevice->SetPixelShader(g_pLandPS);

	int normalindex = g_pPSTableLand->GetSamplerIndex("NormalSampler");
	lpdevice->SetSamplerState(normalindex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	lpdevice->SetTexture(normalindex, g_normaltexture);

	g_pVSTableLand->SetMatrix(lpdevice, "g_world", &g_MatLand);
	g_pVSTableLand->SetMatrix(lpdevice, "g_view", &g_MatView);
	g_pVSTableLand->SetMatrix(lpdevice, "g_projection", &g_MatProjection);

	g_pVSTableLand->SetVector(g_DXGrobj->GetDXDevice(), "g_inv_pos", &inv_pos);
	g_pVSTableLand->SetVector(g_DXGrobj->GetDXDevice(), "g_inv_camera_pos", &inv_camera_pos);
	g_pVSTableLand->SetVector(g_DXGrobj->GetDXDevice(), "g_inv_light_dir", &inv_light_dir);

	tempVec.x = g_camera.x;
	tempVec.y = g_camera.y;
	tempVec.z = g_camera.z;
	tempVec.w = 0;
	g_pVSTableLand->SetVector(lpdevice, "g_camera_pos", &tempVec);
	g_pPSTableLand->SetVector(lpdevice, "g_camera_pos", &tempVec);

	tempVec.x = g_light_dir.x;
	tempVec.y = g_light_dir.y;
	tempVec.z = g_light_dir.z;
	tempVec.w = 0;
	g_pVSTableLand->SetVector(lpdevice, "g_light_dir", &tempVec);
	g_pPSTableLand->SetVector(lpdevice, "g_light_dir", &tempVec);
	

	g_pVSTableLand->SetMatrix(lpdevice, "g_lightposcamera", &g_lightcameramat);
	g_pPSTableLand->SetMatrix(lpdevice, "g_lightposprojection", &g_lightprojectionmat);
	g_pVSTableLand->SetMatrix(lpdevice, "g_matuv", &g_matuv);

	int index = g_pPSTableLand->GetSamplerIndex("ShadowSampler");
	lpdevice->SetTexture(index, g_ShadowTex);
	g_land->Draw(lpdevice, g_pVSTableLand, g_pPSTableLand);
}
//******************************************************************************
//	End of file.
//******************************************************************************