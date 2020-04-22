//=============================================================================
//
// �����蔻��}�l�[�W������ [collider.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _COLLIDER_H_
#define _COLLIDER_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
// �\���̒�`
//=============================================================================
typedef enum
{
	COLLISIONTYPE_NONE = -1,
	COLLISIONTYPE_SPHERE,
	COLLISIONTYPE_BOX,
	COLLISIONTYPE_MAX
} COLLISIONTYPE;

//=============================================================================
// �O���錾
//=============================================================================
class CMeshCube;
class CMeshSphere;

//=============================================================================
// �N���X��`
//=============================================================================
class CCollider
{
public:
	CCollider(COLLISIONTYPE Type);		// �R���X�g���N�^
	virtual ~CCollider();				// �f�X�g���N�^
	virtual HRESULT Init(void) = 0;		// ����������
	virtual void Uninit(void) = 0;		// �J������
	virtual void Update(void) = 0;		// �X�V����
	virtual void Draw(void) = 0;		// �`�揈��

	static void ReleaseAll(void);		// �S�Ă̓����蔻���j��
	static void UpdateAll(void);		// �S�Ă̓����蔻����X�V
	static void DrawAll(void);			// �S�Ă̓����蔻���`��

	void SetPosition(D3DXVECTOR3 pos);	// �ʒu�̐ݒ�
	void SetPosOld(D3DXVECTOR3 pos);	// �O��ʒu�̐ݒ�
	void SetActive(bool bValue);		// �X�V�`��Ώېݒ�
	void Release(void);					// �폜�\��
	void SetTag(std::string sTag);		// �^�O�̐ݒ�
	void SetScene(CScene *pScene);		// �����蔻��̎������ݒ�
	void SetTrigger(bool bValue);		// �����蔻��^�C�v�̐ݒ�
	void SetUse(bool bValue);			// �����蔻��̎g�p�ݒ�
	void SetMoving(bool bValue);		// �ʒu�C���̉ېݒ�
	void SetOffset(D3DXVECTOR3 pos);	// �I�t�Z�b�g�̐ݒ�
	void ColliderCheck(void);			// �����蔻��`�F�b�N

	static bool SphereHitCollider(CCollider *pFirstTarget, CCollider *pSecondTarget);		// ���̂̓����蔻��(hit)
	static bool SphereInCollider(CCollider *pFirstTarget, CCollider *pSecondTarget);		// ���̂̓����蔻��(in)
	static bool BoxHitCollider(CCollider *pFirstTarget, CCollider *pSecondTarget);			// �����̂̓����蔻��(Hit)
	static bool BoxInCollider(CCollider *pFirstTarget, CCollider *pSecondTarget);			// �����̂̓����蔻��(in)
	static bool CollisionHitSphereAndBox(CCollider *pFirstTarget, CCollider *pSecondTarget);// �����̂Ɖ~�̓����蔻��(hit)
	static bool CollisionInSphereAndBox(CCollider *pFirstTarget, CCollider *pSecondTarget); // �����̂Ɖ~�̓����蔻��(in)
	static bool CollisionHitSphereAndPoint(D3DXVECTOR3 pos, CCollider *pSecondTarget);		// �~�Ɠ_�̓����蔻��(in)

	COLLISIONTYPE GetColType(void) { return m_Obj; }	// �F�̎擾
	D3DXVECTOR3 GetPosition(void) { return m_pPos; }	// �ʒu�̎擾
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }	// �O�̈ʒu���擾
	bool GetActive(void) { return m_bActive; }			// �X�V�`��Ώێ擾
	std::string GetTag(void) { return m_sTag; }			// �^�O�̎擾
	CScene *GetScene(void) { return m_pScene; }			// ������̎擾
	bool GetTrigger(void){ return m_bTrigger; }			// �^�C�v���擾
	bool GetUse(void) { return m_bUse; }				// �����蔻��̎g�p��Ԏ擾
	bool GetMoving(void) { return m_bMoving; }			// �ʒu�C���̉ێ擾
	D3DXVECTOR3 GetOffset(void) { return m_offset; }	// �ʒu���擾

private:
	void Delete(void);									// �폜����

	static CCollider *m_apCur[COLLISIONTYPE_MAX];		// �Ō�����
	static CCollider *m_apTop[COLLISIONTYPE_MAX];		// �擪���
	CCollider *m_pPrev[COLLISIONTYPE_MAX];				// �O�̏��
	CCollider *m_pNext[COLLISIONTYPE_MAX];				// ���̏��

	static int m_nNumAll;					// �����蔻��̑���
	COLLISIONTYPE m_colType;				// �����蔻��̎��
	bool m_bTrigger;						// ���m�������ǂ���
	bool m_bUse;							// �����蔻��̎g�p
	bool m_bMoving;							// �����蔻��ɂ��ʒu�C���̉�
	COLLISIONTYPE m_Obj;					//�I�u�W�F�N�g�̎��
	bool	m_bActive;						//�`�悷�邩�ǂ���
	bool	m_bDie;							//Release���邩�ǂ���
	CScene *m_pScene;						// �Ώۂ̃V�[��
	std::string	m_sTag;						// �^�O
	D3DXVECTOR3	m_offset;					// �I�t�Z�b�g�l
	D3DXVECTOR3 m_pPos;						// �ʒu�̃|�C���^
	D3DXVECTOR3 m_posOld;					// �O��̈ʒu
	D3DXVECTOR3 m_pMove;					// �ړ��ʂ̃|�C���^
};
#endif