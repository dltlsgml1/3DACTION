//*****************************************************************************
//!	@file	game.cpp
//!	@brief	
//!	@note	ゲーム処理
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
// グローバル変数
//-----------------------------------------------------------------------------
HANDLE				g_hEventHandle;		// イベントハンドル
bool				g_EndFlag = false;	// 終了フラグ
std::thread			g_gamemainthread;	// ゲームメインスレッド

CShader				*g_pPlayerShader = nullptr;
CShader				*g_pLandShader = nullptr;
CShader				*g_pShadowShader = nullptr;


CDirectXGraphics	*g_DXGrobj = nullptr;		// DirectX Graphicsオブジェクト
CDirect3DXFile		*g_land = nullptr;	// Ｘファイルオブジェクト
CDirect3DXFile		*g_pPlayer = nullptr;

D3DXMATRIX			g_MatView;			// カメラ行列
D3DXMATRIX			g_MatProjection;	// プロジェクション変換行列
D3DXMATRIX			g_MatPlayer;			// ワールド変換行列
D3DXMATRIX			g_MatLand;			//地形の行列
D3DXMATRIX			g_Scale;			//２倍にする行列
D3DXMATRIX			g_Scale2;			//１０倍にする行列
D3DXMATRIX			g_InvMatLand;		//地形の逆行列
D3DXMATRIX  g_matuv = {
	0.5f,  0.0f, 0.0f, 0.0f,
	0.0f, -0.5f, 0.0f, 0.0f,
	0.0f,  0.0f, 1.0f, 0.0f,
	0.5f,  0.5f, 0.0f, 1.0f
};


D3DXVECTOR4			inv_light_dir;
D3DXVECTOR4			inv_camera_pos;
D3DXVECTOR4			inv_pos;


LPDIRECT3DTEXTURE9		g_ShadowTex = nullptr;				// 投影シャドウテクスチャ
LPDIRECT3DSURFACE9		g_ShadowSurface = nullptr;			// 投影シャドウテクスチャサーフェス	
LPDIRECT3DSURFACE9		g_ShadowZbufferSurface = nullptr;		// 投影テクスチャへレンダリングする際のＺバッファ（深度バッファ）

LPDIRECT3DTEXTURE9		g_ShadowTex2 = nullptr;				// 投影シャドウテクスチャ
LPDIRECT3DSURFACE9		g_ShadowSurface2 = nullptr;			// 投影シャドウテクスチャサーフェス	
LPDIRECT3DSURFACE9		g_ShadowZbufferSurface2 = nullptr;		// 投影テクスチャへレンダリングする際のＺバッファ（深度バッファ）

D3DXVECTOR3			g_angle = { 0.0f,0.0f,0.0f };
D3DXVECTOR3			g_trans = { 0.0f,0.0f,0.0f };
D3DXVECTOR4			g_camera = { 0.0f,5.0f,-5.0f,0.0f };

// 光の設定情報
D3DXVECTOR4		g_light_pos;
D3DXVECTOR4		g_light_dir(0.0f, 1.0f, 0.0f, 0.0f);		// 光の方向
D3DXVECTOR4		g_diffuse(1.0f, 1.0f, 1.0f, 1.0f);			// 平行光源の色
D3DXVECTOR4		g_ambient(0.2f, 0.2f, 0.2f, 0.2f);			// 環境光
D3DXVECTOR4		g_specular(1.0f, 1.0f, 1.0f, 0.2f);			// スペキュラ光

D3DXMATRIX			g_lightcameramat;
D3DXMATRIX			g_lightprojectionmat;
D3DXVECTOR4			g_camerapos;

LPDIRECT3DTEXTURE9			g_toontexture;
LPDIRECT3DTEXTURE9			g_normaltexture;
//==============================================================================
//!	@fn		GameInit
//!	@brief	ゲーム初期処理
//!	@param	インスタンス値
//!	@param	ウインドウハンドル値
//!	@param	ウインドウサイズ幅（補正済みの値）
//!	@param	ウインドウサイズ高さ（補正済みの値）
//!	@param　	フルスクリーンフラグ　true ; フルスクリーン　false : ウインドウ
//!	@retval	true 成功　false 失敗
//==============================================================================
bool GameInit(HINSTANCE hinst, HWND hwnd, int width, int height,bool fullscreen)
{
	bool sts;

	g_DXGrobj = new CDirectXGraphics();	// DirectX Graphicsオブジェクト生成
	g_pPlayer = new CDirect3DXFile();
	g_land = new CDirect3DXFile();



	sts = g_DXGrobj->Init(hwnd, fullscreen, width, height);	// DirectX Graphicsオブジェクト初期化

	g_pPlayerShader = new CShader(g_DXGrobj->GetDXDevice(), "basic.hlsl", "vs_3_0", "VS", "ps_3_0", "PS");
	g_pLandShader = new CShader(g_DXGrobj->GetDXDevice(), "LandShader.hlsl", "vs_3_0", "VS", "ps_3_0", "PS");
	g_pShadowShader = new CShader(g_DXGrobj->GetDXDevice(), "shadow.hlsl", "vs_3_0", "VS", "ps_3_0", "PS");

	g_pPlayerShader->InitShader();
	g_pLandShader->InitShader();
	g_pShadowShader->InitShader();
	
	if (!sts){
		MessageBox(hwnd, "ERROR!!", "DirectX 初期化エラー", MB_OK);
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
		MessageBox(nullptr, "ERROR!!", "ShadowTexture 生成エラー", MB_OK);
		return false;
	}
	sts = CreateRenderTarget(g_DXGrobj->GetDXDevice(), TEXMAP_SIZE, TEXMAP_SIZE, D3DFMT_R32F, &g_ShadowTex2, &g_ShadowSurface2, &g_ShadowZbufferSurface2);
	if (!sts) {
		MessageBox(nullptr, "ERROR!!", "ShadowTexture 生成エラー", MB_OK);
		return false;
	}

	// カメラ変換行列作成
	D3DXMatrixLookAtLH(&g_MatView,
		&D3DXVECTOR3(g_camera.x,g_camera.y,g_camera.z),		// 視点
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// 注視点
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));		// 上向き

	// カメラ行列を固定パイプラインへセット
	g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_VIEW, &g_MatView);

	// プロジェクション変換行列作成
	D3DXMatrixPerspectiveFovLH(&g_MatProjection,
		D3DX_PI / 2,					// 視野角
		(float)width / (float)height,	// アスペクト比
		0.1f,						// ニアプレーン
		1000.0f);					// ファープレーン

	// 射影変換行列を固定パイプラインへセット
	g_DXGrobj->GetDXDevice()->SetTransform(D3DTS_PROJECTION, &g_MatProjection);
	// Ｚバッファ有効
	g_DXGrobj->GetDXDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);

	g_DXGrobj->GetDXDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// ライト有効
	g_DXGrobj->GetDXDevice()->SetRenderState(D3DRS_LIGHTING, true);


	
	
	// イベントハンドル生成
	g_hEventHandle = CreateEvent(NULL, false, false, NULL);
	if (g_hEventHandle == NULL){
		MessageBox(hwnd, "ERROR!!", "CreateEvent エラー", MB_OK);
		return false;
	}
	// スレッド生成(ゲームメイン)
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

	//edited
	return	true;
}

//==============================================================================
//!	@fn		GameInput
//!	@brief	ゲーム入力検知処理
//!	@param　	
//!	@retval	なし
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
//!	@brief	ゲーム更新処理
//!	@param　	
//!	@retval	なし
//==============================================================================
void GameUpdate(){
	static int angle = 0;

	UpdateInput();
	MakeWorldMatrix(g_MatPlayer, g_angle, g_trans);
	MakeWorldMatrix(g_MatLand, g_angle, g_trans);
	
	g_MatPlayer *= g_Scale;		//サイズ２倍
	g_MatLand *= g_Scale2;		//サイズ１０倍

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
//!	@brief	ゲーム更新処理
//!	@param　	
//!	@retval	なし
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

	g_DXGrobj->GetDXDevice()->EndScene();	// 描画の終了を待つ

	hr = g_DXGrobj->GetDXDevice()->Present(NULL, NULL, NULL, NULL);	// バックバッファからプライマリバッファへ転送
	if (hr != D3D_OK){
		g_DXGrobj->GetDXDevice()->Reset(&g_DXGrobj->GetDXD3dpp());
	}

}

//==============================================================================
//!	@fn		GameMain
//!	@brief	ゲームループ処理
//!	@param　	
//!	@retval	なし
//==============================================================================
void GameMain()
{
	while (1){
		DWORD sts = WaitForSingleObject(g_hEventHandle, 1000);	// イベントフラグがセットされるのを待つ（1000msでタイムアウト）
		if (sts == WAIT_FAILED){
			break;
		}
		else if (sts == WAIT_TIMEOUT){
			if (g_EndFlag){
				break;
			}
			continue;
		}

		GameInput();					// 入力
		GameUpdate();					// 更新
		GameRender();					// 描画
	}
}

//==============================================================================
//!	@fn		GameExit
//!	@brief	ゲーム終了処理
//!	@param　	
//!	@retval	なし
//==============================================================================
void GameExit()
{

	g_gamemainthread.join();					// ゲームメインスレッドの終了を待つ

	CloseHandle(g_hEventHandle);				// イベントハンドルクローズ

	if (g_DXGrobj != nullptr){
		g_DXGrobj->Exit();						// DirectXのグラフィックオブジェクトの終了処理
	}

	if (g_DXGrobj != nullptr){						// DirectXのグラフィックオブジェクトの解放
		delete g_DXGrobj;
		g_DXGrobj = nullptr;
	}
	UninitInput();
	delete g_pPlayerShader;
	delete g_pLandShader;
	delete g_pShadowShader;
	g_pPlayerShader = nullptr;
	g_pLandShader = nullptr;
	g_pShadowShader = nullptr;
	
}

//==============================================================================
//!	@fn		GameSetEvent
//!	@brief	ゲームループ内のイベントオブジェクトをセットする
//!	@param　	
//!	@retval	なし
//==============================================================================
void GameSetEvent(){
	if (!g_EndFlag){
		SetEvent(g_hEventHandle);	// 終了フラグがセットされていなければイベントをセットする
	}
}

//==============================================================================
//!	@fn		GameSetEndFlag
//!	@brief	ゲームループの終了フラグをセットする
//!	@param　	
//!	@retval	なし
//==============================================================================
void GameSetEndFlag(){
	g_EndFlag = true;				// 終了フラグをオンにする
}



void CreateShadowMap(LPDIRECT3DDEVICE9 lpdevice) {
	
	D3DXVECTOR3 playerpos(g_MatPlayer._41, g_MatPlayer._42, g_MatPlayer._43);
	D3DXVECTOR3 up(0, 1, 0);
	D3DXVECTOR3 light_dir(g_light_dir.x, g_light_dir.y, g_light_dir.z);

	D3DXMatrixLookAtLH(&g_lightcameramat, &light_dir, &playerpos, &up);

	D3DXMatrixPerspectiveFovLH(&g_lightprojectionmat, D3DX_PI / 5, 1.0f, 75.0f, 150.0f);

	// 頂点シェーダーとピクセルシェーダーをセット
	lpdevice->SetVertexShader(g_pShadowShader->GetVertexShader());
	lpdevice->SetPixelShader(g_pShadowShader->GetPixelShader());

	// 定数をセット(頂点シェーダー)
	g_pShadowShader->GetVSTable()->SetMatrix(lpdevice, "g_world", &g_MatPlayer);
	g_pShadowShader->GetVSTable()->SetMatrix(lpdevice, "g_view", &g_lightcameramat);
	g_pShadowShader->GetVSTable()->SetMatrix(lpdevice, "g_projection", &g_lightprojectionmat);

	g_pShadowShader->GetPSTable()->SetVector(lpdevice, "g_diffuse", &g_diffuse);
	g_pShadowShader->GetPSTable()->SetVector(lpdevice, "g_ambient", &g_ambient);
	g_pShadowShader->GetPSTable()->SetVector(lpdevice, "g_specular", &g_specular);
	g_pShadowShader->GetPSTable()->SetVector(lpdevice, "g_light_dir", &g_light_dir);


	g_camera.x = g_light_pos.x;
	g_camera.y = g_light_pos.y;
	g_camera.z = g_light_pos.z;
	g_camera.w = 1.0f;

	g_pShadowShader->GetPSTable()->SetVector(lpdevice, "g_camerapos", &g_camera);

	// ビューポート
	D3DVIEWPORT9 vp = { 0, 0, TEXMAP_SIZE, TEXMAP_SIZE, 0.0f, 1.0f };

	SetRenderTarget(lpdevice, g_ShadowSurface2, g_ShadowZbufferSurface2, vp);
	lpdevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 0, 0, 0), 1.0f, 0);
	g_pShadowShader->GetVSTable()->SetMatrix(lpdevice, "g_world", &g_MatPlayer);
	g_pPlayer->Draw(lpdevice, g_pShadowShader->GetVSTable(), g_pShadowShader->GetPSTable());

	// レンダーターゲット設定
	SetRenderTarget(lpdevice, g_ShadowSurface, g_ShadowZbufferSurface, vp);
	// ターゲットバッファのクリア、Ｚバッファのクリア
	lpdevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 0, 0, 0), 1.0f, 0);

	g_pShadowShader->GetVSTable()->SetMatrix(lpdevice, "g_world", &g_MatLand);
	g_land->Draw(lpdevice, g_pShadowShader->GetVSTable(), g_pShadowShader->GetPSTable());


	g_pShadowShader->GetVSTable()->SetMatrix(lpdevice, "g_world", &g_MatPlayer);
	g_pPlayer->Draw(lpdevice, g_pShadowShader->GetVSTable(), g_pShadowShader->GetPSTable());

	// 地形描画
}

void DrawPlayer()
{
	LPDIRECT3DDEVICE9 lpdevice = g_DXGrobj->GetDXDevice();
	D3DXVECTOR4 tempVec;
	lpdevice->SetVertexShader(g_pPlayerShader->GetVertexShader());
	lpdevice->SetPixelShader(g_pPlayerShader->GetPixelShader());

	
	g_pPlayerShader->GetVSTable()->SetMatrix(lpdevice, "g_world", &g_MatPlayer);
	g_pPlayerShader->GetVSTable()->SetMatrix(lpdevice, "g_view", &g_MatView);
	g_pPlayerShader->GetVSTable()->SetMatrix(lpdevice, "g_projection", &g_MatProjection);

	tempVec.x = g_camera.x;
	tempVec.y = g_camera.y;
	tempVec.z = g_camera.z;
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

	g_pPlayerShader->GetVSTable()->SetMatrix(lpdevice, "g_lightposcamera", &g_lightcameramat);
	g_pPlayerShader->GetVSTable()->SetMatrix(lpdevice, "g_lightposprojection", &g_lightprojectionmat);
	g_pPlayerShader->GetVSTable()->SetMatrix(lpdevice, "g_matuv", &g_matuv);

	
	int toonindex = g_pPlayerShader->GetPSTable()->GetSamplerIndex("ToonSampler1");
	lpdevice->SetSamplerState(toonindex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	lpdevice->SetTexture(toonindex, g_toontexture);
	int index = g_pPlayerShader->GetPSTable()->GetSamplerIndex("ShadowSampler");
	lpdevice->SetTexture(index, g_ShadowTex2);
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
	lpdevice->SetTexture(normalindex, g_normaltexture);

	g_pLandShader->GetVSTable()->SetMatrix(lpdevice, "g_world", &g_MatLand);
	g_pLandShader->GetVSTable()->SetMatrix(lpdevice, "g_view", &g_MatView);
	g_pLandShader->GetVSTable()->SetMatrix(lpdevice, "g_projection", &g_MatProjection);

	g_pLandShader->GetVSTable()->SetVector(g_DXGrobj->GetDXDevice(), "g_inv_pos", &inv_pos);
	g_pLandShader->GetVSTable()->SetVector(g_DXGrobj->GetDXDevice(), "g_inv_camera_pos", &inv_camera_pos);
	g_pLandShader->GetVSTable()->SetVector(g_DXGrobj->GetDXDevice(), "g_inv_light_dir", &inv_light_dir);

	tempVec.x = g_camera.x;
	tempVec.y = g_camera.y;
	tempVec.z = g_camera.z;
	tempVec.w = 0;
	g_pLandShader->GetVSTable()->SetVector(lpdevice, "g_camera_pos", &tempVec);
	g_pLandShader->GetPSTable()->SetVector(lpdevice, "g_camera_pos", &tempVec);

	tempVec.x = g_light_dir.x;
	tempVec.y = g_light_dir.y;
	tempVec.z = g_light_dir.z;
	tempVec.w = 0;
	g_pLandShader->GetVSTable()->SetVector(lpdevice, "g_light_dir", &tempVec);
	g_pLandShader->GetPSTable()->SetVector(lpdevice, "g_light_dir", &tempVec);
	

	g_pLandShader->GetVSTable()->SetMatrix(lpdevice, "g_lightposcamera", &g_lightcameramat);
	g_pLandShader->GetPSTable()->SetMatrix(lpdevice, "g_lightposprojection", &g_lightprojectionmat);
	g_pLandShader->GetVSTable()->SetMatrix(lpdevice, "g_matuv", &g_matuv);

	int index = g_pLandShader->GetPSTable()->GetSamplerIndex("ShadowSampler");
	lpdevice->SetTexture(index, g_ShadowTex);
	g_land->Draw(lpdevice, g_pLandShader->GetVSTable(), g_pLandShader->GetPSTable());
}
//******************************************************************************
//	End of file.
//******************************************************************************
