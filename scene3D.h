//=============================================================================
//
// scene3D���� [scene3D.h]
// Author : masayasu wakita
//
//=============================================================================
#include "main.h"
#include "scene.h"

#ifndef _SCENE3D_H_
#define _SCENE3D_H_

//=============================================================================
// �N���X��`
//=============================================================================
class CScene3D : public CScene
{
public:
	// ���S�ʒu
	typedef enum
	{
		TEXTUREVTX_CENTER = 0,
		TEXTUREVTX_LEFT,
		TEXTUREVTX_MAX
	} TEXTUREVTX;

	CScene3D(PRIORITY obj);									// �R���X�g���N�^
	~CScene3D();											// �f�X�g���N�^
	HRESULT Init(void);										// ����������
	void Uninit(void);										// �J������
	void Update(void);										// �X�V����
	void Draw(void);										// �`�揈��

	static CScene3D *Create(CScene::PRIORITY obj);			// �N���G�C�g����
	void BindTexture(LPDIRECT3DTEXTURE9 tex);				// �e�N�X�`���̐ݒ�

	void MakeVertex(void);									// ���_���̍쐬
	void SetCenter(TEXTUREVTX vtx);							// ���S�ʒu�̐ݒ�
	void SetSize(D3DXVECTOR3 size);							// �T�C�Y�̐ݒ�
	void SetColor(D3DXCOLOR col);							// �F�̐ݒ�

	TEXTUREVTX GetCenter(void) { return	 m_Vtx; }			// ���S�̎擾
	D3DXCOLOR GetColor(void) { return m_col; }				// �F�̎擾
	D3DXVECTOR3 GetSize(void) { return m_size; }			// �T�C�Y�̎擾

	void OnTriggerEnter(CCollider *col) {};
	void OnCollisionEnter(CCollider *col) {};

private:
	LPDIRECT3DTEXTURE9			m_pTexture;					//�e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;					//���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 m_size;										//�傫��
	D3DXVECTOR3 m_rot;										//��]��
	D3DXCOLOR	m_col;										//�F
	D3DXMATRIX	m_mtxWorld;									//���[���h�}�g���b�N�X
	TEXTUREVTX	m_Vtx;										//���S
};
#endif