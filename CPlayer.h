#pragma once
#include "CGameObject.h"
#include "CCamera.h"
#include "CDebug.h"
#include "CLight.h"

class CPlayer : public CGameObject
{
protected:
	CCamera *m_pCamera;
	CCamera *m_pCameraFromLight;
	CLight  *m_pLight;
	D3DXVECTOR4 m_Light_dir; 
	D3DXMATRIX m_MatUV;
	

public:
	CPlayer(CCamera *Camera,CCamera *CameraFromLight);
	~CPlayer();

	void Draw(LPDIRECT3DDEVICE9 lpdevice,
		LPDIRECT3DVERTEXSHADER9 VSShader,
		LPD3DXCONSTANTTABLE VSTable,
		LPDIRECT3DPIXELSHADER9 PSShader,
		LPD3DXCONSTANTTABLE	PSTable);
};