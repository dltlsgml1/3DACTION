#pragma once
#include <Windows.h>

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
