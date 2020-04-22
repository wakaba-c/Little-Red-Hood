//=============================================================================
//
// �R���g���[���[���͏��� [inputController.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _INPUTCONTROLLER_H_
#define _INPUTCONTROLLER_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "input.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define	NUM_KEY_MAX			(256)						// �L�[�̍ő吔
#define COUNT_WAIT_REPEAT	(20)						//���s�[�g�J�n�܂ł̑҂�����
#define INTERVAL_REPEAT		(1)							//���s�[�g�Ԋu
#define	NUM_JOYPAD_MAX		(4)							//�p�b�h�ő吔
#define JOY_MAX_RANGE		(1024.0f)					//�p�b�h�̗L���͈�

//=============================================================================
// �\���̒�`
//=============================================================================
// �L�[�̎��
typedef enum
{
	JOYPADKEY_X = 0,				//[0]�@X�{�^��
	JOYPADKEY_Y,					//[1]�@Y�{�^��
	JOYPADKEY_A,					//[2]�@A�{�^��
	JOYPADKEY_B,					//[3]�@B�{�^��
	JOYPADKEY_LEFT_SHOULDER,		//[4]�@����O�{�^��
	JOYPADKEY_RIGHT_SHOULDER,		//[5]�@�E��O�{�^��
	JOYPADKEY_LEFT_TRIGGER,			//[6]�@���{�^��
	JOYPADKEY_RIGHT_TRIGGER,		//[7]�@�E�{�^��
	JOYPADKEY_LEFT_THUMB,			//[8]�@�E�X�e�B�b�N�{�^��
	JOYPADKEY_RIGHT_THUMB,			//[9]�@���X�e�B�b�N�{�^��
	JOYPADKEY_BACK,					//[10]�@�o�b�N�{�^��
	JOYPADKEY_START,				//[11]�@�X�^�[�g�{�^��
	JOYPADKEY_LEFT,					//[12]�@�����L�[[��]
	JOYPADKEY_RIGHT,				//[13]�@�����L�[[�E]
	JOYPADKEY_UP,					//[14]�@�����L�[[��]
	JOYPADKEY_DOWN,					//[15]�@�����L�[[��]
	JOYPADKEY_MAX
}JOYPADKEY;

//=============================================================================
// �N���X��`
//=============================================================================
class CInputController : public CInput
{
public:
	CInputController();											// �R���X�g���N�^
	~CInputController();										// �f�X�g���N�^
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);				// ����������
	void Uninit(void);											// �J������
	void Update(void);											// �X�V����

	bool GetControllerPress(int nIDPad, JOYPADKEY nKey);		// �v���X��Ԃ̎擾
	bool GetControllerTrigger(int nIDPad, JOYPADKEY nKey);		// �g���K�[��Ԃ̎擾
	bool GetControllerRelease(int nIDPad, JOYPADKEY nKey);		// �����[�X��Ԃ̎擾

	int GetJoypadTriggerLeft(int nIDPad);						// ���g���K�[��Ԃ̎擾
	int GetJoypadTriggerRight(int nIDPad);						// �E�g���K�[��Ԃ̎擾

	void GetJoypadStickLeft(int nIDPad, float *pValueH, float *pValueV);	// ���X�e�B�b�N�̏�Ԏ擾
	void GetJoypadStickRight(int nIDPad, float *pValueH, float *pValueV);	// �E�X�e�B�b�N�̏�Ԏ擾

	void SetKeyStateJoypad(int nIDPad);							// �W���C�p�b�h�̃L�[���ݒ�
	bool GetJoypadUse(int nIDPad);								// �W���C�p�b�h�̎g�p�����m�F

private:
	static BOOL CALLBACK EnumJoyCallbackJoypad(const DIDEVICEINSTANCE* lpddi, VOID* pvRef);				// �W���C�X�e�B�b�N�₢���킹�p�R�[���o�b�N�֐�
	static BOOL CALLBACK EnumAxesCallbackJoypad(const LPCDIDEVICEOBJECTINSTANCE lpddi, LPVOID pvRef);	// �W���C�X�e�B�b�N�₢���킹�p�R�[���o�b�N�֐�

	BYTE m_aJoyKeyState[NUM_KEY_MAX];										// �L�[���
	static LPDIRECTINPUTDEVICE8 m_apDIDevJoypad[NUM_JOYPAD_MAX];			// IDirectInputDevice8�ւ̃|�C���^�i�W���C�p�b�h�j
	DIJOYSTATE2 m_aJoypadState[NUM_JOYPAD_MAX];								// �Q�[���p�b�h ��Ԃ��󂯎�郏�[�N
	bool m_aKeyStateJoypad[NUM_JOYPAD_MAX][NUM_KEY_MAX];					// �Q�[���p�b�h �̉�����Ԃ�ێ����郏�[�N
	bool m_aKeyStateTriggerJoypad[NUM_JOYPAD_MAX][NUM_KEY_MAX];				// �Q�[���p�b�h �̃g���K�[��Ԃ�ێ����郏�[�N
	bool m_aKeyStateReleaseJoypad[NUM_JOYPAD_MAX][NUM_KEY_MAX];				// �Q�[���p�b�h �̃����[�X��Ԃ�ێ����郏�[�N
	D3DXVECTOR3 m_aKeyStateAxis[NUM_JOYPAD_MAX];							// ���ʒu
	static int m_nCntPad;													// �Q�[���p�b�h �̑���
	bool m_bUse[NUM_JOYPAD_MAX];											// �g�p�\�ȏ�Ԃ��ǂ���
};
#endif