//=============================================================================
//
// ���b�V���J�v�Z������ [meshCapsule.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _MESHCAPSULE_H_
#define _MESHCAPSULE_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define RADIUS	50.0f										//���a
#define CAPSULE_DIVISION 100								//������

//=============================================================================
// �N���X��`
//=============================================================================
class CMeshCapsule : public CScene
{
public:
	CMeshCapsule(PRIORITY obj);								// �R���X�g���N�^
	~CMeshCapsule();										// �f�X�g���N�^
	HRESULT Init(void);										// ����������
	void Uninit(void);										// �J������
	void Update(void);										// �X�V����
	void Draw(void);										// �`�揈��

	static CMeshCapsule *Create(void);						// �N���G�C�g����
	static HRESULT Load(void);								// ���[�h����

	void SetSize(D3DXVECTOR3 size);							// �傫����ݒ肷��
	void SetColor(D3DXCOLOR col);							// �F��ݒ肷��

	D3DXVECTOR3 GetSize(void) { return m_size; }			// �傫�����擾
	D3DXCOLOR GetColor(void) { return m_col; }				// �F���擾

	void OnTriggerEnter(CCollider *col) {};
	void OnCollisionEnter(CCollider *col) {};

private:
#ifdef _DEBUG
	void Debug(void);										// �f�o�b�O����
#endif
	void MakeVertex(void);									// ���_���̍X�V

	static LPDIRECT3DTEXTURE9	m_pTexture;								//�e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuffSphere;						//���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9		m_pIdxBuffSphere;						//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^

	D3DXVECTOR3 apNor[CAPSULE_DIVISION * CAPSULE_DIVISION * 2];			//1�ʂ��Ƃ̖@���x�N�g���̔z��

	D3DXVECTOR3				m_rot;							//�|���S���̌���(��])
	D3DXVECTOR3				m_size;							//�|���S���̃T�C�Y
	D3DXCOLOR				m_col;							//�|���S���̐F
	D3DXMATRIX				m_mtxWorld;						//���[���h�}�g���b�N�X

	int m_nNumVertexSphere;									//�����_��
	int m_nNumIndexSphere;									//�C���f�b�N�X��
	int m_nNumPolygonSphere;								//���|���S����

	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;					//���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9		m_pIdxBuff;					//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^

	int m_nNumVertex;										//�����_��
	int m_nNumIndex;										//�C���f�b�N�X��
	int m_nNumPolygon;										//���|���S����
};
#endif