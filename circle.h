//=============================================================================
//
// �~���� [circle.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _CIRCLE_H_
#define _CIRCLE_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define DIVISION_CIRCLE 30					//������
#define DEPTH 	20							//��

//=============================================================================
// �N���X��`
//=============================================================================
class CCircle
{
public:
	CCircle();						// �R���X�g���N�^
	~CCircle();						// �f�X�g���N�^����
	HRESULT Init(void);				// ����������
	void Uninit(void);				// �J������
	void Update(void);				// �X�V����
	void Draw(void);				// �`�揈��

	static CCircle *Create(void);	// �N���G�C�g����

	static HRESULT Load(void);
	static void Unload(void);

	void MakeVertex(void);									// ���_���̍쐬
	void SetPosition(D3DXVECTOR3 pos);						// �ʒu��ݒ肷��
	void SetRotation(D3DXVECTOR3 rot);						// ��]��ݒ肷��
	void SetColor(D3DXCOLOR col);							// �F��ݒ肷��
	void SetRadius(float fValue);							// ���a��ݒ肷��
	void SetSize(D3DXVECTOR3 size);							// �傫����ݒ肷��

	D3DXVECTOR3 GetSize(void) { return m_size; }			// �傫����Ԃ�
	D3DXVECTOR3 GetPosition(void) { return m_pos; }			// �ʒu��Ԃ�
	D3DXVECTOR3 GetRotation(void) { return m_rot; }			// ��]��Ԃ�
	D3DXCOLOR GetColor(void) { return m_col; }				// �F��Ԃ�
	float GetRadius(void) { return m_fRadius; }				// ���a��Ԃ�

private:
#ifdef _DEBUG
	void Debug(void);
#endif

	static LPDIRECT3DTEXTURE9	m_pTexture;					// �e�N�X�`���ւ̃|�C���^

	D3DXVECTOR3				m_pos;							// �ʒu
	D3DXVECTOR3				m_rot;							// ��]
	D3DXVECTOR3				m_size;							// �T�C�Y
	float					m_fRadius;						// ���a
	D3DXCOLOR				m_col;							// �F
	D3DXMATRIX				m_mtxWorld;						// ���[���h�}�g���b�N�X

	int m_nNumVertex;										// �����_��
	int m_nNumIndex;										// �C���f�b�N�X��
	int m_nNumPolygon;										// ���|���S����

	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;					// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9		m_pIdxBuff;					// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
};
#endif