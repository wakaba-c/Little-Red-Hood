//=============================================================================
//
// �O�Տ��� [meshOrbit.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _MESHORBIT_H_
#define _MESHORBIT_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define ORBIT_DEPTH	1
#define ORBIT_WIDE 20

//=============================================================================
// �\���̒�`
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;										//�ʒu
	D3DXVECTOR3 rot;										//��]
	D3DXMATRIX	mtxWorld;									//���[���h�}�g���b�N�X
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffOrbit;					//���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9	pIdxBuffMeshOrbit;				//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	int nNumVertexMeshOrbit;								//�����_��
	int nNumPolygonMeshOrbit;								//���|���S����
} ORBIT;

typedef struct
{
	D3DXVECTOR3 pos;										//�ʒu
	D3DXVECTOR3 rot;										//��]
	D3DXMATRIX	mtxWorld;									//���[���h�}�g���b�N�X
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffOrbit;					//���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9	pIdxBuffMeshOrbit;				//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	int nNumVertexMeshOrbit;								//�����_��
	int nNumPolygonMeshOrbit;								//���|���S����
} ORBITMTX;

//=============================================================================
// �N���X��`
//=============================================================================
class CMeshOrbit : public CScene
{
public:
	CMeshOrbit(PRIORITY obj);								// �R���X�g���N�^
	~CMeshOrbit();											// �f�X�g���N�^
	HRESULT Init(void);										// ����������
	void Uninit(void);										// �J������
	void Update(void);										// �X�V����
	void Draw(void);										// �`�揈��

	static CMeshOrbit *Create(void);						// �N���G�C�g����
	static HRESULT Load(void);								// ���[�h����

	void SetOrbit(bool bValue);								// �O�Ղ̍쐬
	bool GetOrbit(void) { return m_bUse; }					// �O�Ղ̎擾

	void OnTriggerEnter(CCollider *col) {};
	void OnCollisionEnter(CCollider *col) {};

private:
#ifdef _DEBUG
	void Debug(void);
#endif

	void MakeVertex(void);									// ���_���̍쐬

	static LPDIRECT3DTEXTURE9	m_pTexture;					// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DTEXTURE9			m_pTextureOrbit = NULL;		// �e�N�X�`���ւ̃|�C���^
	ORBIT						m_aOrbit;					// �O��
	ORBITMTX					m_aMtxMeshOrbit;			// �O��
	int							m_nNumIndexOrbit;			// �C���f�b�N�X��
	bool						m_bUse;						// �g�����ǂ���

	//������
	D3DXMATRIX					m_mtxWorldMeshOrbit;
	D3DXMATRIX					m_mtxWorldMeshOrbitOld[ORBIT_WIDE];
	//����
	D3DXMATRIX					m_mtxWorldWeapon;
	D3DXMATRIX					m_mtxWorldWeaponOld[ORBIT_WIDE];
};
#endif