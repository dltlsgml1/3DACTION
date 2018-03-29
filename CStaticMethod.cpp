//*****************************************************************************
//!	@file	matuutil.cpp
//!	@brief	
//!	@note	数学系のユーティリティ関数群
//!	@author	
//*****************************************************************************

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include	<d3dx9.h>
#include "CStaticMethod.h"
//==============================================================================
//!	@fn		MakeWorldMatrix
//!	@brief	角度と平行移動量から行列を作る
//!	@param　行列（出力）　　	
//!	@param　	角度（入力）
//!	@param　	平行移動量（入力）
//!	@retval	なし
//==============================================================================
void MakeWorldMatrix(D3DXMATRIX& mat,const D3DXVECTOR3& angle,const D3DXVECTOR3& trans){
	D3DXMATRIX matx, maty, matz, mattrans;;

	D3DXMatrixRotationX(&matx, D3DX_PI*angle.x / 180.0f);
	D3DXMatrixRotationY(&maty, D3DX_PI*angle.y / 180.0f);
	D3DXMatrixRotationZ(&matz, D3DX_PI*angle.z / 180.0f);
	mat = matx*maty*matz;
	D3DXMatrixTranslation(&mattrans, trans.x, trans.y, trans.z);
	
	mat = mat*mattrans;
}
//==============================================================================
//!	@fn		MakeWorldMatrix
//!	@brief	角度と平行移動量から行列を作る(積算行列を計算)
//!	@param　行列（出力）　　	
//!	@param　積算行列（出力）　　	
//!	@param　角度（入力）
//!	@param　平行移動量（入力）
//!	@retval	なし
//==============================================================================

void CStaticMethod::MakeWorldMatrix(D3DXMATRIX& mat, const D3DXVECTOR3& angle, const D3DXVECTOR3& trans)
{
	D3DXMATRIX matx, maty, matz, mattrans;;

	D3DXMatrixRotationX(&matx, D3DX_PI*angle.x / 180.0f);
	D3DXMatrixRotationY(&maty, D3DX_PI*angle.y / 180.0f);
	D3DXMatrixRotationZ(&matz, D3DX_PI*angle.z / 180.0f);
	mat = matx * maty*matz;
	D3DXMatrixTranslation(&mattrans, trans.x, trans.y, trans.z);

	mat = mat * mattrans;
}

void CStaticMethod::Vec3ToVec4(D3DXVECTOR4 &Output, D3DXVECTOR3 &Input, float wValue)
{
	Output.x = Input.x;
	Output.y = Input.y;
	Output.z = Input.z;
	Output.w = wValue;
}

//******************************************************************************
//	End of file.
//******************************************************************************
