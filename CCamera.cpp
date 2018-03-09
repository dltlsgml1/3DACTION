#include "AllHeaderFiles.h"

CCamera::CCamera(float FOV, float Aspect, float Near, float Far)
{
	m_FOV = FOV;
	m_Aspect = Aspect;
	m_Near = Near;
	m_Far = Far;
	m_UpVector = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
}
CCamera::CCamera()
{
	m_UpVector = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
}

CCamera::~CCamera()
{

}

void CCamera::SetCameraMat()
{
	D3DXMatrixLookAtLH(&m_MatView, &m_CameraPos, &m_FocusingPos, &m_UpVector);
	D3DXMatrixPerspectiveFovLH(&m_MatProjection, m_FOV, m_Aspect, m_Near, m_Far);
	

}
void CCamera::SetTransform(LPDIRECT3DDEVICE9 Device)
{
	Device->SetTransform(D3DTS_VIEW, &m_MatView);
	Device->SetTransform(D3DTS_PROJECTION, &m_MatProjection);
}

void CCamera::SetCameraView(D3DXVECTOR3 CameraPos, D3DXVECTOR3 Focus)
{
	m_CameraPos = CameraPos;
	m_FocusingPos = Focus;
}
void CCamera::SetCameraProjection(float FOV, float Aspect, float Near, float Far)
{
	m_FOV = FOV;
	m_Aspect = Aspect;
	m_Near = Near;
	m_Far = Far;
}

D3DXMATRIX CCamera::GetProjectionMatrix()
{
	return m_MatProjection;
}

D3DXMATRIX CCamera::GetViewMatrix()
{
	return m_MatView;
}

D3DXVECTOR3 CCamera::GetCameraPos()
{
	return m_CameraPos;
}