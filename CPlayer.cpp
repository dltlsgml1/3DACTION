#include "CPlayer.h"


CPlayer::CPlayer()
{
	
	m_MatUV = {
		0.5f,  0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f,  0.0f, 1.0f, 0.0f,
		0.5f,  0.5f, 0.0f, 1.0f
	};
}
CPlayer::~CPlayer()
{
	
}

void CPlayer::Draw(LPDIRECT3DDEVICE9 lpdevice,
	LPDIRECT3DVERTEXSHADER9 VSShader,
	LPD3DXCONSTANTTABLE VSTable,
	LPDIRECT3DPIXELSHADER9 PSShader,
	LPD3DXCONSTANTTABLE	PSTable)
{
	CCamera *m_pCamera;
	CCamera *m_pCameraFromLight;
	CLight  *m_pLight;
	m_pCamera = (CCamera*)InstanceAdr[0];
	m_pCameraFromLight = (CCamera*)InstanceAdr[1];
	m_pLight = (CLight*)InstanceAdr[2];


	D3DXVECTOR4 tempVec;
	lpdevice->SetVertexShader(VSShader);
	lpdevice->SetPixelShader(PSShader);

	VSTable->SetMatrix(lpdevice, "g_world", &m_MatWorld);
	VSTable->SetMatrix(lpdevice, "g_view", &m_pCamera->GetViewMatrix());
	VSTable->SetMatrix(lpdevice, "g_projection", &m_pCamera->GetProjectionMatrix());


	CStaticMethod::Vec3ToVec4(tempVec, m_pCamera->GetCameraPos(), 0.0f);


	VSTable->SetVector(lpdevice, "g_camera_pos", &tempVec);
	PSTable->SetVector(lpdevice, "g_camera_pos", &tempVec);

	tempVec = m_pLight->GetDirection();
	tempVec.w = 1.0f;

	VSTable->SetVector(lpdevice, "g_light_dir", &tempVec);
	PSTable->SetVector(lpdevice, "g_light_dir", &tempVec);
	VSTable->SetBool(lpdevice, "drawguideline", false);
	PSTable->SetBool(lpdevice, "drawguideline", false);


	VSTable->SetMatrix(lpdevice, "g_lightposcamera", &m_pCameraFromLight->GetViewMatrix());
	VSTable->SetMatrix(lpdevice, "g_lightposprojection", &m_pCameraFromLight->GetProjectionMatrix());
	VSTable->SetMatrix(lpdevice, "g_matuv", &m_MatUV);


	int toonindex = PSTable->GetSamplerIndex("ToonSampler1");
	lpdevice->SetSamplerState(toonindex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	lpdevice->SetTexture(toonindex, m_ToonTexture);
	int index = PSTable->GetSamplerIndex("ShadowSampler");
	lpdevice->SetTexture(index, m_ShadowTexture);

	DrawWithShader(lpdevice, VSTable, PSTable);
}