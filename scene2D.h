//=============================================================================
//
// scene2D���� [scene2D.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
// �N���X��`
//=============================================================================
class CScene2D : public CScene
{
public:
	// ���S�ʒu
	typedef enum
	{
		TEXTUREVTX_CENTER = 0,
		TEXTUREVTX_LEFT,
		TEXTUREVTX_MAX
	} TEXTUREVTX;

	CScene2D(PRIORITY obj);															// �R���X�g���N�^
	~CScene2D();																	// �f�X�g���N�^
	HRESULT Init(void);																// ����������
	void Uninit(void);																// �J������
	void Update(void);																// �X�V����
	void Draw(void);																// �`�揈��

	static CScene2D *Create(CScene::PRIORITY obj);									// �쐬
	void BindTexture(LPDIRECT3DTEXTURE9 tex);										// �e�N�X�`���̐ݒ�

	void SetTransform(void);														// ���_�����X�V
	void SetTransformTurnOver(void);												// �摜�̔��]
	void SetSize(D3DXVECTOR3 size);													// �傫����ݒ�
	void SetDrawPos(D3DXVECTOR2 tex);												// �e�N�X�`���̕`��ʒu��ݒ�
	void SpriteAnimation(D3DXVECTOR2 patternNo, int nPatternAnim, int nHeight);		// �X�v���C�g�A�j���[�V����
	void SetColor(D3DXCOLOR col);													// �F�̐ݒ�
	void SetCenter(TEXTUREVTX vtx);													// ���S�̐ݒ�
	void SetRotation(D3DXVECTOR3 rot);												// ��]�ʂ̐ݒ�

	bool HitCollision2D(CScene2D *obj, bool bTrigger);								//�����蔻��(�Ԃ���)
	bool InCollision2D(CScene2D *obj);												//�����蔻��(���ɂ���)

	D3DXVECTOR2 GetTex(void) { return m_DrawPos; }									// �e�N�X�`���ʒu�̎擾
	D3DXVECTOR3 GetRotation(void) { return m_rot; }									// ��]�ʂ̎擾
	TEXTUREVTX GetCenter(void) { return m_Vtx; }									// ���S�̎擾
	D3DXCOLOR GetColor(void) { return m_col; }										// �F�̎擾
	D3DXVECTOR3 GetSize(void) { return m_size; }									// �傫�����擾

	void OnTriggerEnter(CCollider *col) {};
	void OnCollisionEnter(CCollider *col) {};

private:
	LPDIRECT3DTEXTURE9			m_pTexture;											// �e�N�X�`���̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;											// ���_�o�b�t�@�ւ̃|�C���^
	TEXTUREVTX					m_Vtx;												// ���S
	D3DXVECTOR3					m_size;												// �傫��
	D3DXVECTOR2					m_DrawPos;											// �e�N�X�`���̕`��ʒu
	D3DXVECTOR3					m_rot;												// ��]��
	D3DXCOLOR					m_col;												// �F
	char						*m_pTexAdd;											// �e�N�X�`���A�h���X
	float m_fAngle;																	// �p�x
	float m_fLength;																// ����
};
#endif