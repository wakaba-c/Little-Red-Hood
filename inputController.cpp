//=============================================================================
//
// �R���g���[���[���� [inputController.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "inputController.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
LPDIRECTINPUTDEVICE8 CInputController::m_apDIDevJoypad[NUM_JOYPAD_MAX] = {};		// IDirectInputDevice8�ւ̃|�C���^�i�W���C�p�b�h�j
int CInputController::m_nCntPad = 0;												// �W���C�p�b�h�̌�

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CInputController::CInputController()
{
	// �l�̏�����
	for (int nCount = 0; nCount < NUM_JOYPAD_MAX; nCount++)
	{
		m_bUse[nCount] = false;		// �g�p�s�\�ɂ���
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CInputController::~CInputController()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CInputController::Init(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;

	// directinput�̏���������
	CInput::Init(hInstance, hWnd);

	hr = m_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoyCallbackJoypad, NULL, DIEDFL_ATTACHEDONLY);

	for (int nCount = 0; nCount < NUM_KEY_MAX; nCount++)
	{
		if (FAILED(hr) || m_apDIDevJoypad[nCount] == NULL)
		{
			return hr;
		}

		// �l�̏�����
		m_aJoypadState[nCount].lX = (LONG)0.0;
		m_aJoypadState[nCount].lY = (LONG)0.0;
		m_aJoypadState[nCount].lZ = (LONG)0.0;
		m_aJoypadState[nCount].lRz = (LONG)0.0;

		// �f�[�^�t�H�[�}�b�g��ݒ�
		hr = m_apDIDevJoypad[nCount]->SetDataFormat(&c_dfDIJoystick2);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�W���C�p�b�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x��", MB_ICONWARNING);
			return hr;
		}

		// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
		hr = m_apDIDevJoypad[nCount]->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�W���C�p�b�h�̋������[�h��ݒ�ł��܂���ł����B", "�x��", MB_ICONWARNING);
			return hr;
		}

		hr = m_apDIDevJoypad[nCount]->EnumObjects(EnumAxesCallbackJoypad, NULL, DIDFT_AXIS);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�W���C�p�b�h��������܂���ł����B", "�x��", MB_ICONWARNING);
			return hr;
		}

		// �f�o�C�X�̐ݒ�
		DIPROPRANGE diprg;

		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);

		diprg.diph.dwObj = DIJOFS_X;
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = -(LONG)JOY_MAX_RANGE;
		diprg.lMax = (LONG)JOY_MAX_RANGE;

		// X�����͈̔͂��w��
		m_apDIDevJoypad[nCount]->SetProperty(DIPROP_RANGE, &diprg.diph);

		diprg.diph.dwObj = DIJOFS_Y;
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = -(LONG)JOY_MAX_RANGE;
		diprg.lMax = (LONG)JOY_MAX_RANGE;

		// Y�����͈̔͂��w��
		m_apDIDevJoypad[nCount]->SetProperty(DIPROP_RANGE, &diprg.diph);

		diprg.diph.dwObj = DIJOFS_Z;
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = -(LONG)JOY_MAX_RANGE;
		diprg.lMax = (LONG)JOY_MAX_RANGE;

		// Z�����͈̔͂��w��
		m_apDIDevJoypad[nCount]->SetProperty(DIPROP_RANGE, &diprg.diph);

		diprg.diph.dwObj = DIJOFS_RZ;
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = -(LONG)JOY_MAX_RANGE;
		diprg.lMax = (LONG)JOY_MAX_RANGE;

		// RZ�����͈̔͂��w��
		m_apDIDevJoypad[nCount]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// �R���g���[���[�ւ̃A�N�Z�X�����l��(���͐���J�n)
		m_apDIDevJoypad[nCount]->Acquire();
		m_bUse[nCount] = true;														// �g�p�\�ɐݒ�

		// ���[�N�N���A
		memset(&m_aJoypadState[nCount], 0, sizeof m_aJoyKeyState[nCount]);
		memset(m_aKeyStateJoypad[nCount], 0, sizeof m_aKeyStateJoypad[nCount]);
		memset(m_aKeyStateTriggerJoypad[nCount], 0, sizeof m_aKeyStateTriggerJoypad[nCount]);
		memset(m_aKeyStateReleaseJoypad[nCount], 0, sizeof m_aKeyStateReleaseJoypad[nCount]);
	}

	return hr;
}

//=============================================================================
// �J������
//=============================================================================
void CInputController::Uninit(void)
{
	// �W���C�p�b�h�̏I������
	for (int nCntPad = 0; nCntPad < NUM_JOYPAD_MAX; nCntPad++)
	{
		// ���̓f�o�C�X(�R���g���[���[)�̊J��
		if (m_apDIDevJoypad[m_nCntPad] != NULL)
		{
			m_apDIDevJoypad[m_nCntPad]->Unacquire();			//�R���g���[���[�ւ̃A�N�Z�X�����J��
			m_apDIDevJoypad[m_nCntPad]->Release();
			m_apDIDevJoypad[m_nCntPad] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CInputController::Update(void)
{
	HRESULT hr;
	bool aKeyStateOld[JOYPADKEY_MAX];

	for (int nCntPad = 0; nCntPad < NUM_JOYPAD_MAX; nCntPad++)
	{
		if (m_apDIDevJoypad[nCntPad] != NULL)
		{
			// �O��̃f�[�^��ۑ�
			for (int nCntKey = 0; nCntKey < JOYPADKEY_MAX; nCntKey++)
			{
				aKeyStateOld[nCntKey] = m_aKeyStateJoypad[nCntPad][nCntKey];
			}

			// �f�o�C�X����f�[�^���擾
			hr = m_apDIDevJoypad[nCntPad]->GetDeviceState(sizeof(m_aJoypadState[nCntPad]), &m_aJoypadState[nCntPad]);
			if (SUCCEEDED(hr))
			{
				// �L�[���ݒ�
				SetKeyStateJoypad(nCntPad);

				// ���ʒu���L�^
				m_aKeyStateAxis[nCntPad].x = (float)m_aJoypadState[nCntPad].lX / JOY_MAX_RANGE;
				m_aKeyStateAxis[nCntPad].y = (float)m_aJoypadState[nCntPad].lY / JOY_MAX_RANGE;
				m_aKeyStateAxis[nCntPad].z = (float)m_aJoypadState[nCntPad].lZ / JOY_MAX_RANGE;

				for (int nCntKey = 0; nCntKey < JOYPADKEY_MAX; nCntKey++)
				{
					// �g���K�[�E�����[�X���̍쐬
					m_aKeyStateReleaseJoypad[nCntPad][nCntKey] = aKeyStateOld[nCntKey] ^ (aKeyStateOld[nCntKey] & m_aKeyStateJoypad[nCntPad][nCntKey]);
					m_aKeyStateTriggerJoypad[nCntPad][nCntKey] = (aKeyStateOld[nCntKey] & m_aKeyStateJoypad[nCntPad][nCntKey]) ^ m_aKeyStateJoypad[nCntPad][nCntKey];
				}
			}
			else
			{
				m_apDIDevJoypad[nCntPad]->Acquire();
			}
		}
	}
#ifdef _DEBUG
	CDebugProc::Log("�Q�[���p�b�h�̑��� : %d\n", m_nCntPad);
#endif
}

//=============================================================================
// �W���C�X�e�B�b�N�₢���킹�p�R�[���o�b�N�֐�
//=============================================================================
BOOL CALLBACK CInputController::EnumJoyCallbackJoypad(const DIDEVICEINSTANCE* lpddi, VOID* pvRef)
{
	static GUID pad_discrimination[NUM_JOYPAD_MAX];	// �e�f�o�C�X�̎��ʎq���i�[
	DIDEVCAPS	diDevCaps;				// �f�o�C�X���
	HRESULT		hRes;

	// �W���C�X�e�B�b�N�p�f�o�C�X�I�u�W�F�N�g���쐬
	hRes = m_pInput->CreateDevice(lpddi->guidInstance, &m_apDIDevJoypad[m_nCntPad], NULL);

	if (FAILED(hRes))
	{// �f�o�C�X�I�u�W�F�N�g �̐������ł��Ȃ������Ƃ�
		MessageBox(NULL, "�f�o�C�X�I�u�W�F�N�g�������ł��܂���ł���", "�x��", MB_ICONWARNING);
		return DIENUM_CONTINUE;			// �񋓂𑱂���
	}

	// �W���C�X�e�B�b�N�̔\�͂𒲂ׂ�
	diDevCaps.dwSize = sizeof(DIDEVCAPS);
	hRes = m_apDIDevJoypad[m_nCntPad]->GetCapabilities(&diDevCaps);
	if (FAILED(hRes))
	{
		if (m_apDIDevJoypad[m_nCntPad])
		{
			m_apDIDevJoypad[m_nCntPad]->Release();
			m_apDIDevJoypad[m_nCntPad] = NULL;
			return DIENUM_CONTINUE;			// �񋓂𑱂���
		}
	}

	// �f�o�C�X�̎��ʎq��ۑ�
	pad_discrimination[m_nCntPad] = lpddi->guidInstance;

	// ���̃f�o�C�X���g���̂ŗ񋓂��I������
	m_nCntPad++;																	// �Q�[���p�b�h �̑�����1�������

	return DIENUM_STOP;			// �񋓂𑱂���
}

//=============================================================================
// �W���C�X�e�B�b�N�₢���킹�p�R�[���o�b�N�֐�
//=============================================================================
BOOL CALLBACK CInputController::EnumAxesCallbackJoypad(const LPCDIDEVICEOBJECTINSTANCE lpddi, LPVOID pvRef)
{
	HRESULT hr;

	for (int nCntPad = 0; nCntPad < NUM_JOYPAD_MAX; nCntPad++)
	{
		DIPROPRANGE diprg;

		// �X�e�B�b�N���̒l�͈̔͂�ݒ�i-32768�`32767�j
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwObj = lpddi->dwType;
		diprg.diph.dwHow = DIPH_BYID;
		diprg.lMin = -32768;
		diprg.lMax = 32767;

		if (m_apDIDevJoypad[nCntPad] != NULL)
		{
			hr = m_apDIDevJoypad[nCntPad]->SetProperty(DIPROP_RANGE, &diprg.diph);
		}
	}

	return DIENUM_CONTINUE;

}

//=============================================================================
// �W���C�p�b�h�̃L�[���ݒ�
//=============================================================================
void CInputController::SetKeyStateJoypad(int nIDPad)
{
	if (m_aJoypadState[nIDPad].rgdwPOV[0] >= 225 * 100 && m_aJoypadState[nIDPad].rgdwPOV[0] <= 315 * 100)
	{//�\���L�[[��]��������Ă���
		m_aKeyStateJoypad[nIDPad][JOYPADKEY_LEFT] = true;
	}
	else
	{
		m_aKeyStateJoypad[nIDPad][JOYPADKEY_LEFT] = false;
	}

	if (m_aJoypadState[nIDPad].rgdwPOV[0] >= 45 * 100 && m_aJoypadState[nIDPad].rgdwPOV[0] <= 135 * 100)
	{//�\���L�[[�E]��������Ă���
		m_aKeyStateJoypad[nIDPad][JOYPADKEY_RIGHT] = true;
	}
	else
	{
		m_aKeyStateJoypad[nIDPad][JOYPADKEY_RIGHT] = false;
	}

	if ((m_aJoypadState[nIDPad].rgdwPOV[0] >= 315 * 100 && m_aJoypadState[nIDPad].rgdwPOV[0] <= 360 * 100) || (m_aJoypadState[nIDPad].rgdwPOV[0] >= 0 * 100 && m_aJoypadState[nIDPad].rgdwPOV[0] <= 45 * 100))
	{//�\���L�[[��]��������Ă���
		m_aKeyStateJoypad[nIDPad][JOYPADKEY_UP] = true;
	}
	else
	{
		m_aKeyStateJoypad[nIDPad][JOYPADKEY_UP] = false;
	}

	if (m_aJoypadState[nIDPad].rgdwPOV[0] >= 135 * 100 && m_aJoypadState[nIDPad].rgdwPOV[0] <= 225 * 100)
	{//�\���L�[[��]��������Ă���
		m_aKeyStateJoypad[nIDPad][JOYPADKEY_DOWN] = true;
	}
	else
	{
		m_aKeyStateJoypad[nIDPad][JOYPADKEY_DOWN] = false;
	}

	for (int nKey = JOYPADKEY_X; nKey <= JOYPADKEY_START; nKey++)
	{
		if (m_aJoypadState[nIDPad].rgbButtons[nKey])
		{//�{�^����������Ă���
			m_aKeyStateJoypad[nIDPad][nKey] = true;
		}
		else
		{
			m_aKeyStateJoypad[nIDPad][nKey] = false;
		}
	}
}

//=============================================================================
// �W���C�p�b�h�̎g�p�����m�F
//=============================================================================
bool CInputController::GetJoypadUse(int nIDPad)
{
	return m_bUse[nIDPad];											// �I�����ꂽ�R���g���[���[�̏�Ԃ�Ԃ�
}

//=============================================================================
// �v���X��Ԃ̎擾
//=============================================================================
bool CInputController::GetControllerPress(int nIDPad, JOYPADKEY nKey)
{
	return m_aKeyStateJoypad[nIDPad][nKey];
}

//=============================================================================
// �g���K�[��Ԃ̎擾
//=============================================================================
bool CInputController::GetControllerTrigger(int nIDPad, JOYPADKEY nKey)
{
	return m_aKeyStateTriggerJoypad[nIDPad][nKey];
}

//=============================================================================
// �����[�X��Ԃ̎擾
//=============================================================================
bool CInputController::GetControllerRelease(int nIDPad, JOYPADKEY nKey)
{
	return m_aKeyStateReleaseJoypad[nIDPad][nKey];
}

//=============================================================================
// �W���C�p�b�h�̃g���K�[�i���j�̒l�擾
//=============================================================================
int CInputController::GetJoypadTriggerLeft(int nIDPad)
{
	return m_aJoypadState[nIDPad].rgbButtons[JOYPADKEY_LEFT_TRIGGER];
}

//=============================================================================
// �W���C�p�b�h�̃g���K�[�i�E�j�̒l�擾
//=============================================================================
int CInputController::GetJoypadTriggerRight(int nIDPad)
{
	return m_aJoypadState[nIDPad].rgbButtons[JOYPADKEY_RIGHT_TRIGGER];
}

//=============================================================================
// �W���C�p�b�h�̃A�i���O�X�e�B�b�N�i���j�̒l�擾
//=============================================================================
void CInputController::GetJoypadStickLeft(int nIDPad, float *pValueH, float *pValueV)
{
	*pValueV = -m_aJoypadState[nIDPad].lY / JOY_MAX_RANGE;
	*pValueH = -m_aJoypadState[nIDPad].lX / JOY_MAX_RANGE;
}

//=============================================================================
// �W���C�p�b�h�̃A�i���O�X�e�B�b�N�i�E�j�̒l�擾
//=============================================================================
void CInputController::GetJoypadStickRight(int nIDPad, float *pValueH, float *pValueV)
{
	*pValueH = -m_aJoypadState[nIDPad].lZ / JOY_MAX_RANGE;
	*pValueV = -m_aJoypadState[nIDPad].lRz / JOY_MAX_RANGE;
}