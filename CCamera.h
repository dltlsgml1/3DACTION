#pragma once
#include <d3d9.h>
#include <d3dx9.h>
class CCamera
{
private:
	D3DXMATRIX m_MatView;				//ビュー行列
	D3DXMATRIX m_MatProjection;			//プロジェクション行列

	D3DXVECTOR3 m_CameraPos;			//カメラの座標
	D3DXVECTOR3 m_FocusingPos;			//注視点の座標
	D3DXVECTOR3 m_UpVector;				//カメラのUPベクトル
	D3DXVECTOR3 m_Angle;				//カメラのアングル

	float m_FOV;						//カメラのFOV
	float m_Aspect;						//カメラのアスペクト比
	float m_Near;						//カメラから見え始める距離
	float m_Far;						//カメラから見えなくなる距離

	D3DXVECTOR3 m_InvCameraPos;			//カメラ位置の逆ベクトル

public:
	CCamera();																		
	CCamera(float FOV, float Aspect, float Near, float Far);						//初期化の時にカメラの設定ができるインストラクタ
	~CCamera();						

	void SetTransform(LPDIRECT3DDEVICE9 Device);					//変更された行列をD3DDEVICEにセットする
	void SetCameraMat();											//変更されたビューとプロジェクションをそれぞれの行列に反映させる

	//Set:すぐその値にセットする
	void SetCameraProjection(float FOV, float Aspect, float Near, float Far);		
	void SetCameraView(D3DXVECTOR3 CameraPos, D3DXVECTOR3 Focus);					

	//Update:引数分＊＋していく
	void UpdateCameraView(D3DXVECTOR3 CameraPos, D3DXVECTOR3 Focus);				
	void UpdateCameraProjection(float FOV, float Aspect, float Near, float Far);	
	void UpdateAngle(D3DXVECTOR3 Angle);											

	void RotateCamera();															//カメラを回転させる（未実装）
	
	D3DXMATRIX GetViewMatrix();														
	D3DXMATRIX GetProjectionMatrix();												
	D3DXVECTOR3 GetCameraPos();	
	D3DXVECTOR4 GetInvCameraPos();
};