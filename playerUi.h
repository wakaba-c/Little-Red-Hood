//=============================================================================
//
// �v���C���[��UI�Ǘ����� [playerUi.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _PLAYERUI_H_
#define _PLAYERUI_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "scene2D.h"

//=============================================================================
// �񋓌^��`
//=============================================================================
typedef enum
{
	DRAWINGTYPE_HPGAUGE = 0,
	DRAWINGTYPE_DEATHGAUGE,
	DRAWINGTYPE_DAMAGE,
	DRAWINGTYPE_BACK,
	DRAWINGTYPE_HP,
	DRAWINGTYPE_DEATHBLOW,
	DRAWINGTYPE_FRONT,
	DRAWINGTYPE_CHARACTER,
	DRAWINGTYPE_MAX
} DRAWINGTYPE;

typedef enum
{
	GAUGETYPE_HP = 0,
	GAUGETYPE_DEATHBLOW,
	GAUGETYPE_MAX
} GAUGETYPE;

//=============================================================================
// �O���錾
//=============================================================================
class CGauge2D;

//=============================================================================
// �N���X��`
//=============================================================================
class CPlayerUi : public CScene2D
{
public:
	CPlayerUi(PRIORITY obj);			// �R���X�g���N�^
	~CPlayerUi();						// �f�X�g���N�^
	HRESULT Init(void);					// ����������
	void Uninit(void);					// �J������
	void Update(void);					// �X�V����
	void Draw(void);					// �`�揈��

	static CPlayerUi *Create(void);		// �N���G�C�g����
	static HRESULT Load(void);			// ���[�h����

	CGauge2D *GetGauge(GAUGETYPE type) { return	m_apGauge2D[type]; }		// �Q�[�W�̎擾

private:
	CScene2D *m_apScene2D[DRAWINGTYPE_MAX] = {};// �V�[���̃|�C���^
	D3DXVECTOR3 m_move;							// �ړ���
	D3DXVECTOR3 m_size;							// �t���[���ȊO�̑傫��
	D3DXVECTOR3 m_frameSize;					// �t���[���̑傫��
	float m_nDamage;							// �_���[�W��
	float m_fHP;								// HP
	int		m_nPage;							// �y�[�W��
	float m_fAngle;								// ���a
	float m_fLength;							// ����
	CGauge2D *m_apGauge2D[GAUGETYPE_MAX];		// �Q�[�W�̃|�C���^
};
#endif