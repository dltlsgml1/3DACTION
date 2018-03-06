#include "shader.h"
CShader::CShader(LPDIRECT3DDEVICE9 device, const char* filename, const char* entrypoint, const char* shadermodel)
{
	m_pDXDevice = device;
	m_Filename = filename;
	m_EntryFunction = entrypoint;
	m_ShaderModel = shadermodel;
}

CShader::~CShader()
{
	m_pVertexShader->Release();
	m_pPixelShader->Release();
	m_pVSConstantTable->Release();
	m_pPSConstantTable->Release();
}

bool CShader::InitShader()
{
	bool sts;
	sts = CreatePixelShader();
	if (sts == false)
	{
		MessageBox(NULL, "ERROR", "ERROR IN CreatePixelShader", MB_OK);
		return false;
	}
	return true;
}


bool CShader::CreatePixelShader()
{
	LPD3DXBUFFER err = nullptr;
	HRESULT hr;

	hr = D3DXCompileShaderFromFile(
		m_Filename,
		nullptr,
		nullptr,
		m_EntryFunction,
		m_ShaderModel,
		0,
		&m_Buffer,
		&err,
		&m_pPSConstantTable);
	if (FAILED(hr))
	{
		if (err)
		{
			MessageBox(NULL, (LPSTR)err->GetBufferPointer(), "ERROR IN D3DXCompileShaderFromFile", MB_OK);
		}
		else
		{
			MessageBox(NULL,"ERROR", "ERROR IN D3DXCompileShaderFromFile", MB_OK);
		}
		return false;
	}

	hr = m_pDXDevice->CreatePixelShader((DWORD*)m_Buffer->GetBufferPointer(), &m_pPixelShader);
	if (FAILED(hr))
	{
		MessageBox(NULL, "ERROR", "ERROR IN CreatePixelShader", MB_OK);
		return false;
	}
	return true;
}
bool CShader::CreateVertexShader()
{

}

bool ShaderCompile(const char* filename,
	const char* entry,
	const char* version,
	LPD3DXBUFFER *code,
	LPD3DXCONSTANTTABLE *ctbl)
{
	LPD3DXBUFFER err = nullptr;
	HRESULT hr;

	hr = D3DXCompileShaderFromFile
	(
		filename,
		nullptr,
		nullptr,
		entry,
		version,
		0,
		code,
		&err,
		ctbl);
	if (FAILED(hr))
	{
		if (err)
		{
			MessageBox(NULL, (LPSTR)err->GetBufferPointer(),
				"error in D3DXCompileShaderFromFile", MB_OK);
		}
		else
		{
			MessageBox(NULL, "error!", "D3DXCompileShaderFromFile", MB_OK);
		}
		return false;
	}
	return true;
}


bool PixelShaderCompile(LPDIRECT3DDEVICE9 device,
	const char* filename,
	const char* entry,
	const char* version,
	LPD3DXCONSTANTTABLE *ctbl,
	LPDIRECT3DPIXELSHADER9 *psh)
{
	bool sts;
	LPD3DXBUFFER code;

	sts = ShaderCompile(
		filename,
		entry,
		version,
		&code,
		ctbl);
	if (sts == false)
	{
		return false;
	}

	HRESULT hr = device->CreatePixelShader((DWORD*)code->GetBufferPointer(), psh);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "createpixelshader error", "createpixelshader error", MB_OK);
		return false;
	}
	return true;

}

bool VertexShaderCompile(LPDIRECT3DDEVICE9 device,
	const char* filename,
	const char* entry,
	const char* version,
	LPD3DXCONSTANTTABLE *ctbl,
	LPDIRECT3DVERTEXSHADER9 *vsh)
{
	bool sts;
	LPD3DXBUFFER code;

	sts = ShaderCompile(
		filename,
		entry,
		version,
		&code,
		ctbl);
	if (sts == false)
	{
		return false;
	}

	HRESULT hr = device->CreateVertexShader((DWORD*)code->GetBufferPointer(), vsh);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "createvertexshader error", "createvertexshader error", MB_OK);
		return false;
	}
	return true;

}

bool InitShader(LPDIRECT3DDEVICE9 device,
				const char *filename,
				LPDIRECT3DVERTEXSHADER9 *vs, 
				LPD3DXCONSTANTTABLE *vstable,
				LPDIRECT3DPIXELSHADER9 *ps, 
				LPD3DXCONSTANTTABLE *pstable)
{
	bool sts;
	sts = VertexShaderCompile(
		device,
		filename,
		"VS",
		"vs_3_0",
		vstable,
		vs);
	if (sts == false)
	{
		return false;
	}
	sts = PixelShaderCompile(
		device,
		filename,
		"PS",
		"ps_3_0",
		pstable,
		ps);
	if (sts == false)
	{
		return false;
	}
	return true;
}

void ExitShader(LPDIRECT3DVERTEXSHADER9 *vs,
				LPD3DXCONSTANTTABLE *vstable,
				LPDIRECT3DPIXELSHADER9 *ps,
				LPD3DXCONSTANTTABLE *pstable)
{
	if (vs != nullptr)
	{
		(*vs)->Release();
		vs = nullptr;
	}
	if (vstable != nullptr)
	{
		(*vstable)->Release();
		vstable = nullptr;
	}
	if (ps != nullptr)
	{
		(*ps)->Release();
		ps = nullptr;
	}
	if (pstable != nullptr)
	{
		(*pstable)->Release();
		pstable = nullptr;
	}
}


bool CreateRenderTarget(LPDIRECT3DDEVICE9 device,
	int width,
	int height,
	D3DFORMAT fmt,
	LPDIRECT3DTEXTURE9 *ppRenderTexture,
	LPDIRECT3DSURFACE9 *ppRenderSurface,
	LPDIRECT3DSURFACE9 *ppZbufferSurface)
{
	HRESULT hr;
	hr = device->CreateTexture( width,
								height,
								1,
								D3DUSAGE_RENDERTARGET,
								fmt,
								D3DPOOL_DEFAULT,
								ppRenderTexture,
								nullptr);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "ERROR", "Failed to Create Texture", MB_OK);
		return false;
	}
	hr = (*ppRenderTexture)->GetSurfaceLevel(0, ppRenderSurface);		//���������e�N�X�`���̃T�[�t�F�X�I�u�W�F�N�g���擾

	hr = device->CreateDepthStencilSurface( width,
											height,
											D3DFMT_D16,
											D3DMULTISAMPLE_NONE,
											0,
											TRUE,
											ppZbufferSurface,
											nullptr);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "ERROR", "Failed to Create Zbuffer", MB_OK);
		return false;
	}
	return true;
}

void GetCurrentRendertarget(
	LPDIRECT3DDEVICE9	device,					// �f�o�C�X�I�u�W�F�N�g
	LPDIRECT3DSURFACE9& currentsurface,			// ���݂̕`��T�[�t�F�X��ۑ����邽�߂̃T�[�t�F�X
	LPDIRECT3DSURFACE9& currentzbuffer, 		// ���݂̂y�o�b�t�@��ۑ����邽�߂̃T�[�t�F�X
	D3DVIEWPORT9& currentvp)					// ���݂̃r���[�|�[�g��ۑ����邽�߂̂���
{
	device->GetRenderTarget(0, &currentsurface);
	device->GetViewport(&currentvp);
	device->GetDepthStencilSurface(&currentzbuffer);
}
void SetRenderTarget(
	LPDIRECT3DDEVICE9  device,					// �f�o�C�X�I�u�W�F�N�g	
	LPDIRECT3DSURFACE9 rendersurface,			// �����_�����O�T�[�t�F�X
	LPDIRECT3DSURFACE9 zbuffer,					// �y�o�b�t�@
	D3DVIEWPORT9&		vp)						// �r���[�|�[�g
{
	device->SetRenderTarget(0, rendersurface);	//0 = COLOR0
	device->SetViewport(&vp);
	device->SetDepthStencilSurface(zbuffer);
}
