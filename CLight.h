#pragma once
#include <d3dx9.h>


class CLight
{
protected:
	D3DXVECTOR3 m_Direction;
	D3DXVECTOR3 m_Pos;
	D3DXVECTOR4 m_InvPos;
public:
	CLight();
	~CLight();
	D3DXVECTOR3 GetDirection() const;
	D3DXVECTOR3 GetPos() const;
	D3DXVECTOR4 GetInvPos() const;
};

