//=============================================================================
//
// ���b�V���X�t�B�A���� [meshSphere.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _MESHSPHERE_H_
#define _MESHSPHERE_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "collider.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define DIVISION 20											//������

//=============================================================================
// �N���X��`
//=============================================================================
class CMeshSphere : public CScene
{
public:
	CMeshSphere(PRIORITY obj);								// �R���X�g���N�^
	~CMeshSphere();											// �f�X�g���N�^
	HRESULT Init(void);										// ����������
	void Uninit(void);										// �J������
	void Update(void);										// �X�V����
	void Draw(void);										// �`�揈��

	static CMeshSphere *Create(float fRadius);				// �N���G�C�g����
	static HRESULT Load(void);								// �f�ނ̎擾

	void SetRotation(D3DXVECTOR3 rot);						// ��]��ݒ�
	D3DXVECTOR3 GetRotation(void) { return m_rot; }			// ��]�̎擾

	void SetVecAxis(D3DXVECTOR3 vecAxis);					// ��]����ݒ�
	D3DXVECTOR3 GetVecAxis(void) { return m_vecAxis; }		// ��]���̎擾

	void SetValueRot(float fValue);							// ��]�p��ݒ�
	float GetValueRot(void) { return m_fValueRot; }			// ��]�p�̎擾

	void SetColor(D3DXCOLOR col);							// �F��ݒ�
	D3DXCOLOR GetColor(void) { return m_col; }				// �F��Ԃ�

	void SetRadius(float fValue);							// ���a��ݒ�
	float GetRadius(void) { return m_fRadius; }				// ���a��Ԃ�

	void MakeVertex(void);									// ���_�����쐬

	void OnTriggerEnter(CCollider *col) {};
	void OnCollisionEnter(CCollider *col) {};

private:
#ifdef _DEBUG
	void Debug(void);										// �f�o�b�O�֐�
#endif

	void MakeIndex(void);									// ���_���쐬�֐�

	static LPDIRECT3DTEXTURE9	m_pTexture;					// �e�N�X�`���ւ̃|�C���^

	D3DXVECTOR3 apNor[DIVISION * DIVISION * 2];				// 1�ʂ��Ƃ̖@���x�N�g���̔z��

	D3DXVECTOR3				m_rot;							// �|���S�� �̌���(��])
	float					m_fRadius;						// �|���S�� �̔��a
	D3DXCOLOR				m_col;							// �|���S�� �̐F
	D3DXMATRIX				m_mtxWorld;						// ���[���h�}�g���b�N�X
	D3DXQUATERNION			m_quat;							// �N�H�[�^�j�I��
	D3DXVECTOR3				m_vecAxis;						// ��]��
	float					m_fValueRot;					// ��]�p(��]��)

	int						m_nNumVertexSphere;				// �����_��
	int						m_nNumIndexSphere;				// �C���f�b�N�X��
 	int						m_nNumPolygonSphere;			// ���|���S����

	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;					// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9		m_pIdxBuff;					// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^

	int m_nNumVertex;										// �����_��
	int m_nNumIndex;										// �C���f�b�N�X��
	int m_nNumPolygon;										// ���|���S����
};
#endif