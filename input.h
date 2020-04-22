//=============================================================================
//
// ���͏��� [input.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �N���X��`
//=============================================================================
class CInput
{
public:
	CInput();														// �R���X�g���N�^
	virtual ~CInput();												// �f�X�g���N�^
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd) = 0;		// ����������
	virtual void Uninit(void) = 0;									// �J������
	virtual void Update(void) = 0;									// �X�V����

protected:
	static LPDIRECTINPUT8 m_pInput;									// �C���v�b�g�I�u�W�F�N�g�̃|�C���^
	LPDIRECTINPUTDEVICE8 m_pDevice;									// �f�o�C�X�̃|�C���^
};
#endif