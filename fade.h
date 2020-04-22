//=============================================================================
//
// �t�F�[�h���� [fade.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "manager.h"

//=============================================================================
// �N���X��`
//=============================================================================
class CFade
{
public:
	typedef enum
	{
		FADE_NONE = 0,		// �����Ȃ����
		FADE_IN,			// �t�F�[�h�C������
		FADE_TELOP,			// �e���b�v����
		FADE_OUT,			// �t�F�[�h�A�E�g����
		FADE_MAX
	} FADE;

	CFade();											// �R���X�g���N�^
	~CFade();											// �f�X�g���N�^
	void Init(CManager::MODE modeNext);					// ����������
	void Uninit(void);									// �J������
	void Update(void);									// �X�V����
	void Draw(void);									// �`�揈��

	static CFade *Create();								// �N���G�C�g����
	static void SetFade(CManager::MODE modeNext);		// �Z�b�g�t�F�[�h����

	static FADE GetFade(void) { return m_fade; }		// �t�F�[�h��Ԃ̎擾

private:
	void MakeVertex(LPDIRECT3DDEVICE9 pDevice);			// ���_���̍쐬

	static LPDIRECT3DVERTEXBUFFER9	m_pVtxBuffFade;		// ���_�o�b�t�@�ւ̃|�C���^
	static FADE						m_fade;				// �t�F�[�h���
	static CManager::MODE			m_modeNext;			// ���̉�ʁi���[�h�j
	static D3DXCOLOR				m_colorFade;		// �t�F�[�h�F
};
#endif
