#include "CGameObject.h"



CGameObject::CGameObject()
{
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Angle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&m_MatWorld);
}


CGameObject::~CGameObject()
{

}

void CGameObject::SetAngle(D3DXVECTOR3 Angle)
{
	m_Angle = Angle;
	CStaticMethod::MakeWorldMatrix(m_MatWorld, m_Angle, m_Pos);
}

void CGameObject::SetPos(D3DXVECTOR3 Pos)
{
	m_Pos = Pos;
	CStaticMethod::MakeWorldMatrix(m_MatWorld, m_Angle, m_Pos);
}
void CGameObject::UpdateAngle(D3DXVECTOR3 Angle)
{
	m_Angle += Angle;
	CStaticMethod::MakeWorldMatrix(m_MatWorld, m_Angle, m_Pos);
}
void CGameObject::UpdatePos(D3DXVECTOR3 Pos)
{
	m_Pos += Pos;
	CStaticMethod::MakeWorldMatrix(m_MatWorld, m_Angle, m_Pos);
}


D3DXVECTOR3 CGameObject::GetPos() const
{
	return m_Pos;
}

D3DXVECTOR3 CGameObject::GetAngle() const
{
	return m_Angle;
}
D3DXMATRIX	CGameObject::GetWorldMatrix() const
{
	return m_MatWorld;
}

D3DXVECTOR4 CGameObject::GetInvPos() const
{
	return m_InvPos;
}
D3DXMATRIX  CGameObject::GetInvMat() const
{
	return m_InvMatWorld;
}


LPDIRECT3DTEXTURE9* CGameObject::GetTexture(TEXTURETYPES TextureType) 

{
	LPDIRECT3DTEXTURE9 *temptexture = nullptr;		//臨時ポインタを宣言

	//要求されたテクスチャ種類によって臨時ポインタが示すものを設定する
	switch (TextureType)
	{
	case TEXTURETYPES::SHADOW:
		temptexture = &m_ShadowTexture;
		break;
	case TEXTURETYPES::NORMALMAP:
		temptexture = &m_NormalTexture;
		break;
	case TEXTURETYPES::MATERIAL:
		temptexture = m_lpmeshtextures;
		break;
	case TEXTURETYPES::TOON:
		temptexture = &m_ToonTexture;

	}
	return temptexture;	//示したらそのアドレスを返す
}


LPDIRECT3DSURFACE9* CGameObject::GetSurface(SURFACETYPES SurfaceType)
{
	LPDIRECT3DSURFACE9 *tempsurface = nullptr;	//臨時ポインタを宣言

	//要求されたサーフェス種類によって臨時ポインタが示すものを設定する
	switch (SurfaceType)
	{
	case SURFACETYPES::SHADOW:
		tempsurface = &m_ShadowSurface;
		break;
	case SURFACETYPES::ZBUFFER:
		tempsurface = &m_ZBufferSurface;
		break;
	}
	return tempsurface;	//示したらそのアドレスを返す
}
void CGameObject::Draw(LPDIRECT3DDEVICE9 lpdevice,
	LPD3DXCONSTANTTABLE VSTable,
	LPD3DXCONSTANTTABLE	PSTable)
{
	DrawWithShader(lpdevice, VSTable, PSTable);
}

void CGameObject::DrawWithShader(LPDIRECT3DDEVICE9 lpdevice, LPD3DXCONSTANTTABLE VSTable, LPD3DXCONSTANTTABLE	PSTable)
{
	unsigned int i;
	D3DXVECTOR4 tempVec;
	for (i = 0; i<m_nummaterial; i++)
	{

		tempVec.x = 1;
		tempVec.y = 1;
		tempVec.z = 1;
		tempVec.w = 1;
		VSTable->SetVector(lpdevice, "g_diffuse_power", &tempVec);

		tempVec.x = m_lpmeshmaterials[i].Ambient.r;
		tempVec.y = m_lpmeshmaterials[i].Ambient.g;
		tempVec.z = m_lpmeshmaterials[i].Ambient.b;
		tempVec.w = m_lpmeshmaterials[i].Ambient.a;
		VSTable->SetVector(lpdevice, "g_ambient_material", &tempVec);
		PSTable->SetVector(lpdevice, "g_ambient_material", &tempVec);

		tempVec.x = m_lpmeshmaterials[i].Diffuse.r;
		tempVec.y = m_lpmeshmaterials[i].Diffuse.g;
		tempVec.z = m_lpmeshmaterials[i].Diffuse.b;
		tempVec.w = m_lpmeshmaterials[i].Diffuse.a;
		VSTable->SetVector(lpdevice, "g_diffuse_material", &tempVec);
		PSTable->SetVector(lpdevice, "g_diffuse_material", &tempVec);

		tempVec.x = m_lpmeshmaterials[i].Specular.r;
		tempVec.y = m_lpmeshmaterials[i].Specular.g;
		tempVec.z = m_lpmeshmaterials[i].Specular.b;
		tempVec.w = m_lpmeshmaterials[i].Specular.a;
		VSTable->SetVector(lpdevice, "g_specular_material", &tempVec);

		if (m_lpmeshtextures[i] != NULL)
		{
			int index = PSTable->GetSamplerIndex("Sampler1");
			PSTable->SetBool(lpdevice, "istheretexture", true);
			lpdevice->SetTexture(index, m_lpmeshtextures[i]);		// テクスチャのセット
		}
		else
		{
			PSTable->SetBool(lpdevice, "istheretexture", false);
		}
		lpdevice->SetMaterial(&m_lpmeshmaterials[i]);	// マテリアルのセット
		lpdevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		m_lpmesh->DrawSubset(i);
	}
}

void CGameObject::SetInstance(void *Instance[])
{
	void* insadr[10];

	insadr[0] = Instance[0];
	insadr[1] = Instance[1];

	CCamera *ins;
	ins = (CCamera*)insadr[1];
	D3DXVECTOR3 pos = ins->GetCameraPos();
}