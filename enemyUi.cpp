//=============================================================================
//
// �G�̐��\������ [enemyUi.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "enemyUi.h"
#include "manager.h"
#include "renderer.h"
#include "number.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 CEnemyUi::m_pTexture = NULL;		// �e�N�X�`�����̃|�C���^

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEnemyUi::CEnemyUi(CScene::PRIORITY obj = CScene::PRIORITY_UI) : CScene2D(obj)
{
	SetObjType(PRIORITY_UI);				//�I�u�W�F�N�g�^�C�v

	//�l�̏�����
	m_nNumber = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEnemyUi::~CEnemyUi()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CEnemyUi::Init(void)
{
	//�e�N�X�`���̊���
	BindTexture(CManager::GetResource("data/tex/enemy.png"));

	//�e�N�X�`���̒��S�ʒu�̐ݒ�
	SetCenter(TEXTUREVTX_CENTER);

	//�ԍ��\��
	for (int nCntNumber = 0; nCntNumber < MAX_ENEMY_NUMBER; nCntNumber++)
	{
		m_apNumber[nCntNumber] = CNumber::Create();

		if (m_apNumber[nCntNumber] != NULL)
		{// ���������݂��Ă����Ƃ�
			m_apNumber[nCntNumber]->SetPosition(D3DXVECTOR3(150.0f + 38 * nCntNumber, 150.0f, 0.0f));		// �ʒu�̐ݒ�
			m_apNumber[nCntNumber]->SetSize(D3DXVECTOR3(50.0f, 100.0f, 0.0f));								// �T�C�Y�̐ݒ�
			m_apNumber[nCntNumber]->Init();																	// ����������
		}
	}

	// ����������
	CScene2D::Init();

	//�X�R�A�̐ݒ�
	SetNumber(4);									// �����̐ݒ�
	SetSize(D3DXVECTOR3(120.0f, 210.0f, 0.0f));		// �T�C�Y�̐ݒ�
	SetPosition(D3DXVECTOR3(300, 100.0f, 0.0f));	// �ʒu�̐ݒ�
	SetTransform();									// ���_���̍X�V
	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CEnemyUi::Uninit(void)
{
	// �J��
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CEnemyUi::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CEnemyUi::Draw(void)
{
	// �`��
	CScene2D::Draw();
}

//=============================================================================
// �N���G�C�g����
//=============================================================================
CEnemyUi *CEnemyUi::Create(void)
{
	CEnemyUi *pEnemyUi;
	pEnemyUi = new CEnemyUi(PRIORITY_UI);

	if (pEnemyUi != NULL)
	{// �G�����݂��Ă����Ƃ�
		pEnemyUi->Init();		// ����������
	}
	return pEnemyUi;
}

//=============================================================================
// ���[�h����
//=============================================================================
HRESULT CEnemyUi::Load(void)
{
	// �e�N�X�`���̓ǂݍ���
	CManager::Load("data/tex/SignBoard3.png");
	CManager::Load("data/tex/enemy.png");
	return S_OK;
}

//=============================================================================
// �����̈ʒu�ݒ�
//=============================================================================
void CEnemyUi::SetNumberPos(D3DXVECTOR3 pos, float fInterval)
{
	// �����̈ʒu�ݒ�
	for (int nCntNumber = 0; nCntNumber < MAX_ENEMY_NUMBER; nCntNumber++)
	{
		if (m_apNumber[nCntNumber] != NULL)
		{// �������݂��Ă����Ƃ�
			m_apNumber[nCntNumber]->SetPosition(D3DXVECTOR3(pos.x + fInterval * nCntNumber, pos.y, 0.0f));		// �ʒu�̐ݒ�
			m_apNumber[nCntNumber]->MakeVertex();																// ���_���̍X�V
		}
	}
}

//=============================================================================
// �T�C�Y�̐ݒ�
//=============================================================================
void CEnemyUi::SetNumderSize(D3DXVECTOR3 size)
{
	// �����̈ʒu�ݒ�
	for (int nCntNumber = 0; nCntNumber < MAX_ENEMY_NUMBER; nCntNumber++)
	{
		if (m_apNumber[nCntNumber] != NULL)
		{// ���������݂��Ă����Ƃ�
			m_apNumber[nCntNumber]->SetSize(size);			// �T�C�Y�̐ݒ�
			m_apNumber[nCntNumber]->MakeVertex();			// ���_���̍X�V
		}
	}
}

//=============================================================================
// ���̍X�V
//=============================================================================
void CEnemyUi::SetNumber(int nValue)
{
	int nNumber;
	m_nNumber = nValue;

	// �����̍X�V
	for (int nCntNumber = 0; nCntNumber < MAX_ENEMY_NUMBER; nCntNumber++)
	{
		nNumber = (int)nValue % (int)pow(10, MAX_ENEMY_NUMBER - nCntNumber) / (int)pow(10, MAX_ENEMY_NUMBER - 1 - nCntNumber);

		if (m_apNumber[nCntNumber] != NULL)
		{// ���������݂��Ă����Ƃ�
			m_apNumber[nCntNumber]->SetNumber(nNumber);			// ���̐ݒ�
		}
	}
}

//=============================================================================
// ���̉��Z
//=============================================================================
void CEnemyUi::AddNumber(int nValue)
{
	m_nNumber += nValue;			// ���Z
	SetNumber(m_nNumber);			// �X�V
}