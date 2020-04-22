//=============================================================================
//
// �X�t�B�A�R���C�_�[���� [colliderSphere.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "colliderSphere.h"
#include "meshSphere.h"

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CColliderSphere::CColliderSphere(COLLISIONTYPE type) : CCollider(type)
{
	m_fRadius = 0.0f;					// ���a�̏�����

#ifdef _DEBUG
	m_pSphere = NULL;					// �X�t�B�A�R���C�_�[�̃|�C���^��������
#endif
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CColliderSphere::~CColliderSphere()
{

}

//==============================================================================
// ����������
//==============================================================================
HRESULT CColliderSphere::Init(void)
{
#ifdef _DEBUG
	m_pSphere = CMeshSphere::Create(m_fRadius);						// �X�t�B�A�R���C�_�[�̐���

	if (m_pSphere != NULL)
	{// �X�t�B�A�R���C�_�[�����݂��Ă����Ƃ�
		m_pSphere->SetPosition(GetPosition());						// �ʒu�̐ݒ�
		m_pSphere->SetColor(D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.2f));		// �F�̐ݒ�
		m_pSphere->MakeVertex();									// ���_���̍X�V
	}
#endif

	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CColliderSphere::Uninit(void)
{
#ifdef _DEBUG
	if (m_pSphere != NULL)
	{// �X�t�B�A�R���C�_�[�����݂��Ă����Ƃ�
		m_pSphere->Release();
	}
#endif
}

//=============================================================================
// �X�V����
//=============================================================================
void CColliderSphere::Update(void)
{
#ifdef _DEBUG
	if (m_pSphere != NULL)
	{// �X�t�B�A�R���C�_�[�����݂��Ă����Ƃ�
		if (GetUse())
		{// �g���Ă���Ƃ�
			if (!m_pSphere->GetActive())
			{// �A�N�e�B�u�ł͂Ȃ������Ƃ�
				m_pSphere->SetActive(true);				// �A�N�e�B�u�ɂ���
			}
		}
		else
		{// �g���Ă��Ȃ��Ƃ�
			if (m_pSphere->GetActive())
			{// �A�N�e�B�u�������Ƃ�
				m_pSphere->SetActive(false);			// ��������
			}
		}

		m_pSphere->SetPosition(GetPosition() + GetOffset());			// �I�t�Z�b�g���l�������ʒu�̐ݒ�
	}
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void CColliderSphere::Draw(void)
{

}

//==============================================================================
// �N���G�C�g����
//==============================================================================
CColliderSphere *CColliderSphere::Create(bool bTrigger, float fRadius)
{
	CColliderSphere *pColliderSphere;
	pColliderSphere = new CColliderSphere(COLLISIONTYPE_SPHERE);

	if (pColliderSphere != NULL)
	{
		pColliderSphere->SetTrigger(bTrigger);							//�g���K�[�̗L����ݒ�
		pColliderSphere->SetRadius(fRadius);							// ���a�̐ݒ�
		pColliderSphere->Init();										// ������
	}
	return pColliderSphere;
}

//=============================================================================
// ���a�̐ݒ�
//==============================================================================
void CColliderSphere::SetRadius(float fValue)
{
	m_fRadius = fValue;				// ���a�̑��

#ifdef _DEBUG
	if (m_pSphere != NULL)
	{// �X�t�B�A�R���C�_�[�����݂��Ă����Ƃ�
		m_pSphere->SetRadius(fValue);			// ���a�̐ݒ�
		m_pSphere->MakeVertex();				// ���_���̍X�V
	}
#endif

}