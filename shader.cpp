#include "shader.h"
CShader::CShader(LPDIRECT3DDEVICE9 device,
	const char* filename,
	const char* VSShaderModel,
	const char* VSEntryPoint,
	const char* PSShaderModel,
	const char* PSEntryPoint)
{
	m_pDXDevice = device;
	m_Filename = filename;
	m_VSEntryPoint = VSEntryPoint;
	m_VSShaderModel = VSShaderModel;
	m_PSEntryPoint = PSEntryPoint;
	m_PSShaderModel = PSShaderModel;
}

CShader::~CShader()
{
	m_pVertexShader->Release();
	m_pPixelShader->Release();
	m_pVSConstantTable->Release();
	m_pPSConstantTable->Release();
	m_pDXDevice->Release();
	m_Buffer->Release();
}

bool CShader::InitShader()
{
	bool sts;
	sts = CreatePixelShader();
	if (sts == false)
	{
		MessageBox(NULL, "ERROR IN InitShader_PS_CreatePixelShader","ERROR" , MB_OK);
		return false;
	}
	sts = CreateVertexShader();
	if (sts == false)
	{
		MessageBox(NULL,  "ERROR IN InitShader_VS_CreateVertexShader", "ERROR", MB_OK);
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
		m_PSEntryPoint,
		m_PSShaderModel,
		0,
		&m_Buffer,
		&err,
		&m_pPSConstantTable);
	if (FAILED(hr))
	{
		if (err)
		{
			MessageBox(NULL, "ERROR IN D3DXCompileShaderFromFile", (LPSTR)err->GetBufferPointer(), MB_OK);
		}
		else
		{
			MessageBox(NULL, "ERROR IN PS_D3DXCompileShaderFromFile","ERROR", MB_OK);
		}
		return false;
	}

	hr = m_pDXDevice->CreatePixelShader((DWORD*)m_Buffer->GetBufferPointer(), &m_pPixelShader);
	if (FAILED(hr))
	{
		MessageBox(NULL, "ERROR IN CreatePixelShader","ERROR", MB_OK);
		return false;
	}
	return true;
}
bool CShader::CreateVertexShader()
{
	LPD3DXBUFFER err = nullptr;
	HRESULT hr;
	hr = D3DXCompileShaderFromFile(
		m_Filename,
		nullptr,
		nullptr,
		m_VSEntryPoint,
		m_VSShaderModel,
		0,
		&m_Buffer,
		&err,
		&m_pVSConstantTable);
	if (FAILED(hr))
	{
		if (err)
		{
			MessageBox(NULL, "ERROR IN VS_D3DXCompileShaderFromFile", (LPSTR)err->GetBufferPointer(), MB_OK);
		}
		else
		{
			MessageBox(NULL, "ERROR IN VS_D3DXCompileShaderFromFile", "ERROR", MB_OK);
		}
		return false;
	}
	hr = m_pDXDevice->CreateVertexShader((DWORD*)m_Buffer->GetBufferPointer(), &m_pVertexShader);
	if (FAILED(hr))
	{
		MessageBox(NULL, "ERROR IN CreateVertexShader","ERROR" , MB_OK);
		return false;
	}
	return true;
}

LPDIRECT3DVERTEXSHADER9 CShader::GetVertexShader()
{
	return m_pVertexShader;
}

LPD3DXCONSTANTTABLE	CShader::GetVSTable()
{
	return m_pVSConstantTable;
}

LPDIRECT3DPIXELSHADER9 CShader::GetPixelShader()
{
	return m_pPixelShader;
}

LPD3DXCONSTANTTABLE	CShader::GetPSTable()
{
	return m_pPSConstantTable;
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
	hr = (*ppRenderTexture)->GetSurfaceLevel(0, ppRenderSurface);		//生成したテクスチャのサーフェスオブジェクトを取得

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
	LPDIRECT3DDEVICE9	device,					// デバイスオブジェクト
	LPDIRECT3DSURFACE9& currentsurface,			// 現在の描画サーフェスを保存するためのサーフェス
	LPDIRECT3DSURFACE9& currentzbuffer, 		// 現在のＺバッファを保存するためのサーフェス
	D3DVIEWPORT9& currentvp)					// 現在のビューポートを保存するためのもの
{
	device->GetRenderTarget(0, &currentsurface);
	device->GetViewport(&currentvp);
	device->GetDepthStencilSurface(&currentzbuffer);
}
void SetRenderTarget(
	LPDIRECT3DDEVICE9  device,					// デバイスオブジェクト	
	LPDIRECT3DSURFACE9 rendersurface,			// レンダリングサーフェス
	LPDIRECT3DSURFACE9 zbuffer,					// Ｚバッファ
	D3DVIEWPORT9&		vp)						// ビューポート
{
	device->SetRenderTarget(0, rendersurface);	//0 = COLOR0
	device->SetViewport(&vp);
	device->SetDepthStencilSurface(zbuffer);
}
