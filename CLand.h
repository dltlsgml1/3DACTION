#pragma once

#include "CGameObject.h"
#include "CCamera.h"
#include "CLight.h"

class CLand : public CGameObject
{
protected:
	D3DXMATRIX m_MatUV;
public:
	CLand();
	~CLand();
	void Draw(LPDIRECT3DDEVICE9 lpdevice,
		LPDIRECT3DVERTEXSHADER9 VSShader,
		LPD3DXCONSTANTTABLE VSTable,
		LPDIRECT3DPIXELSHADER9 PSShader,
		LPD3DXCONSTANTTABLE	PSTable);
};