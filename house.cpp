//=============================================================================
//
// �Ə��� [house.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "house.h"
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "colliderBox.h"
#include "colliderSphere.h"
#include "houseUi.h"
#include "gauge2D.h"
#include "phase.h"
#include "enemyUi.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define HOUSE_LIFE 100								// �Ƃ̑ϋv�l

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
LPD3DXMESH		CHouse::m_pMesh = NULL;				// ���b�V�����̃|�C���^
DWORD			CHouse::m_nNumMat = 0;				// �}�e���A�����̐�
LPD3DXBUFFER	CHouse::m_pBuffMat = NULL;			// �}�e���A�����̃|�C���^

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CHouse::CHouse(CScene::PRIORITY obj = CScene::PRIORITY_MODEL) : CObject(obj)
{
	SetObjType(CScene::PRIORITY_MODEL);				// �I�u�W�F�N�g�^�C�v�̐ݒ�

	m_nLife = HOUSE_LIFE;							// �ϋv�l�̐ݒ�

	m_ColliderBox = NULL;							// �{�b�N�X�R���C�_�[�̏�����
	m_ColliderSphere = NULL;						// �X�t�B�A�R���C�_�[�̏�����
	m_pHouseUi = NULL;								// ��Ui�̏�����
	m_pPhase = NULL;								// �t�F�[�Y���̏�����
	m_pEnemyUi = NULL;								// �G�̎c�萔�̏�����
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CHouse::~CHouse()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CHouse::Init(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	CSceneX::Init();			// ����������

	// ���f���̏����Z�b�g
	BindModel(m_pVtxBuff, m_pMesh, m_nNumMat, m_pBuffMat, NULL);

	D3DXVECTOR3 pos = D3DXVECTOR3(500.0f, 0.0f, 0.0f);

	m_ColliderBox = CColliderBox::Create(false, D3DXVECTOR3(450.0f, 500.0f, 500.0f));		// ����

	if (m_ColliderBox != NULL)
	{// �����̂̓����蔻�肪���݂��Ă����Ƃ�
		m_ColliderBox->SetScene(this);									// ������ݒ�
		m_ColliderBox->SetTag("house");									// �^�O��house�ɐݒ�
		m_ColliderBox->SetPosition(pos);								// �ʒu�̐ݒ�
		m_ColliderBox->SetOffset(D3DXVECTOR3(0.0f, 200.0f, 0.0f));		// �I�t�Z�b�g�̐ݒ�
	}

	m_ColliderSphere = CColliderSphere::Create(true, 1000.0f);								// ����

	if (m_ColliderSphere != NULL)
	{// ���̓����蔻�肪���݂��Ă����Ƃ�
		m_ColliderSphere->SetScene(this);								// ������ݒ�
		m_ColliderSphere->SetTag("house");								// �^�O��house�ɐݒ�
		m_ColliderSphere->SetPosition(pos);								// �ʒu�̐ݒ�
		m_ColliderSphere->SetOffset(D3DXVECTOR3(0.0f, 85.0f, -10.0f));	// �I�t�Z�b�g�̐ݒ�
	}

	SetPosition(pos);			// �ʒu�̐ݒ�
	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CHouse::Uninit(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void CHouse::Update(void)
{
	D3DXVECTOR3 pos = GetPosition();				// �ʒu�̎擾

	if (m_ColliderBox != NULL)
	{// �{�b�N�X�R���C�_�[�����݂��Ă����Ƃ�
		m_ColliderBox->SetPosition(pos);			// �ʒu�̐ݒ�
	}
	if (m_ColliderSphere != NULL)
	{// �X�t�B�A�R���C�_�[�����݂��Ă����Ƃ�
		m_ColliderSphere->SetPosition(pos);			// �ʒu�̐ݒ�
	}

	SetPosition(pos);			// �ʒu�̐ݒ�
#ifdef _DEBUG
	Debug();					// �f�o�b�O����
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void CHouse::Draw(void)
{
	// �`��
	CSceneX::Draw();
}

//=============================================================================
// �N���G�C�g����
//=============================================================================
CHouse *CHouse::Create()
{
	CHouse *pHouse;
	pHouse = new CHouse(CScene::PRIORITY_MODEL);

	if (pHouse != NULL)
	{// �Ƃ����݂��Ă����Ƃ�
		pHouse->Init();			// ����������
	}
	return pHouse;
}

//=============================================================================
// ���[�h����
//=============================================================================
HRESULT CHouse::Load(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X���擾����
	pDevice = pRenderer->GetDevice();

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data/model/house/Grandma'sHouse.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	D3DXMATERIAL	*pMat;							//���݂̃}�e���A���ۑ��p

	// �}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// �}�e���A���̃e�N�X�`���ǂݍ���
	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{// �e�N�X�`�����������Ƃ�
			char *pFileName = NULL;
			pFileName = pMat[nCntMat].pTextureFilename;			// �e�N�X�`���̃A�h���X���擾
			CManager::Load(pFileName);		// �e�N�X�`���̓ǂݍ���
		}
	}
	return S_OK;
}

//=============================================================================
// �����蔻��(trigger)
//=============================================================================
void CHouse::OnTriggerEnter(CCollider *col)
{
	std::string sTag = col->GetTag();		// �^�O�̎擾

	if (col->GetScene()->GetObjType() == PRIORITY_PLAYER)
	{// ���������I�u�W�F�N�g���v���C���[�������Ƃ�
		if (sTag == "player")
		{
			if (m_pHouseUi == NULL)
			{// ���΂������Ƃ̑ϋv�lUI�̐�������Ă��Ȃ������Ƃ�
				m_pHouseUi = CHouseUi::Create();			// ���΂������Ƃ̑ϋv�lUI�̐���

				if (m_pHouseUi != NULL)
				{// ���΂������Ƃ̑ϋv�lUI�̐�������Ă����Ƃ�
					m_pHouseUi->SetPosition(D3DXVECTOR3(SCREEN_WIDTH / 2, 50.0f, 0.0f));	// �ʒu�̐ݒ�
					m_pHouseUi->SetSize(D3DXVECTOR3(450.0f, 80.0f, 0.0f));					// �T�C�Y�̐ݒ�
					m_pHouseUi->SetTransform();				// ���_���̍X�V
				}

				m_ColliderSphere->SetRadius(350.0f);		// �����蔻��̔��a�̑傫���ύX
				m_ColliderSphere->SetTrigger(false);		// �����蔻��̎�ނ�ύX
				m_ColliderSphere->SetMoving(false);			// �Ώۂ̈ړ����֎~����
			}

			if (m_pPhase == NULL)
			{// ���݂�phase����UI����������Ă��Ȃ������Ƃ�
				m_pPhase = CPhase::Create();				// phase����UI�𐶐�

				if (m_pPhase != NULL)
				{
					m_pPhase->SetPosition(D3DXVECTOR3(120, 50.0f, 0.0f));			// �ʒu�̐ݒ�
					m_pPhase->SetSize(D3DXVECTOR3(100.0f, 50.0f, 0.0f));			// �T�C�Y�̐ݒ�
					m_pPhase->SetTransform();				// ���_���̍X�V
				}
			}

			if (m_pEnemyUi == NULL)
			{// ���݂�phase����UI����������Ă��Ȃ������Ƃ�
				m_pEnemyUi = CEnemyUi::Create();				// �G�̎c�萔��UI�𐶐�

				if (m_pEnemyUi != NULL)
				{
					m_pEnemyUi->SetPosition(D3DXVECTOR3(120, 150.0f, 0.0f));		// �ʒu�̐ݒ�
					m_pEnemyUi->SetSize(D3DXVECTOR3(100.0f, 50.0f, 0.0f));			// �T�C�Y�̐ݒ�
					m_pEnemyUi->SetTransform();				// ���_���̍X�V
				}
			}
		}
	}

	else if (col->GetScene()->GetObjType() == PRIORITY_ENEMY)
	{// ���������I�u�W�F�N�g���G�������Ƃ�
		if (sTag == "weapon")
		{// �^�O��weapon�������Ƃ�
			m_nLife--;			// �ϋv�l��-1����

			if (m_pHouseUi != NULL)
			{// �Ƃ̑ϋv�l�Q�[�W�����݂��Ă����Ƃ�
				CGauge2D *pHitPoint = m_pHouseUi->GetGauge();				// �Q�[�W�̎擾
				pHitPoint->SetLifeBar(false, (float)m_nLife / 100.0f);		// �Q�[�W�̍X�V
			}
		}
	}
}

//=============================================================================
// �����蔻��(collision)
//=============================================================================
void CHouse::OnCollisionEnter(CCollider *col)
{
	std::string sTag = col->GetTag();		// �^�O���擾

	if (col->GetScene()->GetObjType() == PRIORITY_ENEMY)
	{// ���������I�u�W�F�N�g���G�������Ƃ�
		if (sTag == "weapon")
		{// �^�O��weapon�������Ƃ�
			m_nLife--;			// �ϋv�l��-1����

			if (m_pHouseUi != NULL)
			{// �Ƃ̑ϋv�l�Q�[�W�����݂��Ă����Ƃ�
				CGauge2D *pHitPoint = m_pHouseUi->GetGauge();				// �Q�[�W�̎擾
				pHitPoint->SetLifeBar(false, (float)m_nLife / 100.0f);		// �Q�[�W�̍X�V
			}
		}
	}
}

#ifdef _DEBUG
//=============================================================================
// �f�o�b�O����
//=============================================================================
void CHouse::Debug(void)
{

}
#endif