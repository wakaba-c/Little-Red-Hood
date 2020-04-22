//=============================================================================
//
// �}�E�X���͏��� [inputMouse.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _INPUTMOUSE_H_
#define _INPUTMOUSE_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "input.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define	NUM_BUTTON_MAX			8		// �{�^��(�}�E�X)�̍ő吔
#define MOUSE_LEFT				0		//���{�^��
#define MOUSE_RIGHT				1		//�E�{�^��
#define MOUSE_CENTER			2		//���{�^��

//=============================================================================
// �N���X��`
//=============================================================================
class CInputMouse : public CInput
{
public:
	CInputMouse();									// �R���X�g���N�^
	~CInputMouse();									// �f�X�g���N�^
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// ����������
	void Uninit(void);								// �J������
	void Update(void);								// �X�V����

	bool GetPressMouse(int nButton);				// �v���X��Ԃ̎擾
	bool GetTriggerMouse(int nButton);				// �g���K�[��Ԃ̎擾
	bool GetReleaseMouse(int nButton);				// �����[�X��Ԃ̎擾
	LONG GetMouseX(void);							// �}�E�X��X���W�̎擾
	LONG GetMouseY(void);							// �}�E�X��Y���W�̎擾

private:
	DIMOUSESTATE2 m_mouseState;						// �}�E�X�̃X�e�[�^�X
	float m_posX;									//X���W
	float m_posY;									//Y���W

	LPDIRECTINPUTDEVICE8	m_pDevMouse = NULL;						// ���̓f�o�C�X(�}�E�X)�ւ̃|�C���^
	BYTE					m_aButtonState[NUM_BUTTON_MAX];			// �}�E�X�̓��͏�񃏁[�N
	BYTE					m_aButtonTrigger[NUM_BUTTON_MAX];		// �}�E�X�̃g���K�[
	BYTE					m_aButtonRelease[NUM_BUTTON_MAX];		// �}�E�X�̃����[�X
	HWND					m_hWnd;									// �E�B���h�E�n���h��
};
#endif