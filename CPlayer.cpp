#include "CPlayer.h"

CPlayer::CPlayer()
{

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
	
	D3DXVECTOR4 tempVec;
	lpdevice->SetVertexShader(VSShader);
	lpdevice->SetPixelShader(PSShader);


	VSTable->SetMatrix(lpdevice, "g_world", &g_pPlayer->GetWorldMatrix());
	VSTable->SetMatrix(lpdevice, "g_view", &g_pCamera->GetViewMatrix());
	VSTable->SetMatrix(lpdevice, "g_projection", &g_pCamera->GetProjectionMatrix());


	CStaticMethod::Vec3ToVec4(tempVec, g_pCamera->GetCameraPos(), 0.0f);


	VSTable->SetVector(lpdevice, "g_camera_pos", &tempVec);
	PSTable->SetVector(lpdevice, "g_camera_pos", &tempVec);

	tempVec = g_light_dir;
	tempVec.w = 1.0f;

	VSTable->SetVector(lpdevice, "g_light_dir", &tempVec);
	PSTable->SetVector(lpdevice, "g_light_dir", &tempVec);
	VSTable->SetBool(lpdevice, "drawguideline", false);
	PSTable->SetBool(lpdevice, "drawguideline", false);

	VSTable->SetMatrix(lpdevice, "g_lightposcamera", &g_pCameraFromLight->GetViewMatrix());
	VSTable->SetMatrix(lpdevice, "g_lightposprojection", &g_pCameraFromLight->GetProjectionMatrix());
	VSTable->SetMatrix(lpdevice, "g_matuv", &g_matuv);


	int toonindex = PSTable->GetSamplerIndex("ToonSampler1");
	lpdevice->SetSamplerState(toonindex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	lpdevice->SetTexture(toonindex, *g_pPlayer->GetTexture(TEXTURETYPES::TOON));
	int index = PSTable->GetSamplerIndex("ShadowSampler");
	lpdevice->SetTexture(index, *g_pPlayer->GetTexture(TEXTURETYPES::SHADOW));

	g_pPlayer->DrawWithShader(lpdevice, VSTable, PSTable);
}