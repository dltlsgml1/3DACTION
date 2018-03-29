#pragma once
#include "CPlayer.h"
#include "CGameObject.h"
#include "CStaticMethod.h"
#include "CShader.h"
#include "CDirect3DXFile.h"
#include "CDirectxGraphics.h"
#include "CCamera.h"
#include "CDebug.h"
#include "CDirectInput.h"

#include <Windows.h>
#include <thread>
#define		SCREEN_X		1280
#define		SCREEN_Y		720
#define		RADIAN(x)		D3DX_PI*(x)/180.0f
#define		TEXMAP_SIZE		4096*2
class CGame
{

private:

	HANDLE				g_hEventHandle;		// イベントハンドル
	bool				g_EndFlag = false;	// 終了フラグ
	std::thread			g_gamemainthread;	// ゲームメインスレッド

	CShader				*g_pPlayerShader = nullptr;
	CShader				*g_pLandShader = nullptr;
	CShader				*g_pShadowShader = nullptr;

	CCamera				*g_pCamera = nullptr;
	CCamera				*g_pCameraFromLight = nullptr;

	CDirectXGraphics	*g_DXGrobj = nullptr;		// DirectX Graphicsオブジェクト
	CGameObject			*g_pLand = nullptr;

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

	// 光の設定情報
	D3DXVECTOR4		g_light_pos;
	D3DXVECTOR4		g_light_dir;// (0.0f, 1.0f, 0.0f, 0.0f);		// 光の方向
	D3DXVECTOR4		g_diffuse;// (1.0f, 1.0f, 1.0f, 1.0f);			// 平行光源の色
	D3DXVECTOR4		g_ambient;// (0.2f, 0.2f, 0.2f, 0.2f);			// 環境光
	D3DXVECTOR4		g_specular;// (1.0f, 1.0f, 1.0f, 0.2f);			// スペキュラ光

	D3DXMATRIX			g_lightcameramat;
	D3DXMATRIX			g_lightprojectionmat;

	LPDIRECT3DTEXTURE9			g_toontexture;

	CPlayer			*g_pPlayer = nullptr;
	CDebug			*g_pDebug = nullptr;
	CDirectInput	*g_pInput = nullptr;



public:
	
	bool GameInit(HINSTANCE hinst, HWND hwnd, int width, int height, bool fullscreen);
	void GameMain();
	void GameExit();
	void GameSetEvent();
	void GameSetEndFlag();
	void GameInput();
	void GameUpdate();
	void GameRender();

	void DrawLand();
	void DrawPlayer();
	void DrawDebug();
	void CreateShadowMap(LPDIRECT3DDEVICE9 lpdevice);

	//******************************************************************************
	//	End of file.
	//******************************************************************************


};
