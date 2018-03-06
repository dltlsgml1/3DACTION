//*****************************************************************************
//!	@file	CDirect3DXFile.cpp
//!	@brief	Xファイル読み込み、描画クラス
//!	@note	
//!	@author	
//*****************************************************************************

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include	"CDirect3DXFile.h"

//==============================================================================
//!	@fn		LoadXFile
//!	@brief	Xファイル読み込み
//!	@param	Xファイル名
//!	@param	デバイスオブジェクト
//!	@retval	TRUE　成功終了/FALSE　失敗終了
//!	@note	
//==============================================================================


bool CDirect3DXFile::LoadXFile( char *xfilename, LPDIRECT3DDEVICE9 lpd3ddevice )
{
	LPD3DXBUFFER	pd3dxmtrlbuffer;
	LPD3DXBUFFER	pd3dxmtrlbuffer2;
	HRESULT			hr;
	unsigned int	i;
	// Ｘファイルを読み込む
	hr = D3DXLoadMeshFromX(	xfilename,				// Ｘファイル名
							D3DXMESH_SYSTEMMEM,		// 読み込みメモリ
							lpd3ddevice,			// デバイスオブジェクト
							NULL,
							&pd3dxmtrlbuffer,		// マテリアルバッファ
							&pd3dxmtrlbuffer2,		// マテリアルバッファ2
							&m_nummaterial,			// マテリアル数
							&m_lpmesh);				// メッシュ

	if( SUCCEEDED(hr) ){
		D3DXMATERIAL	*d3dxmaterials = (D3DXMATERIAL*)pd3dxmtrlbuffer->GetBufferPointer();	// マテリアルのアドレスを取得
		m_lpmeshmaterials = new D3DMATERIAL9[m_nummaterial];		// マテリアルコンテナの作成
		m_lpmeshtextures = new LPDIRECT3DTEXTURE9[m_nummaterial];	// テクスチャコンテナの生成
		
		for( i=0 ; i<m_nummaterial ; i++ ){
			m_lpmeshmaterials[i] = d3dxmaterials[i].MatD3D;
			m_lpmeshmaterials[i].Emissive = m_lpmeshmaterials[i].Specular = m_lpmeshmaterials[i].Ambient = m_lpmeshmaterials[i].Diffuse;
			

			hr = D3DXCreateTextureFromFile(	lpd3ddevice,
											d3dxmaterials[i].pTextureFilename,
											&m_lpmeshtextures[i]);
			if( FAILED(hr) ){
				m_lpmeshtextures[i]=NULL;
			}

		}
		pd3dxmtrlbuffer->Release();		// マテリアルバッファのリリース
		pd3dxmtrlbuffer2->Release();	// マテリアルバッファのリリース
	}else{
		return false;
	}
	
	return true;
}

//==============================================================================
//!	@fn		UnLoadXFile
//!	@brief	Xファイルオブジェクトアンロード
//!	@param	
//!	@retval	なし
//!	@note	
//==============================================================================
void CDirect3DXFile::UnLoadXFile()
{
	unsigned int i;

	if(m_lpmeshmaterials!=NULL){	// マテリアルオブジェクトの解放
		delete [] m_lpmeshmaterials;
		m_lpmeshmaterials=NULL;
	}

	if(m_lpmeshtextures!=NULL){		// テクスチャオブジェクトの解放
		for( i=0 ; i<m_nummaterial ; i++ ){
			if(m_lpmeshtextures[i]!=NULL)	m_lpmeshtextures[i]->Release();
		}
		delete [] m_lpmeshtextures;
		m_lpmeshtextures=NULL;
	}

	if(m_lpmesh!=NULL){		// メッシュ解放
		m_lpmesh->Release();
		m_lpmesh=NULL;
	}
}

//==============================================================================
//!	@fn		Draw
//!	@brief	Xファイル描画
//!	@param	デバイスオブジェクト
//!	@retval	なし
//!	@note	
//==============================================================================
void CDirect3DXFile::Draw(LPDIRECT3DDEVICE9 lpd3ddevice,
LPD3DXCONSTANTTABLE	VSTable,
LPD3DXCONSTANTTABLE	PSTable)
{
	unsigned int i;
	D3DXVECTOR4 tempVec;
	for( i=0 ; i<m_nummaterial ; i++ )
	{

		tempVec.x = 1;
		tempVec.y = 1;
		tempVec.z = 1;
		tempVec.w = 1;
		VSTable->SetVector(lpd3ddevice, "g_diffuse_power", &tempVec);

		tempVec.x = m_lpmeshmaterials[i].Ambient.r;
		tempVec.y = m_lpmeshmaterials[i].Ambient.g;
		tempVec.z = m_lpmeshmaterials[i].Ambient.b;
		tempVec.w = m_lpmeshmaterials[i].Ambient.a;
		VSTable->SetVector(lpd3ddevice, "g_ambient_material", &tempVec);
		PSTable->SetVector(lpd3ddevice, "g_ambient_material", &tempVec);

		tempVec.x = m_lpmeshmaterials[i].Diffuse.r;
		tempVec.y = m_lpmeshmaterials[i].Diffuse.g;
		tempVec.z = m_lpmeshmaterials[i].Diffuse.b;
		tempVec.w = m_lpmeshmaterials[i].Diffuse.a;
		VSTable->SetVector(lpd3ddevice, "g_diffuse_material", &tempVec);
		PSTable->SetVector(lpd3ddevice, "g_diffuse_material", &tempVec);


		tempVec.x = m_lpmeshmaterials[i].Specular.r;
		tempVec.y = m_lpmeshmaterials[i].Specular.g;
		tempVec.z = m_lpmeshmaterials[i].Specular.b;
		tempVec.w = m_lpmeshmaterials[i].Specular.a;
		VSTable->SetVector(lpd3ddevice, "g_specular_material", &tempVec);
		
		
		if (m_lpmeshtextures[i] != NULL)
		{
			int index = PSTable->GetSamplerIndex("Sampler1");
			PSTable->SetBool(lpd3ddevice, "istheretexture", true);
			lpd3ddevice->SetTexture(index, m_lpmeshtextures[i]);		// テクスチャのセット
		}
		else
		{
			PSTable->SetBool(lpd3ddevice, "istheretexture", false);
		}
		lpd3ddevice->SetMaterial(&m_lpmeshmaterials[i]);	// マテリアルのセット
		
		
		lpd3ddevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		m_lpmesh->DrawSubset(i);							// サブセットの描画
		
	}

}

//==============================================================================
//!	@fn		DrawWithAxis
//!	@brief	Xファイル描画(3軸を描画する)
//!	@param	デバイスオブジェクト
//!	@retval	なし
//!	@note	
//==============================================================================
void CDirect3DXFile::DrawWithAxis(LPDIRECT3DDEVICE9 lpd3ddevice)
{
	unsigned int i;

	DrawAxis(lpd3ddevice);	// ３軸の描画

	for(i=0;i<m_nummaterial;i++){	// サブセットの描画
		lpd3ddevice->SetMaterial(&m_lpmeshmaterials[i]);	// マテリアルのセット
		lpd3ddevice->SetTexture(0,m_lpmeshtextures[i]);		// テクスチャのセット
		m_lpmesh->DrawSubset(i);							// サブセットの描画
	}
}

//==============================================================================
//!	@fn		DrawAxis
//!	@brief	3軸を描画する
//!	@param	デバイスオブジェクト
//!	@retval	なし
//!	@note	
//==============================================================================
void CDirect3DXFile::DrawAxis(LPDIRECT3DDEVICE9 lpd3ddevice)
{
	struct VERTEXDATA{
		float		x,y,z;
		D3DCOLOR	color;
	};

	// ３軸の頂点データ
	struct	VERTEXDATA	linevertices[6]={
		-400,   0,   0,  D3DCOLOR_XRGB(255,0,0),	// Ｘ軸始点 赤
		 400,   0,   0,  D3DCOLOR_XRGB(255,0,0),	// Ｘ軸終点 赤
		   0,-400,   0,  D3DCOLOR_XRGB(0,255,0),	// Ｙ軸始点 緑
		   0, 400,   0,  D3DCOLOR_XRGB(0,255,0),	// Ｙ軸終点 緑
		   0,   0,-400,  D3DCOLOR_XRGB(0,0,255),	// Ｚ軸始点 青
		   0,   0, 400,  D3DCOLOR_XRGB(0,0,255)		// Ｚ軸終点 青
	};

	lpd3ddevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);	// 頂点フォーマットをセット
	lpd3ddevice->SetRenderState(D3DRS_LIGHTING,false);	//   光源計算オフ（光源計算を切るとDIFFUSE色がそのままでる）

	lpd3ddevice->DrawPrimitiveUP(D3DPT_LINELIST, 3, &linevertices[0], sizeof(VERTEXDATA));	// ３軸を描画
	lpd3ddevice->SetRenderState(D3DRS_LIGHTING,true);	//   光源計算ON
}

//==============================================================================
//!	@fn		GetMesh
//!	@brief	メッシュオブジェクトポインタを戻す
//!	@param	デバイスオブジェクト
//!	@retval	なし
//!	@note	
//==============================================================================
LPD3DXMESH CDirect3DXFile::GetMesh() const{
	return m_lpmesh;
}

LPDIRECT3DTEXTURE9* CDirect3DXFile::GetTexture() const
{
	return m_lpmeshtextures;
}



bool CDirect3DXFile::AddTangentSpace(LPDIRECT3DDEVICE9 lpdevice)
{
	LPD3DXMESH tempMesh;
	LPD3DXMESH outMesh;
	HRESULT hr;

	const D3DVERTEXELEMENT9 Decl[] =
	{
		{ 0,0,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION,0 },
	{ 0,12,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,0 },
	{ 0,20,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_NORMAL,0 },
	{ 0,32,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TANGENT,0 },
	{ 0,44,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_BINORMAL,0 },
	D3DDECL_END()
	};

	hr = m_lpmesh->CloneMesh(m_lpmesh->GetOptions(), Decl, lpdevice, &tempMesh);
	if (FAILED(hr))
	{
		MessageBox(NULL, "ERROR", "CloneMeshCopyFail", MB_OK);
		return false;
	}

	hr = D3DXComputeNormals(tempMesh, NULL);
	if (FAILED(hr))
	{
		tempMesh->Release();
		MessageBox(NULL, "ERROR", "ComputeNormalFail", MB_OK);
		return false;
	}

	hr = D3DXComputeTangentFrameEx(tempMesh,
		D3DDECLUSAGE_TEXCOORD, 0,				//0番目のテクスチャ座標を使用
		D3DDECLUSAGE_TANGENT, 0,				//0番目の接ベクトルを使用
		D3DDECLUSAGE_BINORMAL, 0,				//0番目の従法線ベクトルを計算する
		D3DDECLUSAGE_NORMAL, 0,					//0番目の法線ベクトルを使用する
		0,
		NULL,
		-1.01f,									//隣接する三角形の閾値
		-0.01f,									//単独とみなされる頂点の閾値
		-1.01f,									//法線の閾値
		&outMesh,								//生成されたメッシュ
		NULL);
	if (FAILED(hr))
	{
		tempMesh->Release();
		MessageBox(NULL, "ERROR", "ComputeTangentFail", MB_OK);
		return false;
	}
	m_lpmesh->Release();
	m_lpmesh = outMesh;
	return true;
}
//******************************************************************************
//	End of file.
//******************************************************************************