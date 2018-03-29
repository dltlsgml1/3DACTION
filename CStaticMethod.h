//*****************************************************************************
//!	@file	mathutil.h
//!	@brief	ÉQÅ[ÉÄèàóù
//!	@note	
//!	@author
//*****************************************************************************
#pragma once

#include	<d3dx9.h>

class CStaticMethod
{
public:
	static void Vec3ToVec4(D3DXVECTOR4 &Output, D3DXVECTOR3 &Input, float wValue);
	static void MakeWorldMatrix(D3DXMATRIX& mat, const D3DXVECTOR3& angle, const D3DXVECTOR3& trans);
};

//******************************************************************************
//	End of file.
//******************************************************************************
