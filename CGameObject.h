#pragma once
#include "CDirect3DXFile.h"
#include "CStaticMethod.h"

enum class TEXTURETYPES
{
	SHADOW,
	NORMALMAP,
	MATERIAL,
	TOON
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
	D3DXMATRIX	m_MatWorld;
	
	LPDIRECT3DTEXTURE9 m_ShadowTexture;
	LPDIRECT3DTEXTURE9 m_NormalTexture;
	LPDIRECT3DTEXTURE9 m_ToonTexture;

	LPDIRECT3DSURFACE9 m_ShadowSurface;
	LPDIRECT3DSURFACE9 m_ZBufferSurface;
	
public:
	//Set:すぐその値にセットする
	void SetPos(D3DXVECTOR3 Pos);
	void SetAngle(D3DXVECTOR3 Angle);

	//Update:引数分＋していく
	void UpdatePos(D3DXVECTOR3 Pos);
	void UpdateAngle(D3DXVECTOR3 Angle);
	
	D3DXVECTOR3 GetPos();
	D3DXVECTOR3 GetAngle();
	D3DXMATRIX	GetWorldMatrix();

	//ほしいテクスチャやサーフェス変数のアドレスを取得する
	LPDIRECT3DTEXTURE9* GetTexture(TEXTURETYPES TextureType);
	LPDIRECT3DSURFACE9* GetSurface(SURFACETYPES SurfaceType);

	virtual void Draw(LPDIRECT3DDEVICE9 lpdevice,
		LPD3DXCONSTANTTABLE VSTable,
		LPD3DXCONSTANTTABLE	PSTable);
	void DrawWithShader(LPDIRECT3DDEVICE9 lpdevice,
		LPD3DXCONSTANTTABLE VSTable,
		LPD3DXCONSTANTTABLE	PSTable);

	CGameObject();
	~CGameObject();
};

