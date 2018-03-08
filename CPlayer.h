#pragma once
class CPlayer : public CDirect3DXFile
{
private:
	D3DXMATRIX m_MatPlayer;
	D3DXMATRIX m_MatScale;
	D3DXVECTOR3 m_Pos;
	D3DXVECTOR3 m_Angle;


public:
	CPlayer();
	~CPlayer();
	void Update();
	
	
};