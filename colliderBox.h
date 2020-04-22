//=============================================================================
//
// �{�b�N�X�R���C�_�[���� [colliderBox.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _COLLIDERBOX_H_
#define _COLLIDERBOX_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "collider.h"

//=============================================================================
// �O���錾
//=============================================================================
class CMeshCube;

//=============================================================================
// �N���X��`
//=============================================================================
class CColliderBox : public CCollider
{
public:
	CColliderBox(COLLISIONTYPE type);						// �R���X�g���N�^����
	~CColliderBox();										// �f�X�g���N�^����
	HRESULT Init(void);										// ����������
	void Uninit(void);										// �J������
	void Update(void);										// �X�V����
	void Draw(void);										// �`�揈��

	static CColliderBox *Create(bool bTrigger, D3DXVECTOR3 size);			// �N���G�C�g����

	void SetSize(D3DXVECTOR3 size);							// �T�C�Y�̐ݒ�
	D3DXVECTOR3 GetSize(void) { return m_size; }			// �T�C�Y�̎擾

	void OnTriggerEnter(CCollider *col) {};					// �����蔻��(Trigger) �̃R�[���o�b�N�֐�
	void OnCollisionEnter(CCollider *col) {};				// �����蔻�� �̃R�[���o�b�N�֐�

private:
#ifdef _DEBUG
	CMeshCube *m_pCube;										// �{�b�N�X�R���C�_�[�|�C���^
#endif

	D3DXVECTOR3				m_size;							// �T�C�Y
};
#endif