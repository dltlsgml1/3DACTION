#include "CGameObject.h"



CGameObject::CGameObject()
{
}


CGameObject::~CGameObject()
{

}

void CGameObject::SetAngle(D3DXVECTOR3 Angle)
{
	m_Angle = Angle;
}

void CGameObject::SetPos(D3DXVECTOR3 Pos)
{
	m_Pos = Pos;
}
void CGameObject::UpdateAngle(D3DXVECTOR3 Angle)
{
	m_Angle += Angle;
}
void CGameObject::UpdatePos(D3DXVECTOR3 Pos)
{
	m_Pos += Pos;
}

D3DXVECTOR3 CGameObject::GetPos()
{
	return m_Pos;
}

D3DXVECTOR3 CGameObject::GetAngle()
{
	return m_Angle;
}
D3DXMATRIX	CGameObject::GetWorldMatrix()
{
	return m_MatWorld;
}
LPDIRECT3DTEXTURE9 CGameObject::GetTexture(int TextureNum)
{
	return *m_lpmeshtextures;
}

LPDIRECT3DSURFACE9 CGameObject::GetSurface(int SurfaceNum)
{
	return m_ShadowSurface;
}
