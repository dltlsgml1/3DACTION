#pragma once
#include "CDirect3DXFile.h"
class CGameObject : public CDirect3DXFile
{
private:
	D3DXVECTOR3 m_Pos;
	D3DXVECTOR3 m_Angle;
	D3DXMATRIX m_MatWorld;

	LPDIRECT3DTEXTURE9 m_ShadowTex;
	LPDIRECT3DSURFACE9 m_ShadowSurface;
	LPDIRECT3DSURFACE9 m_ZBufferSurface;
	LPDIRECT3DTEXTURE9 m_NormalTexture;
	
public:

	void SetPos(D3DXVECTOR3 Pos);
	void SetAngle(D3DXVECTOR3 Angle);
	void UpdatePos(D3DXVECTOR3 Pos);
	void UpdateAngle(D3DXVECTOR3 Angle);
	
	D3DXVECTOR3 GetPos();
	D3DXVECTOR3 GetAngle();
	D3DXMATRIX	GetWorldMatrix();
	LPDIRECT3DTEXTURE9 GetTexture(int TextureNum);
	LPDIRECT3DTEXTURE9 GetZTexture();
	LPDIRECT3DSURFACE9 GetSurface(int SurfaceNum);

	CGameObject();
	~CGameObject();
};

