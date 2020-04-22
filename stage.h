//=============================================================================
//
// �X�e�[�W�Ǘ����� [stage.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _STAGE_H_
#define _STAGE_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_ENEMY 4										// �G�̍ő吔

//=============================================================================
// �O���錾
//=============================================================================
class CEnemy;

//=============================================================================
// �N���X��`
//=============================================================================
class CStage
{
public:
	typedef enum
	{
		FADE_NONE = 0,									// �����Ȃ����
		FADE_IN,										// �t�F�[�h�C������
		FADE_TELOP,										// �e���b�v����
		FADE_OUT,										// �t�F�[�h�A�E�g����
		FADE_MAX
	} FADE;

	CStage();											// �R���X�g���N�^
	~CStage();											// �f�X�g���N�^
	HRESULT Init(void);									// ����������
	void Uninit(void);									// �J������
	void Update(void);									// �X�V����
	void Draw(void);									// �`�揈��

	static CStage *Create();							// �N���G�C�g����
	static void LoadPhase(char *add);					// �t�F�[�Y���Ƃ̓G�ǂݍ���

	static void SetStage(void);							// �X�e�[�W�J�ڗp�t�F�[�h�J�n����
	static void SetPhase(void);							// �t�F�[�Y�J�n����

private:
	void ChengeStage(void);
	void MakeVertexFade(LPDIRECT3DDEVICE9 pDevice);

	static LPDIRECT3DTEXTURE9		m_pTexture;			// �e�N�X�`���ւ̃|�C���^
	static LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;			// ���_�o�b�t�@�ւ̃|�C���^
	static FADE						m_fade;				// �t�F�[�h���
	static bool						m_bEvent;			// �C�x���g�̗L��
	static D3DXVECTOR3				m_playerPos;		// �v���C���[�̗����Ă����ꏊ
	static D3DXCOLOR				m_col;				// �t�F�[�h�F

	static CEnemy					*m_apEnemy[MAX_ENEMY];			// �G�̃|�C���^
};
#endif