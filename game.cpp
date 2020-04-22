//=============================================================================
//
// �Q�[������ [game.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "game.h"
#include "fade.h"
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "inputKeyboard.h"
#include "meshField.h"
#include "player.h"
#include "scene3D.h"
#include "meshSphere.h"
#include "meshCube.h"
#include "meshCapsule.h"
#include "enemy.h"
#include "sky.h"
#include "tree.h"
#include "object.h"
#include "circle.h"
#include "stage.h"
#include "playerUi.h"
#include "meshOrbit.h"
#include "gauge2D.h"
#include "effect.h"
#include "tree.h"
#include "purpose.h"
#include "house.h"
#include "houseUi.h"
#include "phase.h"
#include "number.h"
#include "enemyUi.h"
#include "messageWindow.h"
#include "time.h"
#include "result.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
CMeshField	*CGame::m_pMeshField = NULL;		// ���b�V���t�B�[���h�̃|�C���^
CPlayer		*CGame::m_pPlayer = NULL;			// �v���C���[�̃|�C���^
CMeshSphere *CGame::m_pMeshSphere = NULL;		// ���b�V���X�t�B�A�̃|�C���^
CEnemy		*CGame::m_pEnemy = NULL;			// �G�̃|�C���^
CSky		*CGame::m_pSky = NULL;				// ��̃|�C���^
CPurpose	*CGame::m_pPurpose = NULL;			// �ړI�\���̃|�C���^
CHouse		*CGame::m_pHouse = NULL;			// �Ƃ̃|�C���^

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CGame::CGame()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
//Init����
//=============================================================================
HRESULT CGame::Init(void)
{
	// �G�t�F�N�g�̐���
	CEffect::Create();

	// �v���C���[�̐���
	m_pPlayer = CPlayer::Create();

	// ��̍쐬
	m_pSky = CSky::Create();

	// ���݂̖ړI�\��UI�̐���
	m_pPurpose = CPurpose::Create();

	// ���΂������Ƃ̐���
	m_pHouse = CHouse::Create();

	if (m_pHouse != NULL)
	{// ���΂������̉Ƃ����݂��Ă����Ƃ�
		m_pHouse->SetPosition(D3DXVECTOR3(8001.98f, 85.0f, 8194.22f));			// �ʒu���̐ݒ�
	}

	// �e��A�Z�b�g�̐������ݒu
	CMeshField::LoadRand("data/stage/rand.csv", false);				// �����̓Ǎ�
	CObject::LoadModel("data/stage/object.csv");					// ���f�����̓Ǎ�
	CEnemy::LoadEnemy("data/stage/enemy.csv");						// �G���̓Ǎ�
	CStage::LoadPhase("data/stage/phase_1.csv");					// [phase1]�̓G�Ǎ�

	// ���b�Z�[�W�E�B���h�E�̍쐬
	CMessageWindow::Create(CMessageWindow::MESSAGETYPE_START);

	// ���Ԃ̃N���G�C�g����
	CTime::Create();
	return S_OK;
}

//=============================================================================
// Update�֐�
//=============================================================================
void CGame::Update(void)
{
#ifdef _DEBUG
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	//�Q�[���̑J��
	if (pKeyboard->GetTriggerKeyboard(DIK_RETURN))
	{//�G���^�[�L�[�������ꂽ�Ƃ�
		if (CFade::GetFade() == CFade::FADE_NONE)
		{//�t�F�[�h�����������Ă��Ȃ��Ƃ�
			CResult::SetIdxKill(15);			// Kill�������U���g�ɓn��
			//�t�F�[�h������
			CFade::SetFade(CManager::MODE_RESULT);
		}
	}

	// �{�X�ւ̃V���[�g�J�b�g
	if (pKeyboard->GetTriggerKeyboard(DIK_B))
	{
		if (m_pPlayer != NULL)
		{// �v���C���[�����݂��Ă����Ƃ�
			m_pPlayer->SetPosition(D3DXVECTOR3(5972.14f, 100.0f, 8000.62f));		// �ʒu�̐ݒ�
		}
	}
#endif
}

//=============================================================================
// Draw�֐�
//=============================================================================
void CGame::Draw(void)
{

}

//=============================================================================
// Uninit�֐�
//=============================================================================
void CGame::Uninit(void)
{
	CObject::Unload();

	// �|���S���̊J��
	CScene::ReleaseAll();
}

//=============================================================================
// �A�Z�b�g�̓ǂݍ���
//=============================================================================
void CGame::LoadAsset(void)
{
	// =============== �A�Z�b�g�̓ǂݍ��� ===============//
	CPlayer::Load();
	CSky::Load();
	CMeshField::Load();
	CMeshOrbit::Load();
	CEnemy::Load();
	CObject::Load();
	CMeshSphere::Load();
	CPlayerUi::Load();
	CGauge2D::Load();
	CEffect::Load();
	CPurpose::Load();
	CHouse::Load();
	CNumber::Load();
	CPhase::Load();
	CEnemyUi::Load();
	CMessageWindow::Load();
}