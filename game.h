//*****************************************************************************
//!	@file	game.h
//!	@brief	ゲーム処理
//!	@note	
//!	@author
//*****************************************************************************
#pragma once
#include "AllHeaderFiles.h"

#define		SCREEN_X		1280
#define		SCREEN_Y		720
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
void CreateShadowMap(LPDIRECT3DDEVICE9 lpdevice);

//******************************************************************************
//	End of file.
//******************************************************************************
