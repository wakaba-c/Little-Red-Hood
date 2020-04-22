//=============================================================================
//
// �}�E�X���͏��� [inputMouse.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "inputMouse.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CInputMouse::CInputMouse()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CInputMouse::~CInputMouse()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	// directinput�̏���������
	CInput::Init(hInstance, hWnd);

	// DirectInput�I�u�W�F�N�g�̍쐬
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
	{
		return E_FAIL;
	}

	m_pInput->CreateDevice(GUID_SysMouse, &m_pDevMouse, NULL);
	m_pDevMouse->SetDataFormat(&c_dfDIMouse2); // �}�E�X�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�

	m_pDevMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	// �f�o�C�X�̐ݒ�
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL; // ���Βl���[�h�Őݒ�i��Βl��DIPROPAXISMODE_ABS�j
	m_pDevMouse->SetProperty(DIPROP_AXISMODE, &diprop.diph);

	// ���͐���J�n
	m_pDevMouse->Acquire();

	// ���
	m_hWnd = hWnd;

	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CInputMouse::Uninit(void)
{
	// ���̓f�o�C�X(�}�E�X)�̊J��
	if (m_pDevMouse != NULL)
	{
		m_pDevMouse->Unacquire();	// �}�E�X�ւ̃A�N�Z�X�����J��
		m_pDevMouse->Release();
		m_pDevMouse = NULL;
	}
	// DirectInput�I�u�W�F�N�g�̊J��
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CInputMouse::Update(void)
{
	// �ϐ��錾
	int nCntButton;
	POINT point;

	// �l�̏�����
	m_mouseState.rgbButtons[0] = 0;

	GetCursorPos(&point);
	ScreenToClient(m_hWnd, &point);
	m_posX = (float)point.x;
	m_posY = (float)point.y;

	// �l�̍X�V
	if (SUCCEEDED(m_pDevMouse->GetDeviceState(sizeof(DIMOUSESTATE2), &m_mouseState)))
	{
		for (nCntButton = 0; nCntButton < NUM_BUTTON_MAX; nCntButton++)
		{
			// �r���I�_���a���ǂ������f
			m_aButtonRelease[nCntButton] = m_aButtonState[nCntButton] ^ (m_mouseState.rgbButtons[nCntButton] & m_mouseState.rgbButtons[nCntButton]);
			m_aButtonTrigger[nCntButton] = (m_aButtonState[nCntButton] & m_mouseState.rgbButtons[nCntButton]) ^ m_mouseState.rgbButtons[nCntButton];
			m_aButtonState[nCntButton] = m_mouseState.rgbButtons[nCntButton];	// �L�[�v���X���ۑ�
		}
	}
	else
	{
		m_pDevMouse->Acquire(); // �P���ڂ�Q���ڂɃG���[���o�邪�������Ă悢�B
	}
}

//=============================================================================
// �v���X��Ԃ��擾
//=============================================================================
bool CInputMouse::GetPressMouse(int nButton)
{
	return (m_mouseState.rgbButtons[nButton] & 0x80) ? true : false;
}

//=============================================================================
// �g���K�[��Ԃ̎擾
//=============================================================================
bool CInputMouse::GetTriggerMouse(int nButton)
{
	return (m_aButtonTrigger[nButton] & 0x80) ? true : false;
}

//=============================================================================
// �����[�X��Ԃ̎擾
//=============================================================================
bool CInputMouse::GetReleaseMouse(int nButton)
{
	return (m_aButtonRelease[nButton] & 0x80) ? true : false;
}

//=============================================================================
// �}�E�X��X���W�̎擾
//=============================================================================
LONG CInputMouse::GetMouseX(void)
{
	return (LONG)m_posX;
}

//=============================================================================
// �}�E�X��Y���W�̎擾
//=============================================================================
LONG CInputMouse::GetMouseY(void)
{
	return ((LONG)m_posY);
}