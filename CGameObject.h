#pragma once
#include "CDirect3DXFile.h"
#include "mathutil.h"


enum class TEXTURETYPES
{
	SHADOW,
	NORMALMAP,
	MATERIAL
};

enum class SURFACETYPES
{
	SHADOW,
	ZBUFFER
};

class CGameObject : public CDirect3DXFile
{
protected:
	D3DXVECTOR3 m_Pos;
	D3DXVECTOR3 m_Angle;
	D3DXMATRIX m_MatWorld;
	

	LPDIRECT3DTEXTURE9 m_ShadowTexture;
	LPDIRECT3DSURFACE9 m_ShadowSurface;
	LPDIRECT3DSURFACE9 m_ZBufferSurface;
	LPDIRECT3DTEXTURE9 m_NormalTexture;
	
public:

	void SetPos(D3DXVECTOR3 Pos);
	void SetAngle(D3DXVECTOR3 Angle);
	void UpdatePos(D3DXVECTOR3 Pos);
	void UpdateAngle(D3DXVECTOR3 Angle);
	void SetTextureFromFile(LPDIRECT3DDEVICE9 lpdevice, LPSTR Filename, TEXTURETYPES Texture);
	
	D3DXVECTOR3 GetPos();
	D3DXVECTOR3 GetAngle();
	D3DXMATRIX	GetWorldMatrix();
	LPDIRECT3DTEXTURE9* GetTexture(TEXTURETYPES TextureType);
	LPDIRECT3DSURFACE9* GetSurface(SURFACETYPES SurfaceType);

	CGameObject();
	~CGameObject();
};

