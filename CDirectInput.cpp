//=============================================================================
//
// ���͏��� [input.cpp]
//
//=============================================================================
#include "CDirectInput.h"

//=============================================================================
// ���͏����̏�����
//=============================================================================
HRESULT CDirectInput::InitInput(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	if (!m_pDInput)
	{
		// DirectInput�I�u�W�F�N�g�̍쐬
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
			IID_IDirectInput8, (void**)&m_pDInput, NULL);
	}

	// �L�[�{�[�h�̏�����
	InitKeyboard(hInst, hWnd);

	// �}�E�X�̏�����
	InitMouse(hInst, hWnd);

	m_hwnd = hWnd;

	return hr;
}

//=============================================================================
// ���͏����̏I������
//=============================================================================
void CDirectInput::UninitInput(void)
{
	// �L�[�{�[�h�̏I������
	UninitKeyboard();

	// �}�E�X�̏I������
	UninitMouse();

	if (m_pDInput)
	{
		m_pDInput->Release();
		m_pDInput = NULL;
	}
}

//=============================================================================
// ���͏����̍X�V����
//=============================================================================
void CDirectInput::UpdateInput(void)
{
	// �L�[�{�[�h�̍X�V
	UpdateKeyboard();

	// �}�E�X�̍X�V
	UpdateMouse();
}

//=============================================================================
// �L�[�{�[�h�̏�����
//=============================================================================
HRESULT CDirectInput::InitKeyboard(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// �f�o�C�X�I�u�W�F�N�g���쐬
	hr = m_pDInput->CreateDevice(GUID_SysKeyboard, &m_pDIDevKeyboard, NULL);
	if (FAILED(hr) || m_pDIDevKeyboard == NULL)
	{
		MessageBox(hWnd, "�L�[�{�[�h���˂��I", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = m_pDIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = m_pDIDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	m_pDIDevKeyboard->Acquire();

	return S_OK;
}

//=============================================================================
// �L�[�{�[�h�̏I������
//=============================================================================
void CDirectInput::UninitKeyboard(void)
{
	if (m_pDIDevKeyboard)
	{
		m_pDIDevKeyboard->Unacquire();

		m_pDIDevKeyboard->Release();
		m_pDIDevKeyboard = NULL;
	}
}

//=============================================================================
// �L�[�{�[�h�̍X�V����
//=============================================================================
HRESULT CDirectInput::UpdateKeyboard(void)
{
	HRESULT hr;
	BYTE keyStateOld[256];

	// �O��̃f�[�^��ۑ�
	memcpy(keyStateOld, m_keyState, NUM_KEY_MAX);

	// �f�o�C�X����f�[�^���擾
	hr = m_pDIDevKeyboard->GetDeviceState(sizeof(m_keyState), m_keyState);
	if (SUCCEEDED(hr))
	{
		for (int cnt = 0; cnt < NUM_KEY_MAX; cnt++)
		{
			// �g���K�[�ƃ����[�X���擾
			m_keyStateTrigger[cnt] = (m_keyState[cnt] ^ keyStateOld[cnt])& m_keyState[cnt];
			m_keyStateRelease[cnt] = (m_keyState[cnt] ^ keyStateOld[cnt])& keyStateOld[cnt];

			// �L�[��������Ă���Ȃ烊�s�[�g�̔��菈��
			if (m_keyState[cnt])
			{
				if (m_keyStateRepeatCnt[cnt] < 20)
				{
					m_keyStateRepeatCnt[cnt]++;
					// �u������́v�������́u�{�^���������Ă���20�t���[���o�߁v
					// �����ꍇ�A���s�[�g�p�z��̃f�[�^��L���ɂ���
					//if(  ||  )
					// ��L�����ȊO�̓��s�[�g�p�z��̃f�[�^�𖳌��ɂ���
					//else
					if ((m_keyStateRepeatCnt[cnt] == 1) || m_keyStateRepeatCnt[cnt] >= 20) {
						m_keyStateRepeat[cnt] = m_keyState[cnt];
					}
					else {
						m_keyStateRepeat[cnt] = 0;
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
		// �L�[�{�[�h�ւ̃A�N�Z�X�����擾
		hr = m_pDIDevKeyboard->Acquire();
	}

	return hr;
}

//=============================================================================
// �L�[�{�[�h�̃v���X��Ԃ��擾
//=============================================================================
bool CDirectInput::GetKeyboardPress(int key)
{
	return ((m_keyState[key] & 0x80) != 0);
}

//=============================================================================
// �L�[�{�[�h�̃g���K�[��Ԃ��擾
//=============================================================================
bool CDirectInput::GetKeyboardTrigger(int key)
{
	return ((m_keyStateTrigger[key] & 0x80) != 0);
}

//=============================================================================
// �L�[�{�[�h�̃��s�[�g��Ԃ��擾
//=============================================================================
bool CDirectInput::GetKeyboardRepeat(int key)
{
	return ((m_keyStateRepeat[key] & 0x80) != 0);
}

//=============================================================================
// �L�[�{�[�h�̃����|�X��Ԃ��擾
//=============================================================================
bool CDirectInput::GetKeyboardRelease(int key)
{
	return ((m_keyStateRelease[key] & 0x80) != 0);
}

//=============================================================================
// �}�E�X�̏�����
//=============================================================================
HRESULT CDirectInput::InitMouse(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// �f�o�C�X�I�u�W�F�N�g���쐬
	hr = m_pDInput->CreateDevice(GUID_SysMouse, &m_pDIDevMouse, NULL);
	if (FAILED(hr) || m_pDIDevMouse == NULL)
	{
		MessageBox(hWnd, "�}�E�X���˂��I", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = m_pDIDevMouse->SetDataFormat(&c_dfDIMouse2);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�}�E�X�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = m_pDIDevMouse->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�}�E�X�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �f�o�C�X�̐ݒ�
	{
		DIPROPDWORD dipdw;

		dipdw.diph.dwSize = sizeof(dipdw);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwObj = 0;
		dipdw.diph.dwHow = DIPH_DEVICE;
		dipdw.dwData = DIPROPAXISMODE_REL;

		hr = m_pDIDevMouse->SetProperty(DIPROP_AXISMODE, &dipdw.diph);
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	m_pDIDevMouse->Acquire();

	return S_OK;
}

//=============================================================================
// �}�E�X�̏I������
//=============================================================================
void CDirectInput::UninitMouse(void)
{
	if (m_pDIDevMouse)
	{
		m_pDIDevMouse->Unacquire();

		m_pDIDevMouse->Release();
		m_pDIDevMouse = NULL;
	}
}

//=============================================================================
// �}�E�X�̍X�V����
//=============================================================================
HRESULT CDirectInput::UpdateMouse(void)
{
	HRESULT hr;
	DIMOUSESTATE2 mouseStateOld;

	GetCursorPos(&m_MousePoint);
	ScreenToClient(m_hwnd, &m_MousePoint);

	// �O��̃f�[�^��ۑ�
	mouseStateOld = m_mouseState;

	// �f�o�C�X����f�[�^���擾
	hr = m_pDIDevMouse->GetDeviceState(sizeof(m_mouseState), &m_mouseState);
	if (SUCCEEDED(hr))
	{
		// �g���K�[�̎擾
		//g_mouseStateTrigger.lX = 
		//g_mouseStateTrigger.lY = 
		//g_mouseStateTrigger.lZ = 
		for (int cnt = 0; cnt < 8; cnt++)
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
// �}�E�X�f�[�^�擾(���v���X)
//=============================================================================
bool CDirectInput::GetMouseLeftPress(void)
{
	return ((m_mouseState.rgbButtons[0] & 0x80) != 0);
}

//=============================================================================
// �}�E�X�f�[�^�擾(���g���K�[)
//=============================================================================
bool CDirectInput::GetMouseLeftTrigger(void)
{
	return ((m_mouseStateTrigger.rgbButtons[0] & 0x80) != 0);
}

//=============================================================================
// �}�E�X�f�[�^�擾(�E�v���X)
//=============================================================================
bool CDirectInput::GetMouseRightPress(void)
{
	return ((m_mouseState.rgbButtons[1] & 0x8) != 00);
}

//=============================================================================
// �}�E�X�f�[�^�擾(�E�g���K�[)
//=============================================================================
bool CDirectInput::GetMouseRightTrigger(void)
{
	return ((m_mouseStateTrigger.rgbButtons[1] & 0x80) != 0);
}

//=============================================================================
// �}�E�X�f�[�^�擾(�����v���X)
//=============================================================================
bool CDirectInput::GetMouseCenterPress(void)
{
	return ((m_mouseState.rgbButtons[2] & 0x80) != 0);
}

//=============================================================================
// �}�E�X�f�[�^�擾(�����g���K�[)
//=============================================================================
bool CDirectInput::GetMouseCenterTrigger(void)
{
	return ((m_mouseState.rgbButtons[2] & 0x80) != 0);
}

//=============================================================================
// �}�E�X�f�[�^�擾(�w���ړ�)
//=============================================================================
long CDirectInput::GetMouseAxisX(void)
{
	return m_mouseState.lX;
}

//=============================================================================
// �}�E�X�f�[�^�擾(�x���ړ�)
//=============================================================================
long CDirectInput::GetMouseAxisY(void)
{
	return m_mouseState.lY;
}

//=============================================================================
// �}�E�X�f�[�^�擾(�y���ړ�)
//=============================================================================
long CDirectInput::GetMouseAxisZ(void)
{
	return m_mouseState.lZ;
}

//=============================================================================
// �}�E�X���W�擾(X)
//=============================================================================
long CDirectInput::GetMouseX(void)
{
	return m_MousePoint.x;
}

//=============================================================================
// �}�E�X���W�擾(Y)
//=============================================================================
long CDirectInput::GetMouseY(void)
{
	return m_MousePoint.y;
}

