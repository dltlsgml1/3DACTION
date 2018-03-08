#include "AllHeaderFiles.h"

CCamera::CCamera(float FOV, float Aspect, float Near, float Far)
{
	m_FOV = FOV;
	m_Aspect = Aspect;
	m_Near = Near;
	m_Far = Far;
}

CCamera::~CCamera()
{

}

void CCamera::SetCameraView(LPDIRECT3DDEVICE9 Device, D3DXVECTOR3 &CameraPos, D3DXVECTOR3 &Focus)
{
	D3DXMatrixLookAtLH(&m_MatView, &m_CameraPos, &m_FocusingPos, &m_UpVector);
	D3DXMatrixPerspectiveFovLH(&m_MatProjection, m_FOV, m_Aspect, m_Near, m_Far);

	Device->SetTransform(D3DTS_VIEW, &m_MatView);
	Device->SetTransform(D3DTS_PROJECTION, &m_MatProjection);

}