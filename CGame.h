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
#include "CLight.h"
#include "ClassInstance.h"


#include <Windows.h>
#include <thread>
#define		SCREEN_X		1280							
#define		SCREEN_Y		720
#define		RADIAN(x)		D3DX_PI*(x)/180.0f
#define		TEXMAP_SIZE		4096*2
#define		NUM_INSTANCE	20
class CGame
{
private:

	HANDLE				m_hEventHandle;		// イベントハンドル
	bool				m_EndFlag = false;	// 終了フラグ
	std::thread			m_GameMainThread;	// ゲームメインスレッド

	CShader				*m_pPlayerShader = nullptr;
	CShader				*m_pLandShader = nullptr;
	CShader				*m_pShadowShader = nullptr;

	CCamera				*m_pCamera = nullptr;
	CCamera				*m_pCameraFromLight = nullptr;

	CLight				*m_pLight;
	CDirectXGraphics	*m_DXGrobj = nullptr;		// DirectX Graphicsオブジェクト
	CGameObject			*m_pLand = nullptr;

	D3DXMATRIX			g_InvMatLand;		//地形の逆行列
	D3DXMATRIX  g_matuv = {
		0.5f,  0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f,  0.0f, 1.0f, 0.0f,
		0.5f,  0.5f, 0.0f, 1.0f
	};

	// 光の設定情報
	D3DXVECTOR4		g_light_pos;
	D3DXVECTOR4		g_light_dir = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 0.0f);			// 光の方向
	D3DXVECTOR4		g_diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);			// 平行光源の色
	D3DXVECTOR4		g_ambient = D3DXVECTOR4(0.2f, 0.2f, 0.2f, 0.2f);			// 環境光
	D3DXVECTOR4		g_specular = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.2f);			// スペキュラ光


	CPlayer			*m_pPlayer = nullptr;
	CDebug			*m_pDebug = nullptr;
	CDirectInput	*m_pInput = nullptr;

	void			*InstanceAdr[NUM_INSTANCE];


public:
	CGame();
	~CGame();

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
