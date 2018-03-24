#pragma once
#include <d3d9.h>
#include <d3dx9.h>
class CCamera
{
private:
	D3DXMATRIX m_MatView;
	D3DXMATRIX m_MatProjection;

	D3DXVECTOR3 m_CameraPos;
	D3DXVECTOR3 m_FocusingPos;
	D3DXVECTOR3 m_UpVector;
	D3DXVECTOR3 m_Angle;

	float m_FOV;
	float m_Aspect;
	float m_Near;
	float m_Far;

	D3DXVECTOR3 m_InvCameraPos;

public:
	CCamera();
	CCamera(float FOV, float Aspect, float Near, float Far);
	~CCamera();
	void SetCameraMat();
	void SetCameraProjection(float FOV, float Aspect, float Near, float Far);
	void SetCameraView(D3DXVECTOR3 CameraPos, D3DXVECTOR3 Focus);
	void SetTransform(LPDIRECT3DDEVICE9 Device);
	void UpdateCameraView(D3DXVECTOR3 CameraPos, D3DXVECTOR3 Focus);
	void UpdateCameraProjection(float FOV, float Aspect, float Near, float Far);

	void UpdateAngle(D3DXVECTOR3 Angle);
	void RotateCamera();
	
	D3DXMATRIX GetViewMatrix();
	D3DXMATRIX GetProjectionMatrix();
	D3DXVECTOR3 GetCameraPos();
};