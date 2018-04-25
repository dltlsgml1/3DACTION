#pragma once
#include <d3d9.h>
#include <d3dx9.h>
class CCamera
{
private:
	D3DXMATRIX m_MatView;				//�r���[�s��
	D3DXMATRIX m_MatProjection;			//�v���W�F�N�V�����s��

	D3DXVECTOR3 m_CameraPos;			//�J�����̍��W
	D3DXVECTOR3 m_FocusingPos;			//�����_�̍��W
	D3DXVECTOR3 m_UpVector;				//�J������UP�x�N�g��
	D3DXVECTOR3 m_Angle;				//�J�����̃A���O��

	float m_FOV;						//�J������FOV
	float m_Aspect;						//�J�����̃A�X�y�N�g��
	float m_Near;						//�J�������猩���n�߂鋗��
	float m_Far;						//�J�������猩���Ȃ��Ȃ鋗��

	D3DXVECTOR3 m_InvCameraPos;			//�J�����ʒu�̋t�x�N�g��

public:
	CCamera();																		
	CCamera(float FOV, float Aspect, float Near, float Far);						//�������̎��ɃJ�����̐ݒ肪�ł���C���X�g���N�^
	~CCamera();						

	void SetTransform(LPDIRECT3DDEVICE9 Device);					//�ύX���ꂽ�s���D3DDEVICE�ɃZ�b�g����
	void SetCameraMat();											//�ύX���ꂽ�r���[�ƃv���W�F�N�V���������ꂼ��̍s��ɔ��f������

	//Set:�������̒l�ɃZ�b�g����
	void SetCameraProjection(float FOV, float Aspect, float Near, float Far);		
	void SetCameraView(D3DXVECTOR3 CameraPos, D3DXVECTOR3 Focus);					

	//Update:���������{���Ă���
	void UpdateCameraView(D3DXVECTOR3 CameraPos, D3DXVECTOR3 Focus);				
	void UpdateCameraProjection(float FOV, float Aspect, float Near, float Far);	
	void UpdateAngle(D3DXVECTOR3 Angle);											

	void RotateCamera();															//�J��������]������i�������j
	
	D3DXMATRIX GetViewMatrix();														
	D3DXMATRIX GetProjectionMatrix();												
	D3DXVECTOR3 GetCameraPos();	
	D3DXVECTOR4 GetInvCameraPos();
};