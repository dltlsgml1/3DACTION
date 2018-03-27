//=============================================================================
//
// 入力処理 [input.cpp]
//
//=============================================================================
#include "input.h"

//*****************************************************************************
// 定数定義
//*****************************************************************************
const int NUM_KEY_MAX = 256;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd);
void UninitKeyboard(void);
HRESULT UpdateKeyboard(void);
HRESULT InitMouse(HINSTANCE hInst, HWND hWnd);
void UninitMouse(void);
HRESULT UpdateMouse(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECTINPUT8			m_pDInput = NULL;					// IDirectInput8インターフェースへのポインタ
LPDIRECTINPUTDEVICE8	m_pDIDevKeyboard = NULL;			// IDirectInputDevice8インターフェースへのポインタ(キーボード)
LPDIRECTINPUTDEVICE8	m_pDIDevMouse = NULL;				// IDirectInputDevice8インターフェースへのポインタ(マウス)
BYTE					m_keyState[NUM_KEY_MAX];			// キーボードの状態を受け取るワーク
BYTE					m_keyStateTrigger[NUM_KEY_MAX];		// キーボードの状態を受け取るワーク
BYTE					m_keyStateRepeat[NUM_KEY_MAX];		// キーボードの状態を受け取るワーク
BYTE					m_keyStateRelease[NUM_KEY_MAX];		// キーボードの状態を受け取るワーク
int						m_keyStateRepeatCnt[NUM_KEY_MAX];	// キーボードのリピートカウンタ
DIMOUSESTATE2			m_mouseState;						// マウスの状態を受け取るワーク
DIMOUSESTATE2			m_mouseStateTrigger;				// マウスの状態を受け取るワーク
POINT					m_MousePoint;						// マウスの現在座標
HWND					m_hwnd;

//=============================================================================
// 入力処理の初期化
//=============================================================================
HRESULT InitInput(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	if(!m_pDInput)
	{
		// DirectInputオブジェクトの作成
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
									IID_IDirectInput8, (void**)&m_pDInput, NULL);
	}

	// キーボードの初期化
	InitKeyboard(hInst, hWnd);

	// マウスの初期化
	InitMouse(hInst, hWnd);

	m_hwnd=hWnd;

	return hr;
}

//=============================================================================
// 入力処理の終了処理
//=============================================================================
void UninitInput(void)
{
	// キーボードの終了処理
	UninitKeyboard();

	// マウスの終了処理
	UninitMouse();

	if(m_pDInput)
	{
		m_pDInput->Release();
		m_pDInput = NULL;
	}
}

//=============================================================================
// 入力処理の更新処理
//=============================================================================
void UpdateInput(void)
{
	// キーボードの更新
	UpdateKeyboard();

	// マウスの更新
	UpdateMouse();
}

//=============================================================================
// キーボードの初期化
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// デバイスオブジェクトを作成
	hr = m_pDInput->CreateDevice(GUID_SysKeyboard, &m_pDIDevKeyboard, NULL);
	if(FAILED(hr) || m_pDIDevKeyboard == NULL)
	{
		MessageBox(hWnd, "キーボードがねぇ！", "警告！", MB_ICONWARNING);
		return hr;
	}

	// データフォーマットを設定
	hr = m_pDIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "キーボードのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = m_pDIDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(hr))
	{
		MessageBox(hWnd, "キーボードの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	m_pDIDevKeyboard->Acquire();

	return S_OK;
}

//=============================================================================
// キーボードの終了処理
//=============================================================================
void UninitKeyboard(void)
{
	if(m_pDIDevKeyboard)
	{
		m_pDIDevKeyboard->Unacquire();

		m_pDIDevKeyboard->Release();
		m_pDIDevKeyboard = NULL;
	}
}

//=============================================================================
// キーボードの更新処理
//=============================================================================
HRESULT UpdateKeyboard(void)
{
	HRESULT hr;
	BYTE keyStateOld[256];

	// 前回のデータを保存
	memcpy(keyStateOld, m_keyState, NUM_KEY_MAX);

	// デバイスからデータを取得
	hr = m_pDIDevKeyboard->GetDeviceState(sizeof(m_keyState), m_keyState);
	if(SUCCEEDED(hr))
	{
		for(int cnt = 0; cnt < NUM_KEY_MAX; cnt++)
		{
			// トリガーとリリースを取得
			m_keyStateTrigger[cnt] = ( m_keyState[cnt] ^ keyStateOld[cnt] )& m_keyState[cnt];
			m_keyStateRelease[cnt] = ( m_keyState[cnt] ^ keyStateOld[cnt] )& keyStateOld[cnt];
			
			// キーが押されているならリピートの判定処理
			if(m_keyState[cnt])
			{
				if(m_keyStateRepeatCnt[cnt] < 20)
				{
					m_keyStateRepeatCnt[cnt]++;
					// 「初回入力」もしくは「ボタンを押してから20フレーム経過」
					// した場合、リピート用配列のデータを有効にする
					//if(  ||  )
					// 上記条件以外はリピート用配列のデータを無効にする
					//else
					if((m_keyStateRepeatCnt[cnt]==1)  || m_keyStateRepeatCnt[cnt] >=20){
						m_keyStateRepeat[cnt]=m_keyState[cnt];
					}
					else{
						m_keyStateRepeat[cnt]=0;
					}
				}
			}
			else
			{
				m_keyStateRepeatCnt[cnt] = 0;
				m_keyStateRepeat[cnt] = 0;
			}
		}
	}
	else
	{
		// キーボードへのアクセス権を取得
		hr = m_pDIDevKeyboard->Acquire();
	}

	return hr;
}

//=============================================================================
// キーボードのプレス状態を取得
//=============================================================================
bool GetKeyboardPress(int key)
{
	return ((m_keyState[key] & 0x80) != 0);
}

//=============================================================================
// キーボードのトリガー状態を取得
//=============================================================================
bool GetKeyboardTrigger(int key)
{
	return ((m_keyStateTrigger[key] & 0x80) != 0);
}

//=============================================================================
// キーボードのリピート状態を取得
//=============================================================================
bool GetKeyboardRepeat(int key)
{
	return ((m_keyStateRepeat[key] & 0x80) != 0);
}

//=============================================================================
// キーボードのリリ−ス状態を取得
//=============================================================================
bool GetKeyboardRelease(int key)
{
	return ((m_keyStateRelease[key] & 0x80) != 0);
}

//=============================================================================
// マウスの初期化
//=============================================================================
HRESULT InitMouse(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// デバイスオブジェクトを作成
	hr = m_pDInput->CreateDevice(GUID_SysMouse, &m_pDIDevMouse, NULL);
	if(FAILED(hr) || m_pDIDevMouse == NULL)
	{
		MessageBox(hWnd, "マウスがねぇ！", "警告！", MB_ICONWARNING);
		return hr;
	}

	// データフォーマットを設定
	hr = m_pDIDevMouse->SetDataFormat(&c_dfDIMouse2);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "マウスのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = m_pDIDevMouse->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(hr))
	{
		MessageBox(hWnd, "マウスの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// デバイスの設定
	{
		DIPROPDWORD dipdw;

		dipdw.diph.dwSize = sizeof(dipdw);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwObj = 0;
		dipdw.diph.dwHow = DIPH_DEVICE;
		dipdw.dwData = DIPROPAXISMODE_REL;

		hr = m_pDIDevMouse->SetProperty(DIPROP_AXISMODE, &dipdw.diph);
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	m_pDIDevMouse->Acquire();

	return S_OK;
}

//=============================================================================
// マウスの終了処理
//=============================================================================
void UninitMouse(void)
{
	if(m_pDIDevMouse)
	{
		m_pDIDevMouse->Unacquire();

		m_pDIDevMouse->Release();
		m_pDIDevMouse = NULL;
	}
}

//=============================================================================
// マウスの更新処理
//=============================================================================
HRESULT UpdateMouse(void)
{
	HRESULT hr;
	DIMOUSESTATE2 mouseStateOld;

	GetCursorPos(&m_MousePoint);
	ScreenToClient(m_hwnd, &m_MousePoint);

	// 前回のデータを保存
	mouseStateOld = m_mouseState;

	// デバイスからデータを取得
	hr = m_pDIDevMouse->GetDeviceState(sizeof(m_mouseState), &m_mouseState);
	if(SUCCEEDED(hr))
	{
		// トリガーの取得
		//g_mouseStateTrigger.lX = 
		//g_mouseStateTrigger.lY = 
		//g_mouseStateTrigger.lZ = 
		for(int cnt = 0; cnt < 8; cnt++)
		{
			m_mouseStateTrigger.rgbButtons[cnt] = ((mouseStateOld.rgbButtons[cnt] ^ m_mouseState.rgbButtons[cnt]) & m_mouseState.rgbButtons[cnt]);
		}
	}
	else
	{
		m_pDIDevMouse->Acquire();
	}

	return S_OK;
}

//=============================================================================
// マウスデータ取得(左プレス)
//=============================================================================
bool GetMouseLeftPress(void)
{
	return ((m_mouseState.rgbButtons[0] & 0x80) != 0);
}

//=============================================================================
// マウスデータ取得(左トリガー)
//=============================================================================
bool GetMouseLeftTrigger(void)
{
	return ((m_mouseStateTrigger.rgbButtons[0] & 0x80) != 0);
}

//=============================================================================
// マウスデータ取得(右プレス)
//=============================================================================
bool GetMouseRightPress(void)
{
	return ((m_mouseState.rgbButtons[1] & 0x8) != 00);
}

//=============================================================================
// マウスデータ取得(右トリガー)
//=============================================================================
bool GetMouseRightTrigger(void)
{
	return ((m_mouseStateTrigger.rgbButtons[1] & 0x80) != 0);
}

//=============================================================================
// マウスデータ取得(中央プレス)
//=============================================================================
bool GetMouseCenterPress(void)
{
	return ((m_mouseState.rgbButtons[2] & 0x80) != 0);
}

//=============================================================================
// マウスデータ取得(中央トリガー)
//=============================================================================
bool GetMouseCenterTrigger(void)
{
	return ((m_mouseState.rgbButtons[2] & 0x80) != 0);
}

//=============================================================================
// マウスデータ取得(Ｘ軸移動)
//=============================================================================
long GetMouseAxisX(void)
{
	return m_mouseState.lX;
}

//=============================================================================
// マウスデータ取得(Ｙ軸移動)
//=============================================================================
long GetMouseAxisY(void)
{
	return m_mouseState.lY;
}

//=============================================================================
// マウスデータ取得(Ｚ軸移動)
//=============================================================================
long GetMouseAxisZ(void)
{
	return m_mouseState.lZ;
}

//=============================================================================
// マウス座標取得(X)
//=============================================================================
long GetMouseX(void)
{
	return m_MousePoint.x;
}

//=============================================================================
// マウス座標取得(Y)
//=============================================================================
long GetMouseY(void)
{
	return m_MousePoint.y;
}

