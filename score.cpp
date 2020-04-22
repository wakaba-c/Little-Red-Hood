//=============================================================================
//
// �X�R�A���� [score.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "score.h"
#include "manager.h"
#include "renderer.h"
#include "number.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CScore::CScore(CScene::PRIORITY obj = CScene::PRIORITY_UI) : CScene2D(obj)
{
	SetObjType(PRIORITY_UI);				//�I�u�W�F�N�g�^�C�v

	//�l�̏�����
	m_nNumber = 0;

	for (int nCount = 0; nCount < MAX_SCORE_NUMBER; nCount++)
	{
		m_apNumber[nCount] = NULL;
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScore::~CScore()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CScore::Init(void)
{
	//�e�N�X�`���̊���
	BindTexture(CManager::GetResource("data/tex/total.png"));

	//�e�N�X�`���̒��S�ʒu�̐ݒ�
	SetCenter(TEXTUREVTX_CENTER);

	//�ԍ��\��
	for (int nCntNumber = 0; nCntNumber < MAX_SCORE_NUMBER; nCntNumber++)
	{
		m_apNumber[nCntNumber] = CNumber::Create();

		if (m_apNumber[nCntNumber] != NULL)
		{// ���������݂��Ă����Ƃ�
			m_apNumber[nCntNumber]->SetPosition(D3DXVECTOR3(150.0f + 38 * nCntNumber, 150.0f, 0.0f));		// �ʒu�̐ݒ�
			m_apNumber[nCntNumber]->Init();																	// ����������
		}
	}

	CScene2D::Init();

	//�X�R�A�̐ݒ�
	SetNumber(4);									// ���̐ݒ�
	SetSize(D3DXVECTOR3(120.0f, 210.0f, 0.0f));		// �T�C�Y�̐ݒ�
	SetPosition(D3DXVECTOR3(300, 100.0f, 0.0f));	// �ʒu�̐ݒ�
	SetTransform();									// ���_���̍X�V
	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CScore::Uninit(void)
{
	//UI�̊J��
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CScore::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CScore::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// �N���G�C�g����
//=============================================================================
CScore *CScore::Create(void)
{
	CScore *pScore;
	pScore = new CScore(PRIORITY_UI);
	pScore->Init();
	return pScore;
}

//=============================================================================
// ���[�h����
//=============================================================================
HRESULT CScore::Load(void)
{
	CManager::Load("data/tex/total.png");
	return S_OK;
}

//=============================================================================
// �����̈ʒu�ݒ�
//=============================================================================
void CScore::SetNumberPos(D3DXVECTOR3 pos, float fInterval)
{
	// �����̈ʒu�ݒ�
	for (int nCntNumber = 0; nCntNumber < MAX_SCORE_NUMBER; nCntNumber++)
	{
		if (m_apNumber[nCntNumber] != NULL)
		{
			m_apNumber[nCntNumber]->SetPosition(D3DXVECTOR3(pos.x + fInterval * nCntNumber, pos.y, 0.0f));
			m_apNumber[nCntNumber]->MakeVertex();
		}
	}
}

//=============================================================================
// �����̃T�C�Y�ݒ�
//=============================================================================
void CScore::SetNumderSize(D3DXVECTOR3 size)
{
	// �����̈ʒu�ݒ�
	for (int nCntNumber = 0; nCntNumber < MAX_SCORE_NUMBER; nCntNumber++)
	{
		if (m_apNumber[nCntNumber] != NULL)
		{
			m_apNumber[nCntNumber]->SetSize(size);
			m_apNumber[nCntNumber]->MakeVertex();
		}
	}
}

//=============================================================================
// ���̍X�V
//=============================================================================
void CScore::SetNumber(int nValue)
{
	int nNumber;
	m_nNumber = nValue;

	// ���̍X�V
	for (int nCntNumber = 0; nCntNumber < MAX_SCORE_NUMBER; nCntNumber++)
	{
		nNumber = (int)nValue % (int)pow(10, MAX_SCORE_NUMBER - nCntNumber) / (int)pow(10, MAX_SCORE_NUMBER - 1 - nCntNumber);
		if (m_apNumber[nCntNumber] != NULL)
		{
			m_apNumber[nCntNumber]->SetNumber(nNumber);
		}
	}
}

//=============================================================================
// ���̉��Z
//=============================================================================
void CScore::AddNumber(int nValue)
{
	m_nNumber += nValue;

	SetNumber(m_nNumber);
}