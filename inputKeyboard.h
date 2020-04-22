//=============================================================================
//
// ���͏��� [inputKeyboard.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _INPUTKEYBOARD_H_
#define _INPUTKEYBOARD_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "input.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define	NUM_KEY_MAX			(256)	// �L�[�̍ő吔

//=============================================================================
// �N���X��`
//=============================================================================
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard();								// �R���X�g���N�^
	~CInputKeyboard();								// �f�X�g���N�^
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// ����������
	void Uninit(void);								// �J������
	void Update(void);								// �X�V����

	bool GetTriggerKeyboard(int nKey);				// �g���K�[��Ԃ��擾
	bool GetReleaseKeyboard(int nKey);				// ���s�[�g��Ԃ��擾
	bool GetPressKeyboard(int nKey);				// �v���X��Ԃ��擾

private:
	BYTE m_aKeyState[NUM_KEY_MAX] = {};				// �v���X���
	BYTE m_akeyStateTrigger[NUM_KEY_MAX] = {};		// �g���K�[���
	BYTE m_akeyStateUp[NUM_KEY_MAX] = {};			// �����[�X���
};
#endif