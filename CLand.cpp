#include "CLand.h"

CLand::CLand()
{
	m_MatUV = {
		0.5f,  0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f,  0.0f, 1.0f, 0.0f,
		0.5f,  0.5f, 0.0f, 1.0f
	};
}
CLand::~CLand()
{

}

void CLand::Draw(LPDIRECT3DDEVICE9 lpdevice,
	LPDIRECT3DVERTEXSHADER9 VSShader,
	LPD3DXCONSTANTTABLE VSTable,
	LPDIRECT3DPIXELSHADER9 PSShader,
	LPD3DXCONSTANTTABLE	PSTable)
{
	CCamera *m_pCamera;
	CCamera *m_pCameraFromLight;
	CLight *m_pLight;
	m_pCamera = (CCamera*)InstanceAdr[CAMERA];
	m_pCameraFromLight = (CCamera*)InstanceAdr[CAMERAFROMLIGHT];
	m_pLight = (CLight*)InstanceAdr[LIGHT];

	D3DXVECTOR4 tempVec;
	lpdevice->SetVertexShader(VSShader);
	lpdevice->SetPixelShader(PSShader);

	int normalindex = PSTable->GetSamplerIndex("NormalSampler");
	lpdevice->SetSamplerState(normalindex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	lpdevice->SetTexture(normalindex, m_NormalTexture);

	VSTable->SetMatrix(lpdevice, "g_world", &m_MatWorld);
	VSTable->SetMatrix(lpdevice, "g_view", &m_pCamera->GetViewMatrix());
	VSTable->SetMatrix(lpdevice, "g_projection", &m_pCamera->GetProjectionMatrix());

	VSTable->SetVector(lpdevice, "g_inv_pos", &m_InvPos);
	VSTable->SetVector(lpdevice, "g_inv_camera_pos", &m_pCamera->GetInvCameraPos());
	VSTable->SetVector(lpdevice, "g_inv_light_dir", &m_pLight->GetInvDirection());

	CStaticMethod::Vec3ToVec4(tempVec, m_pCamera->GetCameraPos(), 0.0f);

	VSTable->SetVector(lpdevice, "g_camera_pos", &tempVec);
	PSTable->SetVector(lpdevice, "g_camera_pos", &tempVec);

	tempVec = m_pLight->GetDirection();
	tempVec.w = 1.0f;

	VSTable->SetVector(lpdevice, "g_light_dir", &tempVec);
	PSTable->SetVector(lpdevice, "g_light_dir", &tempVec);


	VSTable->SetMatrix(lpdevice, "g_lightposcamera", &m_pCameraFromLight->GetViewMatrix());
	PSTable->SetMatrix(lpdevice, "g_lightposprojection", &m_pCameraFromLight->GetProjectionMatrix());
	VSTable->SetMatrix(lpdevice, "g_matuv",&m_MatUV);

	int index = PSTable->GetSamplerIndex("ShadowSampler");
	lpdevice->SetTexture(index, m_ShadowTexture);
	DrawWithShader(lpdevice,VSTable, PSTable);
}