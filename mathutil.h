//*****************************************************************************
//!	@file	mathutil.h
//!	@brief	ÉQÅ[ÉÄèàóù
//!	@note	
//!	@author
//*****************************************************************************
#pragma once

#include	<d3dx9.h>

class CMathMethod
{
public:

	static void MakeWorldMatrix(D3DXMATRIX& mat, const D3DXVECTOR3& angle, const D3DXVECTOR3& trans);
};

void MakeWorldMatrix(D3DXMATRIX& mat, const D3DXVECTOR3& angle, const D3DXVECTOR3& trans);
void MakeWorldMatrix(D3DXMATRIX& mat, D3DXMATRIX& mattotal, const D3DXVECTOR3& angle, const D3DXVECTOR3& trans);

//******************************************************************************
//	End of file.
//******************************************************************************
