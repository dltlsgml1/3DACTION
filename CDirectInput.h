#pragma once
#define DIRECTINPUT_VERSION 0x0800
#define NUM_KEY_MAX 256
#include <dinput.h>
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")

class CDirectInput
{
protected:
	LPDIRECTINPUT8			m_pDInput = NULL;					// IDirectInput8�C���^�[�t�F�[�X�ւ̃|�C���^
	LPDIRECTINPUTDEVICE8	m_pDIDevKeyboard = NULL;			// IDirectInputDevice8�C���^�[�t�F�[�X�ւ̃|�C���^(�L�[�{�[�h)
	LPDIRECTINPUTDEVICE8	m_pDIDevMouse = NULL;				// IDirectInputDevice8�C���^�[�t�F�[�X�ւ̃|�C���^(�}�E�X)
	BYTE					m_keyState[NUM_KEY_MAX];			// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
	BYTE					m_keyStateTrigger[NUM_KEY_MAX];		// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
	BYTE					m_keyStateRepeat[NUM_KEY_MAX];		// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
	BYTE					m_keyStateRelease[NUM_KEY_MAX];		// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
	int						m_keyStateRepeatCnt[NUM_KEY_MAX];	// �L�[�{�[�h�̃��s�[�g�J�E���^
	DIMOUSESTATE2			m_mouseState;						// �}�E�X�̏�Ԃ��󂯎�郏�[�N
	DIMOUSESTATE2			m_mouseStateTrigger;				// �}�E�X�̏�Ԃ��󂯎�郏�[�N
	POINT					m_MousePoint;						// �}�E�X�̌��ݍ��W
	HWND					m_hwnd;

public:
	HRESULT InitInput(HINSTANCE hInstance, HWND hWnd);
	HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd);
	HRESULT InitMouse(HINSTANCE hInst, HWND hWnd);

	HRESULT UpdateKeyboard(void);
	HRESULT UpdateMouse(void);
	void UpdateInput(void);

	void UninitKeyboard(void);
	void UninitMouse(void);
	void UninitInput(void);
	

	bool GetKeyboardPress(int key);
	bool GetKeyboardTrigger(int key);
	bool GetKeyboardRepeat(int key);
	bool GetKeyboardRelease(int key);

	bool GetMouseLeftPress(void);
	bool GetMouseLeftTrigger(void);
	bool GetMouseRightPress(void);
	bool GetMouseRightTrigger(void);
	bool GetMouseCenterPress(void);
	bool GetMouseCenterTrigger(void);
	long GetMouseAxisX(void);
	long GetMouseAxisY(void);
	long GetMouseAxisZ(void);
	long GetMouseX(void);
	long GetMouseY(void);
};