//=============================================================================
//
// �I�u�W�F�N�g���� [object.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "sceneX.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define DEPTH	20											// ���s��
#define WIDE 20												// ��

//=============================================================================
// �\���̒�`
//=============================================================================
typedef enum
{
	OBJTYPE_TREE_1 = 0,
	OBJTYPE_TREE_2,
	OBJTYPE_TREE_3,
	OBJTYPE_FENCE,
	OBJTYPE_HOUSE_1,
	OBJTYPE_HOUSE_2,
	OBJTYPE_HOUSE_3,
	OBJTYPE_HOUSE_4,
	OBJTYPE_ARROW,
	OBJTYPE_MAX
} OBJTYPE;

//=============================================================================
// �O���錾
//=============================================================================
class CColliderBox;
class CColliderSphere;

//=============================================================================
// �N���X��`
//=============================================================================
class CObject : public CSceneX
{
public:
	CObject(PRIORITY obj);							// �R���X�g���N�^
	~CObject();										// �f�X�g���N�^
	HRESULT Init(void);								// ����������
	void Uninit(void);								// �J������
	void Update(void);								// �X�V����
	void Draw(void);								// �`�揈��

	static CObject *Create(OBJTYPE type);			// �N���G�C�g����
	static HRESULT Load(void);						// ���[�h����
	static void Unload(void);						// �e�N�X�`���̊J������

	void SetType(OBJTYPE type);						// ���f���^�C�v�̐ݒ�
	static void LoadModel(char *add);				// ���f���̃��[�h����

	OBJTYPE GetType(void) { return m_Type; }		// �^�C�v�̎擾

	void OnTriggerEnter(CCollider *col) {};
	void OnCollisionEnter(CCollider *col) {};

private:
#ifdef _DEBUG
	void Debug(void);								// �f�o�b�O����
#endif

	static LPDIRECT3DTEXTURE9	*m_pTexture[OBJTYPE_MAX];					// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;										// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;										// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^

	static LPD3DXMESH		m_pMesh[OBJTYPE_MAX];							// ���b�V�����ւ̃|�C���^
	static DWORD			m_nNumMat[OBJTYPE_MAX];							// �}�e���A�����̐�
	static LPD3DXBUFFER		m_pBuffMat[OBJTYPE_MAX];						// �}�e���A�����ւ̃|�C���^

	CColliderBox *m_ColliderBox;											// �{�b�N�X�R���C�_�[�ւ̃|�C���^
	CColliderSphere *m_ColliderSphere;										// �X�t�B�A�R���C�_�[�ւ̃|�C���^
	OBJTYPE m_Type;															// �^�C�v
};
#endif