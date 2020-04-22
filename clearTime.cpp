//=============================================================================
//
// �N���A�^�C������ [clearTime.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "clearTime.h"
#include "manager.h"
#include "renderer.h"
#include "number.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 CClearTime::m_pTexture = NULL;		// �e�N�X�`�����̏�����

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CClearTime::CClearTime(CScene::PRIORITY obj = CScene::PRIORITY_UI) : CScene2D(obj)
{
	SetObjType(PRIORITY_UI);				// �I�u�W�F�N�g�^�C�v

	// �l�̏�����
	m_nNumber = 0;
	m_pScene2D = NULL;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CClearTime::~CClearTime()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CClearTime::Init(void)
{
	// �e�N�X�`���̊���
	BindTexture(CManager::GetResource("data/tex/cleartime.png"));

	// �e�N�X�`���̒��S�ʒu�̐ݒ�
	SetCenter(TEXTUREVTX_CENTER);

	D3DXVECTOR3 pos;				// �ʒu�ݒ�p�ϐ�

	// �ԍ��\��
	for (int nCntNumber = 0; nCntNumber < MAX_CLEARTIME_NUMBER; nCntNumber++)
	{
		m_apNumber[nCntNumber] = CNumber::Create();
		pos = D3DXVECTOR3(150.0f + 38 * nCntNumber, 150.0f, 0.0f);

		if (nCntNumber > 1)
		{
			pos.x += 50;
		}

		m_apNumber[nCntNumber]->SetPosition(pos);
		m_apNumber[nCntNumber]->Init();
	}

	CScene2D::Init();

	// �X�R�A�̐ݒ�
	SetNumber(4);
	SetSize(D3DXVECTOR3(120.0f, 210.0f, 0.0f));
	SetPosition(D3DXVECTOR3(300, 100.0f, 0.0f));
	SetTransform();

	m_pScene2D = CScene2D::Create(PRIORITY_UI);

	if (m_pScene2D != NULL)
	{
		m_pScene2D->BindTexture(CManager::GetResource("data/tex/colon.png"));
		m_pScene2D->SetPosition(D3DXVECTOR3(150.0f + 38.0f * MAX_CLEARTIME_NUMBER / 2, 150.0f, 0.0f));
		m_pScene2D->SetSize(D3DXVECTOR3(120.0f, 210.0f, 0.0f));
		m_pScene2D->SetTransform();
	}
	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CClearTime::Uninit(void)
{
	//UI�̊J��
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CClearTime::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CClearTime::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// �N���G�C�g����
//=============================================================================
CClearTime *CClearTime::Create(void)
{
	CClearTime *pClearTime;
	pClearTime = new CClearTime(PRIORITY_UI);

	if (pClearTime != NULL)
	{// �N���A�^�C�������݂��Ă����Ƃ�
		pClearTime->Init();			// ������
	}
	return pClearTime;
}

//=============================================================================
// �e�N�X�`���̃��[�h�֐�
//=============================================================================
HRESULT CClearTime::Load(void)
{
	// �e�N�X�`���ǂݍ���
	CManager::Load("data/tex/cleartime.png");
	CManager::Load("data/tex/colon.png");
	return S_OK;
}

//=============================================================================
// �����̈ʒu�ݒ�
//=============================================================================
void CClearTime::SetNumberPos(D3DXVECTOR3 pos, float fInterval)
{
	D3DXVECTOR3 workPos;				// �ʒu�ݒ�p�ϐ�

	// �����̈ʒu�ݒ�
	for (int nCntNumber = 0; nCntNumber < MAX_CLEARTIME_NUMBER; nCntNumber++)
	{
		if (m_apNumber[nCntNumber] != NULL)
		{
			workPos = D3DXVECTOR3(pos.x + fInterval * nCntNumber, pos.y, 0.0f);		// �ʒu�ݒ�

			if (nCntNumber > 1)
			{// 1�ȏゾ�����Ƃ�
				workPos.x += 50;
			}

			m_apNumber[nCntNumber]->SetPosition(workPos);		// �ʒu�ݒ�
			m_apNumber[nCntNumber]->MakeVertex();				// ���_���̍X�V
		}
	}

	if (m_pScene2D != NULL)
	{
		m_pScene2D->BindTexture(CManager::GetResource("data/tex/colon.png"));
		m_pScene2D->SetPosition(D3DXVECTOR3(pos.x + fInterval * MAX_CLEARTIME_NUMBER / 2, pos.y, 0.0f));
		m_pScene2D->SetTransform();
	}
}

//=============================================================================
// �����̃T�C�Y�ݒ�
//=============================================================================
void CClearTime::SetNumderSize(D3DXVECTOR3 size)
{
	// �����̈ʒu�ݒ�
	for (int nCntNumber = 0; nCntNumber < MAX_CLEARTIME_NUMBER; nCntNumber++)
	{
		if (m_apNumber[nCntNumber] != NULL)
		{// ���������݂��Ă����Ƃ�
			m_apNumber[nCntNumber]->SetSize(size);		// �T�C�Y�̐ݒ�
			m_apNumber[nCntNumber]->MakeVertex();		// ���_���̍X�V
		}
	}
}

//=============================================================================
// ���̍X�V
//=============================================================================
void CClearTime::SetNumber(int nValue)
{
	int nNumber;
	m_nNumber = nValue;

	for (int nCntNumber = 0; nCntNumber < MAX_CLEARTIME_NUMBER; nCntNumber++)
	{
		nNumber = (int)nValue % (int)pow(10, MAX_CLEARTIME_NUMBER - nCntNumber) / (int)pow(10, MAX_CLEARTIME_NUMBER - 1 - nCntNumber);
		if (m_apNumber[nCntNumber] != NULL)
		{
			m_apNumber[nCntNumber]->SetNumber(nNumber);
		}
	}
}

//=============================================================================
// ���̉��Z
//=============================================================================
void CClearTime::AddNumber(int nValue)
{
	m_nNumber += nValue;

	SetNumber(m_nNumber);
}