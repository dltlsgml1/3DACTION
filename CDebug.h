#pragma once
#include <Windows.h>
#include <d3dx9.h>

class CDebug 
{
	LPD3DXFONT		m_pFont;
public:
	CDebug(LPDIRECT3DDEVICE9 lpdevice) :m_pFont(NULL) 
	{
		CreateFont(lpdevice);
	}
	~CDebug() 
	{
		if (m_pFont != NULL) 
		{
			m_pFont->Release();
		}
		m_pFont = NULL;
	}

	void CreateFont(LPDIRECT3DDEVICE9 lpdev);
	void DrawText(int x, int y, char* str);
	
};
