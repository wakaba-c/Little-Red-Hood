//=============================================================================
//
// �Q�[�W���� [gauge2D.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _GAUGE2D_H_
#define _GAUGE2D_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "scene2D.h"

//=============================================================================
// �N���X��`
//=============================================================================
class CGauge2D : public CScene2D
{
public:
	CGauge2D(PRIORITY obj);		// �R���X�g���N�^
	~CGauge2D();				// �f�X�g���N�^
	HRESULT Init(void);			// ����������
	void Uninit(void);			// �J������
	void Update(void);			// �X�V����
	void Draw(void);			// �`�揈��

	static CGauge2D *Create(void);		// �N���G�C�g����
	static HRESULT Load(void);										// ���[�h����

	void SetPosition(D3DXVECTOR3 pos);								// �ʒu�̐ݒ�
	void SetLifeBar(bool bDie, float nValue);						// �Q�[�W�̕\�������ݒ�
	void SetGaugeSize(D3DXVECTOR3 size, D3DXVECTOR3 frameSize);		// �Q�[�W�̑傫���ݒ�

private:
	// �Q�[�W�̎��
	typedef enum
	{
		DRAWINGTYPE_GLAS = 0,
		DRAWINGTYPE_DAMAGE,
		DRAWINGTYPE_BACK,
		DRAWINGTYPE_FRONT,
		DRAWINGTYPE_MAX
	} DRAWINGTYPE;

	CScene2D *m_apScene2D[DRAWINGTYPE_MAX] = {};		// �Q�[�W�p�[�c�̃|�C���^
	D3DXVECTOR3 m_pos;									// �|�W�V����
	D3DXVECTOR3 m_move;									// �ړ���
	D3DXVECTOR3 m_size;									// �t���[���ȊO�̑傫��
	D3DXVECTOR3 m_frameSize;							// �t���[���̑傫��
	float m_nDamage;									// �_���[�W��
	float m_fHP;										// HP

	float m_fAngle;										// ���a
	float m_fLength;									// ����
};
#endif