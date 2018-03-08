#include "AllHeaderFiles.h"

CCamera::CCamera(float FOV, float Aspect, float Near, float Far)
{
	m_FOV = FOV;
	m_Aspect = Aspect;
	m_Near = Near;
	m_Far = Far;
	m_UpVector = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
}

CCamera::~CCamera()
{

}

void CCamera::SetCameraView(LPDIRECT3DDEVICE9 Device, D3DXVECTOR3 &CameraPos, D3DXVECTOR3 &Focus)
{
	D3DXMatrixLookAtLH(&m_MatView, &CameraPos, &Focus, &m_UpVector);
	Device->SetTransform(D3DTS_VIEW, &m_MatView);

	D3DXMatrixPerspectiveFovLH(&m_MatProjection, m_FOV, m_Aspect, m_Near, m_Far);
	Device->SetTransform(D3DTS_PROJECTION, &m_MatProjection);

}

D3DXMATRIX CCamera::GetProjectionMatrix()
{
	return m_MatProjection;
}

D3DXMATRIX CCamera::GetViewMatrix()
{
	return m_MatView;
}