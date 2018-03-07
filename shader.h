#pragma once
#include <d3dx9.h>

class CShader
{
private:
	LPDIRECT3DDEVICE9			m_pDXDevice;
	LPDIRECT3DVERTEXSHADER9		m_pVertexShader;
	LPD3DXCONSTANTTABLE			m_pVSConstantTable;
	LPDIRECT3DPIXELSHADER9		m_pPixelShader;
	LPD3DXCONSTANTTABLE			m_pPSConstantTable;
	LPD3DXBUFFER				m_Buffer;
	const char*					m_Filename;
	const char*					m_VSEntryPoint;
	const char*					m_PSEntryPoint;
	const char*					m_VSShaderModel;
	const char*					m_PSShaderModel;
	
public:
	CShader(LPDIRECT3DDEVICE9 device, 
		const char* filename, 
		const char* VSShaderModel,
		const char* VSEntryPoint,
		const char* PSShaderModel,
		const char* PSEntryPoint);
	~CShader();
	bool						InitShader();
	bool						CreatePixelShader();
	bool						CreateVertexShader();
	LPDIRECT3DVERTEXSHADER9		GetVertexShader();
	LPD3DXCONSTANTTABLE			GetVSTable();
	LPDIRECT3DPIXELSHADER9		GetPixelShader();
	LPD3DXCONSTANTTABLE			GetPSTable();
	

};

bool ShaderCompile(const char* filename,
					const char* entry,
					const char* version,
					LPD3DXBUFFER *code,
					LPD3DXCONSTANTTABLE *ctbl);
bool PixelShaderCompile(LPDIRECT3DDEVICE9 device,
						const char* filename,
						const char* entry,
						const char* version,
						LPD3DXCONSTANTTABLE *ctbl,
						LPDIRECT3DPIXELSHADER9 *psh);
bool VertexShaderCompile(LPDIRECT3DDEVICE9 device,
						const char* filename,
						const char* entry,
						const char* version,
						LPD3DXCONSTANTTABLE *ctbl,
						LPDIRECT3DVERTEXSHADER9 *vsh);

bool CreateRenderTarget(LPDIRECT3DDEVICE9 device,
	int width,
	int height,
	D3DFORMAT fmt,
	LPDIRECT3DTEXTURE9 *ppRenderTexture,
	LPDIRECT3DSURFACE9 *ppRenderSurface,
	LPDIRECT3DSURFACE9 *ppZbufferSurface);

void GetCurrentRendertarget(
	LPDIRECT3DDEVICE9	device,					// �f�o�C�X�I�u�W�F�N�g
	LPDIRECT3DSURFACE9& currentsurface,			// ���݂̕`��T�[�t�F�X��ۑ����邽�߂̃T�[�t�F�X
	LPDIRECT3DSURFACE9& currentzbuffer, 		// ���݂̂y�o�b�t�@��ۑ����邽�߂̃T�[�t�F�X
	D3DVIEWPORT9& currentvp);					// ���݂̃r���[�|�[�g��ۑ����邽�߂̂���

void SetRenderTarget(
	LPDIRECT3DDEVICE9  device,					// �f�o�C�X�I�u�W�F�N�g	
	LPDIRECT3DSURFACE9 rendersurface,			// �����_�[�^�[�Q�b�g
	LPDIRECT3DSURFACE9 zbuffer,					// �y�o�b�t�@
	D3DVIEWPORT9&		vp);					// �r���[�|�[�g


bool InitShader(LPDIRECT3DDEVICE9 device,
				const char* filename,
				LPDIRECT3DVERTEXSHADER9 *vs,
				LPD3DXCONSTANTTABLE *vstable,
				LPDIRECT3DPIXELSHADER9 *ps,
				LPD3DXCONSTANTTABLE *pstable);
void ExitShader(LPDIRECT3DVERTEXSHADER9 *vs,
				LPD3DXCONSTANTTABLE *vstable, 
				LPDIRECT3DPIXELSHADER9 *ps, 
				LPD3DXCONSTANTTABLE *pstable);
