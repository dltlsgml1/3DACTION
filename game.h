//*****************************************************************************
//!	@file	game.h
//!	@brief	ゲーム処理
//!	@note	
//!	@author
//*****************************************************************************
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
//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
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
