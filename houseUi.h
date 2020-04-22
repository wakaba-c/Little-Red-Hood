//=============================================================================
//
// ���΂������Ƃ�UI�Ǘ����� [houseUi.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _HOUSEUI_H_
#define _HOUSEUI_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "scene2D.h"

//=============================================================================
// �O���錾
//=============================================================================
class CGauge2D;

//=============================================================================
// �N���X��`
//=============================================================================
class CHouseUi : public CScene2D
{
public:
	// ���
	typedef enum
	{
		DRAWINGTYPE_HPGAUGE = 0,
		DRAWINGTYPE_DAMAGE,
		DRAWINGTYPE_HP,
		DRAWINGTYPE_MAX
	} DRAWINGTYPE;

	CHouseUi(PRIORITY obj);									// �R���X�g���N�^
	~CHouseUi();											// �f�X�g���N�^
	HRESULT Init(void);										// ����������
	void Uninit(void);										// �J������
	void Update(void);										// �X�V����
	void Draw(void);										// �`�揈��

	static CHouseUi *Create(void);							// �N���G�C�g����
	static HRESULT Load(void);								// ���[�h����

	CGauge2D *GetGauge(void) { return m_pGauge2D; }			// �Q�[�W�̎擾

private:
	CScene2D *m_apScene2D[DRAWINGTYPE_MAX] = {};			// �Ƃ̑ϋv�l�\���p�[�c�ʃ|�C���^
	D3DXVECTOR3 m_move;										// �ړ���
	D3DXVECTOR3 m_size;										// �t���[���ȊO�̑傫��
	D3DXVECTOR3 m_frameSize;								// �t���[���̑傫��
	float m_nDamage;										// �_���[�W��
	float m_fHP;											// HP
	int	m_nPage;											// �y�[�W��
	float m_fAngle;											// ���a
	float m_fLength;										// ����
	CGauge2D *m_pGauge2D;									// �ϋv�l�\���p�Q�[�W
};
#endif