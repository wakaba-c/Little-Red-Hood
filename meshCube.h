//=============================================================================
//
// ���b�V���L���[�u���� [meshCube.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _MESHCUBE_H_
#define _MESHCUBE_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define DEPTH	20											// ���s��
#define WIDE 20												// ��

//=============================================================================
// �N���X��`
//=============================================================================
class CMeshCube : public CScene
{
public:
	CMeshCube(PRIORITY obj);								// �R���X�g���N�^
	~CMeshCube();											// �f�X�g���N�^
	HRESULT Init(void);										// ����������
	void Uninit(void);										// �J������
	void Update(void);										// �X�V����
	void Draw(void);										// �`�揈��

	static CMeshCube *Create(void);							// �N���G�C�g����
	static HRESULT Load(void);								// ���[�h����

	void SetSize(D3DXVECTOR3 size);							// �|���S���̑傫����ݒ肷��
	void SetColor(D3DXCOLOR col);							// �|���S���̐F��ݒ肷��
	void MakeVertex(void);									// ���_���̍X�V

	D3DXVECTOR3 GetSize(void) { return m_size; }			// �|���S���̑傫����Ԃ�
	D3DXCOLOR GetColor(void) { return m_col; }				// �|���S���̐F��Ԃ�

	void OnTriggerEnter(CCollider *col) {};
	void OnCollisionEnter(CCollider *col) {};

private:
#ifdef _DEBUG
	void Debug(void);										// �f�o�b�O����
#endif

	static LPDIRECT3DTEXTURE9	m_pTexture;					// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;						// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;						// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^

	D3DXVECTOR3 apNor[DEPTH * WIDE * 2];					// 1�ʂ��Ƃ̖@���x�N�g���̔z��

	D3DXVECTOR3				m_rot;							// �|���S���̌���(��])
	D3DXVECTOR3				m_size;							// �|���S���̃T�C�Y
	D3DXCOLOR				m_col;							// �|���S���̐F
	D3DXMATRIX				m_mtxWorld;						// ���[���h�}�g���b�N�X

	D3DXMATRIX				*m_mtxWorldParent;				// �e�̃��[���h�}�g���b�N�X

	int m_nNumVertex;										// �����_��
	int m_nNumIndex;										// �C���f�b�N�X��
	int m_nNumPolygon;										// ���|���S����
};
#endif