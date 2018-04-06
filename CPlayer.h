#pragma once
#include "CGameObject.h"
class CPlayer : public CGameObject
{
private:
	
public:
	CPlayer();
	~CPlayer();

	virtual void Draw(LPDIRECT3DDEVICE9 lpdevice,
		LPD3DXCONSTANTTABLE VSTable,
		LPD3DXCONSTANTTABLE	PSTable);
};