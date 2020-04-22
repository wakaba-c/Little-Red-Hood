//=============================================================================
//
// �^�C�g������ [title.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "fade.h"
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "inputKeyboard.h"
#include "title.h"
#include "bg.h"
#include "inputController.h"
#include "player.h"
#include "meshField.h"
#include "object.h"
#include "camera.h"
#include "sky.h"
#include "house.h"
#include "titleLogo.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
CTitlelogo	*CTitle::m_pTitleLogo = NULL;		// �^�C�g�����S�̃|�C���^

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTitle::CTitle()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTitle::Init(void)
{
	CCamera *pCamera = CManager::GetCamera();		// �J�����̎擾

	if (pCamera != NULL)
	{// �J���������݂��Ă����Ƃ�
		pCamera->SetPosCamera(D3DXVECTOR3(7099.40f, 187.26f, 7523.8f), D3DXVECTOR3(0.1f, -2.33f, 0.0f));		// �ʒu�̐ݒ�
	}

	m_pTitleLogo = CTitlelogo::Create();		// �^�C�g�����S�̐���

	// ��̍쐬
	CSky::Create();

	// ���΂������Ƃ̐���
	CHouse *pHouse = CHouse::Create();

	if (pHouse != NULL)
	{// ���΂������̉Ƃ����݂��Ă����Ƃ�
		pHouse->SetPosition(D3DXVECTOR3(8001.98f, 85.0f, 8194.22f));			// �ʒu���̐ݒ�
	}

	// �e��A�Z�b�g�̐������ݒu
	CMeshField::LoadRand("data/stage/rand.csv", false);				// �����̓Ǎ�
	CObject::LoadModel("data/stage/object.csv");					// ���f�����̓Ǎ�
	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CTitle::Uninit(void)
{
	// �^�C�g�����S�̊J��
	if (m_pTitleLogo != NULL)
	{// ���݂��Ă����Ƃ�
		m_pTitleLogo->Uninit();			// �J������
		delete m_pTitleLogo;			// �폜
		m_pTitleLogo = NULL;			// NULL�̑��
	}

	//�|���S���̊J��
	CScene::ReleaseAll();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTitle::Update(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();
	CInputController *pInputController = CManager::GetInputController();

	if (CFade::GetFade() == CFade::FADE_NONE)
	{//�t�F�[�h�����������Ă��Ȃ��Ƃ�
		if (pInputKeyboard != NULL)
		{// �L�[�{�[�h�����݂��Ă����Ƃ�
			if (pInputKeyboard->GetTriggerKeyboard(DIK_RETURN))
			{// �w��̃L�[�������ꂽ�Ƃ�
				CFade::SetFade(CManager::MODE_GAME);					//�t�F�[�h������
			}
		}
		if (pInputController->GetJoypadUse(0))
		{// �R���g���[���[����������Ă���Ƃ�
			//�Q�[���̑J��
			if (pInputController->GetControllerTrigger(0, JOYPADKEY_A) ||			// �Q�[���p�b�h��A�{�_���������ꂽ�Ƃ�
				pInputController->GetControllerTrigger(0, JOYPADKEY_START))			// �Q�[���p�b�h��START�{�^���������ꂽ�Ƃ�
			{
				CFade::SetFade(CManager::MODE_GAME);					//�t�F�[�h������
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTitle::Draw(void)
{

}

//=============================================================================
// �A�Z�b�g�̓ǂݍ���
//=============================================================================
void CTitle::LoadAsset(void)
{
	CTitlelogo::Load();
}