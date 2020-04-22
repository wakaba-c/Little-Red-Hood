//=============================================================================
//
// �{�b�N�X�R���C�_�[���� [colliderBox.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "colliderBox.h"
#include "meshCube.h"

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CColliderBox::CColliderBox(COLLISIONTYPE type) : CCollider(type)
{
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �T�C�Y�̏�����

#ifdef _DEBUG
	m_pCube = NULL;										// �{�b�N�X�R���C�_�[�|�C���^�̏�����
#endif
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CColliderBox::~CColliderBox()
{

}

//==============================================================================
// ����������
//==============================================================================
HRESULT CColliderBox::Init(void)
{
#ifdef _DEBUG
	m_pCube = CMeshCube::Create();								// �{�b�N�X�R���C�_�[�̐���

	if (m_pCube != NULL)
	{// �{�b�N�X�R���C�_�[�����݂��Ă����Ƃ�
		m_pCube->SetPosition(GetPosition());					// �ʒu�̐ݒ�
		m_pCube->SetSize(m_size);								// �T�C�Y�̐ݒ�
		m_pCube->SetColor(D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.2f));	// �F�̐ݒ�
		m_pCube->MakeVertex();									// ���_���̍X�V
	}
#endif
	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CColliderBox::Uninit(void)
{
#ifdef _DEBUG
	if (m_pCube != NULL)
	{// �{�b�N�X�R���C�_�[�����݂��Ă����Ƃ�
		m_pCube->Release();
	}
#endif
}

//=============================================================================
// �X�V����
//=============================================================================
void CColliderBox::Update(void)
{
#ifdef _DEBUG
	if (m_pCube != NULL)
	{// �{�b�N�X�R���C�_�[�����݂��Ă����Ƃ�
		if (GetUse())
		{// �g���Ă���Ƃ�
			if (!m_pCube->GetActive())
			{// �A�N�e�B�u�ł͂Ȃ������Ƃ�
				m_pCube->SetActive(true);			// �A�N�e�B�u�ɂ���
			}
		}
		else
		{// �g���Ă��Ȃ��Ƃ�
			if (m_pCube->GetActive())
			{// �A�N�e�B�u�������Ƃ�
				m_pCube->SetActive(false);			// ��������
			}
		}

		m_pCube->SetPosition(GetPosition() + GetOffset());		// �I�t�Z�b�g���l�������ʒu�̐ݒ�
	}
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void CColliderBox::Draw(void)
{

}

//==============================================================================
// �N���G�C�g����
//==============================================================================
CColliderBox *CColliderBox::Create(bool bTrigger, D3DXVECTOR3 size)
{
	CColliderBox *pColliderBox;
	pColliderBox = new CColliderBox(COLLISIONTYPE_BOX);

	if (pColliderBox != NULL)
	{// �{�b�N�X�R���C�_�[�����݂��Ă����Ƃ�
		pColliderBox->SetTrigger(bTrigger);		// �g���K�[�̗L����ݒ�
		pColliderBox->SetSize(size);			// �T�C�Y�̐ݒ�
		pColliderBox->Init();					// ����������
	}
	return pColliderBox;
}

//=============================================================================
// �T�C�Y�̐ݒ�
//=============================================================================
void CColliderBox::SetSize(D3DXVECTOR3 size)
{
	m_size = size;
}