#include "CLight.h"

CLight::CLight()
{
}
CLight::CLight(D3DXVECTOR3 Direction, D3DXVECTOR3 Pos)
{
	m_Direction = Direction;
	m_Pos = Pos;

}

CLight::~CLight()
{
}

D3DXVECTOR3 CLight::GetDirection() const
{
	return m_Direction;
}
D3DXVECTOR3 CLight::GetPos() const
{
	return m_Pos;
}
D3DXVECTOR4 CLight::GetInvPos() const
{
	return m_InvPos;
}

void CLight::UpdatePos(D3DXVECTOR3 Pos)
{
	m_Pos += Pos;
}
void CLight::UpdateFocus(D3DXVECTOR3 Focus)
{
	
}
void CLight::SetFocus(D3DXVECTOR3 Focus)
{

}
void CLight::SetPos(D3DXVECTOR3 Pos)
{
	m_Pos = Pos;
}