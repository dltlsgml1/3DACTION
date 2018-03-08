#include "AllHeaderFiles.h"

CPlayer::CPlayer()
{
	D3DXMatrixIdentity(&m_MatPlayer);
	D3DXMatrixIdentity(&m_MatScale);
	m_Pos.x = m_Pos.y = m_Pos.z = 0.0f;
	m_Angle.x = m_Angle.y = m_Angle.z = 0.0f;
}
CPlayer::~CPlayer()
{

}