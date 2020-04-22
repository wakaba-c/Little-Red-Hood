#ifdef _DEBUG
//=============================================================================
//
// �f�o�b�O���� [debug.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _DEBUG_H_
#define _DEBUG_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "imgui\imgui.h"
#include "imgui\imgui_impl_dx9.h"
#include "imgui\imgui_impl_win32.h"

//=============================================================================
// �\���̒�`
//=============================================================================
typedef enum
{
	DEBUGMODE_NONE = 0,
	DEBUGMODE_RAND,
	DEBUGMODE_MANY,
	DEBUGMODE_INDIVIDUAL,
	DEBUGMODE_DELETE,
	DEBUGMODE_PAINT,
	DEBUGMODE_ENEMY,
	DEBUGMODE_MAX
} DEBUGMODE;

//=============================================================================
// �O���錾
//=============================================================================
class CCircle;
class CObject;
class CEnemy;

//=============================================================================
// �N���X��`
//=============================================================================
class CDebugProc
{
public:
	CDebugProc();						// �R���X�g���N�^
	~CDebugProc();						// �f�X�g���N�^
	HRESULT Init(HWND hWnd);			// ����������
	void Uninit(void);					// �J������
	void Update(void);					// �X�V����
	void Draw(void);					// �`�揈��

	static void Log(char* fmt, ...);	// �f�o�b�O���O�̏���

	static CCircle *GetCircle(void) { return m_pCircle; }	// �~�͈͓̔��ɂ��邩
	static bool GetDebugState(void) { return m_bDebug; }	// �f�o�b�O���g�p����

private:
	static void Debug(void);								// �f�o�b�O����
	static void CreateRand(D3DXVECTOR3 &worldPos);			// �n�`�ҏW����
	static void Paint(D3DXVECTOR3 &worldPos);				// �y�C���g����
	static void CreateEnemy(D3DXVECTOR3 &worldPos);			// �G�l�~�[�z�u����
	static void CreateObject(D3DXVECTOR3 &worldPos);		// �ؔz�u����
	static void CreateIndividual(D3DXVECTOR3 &worldPos);	// �P�̔z�u����
	static void	DeleteObject(D3DXVECTOR3 &worldPos);		// �I�u�W�F�N�g�͈͓��폜����

	LPD3DXFONT	m_pFont;									// �t�H���g�ւ̃|�C���^
	static char m_aStr[1024];								// ������
	static CObject *m_pObject;								// ���{�p�I�u�W�F�N�g�|�C���^
	static CEnemy *m_pEnemy;								// ���{�p�G�|�C���^
	static float m_fPaintSize;								// �n�`�ҏW�u���V�̑傫��
	static int m_nCreateIndex;								// 1�t���[���̐�����
	static bool m_bDebug;									// �f�o�b�O���[�h�̐؂�ւ�
	static int m_nCntGeneration;							// ������
	static int m_nMode;										// ���[�h�I��
	static int m_nType;										// ���f���^�C�v
	static D3DXVECTOR3 m_mouseOld;							// �}�E�X�̑O�̃��[���h���W
	static int m_nCntContinue;								// �ĉ��Z��
	static CCircle *m_pCircle;								// �~�̃|�C���^
};
#endif
#endif