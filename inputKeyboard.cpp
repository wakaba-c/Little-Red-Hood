//=============================================================================
//
// �L�[�{�[�h���͏��� [inputKeyboard.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "inputKeyboard.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CInputKeyboard::CInputKeyboard()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CInputKeyboard::~CInputKeyboard()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	// directinput�̏���������
	CInput::Init(hInstance, hWnd);

	// ���̓f�o�C�X�i�L�[�{�[�h�j�̍쐬
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	m_pDevice->Acquire();

	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CInputKeyboard::Uninit(void)
{
	// ���̓f�o�C�X(�L�[�{�[�h)�̊J��
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();			//�L�[�{�[�h�ւ̃A�N�Z�X�����J��
		m_pDevice->Release();
		m_pDevice = NULL;
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
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];				//�L�[�{�[�h�̓��͏��
	int nCntKey;

	// �f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), aKeyState)))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			// �g���K�[�E�����[�X���̍쐬
			m_akeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] & aKeyState[nCntKey]) ^ aKeyState[nCntKey];
			m_akeyStateUp[nCntKey] = m_aKeyState[nCntKey] ^ (m_aKeyState[nCntKey] & aKeyState[nCntKey]);
			m_aKeyState[nCntKey] = aKeyState[nCntKey];	//�L�[�v���X���ۑ�
		}
	}
	else
	{
		m_pDevice->Acquire();				//�L�[�{�[�h�ւ̃A�N�Z�X�����擾
	}
}

//=============================================================================
// �L�[�{�[�h�̃g���K�[��Ԃ��擾
//=============================================================================
bool CInputKeyboard::GetTriggerKeyboard(int nKey)
{
	return(m_akeyStateTrigger[nKey] & 0x80) ? true : false;
}

//=============================================================================
// �L�[�{�[�h�̃����[�X��Ԃ��擾
//=============================================================================
bool CInputKeyboard::GetReleaseKeyboard(int nKey)
{
	return(m_akeyStateUp[nKey] & 0x80) ? true : false;
}

//=============================================================================
// �L�[�{�[�h�̃v���X��Ԃ��擾
//=============================================================================
bool CInputKeyboard::GetPressKeyboard(int nKey)
{
	return(m_aKeyState[nKey] & 0x80) ? true : false;
}