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
LPDIRECT3DTEXTURE9 CGameObject::GetTexture(TEXTURETYPES TextureType)
{
	LPDIRECT3DTEXTURE9 *temptexture;
	switch (TextureType)
	{
	case TEXTURETYPES::SHADOW:
		*temptexture = m_ShadowTexture;
	case TEXTURETYPES::NORMALMAP:
		*temptexture = m_NormalTexture;
	case TEXTURETYPES::MATERIAL:
		*temptexture = *m_lpmeshtextures;
	}
	return *temptexture;
}

LPDIRECT3DSURFACE9 CGameObject::GetSurface(SURFACETYPES SurfaceType)
{
	switch (SurfaceType)
	{
	case SURFACETYPES::SHADOW:
		return m_ShadowSurface;
	case SURFACETYPES::ZBUFFER:
		return m_ZBufferSurface;
	}
}
