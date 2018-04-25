#include "CLight.h"

CLight::CLight()
{
}
CLight::CLight(D3DXVECTOR4 Direction, D3DXVECTOR3 Pos)
{
	m_Direction = Direction;
	m_Pos = Pos;

}

CLight::~CLight()
{
}

D3DXVECTOR4 CLight::GetDirection() const
{
	return m_Direction;
}

D3DXVECTOR4 CLight::GetInvDirection() const
{
	return m_InvDirection;
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
void CLight::UpdateDirection(D3DXVECTOR4 Direction)
{
	m_Direction += Direction;
}
void CLight::SetDirection(D3DXVECTOR4 Direction)
{
	m_Direction = Direction;
}

void CLight::SetInvDirection(D3DXVECTOR4 InvDirection)
{
	m_InvDirection = InvDirection;
}
void CLight::SetPos(D3DXVECTOR3 Pos)
{
	m_Pos = Pos;
}