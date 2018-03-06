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
	const char*					m_EntryFunction;
	const char*					m_ShaderModel;
public:
	CShader(LPDIRECT3DDEVICE9 device, const char* filename, const char* entrypoint,const char* shadermodel);
	~CShader();
	bool InitShader();
	bool ShaderCompile();
    bool CreatePixelShader();
	bool CreateVertexShader();
	
	

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
	LPDIRECT3DDEVICE9	device,					// デバイスオブジェクト
	LPDIRECT3DSURFACE9& currentsurface,			// 現在の描画サーフェスを保存するためのサーフェス
	LPDIRECT3DSURFACE9& currentzbuffer, 		// 現在のＺバッファを保存するためのサーフェス
	D3DVIEWPORT9& currentvp);					// 現在のビューポートを保存するためのもの

void SetRenderTarget(
	LPDIRECT3DDEVICE9  device,					// デバイスオブジェクト	
	LPDIRECT3DSURFACE9 rendersurface,			// レンダーターゲット
	LPDIRECT3DSURFACE9 zbuffer,					// Ｚバッファ
	D3DVIEWPORT9&		vp);					// ビューポート


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
