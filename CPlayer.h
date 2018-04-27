#pragma once
#include "CGameObject.h"
#include "CCamera.h"
#include "CDebug.h"
#include "CLight.h"

class CPlayer : public CGameObject
{
protected:
	
	D3DXMATRIX m_MatUV;
	

public:
	CPlayer();
	~CPlayer();

	void Draw(LPDIRECT3DDEVICE9 lpdevice,
		LPDIRECT3DVERTEXSHADER9 VSShader,
		LPD3DXCONSTANTTABLE VSTable,
		LPDIRECT3DPIXELSHADER9 PSShader,
		LPD3DXCONSTANTTABLE	PSTable);
};