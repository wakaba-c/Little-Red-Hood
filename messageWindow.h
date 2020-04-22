//=============================================================================
//
// �ړI�\������ [messageWindow.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _MESSAGEWINDOW_H_
#define _MESSAGEWINDOW_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "scene2D.h"

//=============================================================================
// �񋓌^��`
//=============================================================================
typedef enum
{
	FADE_NONE = 0,		// �����Ȃ����
	FADE_IN,			// �t�F�[�h�C������
	FADE_TELOP,			// �e���b�v����
	FADE_OUT,			// �t�F�[�h�A�E�g����
	FADE_MAX
} FADE;

//=============================================================================
// �N���X��`
//=============================================================================
class CMessageWindow : public CScene2D
{
public:
	// �e���b�v�̎��
	typedef enum
	{
		MESSAGETYPE_NONE = -1,		// ���b�Z�[�W�Ȃ�
		MESSAGETYPE_START = 0,		// �Ɩh�q�J�n
		MESSAGETYPE_CLEAR,			// �Ɩh�q�N���A
		MESSAGETYPE_MAX
	} MESSAGETYPE;

	CMessageWindow(PRIORITY obj);			// �R���X�g���N�^
	~CMessageWindow();						// �f�X�g���N�^
	HRESULT Init(void);						// ����������
	void Uninit(void);						// �J������
	void Update(void);						// �X�V����
	void Draw(void);						// �`�揈��

	static CMessageWindow *Create(MESSAGETYPE type);		// �N���G�C�g����
	static HRESULT Load(void);								// ���[�h����

private:
	CScene2D *m_pMessage;					// ���b�Z�[�W�̃|�C���^
	D3DXCOLOR	m_col;						// �F
	D3DXVECTOR3 m_size;						// �t���[���ȊO�̑傫��
	D3DXVECTOR3 m_frameSize;				// �t���[���̑傫��
	MESSAGETYPE m_type;						// ���b�Z�[�W�^�C�v
	FADE		m_fade;						// �t�F�[�h�̃^�C�v
	float m_nDamage;						// �_���[�W��
	float m_fHP;							// HP
	int		m_nPage;						// �y�[�W��
	int		m_nCount;						// �J�E���^�[
	float m_fAngle;							// ���a
	float m_fLength;						// ����
};
#endif