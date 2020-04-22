//=============================================================================
//
// �Ə��� [house.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _HOUSE_H_
#define _HOUSE_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "object.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define DEPTH	20											// ���s��
#define WIDE 20												// ��

//=============================================================================
// �O���錾
//=============================================================================
class CHouseUi;
class CPhase;
class CEnemyUi;

//=============================================================================
// �N���X��`
//=============================================================================
class CHouse : public CObject
{
public:
	CHouse(PRIORITY obj);										// �R���X�g���N�^
	~CHouse();													// �f�X�g���N�^
	HRESULT Init(void);											// ����������
	void Uninit(void);											// �J������
	void Update(void);											// �X�V����
	void Draw(void);											// �`�揈��

	static CHouse *Create();									// �N���G�C�g����
	static HRESULT Load(void);									// ���[�h����

	CPhase *GetPhase(void) { return m_pPhase; }					// �t�F�[�Y���̃|�C���^�擾
	CEnemyUi *GetEnemyUi(void) { return m_pEnemyUi; }			// �G�̎c�萔�̃|�C���^�擾

	void OnTriggerEnter(CCollider *col);
	void OnCollisionEnter(CCollider *col);

private:
#ifdef _DEBUG
	void Debug(void);											// �f�o�b�O����
#endif

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;							// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;							// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^

	static LPD3DXMESH m_pMesh;									// ���b�V�����ւ̃|�C���^
	static DWORD m_nNumMat;										// �}�e���A�����̐�
	static LPD3DXBUFFER	m_pBuffMat;								// �}�e���A�����ւ̃|�C���^

	CHouseUi *m_pHouseUi;										// ���΂������Ƃ̑ϋv�lUI
	CPhase *m_pPhase;											// ���݂�phase����UI
	CEnemyUi *m_pEnemyUi;										// �G�̎c�萔��UI

	int						m_nLife;							// �Ƃ̑ϋv�l

	CColliderBox *m_ColliderBox;								// �{�b�N�X�R���C�_�[�̃|�C���^
	CColliderSphere *m_ColliderSphere;							// �X�t�B�A�R���C�_�[�̃|�C���^
};
#endif