//=============================================================================
//
// �t�F�[�Y���\������ [phase.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "phase.h"
#include "manager.h"
#include "renderer.h"
#include "number.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPhase::CPhase(CScene::PRIORITY obj = CScene::PRIORITY_UI) : CScene2D(obj)
{
	SetObjType(PRIORITY_UI);				//�I�u�W�F�N�g�^�C�v

	//�l�̏�����
	m_nNumber = 0;

	for (int nCount = 0; nCount < MAX_DAYNUMBER; nCount++)
	{
		m_apNumber[nCount] = NULL;
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPhase::~CPhase()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPhase::Init(void)
{
	CScene2D::Init();

	//�e�N�X�`���̊���
	BindTexture(CManager::GetResource("data/tex/Phase.png"));

	//�e�N�X�`���̒��S�ʒu�̐ݒ�
	SetCenter(TEXTUREVTX_CENTER);

	//�ԍ��\��
	for (int nCntNumber = 0; nCntNumber < MAX_DAYNUMBER; nCntNumber++)
	{
		m_apNumber[nCntNumber] = CNumber::Create();			// ����

		if (m_apNumber[nCntNumber] != NULL)
		{
			m_apNumber[nCntNumber]->SetPosition(D3DXVECTOR3(180.0f + 30 * nCntNumber, 50.0f, 0.0f));		// �ʒu�̐ݒ�
			m_apNumber[nCntNumber]->SetSize(D3DXVECTOR3(50.0f, 100.0f, 0.0f));								// �T�C�Y�̐ݒ�
			m_apNumber[nCntNumber]->Init();																	// ����������
		}
	}

	//�X�R�A�̐ݒ�
	SetNumber(1);									// �����̐ݒ�
	SetSize(D3DXVECTOR3(50.0f, 50.0f, 0.0f));		// �T�C�Y�̐ݒ�
	SetPosition(D3DXVECTOR3(300, 60.0f, 0.0f));		// �ʒu�̐ݒ�
	SetTransform();									// ���_���̍X�V
	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CPhase::Uninit(void)
{
	//UI�̊J��
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPhase::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CPhase::Draw(void)
{
	// �`�揈��
	CScene2D::Draw();

	for (int nCntNumber = 0; nCntNumber < MAX_DAYNUMBER; nCntNumber++)
	{
		if (m_apNumber[nCntNumber] != NULL)
		{// ���������݂��Ă����Ƃ�
			m_apNumber[nCntNumber]->Draw();		// �`�揈��
		}
	}
}

//=============================================================================
// �N���G�C�g����
//=============================================================================
CPhase *CPhase::Create(void)
{
	CPhase *pPhase;
	pPhase = new CPhase(PRIORITY_UI);
	pPhase->Init();
	return pPhase;
}

//=============================================================================
// ���[�h����
//=============================================================================
HRESULT CPhase::Load(void)
{
	CManager::Load("data/tex/SignBoard3.png");
	return S_OK;
}

//=============================================================================
// �t�F�[�Y���̐ݒ�
//=============================================================================
void CPhase::SetNumber(int nValue)
{
	int nNumber;
	m_nNumber = nValue;

	for (int nCntNumber = 0; nCntNumber < MAX_DAYNUMBER; nCntNumber++)
	{
		nNumber = (int)nValue % (int)pow(10, MAX_DAYNUMBER - nCntNumber) / (int)pow(10, MAX_DAYNUMBER - 1 - nCntNumber);

		if (m_apNumber[nCntNumber] != NULL)
		{
			m_apNumber[nCntNumber]->SetNumber(nNumber);
		}
	}
}

//=============================================================================
// �t�F�[�Y���̉��Z
//=============================================================================
void CPhase::AddNumber(int nValue)
{
	m_nNumber += nValue;

	SetNumber(m_nNumber);
}