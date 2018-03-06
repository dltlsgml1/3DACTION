//*****************************************************************************
//!	@file	game.h
//!	@brief	�Q�[������
//!	@note	
//!	@author
//*****************************************************************************
#pragma once
#include <d3d9.h>


//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
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
