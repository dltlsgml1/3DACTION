#include "CLight.h"

CLight::CLight()
{
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