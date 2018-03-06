//*****************************************************************************
//!	@file	CDirect3DXFile.cpp
//!	@brief	X�t�@�C���ǂݍ��݁A�`��N���X
//!	@note	
//!	@author	
//*****************************************************************************

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include	"CDirect3DXFile.h"

//==============================================================================
//!	@fn		LoadXFile
//!	@brief	X�t�@�C���ǂݍ���
//!	@param	X�t�@�C����
//!	@param	�f�o�C�X�I�u�W�F�N�g
//!	@retval	TRUE�@�����I��/FALSE�@���s�I��
//!	@note	
//==============================================================================


bool CDirect3DXFile::LoadXFile( char *xfilename, LPDIRECT3DDEVICE9 lpd3ddevice )
{
	LPD3DXBUFFER	pd3dxmtrlbuffer;
	LPD3DXBUFFER	pd3dxmtrlbuffer2;
	HRESULT			hr;
	unsigned int	i;
	// �w�t�@�C����ǂݍ���
	hr = D3DXLoadMeshFromX(	xfilename,				// �w�t�@�C����
							D3DXMESH_SYSTEMMEM,		// �ǂݍ��݃�����
							lpd3ddevice,			// �f�o�C�X�I�u�W�F�N�g
							NULL,
							&pd3dxmtrlbuffer,		// �}�e���A���o�b�t�@
							&pd3dxmtrlbuffer2,		// �}�e���A���o�b�t�@2
							&m_nummaterial,			// �}�e���A����
							&m_lpmesh);				// ���b�V��

	if( SUCCEEDED(hr) ){
		D3DXMATERIAL	*d3dxmaterials = (D3DXMATERIAL*)pd3dxmtrlbuffer->GetBufferPointer();	// �}�e���A���̃A�h���X���擾
		m_lpmeshmaterials = new D3DMATERIAL9[m_nummaterial];		// �}�e���A���R���e�i�̍쐬
		m_lpmeshtextures = new LPDIRECT3DTEXTURE9[m_nummaterial];	// �e�N�X�`���R���e�i�̐���
		
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
		pd3dxmtrlbuffer->Release();		// �}�e���A���o�b�t�@�̃����[�X
		pd3dxmtrlbuffer2->Release();	// �}�e���A���o�b�t�@�̃����[�X
	}else{
		return false;
	}
	
	return true;
}

//==============================================================================
//!	@fn		UnLoadXFile
//!	@brief	X�t�@�C���I�u�W�F�N�g�A�����[�h
//!	@param	
//!	@retval	�Ȃ�
//!	@note	
//==============================================================================
void CDirect3DXFile::UnLoadXFile()
{
	unsigned int i;

	if(m_lpmeshmaterials!=NULL){	// �}�e���A���I�u�W�F�N�g�̉��
		delete [] m_lpmeshmaterials;
		m_lpmeshmaterials=NULL;
	}

	if(m_lpmeshtextures!=NULL){		// �e�N�X�`���I�u�W�F�N�g�̉��
		for( i=0 ; i<m_nummaterial ; i++ ){
			if(m_lpmeshtextures[i]!=NULL)	m_lpmeshtextures[i]->Release();
		}
		delete [] m_lpmeshtextures;
		m_lpmeshtextures=NULL;
	}

	if(m_lpmesh!=NULL){		// ���b�V�����
		m_lpmesh->Release();
		m_lpmesh=NULL;
	}
}

//==============================================================================
//!	@fn		Draw
//!	@brief	X�t�@�C���`��
//!	@param	�f�o�C�X�I�u�W�F�N�g
//!	@retval	�Ȃ�
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
			lpd3ddevice->SetTexture(index, m_lpmeshtextures[i]);		// �e�N�X�`���̃Z�b�g
		}
		else
		{
			PSTable->SetBool(lpd3ddevice, "istheretexture", false);
		}
		lpd3ddevice->SetMaterial(&m_lpmeshmaterials[i]);	// �}�e���A���̃Z�b�g
		
		
		lpd3ddevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		m_lpmesh->DrawSubset(i);							// �T�u�Z�b�g�̕`��
		
	}

}

//==============================================================================
//!	@fn		DrawWithAxis
//!	@brief	X�t�@�C���`��(3����`�悷��)
//!	@param	�f�o�C�X�I�u�W�F�N�g
//!	@retval	�Ȃ�
//!	@note	
//==============================================================================
void CDirect3DXFile::DrawWithAxis(LPDIRECT3DDEVICE9 lpd3ddevice)
{
	unsigned int i;

	DrawAxis(lpd3ddevice);	// �R���̕`��

	for(i=0;i<m_nummaterial;i++){	// �T�u�Z�b�g�̕`��
		lpd3ddevice->SetMaterial(&m_lpmeshmaterials[i]);	// �}�e���A���̃Z�b�g
		lpd3ddevice->SetTexture(0,m_lpmeshtextures[i]);		// �e�N�X�`���̃Z�b�g
		m_lpmesh->DrawSubset(i);							// �T�u�Z�b�g�̕`��
	}
}

//==============================================================================
//!	@fn		DrawAxis
//!	@brief	3����`�悷��
//!	@param	�f�o�C�X�I�u�W�F�N�g
//!	@retval	�Ȃ�
//!	@note	
//==============================================================================
void CDirect3DXFile::DrawAxis(LPDIRECT3DDEVICE9 lpd3ddevice)
{
	struct VERTEXDATA{
		float		x,y,z;
		D3DCOLOR	color;
	};

	// �R���̒��_�f�[�^
	struct	VERTEXDATA	linevertices[6]={
		-400,   0,   0,  D3DCOLOR_XRGB(255,0,0),	// �w���n�_ ��
		 400,   0,   0,  D3DCOLOR_XRGB(255,0,0),	// �w���I�_ ��
		   0,-400,   0,  D3DCOLOR_XRGB(0,255,0),	// �x���n�_ ��
		   0, 400,   0,  D3DCOLOR_XRGB(0,255,0),	// �x���I�_ ��
		   0,   0,-400,  D3DCOLOR_XRGB(0,0,255),	// �y���n�_ ��
		   0,   0, 400,  D3DCOLOR_XRGB(0,0,255)		// �y���I�_ ��
	};

	lpd3ddevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);	// ���_�t�H�[�}�b�g���Z�b�g
	lpd3ddevice->SetRenderState(D3DRS_LIGHTING,false);	//   �����v�Z�I�t�i�����v�Z��؂��DIFFUSE�F�����̂܂܂ł�j

	lpd3ddevice->DrawPrimitiveUP(D3DPT_LINELIST, 3, &linevertices[0], sizeof(VERTEXDATA));	// �R����`��
	lpd3ddevice->SetRenderState(D3DRS_LIGHTING,true);	//   �����v�ZON
}

//==============================================================================
//!	@fn		GetMesh
//!	@brief	���b�V���I�u�W�F�N�g�|�C���^��߂�
//!	@param	�f�o�C�X�I�u�W�F�N�g
//!	@retval	�Ȃ�
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
		D3DDECLUSAGE_TEXCOORD, 0,				//0�Ԗڂ̃e�N�X�`�����W���g�p
		D3DDECLUSAGE_TANGENT, 0,				//0�Ԗڂ̐ڃx�N�g�����g�p
		D3DDECLUSAGE_BINORMAL, 0,				//0�Ԗڂ̏]�@���x�N�g�����v�Z����
		D3DDECLUSAGE_NORMAL, 0,					//0�Ԗڂ̖@���x�N�g�����g�p����
		0,
		NULL,
		-1.01f,									//�אڂ���O�p�`��臒l
		-0.01f,									//�P�ƂƂ݂Ȃ���钸�_��臒l
		-1.01f,									//�@����臒l
		&outMesh,								//�������ꂽ���b�V��
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