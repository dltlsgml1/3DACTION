#pragma once
#include "CGameObject.h"
class CPlayer : public CGameObject
{
private:
	
public:
	CPlayer();
	~CPlayer();

	void Draw(LPDIRECT3DDEVICE9 lpdevice,
		LPDIRECT3DVERTEXSHADER9 VSShader,
		LPD3DXCONSTANTTABLE VSTable,
		LPDIRECT3DPIXELSHADER9 PSShader,
		LPD3DXCONSTANTTABLE	PSTable);
};