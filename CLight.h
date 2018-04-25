#pragma once
#include <d3dx9.h>


class CLight
{
protected:
	D3DXVECTOR4 m_Direction;
	D3DXVECTOR3 m_Pos;
	D3DXVECTOR4 m_InvPos;
	D3DXVECTOR4 m_InvDirection;
public:
	CLight();
	CLight(D3DXVECTOR4 Direction,D3DXVECTOR3 Pos);
	~CLight();
	D3DXVECTOR4 GetDirection() const;
	D3DXVECTOR4 GetInvDirection() const;
	D3DXVECTOR3 GetPos() const;
	D3DXVECTOR4 GetInvPos() const;
	void UpdatePos(D3DXVECTOR3 Pos);
	void UpdateDirection(D3DXVECTOR4 Direction);
	void SetDirection(D3DXVECTOR4 Direction);
	void SetInvDirection(D3DXVECTOR4 InvDirection);
	void SetPos(D3DXVECTOR3 pos);
	
};

