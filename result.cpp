//=============================================================================
//
// ���U���g���� [result.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "result.h"
#include "fade.h"
#include "manager.h"
#include "scene.h"
#include "inputKeyboard.h"
#include "inputController.h"
#include "bg.h"
#include "enemyUi.h"
#include "score.h"
#include "number.h"
#include "clearTime.h"
#include "ranking.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_MAGNIFICATION 5						// �{��

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
int CResult::m_nKill = 0;						// �|������
int CResult::m_nSeconds = 0;					// �b��
int CResult::m_nMinutes = 0;					// ����

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CResult::CResult()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CResult::~CResult()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CResult::Init(void)
{
	int nTotal = 0;

	CBg::Create();			// �w�i�̍쐬

	// =============== �^�C�g�� ================ //
	CScene2D *pScene2D = CScene2D::Create(CScene::PRIORITY_UI);

	if (pScene2D != NULL)
	{// pScene2D�����݂��Ă����Ƃ�
		pScene2D->BindTexture(CManager::GetResource("data/tex/result.png"));		// �摜�̓ǂݍ���
		pScene2D->SetPosition(D3DXVECTOR3(300.0f, 150.0f, 0.0f));					// �ʒu�ݒ�
		pScene2D->SetSize(D3DXVECTOR3(500.0f, 150.0f, 0.0f));						// �傫���ݒ�
		pScene2D->SetTransform();													// ���_���̍X�V
	}

	// =============== �G�̓����� =============== //
	CEnemyUi *pEnemyUi = CEnemyUi::Create();

	if (pEnemyUi != NULL)
	{// pEnemyUi�����݂��Ă����Ƃ�
		pEnemyUi->SetPosition(D3DXVECTOR3(350.0f, 370.0f, 0.0f));					// �ʒu�̐ݒ�
		pEnemyUi->SetSize(D3DXVECTOR3(300.0f, 100.0f, 0.0f));						// �T�C�Y�̐ݒ�
		pEnemyUi->SetNumderSize(D3DXVECTOR3(70.0f, 180.0f, 0.0f));					// �����̃T�C�Y�ݒ�
		pEnemyUi->SetNumberPos(D3DXVECTOR3(730.0f, 368.0f, 0.0f), 55.0f);			// �����̈ʒu�ݒ�
		pEnemyUi->SetNumber(m_nKill);												// �����̐ݒ�
		pEnemyUi->SetTransform();													// ���_���̍X�V
	}

	// =============== �N���A�^�C�� =============== //
	CClearTime *pClearTime = CClearTime::Create();

	if (pClearTime != NULL)
	{// pClearTime�����݂��Ă����Ƃ�
		pClearTime->SetPosition(D3DXVECTOR3(350.0f, 250.0f, 0.0f));					// �ʒu�̐ݒ�
		pClearTime->SetSize(D3DXVECTOR3(300.0f, 100.0f, 0.0f));						// �T�C�Y�̐ݒ�
		pClearTime->SetNumderSize(D3DXVECTOR3(70.0f, 180.0f, 0.0f));				// �����̃T�C�Y�ݒ�
		pClearTime->SetNumberPos(D3DXVECTOR3(730.0f, 248.0f, 0.0f), 55.0f);			// �����̈ʒu�ݒ�

		m_nMinutes *= 100;
		pClearTime->SetNumber(m_nMinutes + m_nSeconds);								// �����̐ݒ�
		pClearTime->SetTransform();													// ���_���̍X�V
	}

	// =============== �����_ =============== //
	CScore *pScore = CScore::Create();

	if (pScore != NULL)
	{// pEnemyUi�����݂��Ă����Ƃ�
		pScore->SetPosition(D3DXVECTOR3(350.0f, 620.0f, 0.0f));						// �ʒu�̐ݒ�
		pScore->SetSize(D3DXVECTOR3(300.0f, 100.0f, 0.0f));							// �T�C�Y�̐ݒ�
		pScore->SetNumderSize(D3DXVECTOR3(70.0f, 180.0f, 0.0f));					// �����̃T�C�Y�ݒ�
		pScore->SetNumberPos(D3DXVECTOR3(730.0f, 608.0f, 0.0f), 55.0f);				// �����̈ʒu�ݒ�

		pScore->SetNumber(m_nKill * ((MAX_MAGNIFICATION - m_nMinutes) * 100));		// �����̐ݒ�
		pScore->SetTransform();														// ���_���̍X�V
	}

	CRanking::SetResultIndex(m_nKill * ((MAX_MAGNIFICATION - m_nMinutes) * 100));	// �����L���O�ɍ���̓��_�𑗂�
	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CResult::Uninit(void)
{
	m_nKill = 0;		// Kill����������
	m_nSeconds = 0;		// �b����������
	m_nMinutes = 0;		// ������������

						//�|���S���̊J��
	CScene::ReleaseAll();
}

//=============================================================================
// �X�V����
//=============================================================================
void CResult::Update(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();
	CInputController *pInputController = CManager::GetInputController();

	if (CFade::GetFade() == CFade::FADE_NONE)
	{//�t�F�[�h�����������Ă��Ȃ��Ƃ�
		if (pInputKeyboard != NULL)
		{// �L�[�{�[�h�����݂��Ă����Ƃ�
			if (pInputKeyboard->GetTriggerKeyboard(DIK_RETURN))
			{// �w��̃L�[�������ꂽ�Ƃ�
				CFade::SetFade(CManager::MODE_RANKING);					//�t�F�[�h������
			}
		}
		if (pInputController->GetJoypadUse(0))
		{// �R���g���[���[����������Ă���Ƃ�
		 //�Q�[���̑J��
			if (pInputController->GetControllerTrigger(0, JOYPADKEY_A) ||			// �Q�[���p�b�h��A�{�_���������ꂽ�Ƃ�
				pInputController->GetControllerTrigger(0, JOYPADKEY_START))			// �Q�[���p�b�h��START�{�^���������ꂽ�Ƃ�
			{
				CFade::SetFade(CManager::MODE_RANKING);					//�t�F�[�h������
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CResult::Draw(void)
{

}

//=============================================================================
// �A�Z�b�g�̓ǂݍ���
//=============================================================================
void CResult::LoadAsset(void)
{
	CBg::Load();
	CScore::Load();
	CNumber::Load();
	CClearTime::Load();

	CManager::Load("data/tex/result.png");
}

//=============================================================================
// �G�̓������̐ݒ�
//=============================================================================
void CResult::SetIdxKill(int nValue)
{
	m_nKill = nValue;
}

//=============================================================================
// �b���̐ݒ�
//=============================================================================
void CResult::SetSeconds(int nValue)
{
	m_nSeconds = nValue;
}

//=============================================================================
// �����̐ݒ�
//=============================================================================
void CResult::SetMinutes(int nValue)
{
	m_nMinutes = nValue;
}