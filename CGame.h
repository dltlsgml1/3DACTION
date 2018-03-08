#pragma once
#include "AllHeaderFiles.h"

class CGame
{
private:

public:
	bool GameInit(HINSTANCE hInstance, HWND hwnd, int width, int height, bool fullscreen);
	void GameMain();
	void GameExit();
	void GameSetEvent();
	void GameSetEndFlag();
	void GameInput();
	void GameUpdate();
	void GameRender();


};
