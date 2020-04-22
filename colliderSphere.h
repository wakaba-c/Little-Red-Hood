//=============================================================================
//
// �X�t�B�A�R���C�_�[���� [colliderSphere.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _COLLIDERSPHERE_H_
#define _COLLIDERSPHERE_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "collider.h"

//=============================================================================
// �O���錾
//=============================================================================
class CMeshSphere;

//=============================================================================
// �N���X��`
//=============================================================================
class CColliderSphere : public CCollider
{
public:
	CColliderSphere(COLLISIONTYPE type);					// �R���X�g���N�^
	~CColliderSphere();										// �f�X�g���N�^
	HRESULT Init(void);										// ����������
	void Uninit(void);										// �J������
	void Update(void);										// �X�V����
	void Draw(void);										// �`�揈��

	static CColliderSphere *Create(bool bTrigger, float fRadius);			// �N���G�C�g����

	void SetRadius(float fValue);							// ���a�̐ݒ�
	float GetRadius(void) { return m_fRadius; }				// ���a�̎擾

	void OnTriggerEnter(CCollider *col) {};					// �����蔻��(Trigger) �̃R�[���o�b�N�֐�
	void OnCollisionEnter(CCollider *col) {};				// �����蔻�� �̃R�[���o�b�N�֐�

private:
#ifdef _DEBUG
	CMeshSphere *m_pSphere;									// �X�t�B�A�R���C�_�[�|�C���^
#endif

	float m_fRadius;						// ���a
};
#endif