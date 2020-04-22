//=============================================================================
//
// �G���� [enemy.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "enemy.h"
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "scene3D.h"
#include "meshField.h"
#include "title.h"
#include "model.h"
#include "colliderSphere.h"
#include "meshCapsule.h"
#include "meshCube.h"
#include "inputKeyboard.h"
#include "player.h"
#include "colliderBox.h"
#include "life.h"
#include "fade.h"
#include "purpose.h"
#include "stage.h"
#include "house.h"
#include "phase.h"
#include "enemyUi.h"
#include "effect.h"
#include "messageWindow.h"
#include "result.h"
#include "sound.h"
#include "playerUi.h"
#include "gauge2D.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define SCRIPT_ENEMY "data/animation/wolf.txt"		// �G�z�u���̃A�h���X
#define INTERVAL 100								// �C���^�[�o��
#define MAX_LIFE 10									// �̗͂̍ő�l

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
PDIRECT3DTEXTURE9 CEnemy::m_pTexture = NULL;		// �e�N�X�`�����̃|�C���^
LPD3DXMESH CEnemy::m_pMesh = NULL;					// ���b�V�����
DWORD CEnemy::m_nNumMat = NULL;						// �}�e���A�����
LPD3DXBUFFER CEnemy::m_pBuffMat = NULL;				// �o�b�t�@
int	CEnemy::m_nKill = 0;							// �|������

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CEnemy::CEnemy(CScene::PRIORITY obj = CScene::PRIORITY_ENEMY) : CScene(obj)
{
	// �D��x�̐ݒ�
	SetObjType(CScene::PRIORITY_ENEMY);

	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// ��]�̏�����
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �ړ��ʂ̏�����
	m_dest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �ړ���̏�����
	m_difference = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ���̏�����
	m_nNumParts = 0;									// �p�[�c�����̏�����
	m_bAnimSwitch = true;								// �A�j���[�V�����X�C�b�`�̏�����
	m_bJump = false;									// �W�����v�̏�����
	m_nCurrentFrame = 0;								// ���݂̃t���[�����̏�����
	m_nCurrentKey = 0;									// ���݂̃L�[���̏�����
	m_nLife = MAX_LIFE;									// �̗͂̏�����
	m_pSphere = NULL;									// �����蔻��(��)�̏�����
	m_pAttack = NULL;									// �����蔻��(�U��)�̏�����
	m_apCurrentPos = NULL;								// 1�t���[��������̈ʒu�̈ړ���
	m_apCurrentRot = NULL;								// 1�t���[��������̉�]�̈ړ���
	m_pModel = NULL;									// ���f���̃|�C���^��������
	m_pBox = NULL;										// �����蔻��̏�����
	m_nInterval = 0;									// �C���^�[�o���̏�����
	m_AnimationType = ANIMATIONTYPE_NONE;				// �A�j���[�V�����̏�����

	// �A�j���[�V�������̏�����
	for (int nCount = 0; nCount < ANIMATIONTYPE_MAX; nCount++)
	{
		m_pAnimation[nCount].apKeyInfo = NULL;
	}

	m_apCurrentPos = NULL;								// ���݈ʒu�̏�����
	m_apCurrentRot = NULL;								// ���݉�]�̏�����
	m_pModel = NULL;									// ���f���|�C���^�̏�����
	m_pLife = NULL;										// ���C�t�o�[�|�C���^�̏�����
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEnemy::~CEnemy()
{

}

//==============================================================================
// ����������
//==============================================================================
HRESULT CEnemy::Init(void)
{
	m_AnimationType = ANIMATIONTYPE_NEUTRAL;			// �A�j���[�V�����^�C�v �̏�����

	// �G���f�����̓ǂݍ���
	LoadScript();

	//���̂̐���
	m_pSphere = CColliderSphere::Create(false, 70.0f);

	//���̂̃|�C���^��NULL�ł͂Ȃ��Ƃ�
	if (m_pSphere != NULL)
	{
		m_pSphere->SetScene(this);
		m_pSphere->SetTag("enemy");
		m_pSphere->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_pSphere->SetOffset(D3DXVECTOR3(0.0f, 80.0f, 0.0f));
		m_pSphere->SetMoving(false);
	}

	m_pAttack = CColliderSphere::Create(true, 50.0f);

	//���̂̃|�C���^��NULL�ł͂Ȃ��Ƃ�
	if (m_pAttack != NULL)
	{
		m_pAttack->SetScene(this);
		m_pAttack->SetUse(false);
		m_pAttack->SetTag("weapon");
		m_pAttack->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_pAttack->SetOffset(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_pAttack->SetMoving(false);
	}

	SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CEnemy::Uninit(void)
{
	if (m_apCurrentPos != NULL)
	{// �p�[�c��1�t���[�����Ƃ̈ړ��ʂ��i�[����ϐ���NULL����Ȃ������Ƃ�
		delete[] m_apCurrentPos;
		m_apCurrentPos = NULL;
	}

	if (m_apCurrentRot != NULL)
	{// �p�[�c��1�t���[�����Ƃ̉�]�ʂ��i�[����ϐ���NULL����Ȃ������Ƃ�
		delete[] m_apCurrentRot;
		m_apCurrentRot = NULL;
	}

	if (m_pModel != NULL)
	{//�p�[�c�����݂���ꍇ
		for (int nCount = 0; nCount < m_nNumParts; nCount++)
		{
			m_pModel[nCount].Uninit();								// �J������
		}

		delete[] m_pModel;											// �p�[�c�̑������̃��������J������
		m_pModel = NULL;											// �p�[�c�̃A�h���X��NULL������
	}

	//�A�j���[�V��������
	for (int nCount = 0; nCount < ANIMATIONTYPE_MAX; nCount++)
	{
		if (m_pAnimation[nCount].apKeyInfo != NULL)
		{//NULL�ł͂Ȃ��Ƃ�
			delete[] m_pAnimation[nCount].apKeyInfo;										//�p�[�c�̑��������������擾����
			m_pAnimation[nCount].apKeyInfo = NULL;											//�p�[�c���̃A�h���X��NULL������
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CEnemy::Update(void)
{
	float fHeight = 0.0f;
	D3DXVECTOR3 pos = GetPosition();		// �ʒu�̎擾
	CScene *pScene = NowFloor(pos);			// ���݂���t�B�[���h���擾

	if (pScene != NULL)
	{// �������Ă��鏰�����݂����Ƃ�
		CMeshField *pFloor = (CMeshField*)pScene;		// �L���X�g
		fHeight = pFloor->GetHeight(pos);				// ���̍��������߂�
	}

	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();		// �L�[�{�[�h�̎擾

	if (m_pSphere != NULL)
	{//���̂̃|�C���^��NULL�ł͂Ȃ��Ƃ�
		m_pSphere->SetPosition(pos);				// �ʒu�̐ݒ�
	}

	if (m_pAttack != NULL)
	{
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXMATRIX	mtxRot, mtxTrans, mtxView, mtxMeshRot, mtxMeshTrans;		//�v�Z�p�}�g���b�N�X
		D3DXMATRIX mtx;				// ����̃}�g���b�N�X

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtx);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, rot.y, rot.x, rot.z);
		D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxMeshTrans, pos.x, pos.y, pos.z);
		D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshTrans);

		// �}�g���b�N�X�̍���
		D3DXMatrixMultiply(&mtx, &mtx, &m_pModel[7].GetMtxWorld());

		m_pAttack->SetPosition(D3DXVECTOR3(mtx._41, mtx._42, mtx._43));			// �ʒu�̐ݒ�
	}

	if (m_pBox != NULL)
	{// �{�b�N�X�R���C�_�[�����݂��Ă����Ƃ�
		m_pBox->SetPosition(pos);				// �ʒu�̐ݒ�
	}

	if (pKeyboard->GetTriggerKeyboard(DIK_SPACE))
	{// �X�y�[�X�L�[�������ꂽ�Ƃ�
		if (!m_bJump)
		{
			m_move = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
			m_bJump = true;
		}
	}

	if (!m_bJump)
	{// �W�����v���Ă��Ȃ��Ƃ�
		// �s������
		Move(pos);
	}

	if (m_bAnimSwitch)
	{// ���[�V�������Č����邩�ǂ���
		// �A�j���[�V�����X�V
		Animation();
	}

	// �����蔻��̍X�V
	Collider();

	// �ʒu�X�V
	pos += m_move;

	// ����
	m_move.x += (0 - m_move.x) * 0.12f;
	m_move.z += (0 - m_move.z) * 0.12f;

	//�d�͏���
	if (m_bJump)
	{
		if (pos.y > fHeight)
		{// �������n�ʂ̍�����荂�������Ƃ�
			m_move.y += -0.7f;					// �ړ��ʂɉ��Z����
		}
		else
		{
			pos.y = fHeight;					// ���̍��������߂�
			m_move.y = 0.0f;					// �ړ��ʂ�0�ɂ���
			if (m_bJump)
			{// �W�����v���Ă�����
				m_bJump = false;				// �W�����v�̗L����ύX
			}
		}
	}
	else
	{
		pos.y = fHeight;						// ���̍��������߂�
	}

	if (m_pLife != NULL)
	{// �̗̓Q�[�W�����݂��Ă����Ƃ�
		if (m_AnimationType == ANIMATIONTYPE_ATTACK || m_AnimationType == ANIMATIONTYPE_RUN)
		{// �A�j���[�V�������U���Ƒ����Ă���Ƃ�
			m_pLife->SetPosition(D3DXVECTOR3(pos.x, pos.y + 200.0f, pos.z));			// ���C�t�o�[�̕\���ʒu��ύX
		}
		else
		{
			m_pLife->SetPosition(D3DXVECTOR3(pos.x, pos.y + 150.0f, pos.z));			// ���C�t�o�[�̕\���ʒu��ύX
		}
	}

	if (m_AnimationType == ANIMATIONTYPE_DIE)
	{
		D3DXVECTOR3 pos = GetPosition();				// �ʒu�̎擾
		CEffect::Purification(pos);						// �G�t�F�N�g�̔���
	}

	SetPosition(pos);		// �ʒu���̍X�V

#ifdef _DEBUG
	if (pKeyboard->GetTriggerKeyboard(DIK_0))
	{// 0�������ꂽ�Ƃ�
		AnimationSwitch(ANIMATIONTYPE_ATTACK);			// �A�j���[�V�����̕ύX
	}
	if (pKeyboard->GetTriggerKeyboard(DIK_5))
	{// 5�������ꂽ�Ƃ�
		AnimationSwitch(ANIMATIONTYPE_DIE);				// �A�j���[�V�����̕ύX
	}

	// �f�o�b�O����
	Debug();
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void CEnemy::Draw(void)
{
	D3DXVECTOR3 pos = GetPosition();
	D3DXMATRIX		mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	for (int nCount = 0; nCount < m_nNumParts; nCount++)
	{
		if (m_pModel != NULL)
		{//�p�[�c�����݂���ꍇ
			m_pModel[nCount].Draw(&m_mtxWorld);		//�`��
		}
	}
}

//=============================================================================
// �N���G�C�g�֐�
//=============================================================================
CEnemy *CEnemy::Create(void)
{
	CEnemy *pEnemy;
	pEnemy = new CEnemy(CScene::PRIORITY_ENEMY);

	if (pEnemy != NULL)
	{// �G�����݂��Ă����Ƃ�
		pEnemy->Init();				// ����������
	}
	return pEnemy;
}

//=============================================================================
// ���f���̃��[�h�֐�
//=============================================================================
HRESULT CEnemy::Load(void)
{
	return S_OK;
}

//=============================================================================
// ���f���̓Ǎ�
//=============================================================================
void CEnemy::LoadEnemy(char *add)
{
	FILE *pFile = NULL;						// �t�@�C��
	char cReadText[128] = {};				// ����
	char cHeadText[128] = {};				// ��r
	float aData[3];							// ����
	CEnemy *pEnemy = NULL;

	pFile = fopen(add, "r");				// �t�@�C�����J���܂��͍��

	if (pFile != NULL)						// �t�@�C�����ǂݍ��߂��ꍇ
	{
		fgets(cReadText, sizeof(cReadText), pFile);			// �s���΂�
		fgets(cReadText, sizeof(cReadText), pFile);			// �s���΂�
		fgets(cReadText, sizeof(cReadText), pFile);			// �s���΂�

		while (strcmp(cHeadText, "End") != 0)
		{
			CManager::ConvertStringToFloat(cReadText, ",", aData);		// ���������������R���}�ŋ�؂�i�[����

			pEnemy = CEnemy::Create();			// �N���G�C�g����

			if (pEnemy != NULL)
			{// �G�����݂��Ă����Ƃ�
				pEnemy->SetPosition(D3DXVECTOR3(aData[0], aData[1], aData[2]));			// �|�W�V���������߂�
				pEnemy->SetTarget(TARGETTYPE_PLAYER);				// �U���Ώۂ̐ݒ�
			}

			fgets(cReadText, sizeof(cReadText), pFile);				// �s���΂�
			sscanf(cReadText, "%s", &cHeadText);					// �s��ǂݍ���
		}

		fclose(pFile);				// �t�@�C�������
	}
	else
	{
		MessageBox(NULL, "���f�����̃A�N�Z�X���s�I", "WARNING", MB_ICONWARNING);	// ���b�Z�[�W�{�b�N�X�̐���
	}
}

//=============================================================================
// �U���Ώۂ̐ݒ菈��
//=============================================================================
void CEnemy::SetTarget(TARGETTYPE target)
{
	m_target = target;
}

//=============================================================================
// �����蔻�� �R���C�_�[
//=============================================================================
void CEnemy::OnTriggerEnter(CCollider *col)
{
	std::string sTag = col->GetTag();

	if (m_AnimationType != ANIMATIONTYPE_DIE)
	{
		if (col->GetScene()->GetObjType() == PRIORITY_PLAYER)
		{
			if (sTag == "weapon")
			{// �^�O�� ���� �������Ƃ�
				if (m_AnimationType != ANIMATIONTYPE_DIE)
				{
					CPlayer *pPlayer = CGame::GetPlayer();				// �v���C���[�̎擾
					CSound *pSound = CManager::GetSound();				// �T�E���h�̎擾

					pSound->PlaySoundA(SOUND_LABEL_SE_PUNCH);			// �_���[�W���̍Đ�

					if (m_target != TARGETTYPE_PLAYER)
					{// �U���Ώۂ��v���C���[�ȊO�������Ƃ�
						m_target = TARGETTYPE_PLAYER;				// �W�I���v���C���[�ɒ�߂�
					}

					if (m_pLife != NULL)
					{
						m_pLife->SetLifeBar((float)m_nLife / MAX_LIFE);
					}
					else
					{// �̗̓o�[�̐���
						m_pLife = CLife::Create();
						m_pLife->SetLifeBar((float)m_nLife / MAX_LIFE);
					}

					if (pPlayer != NULL)
					{// �v���C���[�����݂��Ă����Ƃ�
						if (pPlayer->GetState() == pPlayer->PLAYERSTATE_FLOWER)
						{// �v���C���[���\�����������Ƃ�
							CEffect::Slashing(GetPosition(), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
							m_nLife -= 2;				// �̗͂����炷
						}
						else
						{
							m_nLife--;				// �̗͂����炷
						}

						CPlayerUi *pPlayerUi = pPlayer->GetPlayerUi();				// �v���C���[UI�̎擾

						if (pPlayerUi != NULL)
						{// �v���C���[UI�����݂��Ă����Ƃ�
							float fDeathblow = pPlayer->GetDeathblow();				// ���݂̕K�E�Z�|�C���g���擾

							if (fDeathblow < 50.0f)
							{// �K�E�Z�|�C���g��5��菬���������Ƃ�
								CGauge2D *pGauge2D = pPlayerUi->GetGauge(GAUGETYPE_DEATHBLOW);				// �K�E�Z�Q�[�W�̎擾
								fDeathblow++;					// �K�E�Z�|�C���g��1���Z����
								pPlayer->SetDeathblow(fDeathblow);
								pGauge2D->SetLifeBar(false, (float)fDeathblow / 50.0f);				// �Q�[�W�̍X�V
							}
						}

						if (pPlayer->GetAnimType() == CPlayer::ANIMATIONTYPE_ATTACK_1 ||
							pPlayer->GetAnimType() == CPlayer::ANIMATIONTYPE_ATTACK_2)
						{// �m�b�N�o�b�N����
							D3DXVECTOR3 vec;

							vec = GetPosition() - pPlayer->GetPosition();		//���������߂�(���������߂邽��)
							D3DXVec3Normalize(&vec, &vec);			//���K������

							m_move.x = vec.x * 10;
							m_move.z = vec.z * 10;
						}

						if (pPlayer->GetAnimType() == CPlayer::ANIMATIONTYPE_ATTACK_3)
						{// �m�b�N�o�b�N����
							D3DXVECTOR3 vec;

							vec = GetPosition() - pPlayer->GetPosition();		//���������߂�(���������߂邽��)
							D3DXVec3Normalize(&vec, &vec);			//���K������

							m_move.x = vec.x * 10;
							m_move.z = vec.z * 10;
							m_move.y = 10;
							m_bJump = true;
						}

						if (pPlayer->GetAnimType() == CPlayer::ANIMATIONTYPE_ATTACK_5)
						{// �m�b�N�o�b�N����
							D3DXVECTOR3 vec;

							vec = GetPosition() - pPlayer->GetPosition();		//���������߂�(���������߂邽��)
							D3DXVec3Normalize(&vec, &vec);			//���K������

							m_move.x = vec.x * 100;
							m_move.z = vec.z * 100;
							m_move.y = 5;
							m_bJump = true;
						}
					}

					if (m_nLife < 1)
					{// �̗͂� 1 ����������Ƃ�
						CPurpose *pPurpose = CGame::GetPurpose();
						D3DXVECTOR3 vec;

						AnimationSwitch(ANIMATIONTYPE_DIE);
						m_nKill++;

						// �̂̓����蔻��̊J��
						if (m_pSphere != NULL)
						{// �̂̓����蔻�肪���݂��Ă����Ƃ�
							m_pSphere->Release();
							m_pSphere = NULL;
						}

						// ���̓����蔻��̊J��
						if (m_pAttack != NULL)
						{// ���̓����蔻�肪���݂��Ă����Ƃ�
							m_pAttack->Release();
							m_pAttack = NULL;
						}

						// �̗̓Q�[�W�̊J��
						if (m_pLife != NULL)
						{// �̗͂̊J�������݂��Ă����Ƃ�
							m_pLife->Uninit();
							m_pLife->Release();
							m_pLife = NULL;
						}

						if (pPlayer != NULL)
						{// �v���C���[�����݂��Ă����Ƃ�
							vec = GetPosition() - pPlayer->GetPosition();		//���������߂�(���������߂邽��)
						}
						D3DXVec3Normalize(&vec, &vec);			//���K������

						m_move.x = vec.x * 25;
						m_move.z = vec.z * 25;
						m_move.y = 10;
						m_bJump = true;

						if (pPurpose->GetPurpose() == MESSAGETYPE_HOUSEGUARD)
						{
							CHouse *pHouse = CGame::GetHouse();
							CEnemyUi *pEnemyUi = pHouse->GetEnemyUi();
							if (pEnemyUi != NULL)
							{
								pEnemyUi->AddNumber(-1);
							}

							if (pEnemyUi != NULL)
							{// �G��UI�����������݂��Ă����Ƃ�
								if (pEnemyUi->GetNumber() <= 0)
								{// �c��̓G����0�ȉ��ɂȂ����Ƃ�
									CMessageWindow::Create(CMessageWindow::MESSAGETYPE_CLEAR);
									CResult::SetIdxKill(m_nKill);			// Kill�������U���g�ɓn��
									m_nKill = 0;			// Kill���̃��Z�b�g
								}
							}
						}
					}
					else
					{
						AnimationSwitch(ANIMATIONTYPE_DAMAGE);									// �_���[�W���[�V����
					}
				}
			}
		}
	}
}

//========================================================================================
// �p�[�c�̃��[�h
//========================================================================================
void CEnemy::LoadScript(void)
{
	FILE *pFile;
	char cReadText[128];		// ����
	char cHeadText[128];		// ��r
	char cDie[128];
	int nCntPointer = 0;			// �|�C���^�[�̐��l

	char sAdd[64];											// ���f���̃A�h���X
	int nIndex;
	int nParent;

	int nCntMotion = 0;												// �Q�Ƃ���|�C���^�̒l��������
	int nCntKey = 0;

	int nMaxModel = 0;

	int nCntMaya = 0;				// maya�̃f�[�^�Q��

	// �e�L�X�g�f�[�^���[�h
	pFile = fopen(SCRIPT_ENEMY, "r");

	if (pFile != NULL)
	{
		// �|�C���^�[�̃��Z�b�g
		nCntPointer = 0;

		// �X�N���v�g������܂Ń��[�v
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			fgets(cReadText, sizeof(cReadText), pFile);
			sscanf(cReadText, "%s", &cHeadText);
		}

		// �X�N���v�g��������
		if (strcmp(cHeadText, "SCRIPT") == 0)
		{
			// �G���h�X�N���v�g������܂�
			while (strcmp(cHeadText, "END_SCRIPT") != 0)
			{
				fgets(cReadText, sizeof(cReadText), pFile);
				sscanf(cReadText, "%s", &cHeadText);

				// ���s
				if (strcmp(cReadText, "\n") != 0)
				{
					if (strcmp(cHeadText, "NUM_MODEL") == 0)
					{// �p�[�c�����̂Ƃ�
						if (m_pModel == NULL)
						{
							sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nMaxModel);				// �p�[�c�̐����擾

							const int nNumParts = nMaxModel;										// �p�[�c�̐���萔�Ƃ��Ē�`

							if (m_pModel == NULL)
							{// ���f�����i�[����ϐ���NULL�������Ƃ�
								m_pModel = new CModel[nNumParts];									// �p�[�c�̑��������������擾����
							}

							if (m_apCurrentPos == NULL)
							{// �p�[�c��1�t���[�����Ƃ̈ړ��ʂ��i�[����ϐ���NULL�������Ƃ�
								m_apCurrentPos = new D3DXVECTOR3[nNumParts];
							}

							if (m_apCurrentRot == NULL)
							{// �p�[�c��1�t���[�����Ƃ̉�]�ʂ��i�[����ϐ���NULL�������Ƃ�
								m_apCurrentRot = new D3DXVECTOR3[nNumParts];
							}

							for (int nCount = 0; nCount < ANIMATIONTYPE_MAX; nCount++)
							{
								if (m_pAnimation[nCount].apKeyInfo == NULL)
								{
									m_pAnimation[nCount].apKeyInfo = new KEY_INFO[nNumParts];		// �p�[�c�̑��������������擾����
								}
							}

							m_nNumParts = nNumParts;												// �p�[�c�̑������i�[����
						}
					}
					else if (strcmp(cHeadText, "MODEL_FILENAME") == 0)
					{// �p�[�c���f���̃A�h���X���̂Ƃ�
						sscanf(cReadText, "%s %s %s", &cDie, &cDie, &sAdd[0]);						// �A�h���X�̎擾

						if (m_pModel != NULL)
						{
							m_pModel[nCntPointer].Load(sAdd);										// �p�[�c���f���̃��[�h
							m_pModel[nCntPointer].Init();											// �p�[�c���f���̏�����
							nCntPointer++;															// �|�C���^�[��4B�v���X
						}
					}

					else if (strcmp(cHeadText, "CHARACTERSET") == 0)
					{// �L�����N�^�[�̏������̂Ƃ�
						nCntPointer = 0;															// �Q�Ƃ���|�C���^�̒l��������

						// �G���h�L�����N�^�[�Z�b�g������܂Ń��[�v
						while (strcmp(cHeadText, "END_CHARACTERSET") != 0)
						{
							fgets(cReadText, sizeof(cReadText), pFile);
							sscanf(cReadText, "%s", &cHeadText);

							// �p�[�c�Z�b�g��������
							if (strcmp(cHeadText, "PARTSSET") == 0)
							{
								// �G���h�p�[�c�Z�b�g������܂Ń��[�v
								while (strcmp(cHeadText, "END_PARTSSET") != 0)
								{
									fgets(cReadText, sizeof(cReadText), pFile);
									sscanf(cReadText, "%s", &cHeadText);

									//�C���f�b�N�X
									if (strcmp(cHeadText, "INDEX") == 0)
									{
										sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nIndex);			// �C���f�b�N�X�̒l���擾
										m_pModel[nCntPointer].SetIndex(nIndex);							// �C���f�b�N�X�̒l�����ݎQ�ƒ��̃p�[�c���f���Ɋi�[
									}

									// �e
									if (strcmp(cHeadText, "PARENT") == 0)
									{
										sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nParent);			// �e�̒l���擾

										if (nParent == -1)
										{// �e�̒l��-1�������Ƃ�
											m_pModel[nCntPointer].SetParent(NULL);				// NULL���i�[����
										}
										else
										{// �e�̒l��-1�ł͂Ȃ��Ƃ�
											if (m_pModel != NULL)
											{// ���f���̒��g��NULL�ł͂Ȃ��Ƃ�

											 // ���݃��[�h����Ă��郂�f�����̒�����Ώۂ�������܂ŉ�
												for (int nCount = 0; nCount < nMaxModel; nCount++)
												{
													if (nParent == m_pModel[nCount].GetIndex())
													{// �p�[�c���f���̃C���f�b�N�l�Ǝ����̐e�̒l����v�����Ƃ�
														m_pModel[nCntPointer].SetParent(&m_pModel[nCount]);				// �e�̃|�C���^���i�[����
														break;
													}
												}
											}
										}
									}

									if (strcmp(cHeadText, "POS") == 0)
									{//�ʒu
										D3DXVECTOR3 originPos;
										sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
											&originPos.x,
											&originPos.y,
											&originPos.z);

										m_pModel[nCntPointer].SetPosPreset(originPos);
										m_pModel[nCntPointer].SetPosition(originPos);
									}

									if (strcmp(cHeadText, "ROT") == 0)
									{//��]
										D3DXVECTOR3 originRot;
										sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
											&originRot.x,
											&originRot.y,
											&originRot.z);

										m_pModel[nCntPointer].SetRotPreset(originRot);
										m_pModel[nCntPointer].SetRotation(originRot);
									}
								}

								nCntPointer++;											//�Q�Ƃ���|�C���^�̒l��i�߂�
							}
						}
					}
					else if (strcmp(cHeadText, "MOTIONSET") == 0)
					{// �L�����N�^�[�Z�b�g��������

					 // �G���h�L�����N�^�[�Z�b�g������܂Ń��[�v
						while (strcmp(cHeadText, "END_MOTIONSET") != 0)
						{
							fgets(cReadText, sizeof(cReadText), pFile);
							sscanf(cReadText, "%s", &cHeadText);

							if (strcmp(cHeadText, "LOOP") == 0)
							{// ���[�v���邩�ǂ���
								sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_pAnimation[nCntMotion].nLoop);
							}
							else if (strcmp(cHeadText, "NUM_KEY") == 0)
							{// �L�[��
								sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_pAnimation[nCntMotion].nMaxKey);
							}

							if (strcmp(cHeadText, "KEYSET") == 0)
							{// �L�[�Z�b�g��������

							 // �Q�Ƃ���|�C���^�̒l��������
								nCntPointer = 0;
								// �G���h�L�[�Z�b�g������܂Ń��[�v
								while (strcmp(cHeadText, "END_KEYSET") != 0)
								{
									fgets(cReadText, sizeof(cReadText), pFile);
									sscanf(cReadText, "%s", &cHeadText);

									// �t���[����
									if (strcmp(cHeadText, "FRAME") == 0)
									{
										sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_pAnimation[nCntMotion].apKeyInfo[nCntKey].nFrame);
									}

									// �L�[��������
									if (strcmp(cHeadText, "KEY") == 0)
									{
										// �G���h�L�[������܂Ń��[�v
										while (strcmp(cHeadText, "END_KEY") != 0)
										{
											fgets(cReadText, sizeof(cReadText), pFile);
											sscanf(cReadText, "%s", &cHeadText);

											// �ʒu
											if (strcmp(cHeadText, "POS") == 0)
											{
												sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
													&m_pAnimation[nCntMotion].apKeyInfo[nCntPointer].aKey[nCntKey].pos.x,
													&m_pAnimation[nCntMotion].apKeyInfo[nCntPointer].aKey[nCntKey].pos.y,
													&m_pAnimation[nCntMotion].apKeyInfo[nCntPointer].aKey[nCntKey].pos.z);
											}

											// ��]
											if (strcmp(cHeadText, "ROT") == 0)
											{
												sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
													&m_pAnimation[nCntMotion].apKeyInfo[nCntPointer].aKey[nCntKey].rot.x,
													&m_pAnimation[nCntMotion].apKeyInfo[nCntPointer].aKey[nCntKey].rot.y,
													&m_pAnimation[nCntMotion].apKeyInfo[nCntPointer].aKey[nCntKey].rot.z);
											}
										}

										// �p�[�c�̃L�[�����i�[����ꏊ��i�߂�
										nCntPointer++;
									}
								}

								// key�̃J�E���^�[��1�v���X
								nCntKey++;
								nCntPointer = 0;
							}
						}

						// �A�j���[�V�����̎�ނ�1�v���X����
						nCntMotion++;
						nCntKey = 0;
					}
				}
			}
		}

		// �t�@�C����
		fclose(pFile);
	}
	else
	{

	}
}

//=============================================================================
// �s���֐�
//=============================================================================
void CEnemy::Move(D3DXVECTOR3 &pPos)
{
	CPurpose *pPurpose = CGame::GetPurpose();
	CPlayer *pPlayer = CGame::GetPlayer();
	CHouse *pHouse = CGame::GetHouse();
	D3DXVECTOR3 targetPos;

	if (m_target == TARGETTYPE_HOUSE)
	{//	���݂̑Ώۂ�[��]�������Ƃ�
		targetPos = pHouse->GetPosition();
	}
	else if (m_target == TARGETTYPE_PLAYER)
	{// ���݂̍U���Ώۂ�[�v���C���[]�������Ƃ�
		targetPos = pPlayer->GetPosition();
	}

	D3DXVECTOR3 pos = pPos;

	float fDifference_x = pos.x - targetPos.x;
	float fDifference_z = pos.z - targetPos.z;
	float fDifference = sqrtf(fDifference_x * fDifference_x + fDifference_z * fDifference_z);

	// �G���U�������ǂ���
	if (m_AnimationType != ANIMATIONTYPE_ATTACK && m_AnimationType != ANIMATIONTYPE_DAMAGE && m_AnimationType != ANIMATIONTYPE_DIE)
	{
		if (m_nInterval > INTERVAL)
		{
			if (m_target == TARGETTYPE_PLAYER)
			{
				if (pPurpose->GetPurpose() == MESSAGETYPE_HOUSEGUARD)
				{
					// �G���͈͓��ɓ����Ă������ǂ���
					if (fDifference < 1000.0f && fDifference > 150)
					{// �v���C���[�Ɍ������đ���
						D3DXVECTOR3 nor = targetPos - pos;

						D3DXVec3Normalize(&nor, &nor);

						nor.y = 0;
						m_move = nor * 10;
						m_rot.y = atan2f(fDifference_x, fDifference_z);
					}
					else
					{// �͈͊O�������Ƃ�
						if (m_target != TARGETTYPE_HOUSE)
						{// �U���Ώۂ�[��]�ȊO�Ɍ����Ă����Ƃ�
							m_target = TARGETTYPE_HOUSE;			// [��]�ɐݒ�
						}
					}
				}
				else
				{
					// �G���͈͓��ɓ����Ă������ǂ���
					if (fDifference < 2000.0f && fDifference > 150)
					{// �v���C���[�Ɍ������đ���
						D3DXVECTOR3 nor = targetPos - pos;

						D3DXVec3Normalize(&nor, &nor);

						nor.y = 0;
						m_move = nor * 10;
						m_rot.y = atan2f(fDifference_x, fDifference_z);
					}
				}
			}
			else if (m_target == TARGETTYPE_HOUSE)
			{
				// �G���͈͓��ɓ����Ă������ǂ���
				if (fDifference < 3000.0f && fDifference > 450)
				{// �v���C���[�Ɍ������đ���
					D3DXVECTOR3 nor = targetPos - pos;

					D3DXVec3Normalize(&nor, &nor);

					nor.y = 0;
					m_move = nor * 10;
					m_rot.y = atan2f(fDifference_x, fDifference_z);
				}
			}

			if (m_target == TARGETTYPE_PLAYER)
			{
				// �N�[���^�C�����I���Ă��邩�ǂ���
				if (fDifference < 200)
				{// �w�肵���������߂Â��Ă����Ƃ�
					if (m_AnimationType != ANIMATIONTYPE_ATTACK && m_AnimationType != ANIMATIONTYPE_DAMAGE)
					{
						if (CManager::GetRand(10) > 8)
						{
							AnimationSwitch(ANIMATIONTYPE_ATTACK);
							m_nInterval = 0;
						}
					}
				}
			}
			else if (m_target == TARGETTYPE_HOUSE)
			{
				// �N�[���^�C�����I���Ă��邩�ǂ���
				if (fDifference < 460)
				{// �w�肵���������߂Â��Ă����Ƃ�
					if (m_AnimationType != ANIMATIONTYPE_ATTACK && m_AnimationType != ANIMATIONTYPE_DAMAGE)
					{
						if (CManager::GetRand(10) > 8)
						{
							AnimationSwitch(ANIMATIONTYPE_ATTACK);
							m_nInterval = 0;
						}
					}
				}
			}
		}
		else
		{
			m_nInterval++;
		}
	}

	pPos = pos;
}

//=============================================================================
// �A�j���[�V�����֘A�֐�
//=============================================================================
void CEnemy::Animation(void)
{
	// �ŏ��̃t���[���̎�
	if (m_nCurrentFrame == 0)
	{
		for (int nCount = 0; nCount < m_nNumParts; nCount++)
		{
			D3DXVECTOR3 pos = m_pModel[nCount].GetPosition();
			D3DXVECTOR3 rot = m_pModel[nCount].GetRotation();

			// pos�̌v�Z((�ڕW��key + �v���Z�b�g�̔z�u) - ���݂̃L�[)
			m_apCurrentPos[nCount] = ((m_pAnimation[m_AnimationType].apKeyInfo[nCount].aKey[m_nCurrentKey].pos + m_pModel[nCount].GetPosPreset()) - pos) / (float)m_pAnimation[m_AnimationType].apKeyInfo[m_nCurrentKey].nFrame;

			// rot�̌v�Z((�ڕW��key + �v���Z�b�g�̔z�u) - ���݂̃L�[)
			m_apCurrentRot[nCount] = ((m_pAnimation[m_AnimationType].apKeyInfo[nCount].aKey[m_nCurrentKey].rot + m_pModel[nCount].GetRotPreset()) - rot) / (float)m_pAnimation[m_AnimationType].apKeyInfo[m_nCurrentKey].nFrame;
		}
	}
	else
	{// ����ȊO�̃t���[��
		for (int nCount = 0; nCount < m_nNumParts; nCount++)
		{
			D3DXVECTOR3 pos = m_pModel[nCount].GetPosition();
			D3DXVECTOR3 rot = m_pModel[nCount].GetRotation();

			// rot�̈ړ�
			m_pModel[nCount].SetRotation(rot + m_apCurrentRot[nCount]);

			// pos�̈ړ�
			m_pModel[nCount].SetPosition(pos + m_apCurrentPos[nCount]);
		}
	}

	if (m_pAnimation[m_AnimationType].apKeyInfo != NULL)
	{// �L�[���NULL�ł͂Ȃ��Ƃ�
	 // �t���[���̍ő吔�ɖ����Ȃ��ꍇ
		if (m_nCurrentFrame < m_pAnimation[m_AnimationType].apKeyInfo[m_nCurrentKey].nFrame)
		{
			m_nCurrentFrame++;
		}
		// �t���[�����̍ő�l�ɒB�����ꍇ
		else if (m_nCurrentFrame >= m_pAnimation[m_AnimationType].apKeyInfo[m_nCurrentKey].nFrame)
		{
			m_nCurrentKey++;
			m_nCurrentFrame = 0;
		}
	}

	// �L�[���̍ő�l�ɒB�����ꍇ
	if (m_nCurrentKey >= m_pAnimation[m_AnimationType].nMaxKey)
	{
		// ���[�v���邩�ǂ���
		if (m_AnimationType == ANIMATIONTYPE_DIE)
		{
			Release();
			m_bAnimSwitch = false;
		}
		else if (m_pAnimation[m_AnimationType].nLoop)
		{
			// �L�[�̃��Z�b�g
			m_nCurrentKey = 0;
			m_nCurrentFrame = 0;
		}
		else
		{
			// �j���[�g�������[�V����
			m_AnimationType = ANIMATIONTYPE_NEUTRAL;
			m_nCurrentFrame = 0;

			// �L�[�̃��Z�b�g
			m_nCurrentKey = 0;
		}
	}

	if (m_AnimationType != ANIMATIONTYPE_ATTACK && m_AnimationType != ANIMATIONTYPE_DAMAGE && m_AnimationType != ANIMATIONTYPE_DIE)
	{
		if (m_nInterval > INTERVAL)
		{// ���݂̃J�E���g���ڕW�l�𒴂����Ƃ�
			if (fabs(m_move.x) > 2 || fabs(m_move.z) > 2)
			{
				if (m_AnimationType != ANIMATIONTYPE_RUN)
				{// �A�j���[�V����������ȊO�̂Ƃ�
					AnimationSwitch(ANIMATIONTYPE_RUN);			// ���郂�[�V�����ɕύX
				}
			}
			else
			{
				if (m_AnimationType != ANIMATIONTYPE_NEUTRAL)
				{// �ҋ@���[�V�����ȊO�������Ƃ�
					AnimationSwitch(ANIMATIONTYPE_NEUTRAL);		// �ҋ@���[�V�����ɕύX
				}
			}
		}
		else
		{
			if (m_AnimationType != ANIMATIONTYPE_NEUTRAL)
			{// �ҋ@���[�V�����ȊO�������Ƃ�
				AnimationSwitch(ANIMATIONTYPE_NEUTRAL);			// �ҋ@���[�V����
			}
		}
	}
}

//=============================================================================
// �A�j���[�V�����̐؂�ւ�
//=============================================================================
void CEnemy::AnimationSwitch(ANIMATIONTYPE Type)
{
	m_AnimationType = Type;					// �A�j���[�V�����̐؂�ւ�
	m_nCurrentFrame = 0;					// ���݂̃t���[���������Z�b�g
	m_nCurrentKey = 0;						// ���݂̃L�[�������Z�b�g
}

//=============================================================================
// �����蔻�菈��
//=============================================================================
void CEnemy::Collider(void)
{
	switch (m_AnimationType)
	{
	case ANIMATIONTYPE_NEUTRAL:					// �ҋ@���[�V�����̂Ƃ�
		if (m_pAttack != NULL)
		{// �����蔻�肪���݂��Ă����Ƃ�
			if (m_pAttack->GetUse())
			{// �����蔻��̑Ώۂ������Ƃ�
				m_pAttack->SetUse(false);		// �Ώۂ���O��
			}
		}
		break;
	case ANIMATIONTYPE_ATTACK:				// �U�����[�V�����̂Ƃ�
		if (m_pAttack != NULL)
		{
			if (m_nCurrentKey >= 1)
			{
				if (!m_pAttack->GetUse())
				{// �����蔻��̑ΏۊO�������Ƃ�
					m_pAttack->SetUse(true);		// �Ώۂɂ���
				}
			}
		}
		break;
	}
}

#ifdef _DEBUG
//=============================================================================
// �f�o�b�O����
//=============================================================================
void CEnemy::Debug(void)
{
	ImGui::Begin("System");													// �uSystem�v�E�B���h�E�ɒǉ����܂��B�Ȃ���΍쐬���܂��B
	D3DXVECTOR3 size;
	D3DXVECTOR3 pos = GetPosition();

	if (ImGui::TreeNode("enemy"))
	{//�A�j���[�V�����̃c���[�m�[�h��T���܂��B�Ȃ���΍쐬���܂��B
		ImGui::Text("pos%d = %.2f, %.2f, %.2f", pos.x, pos.y, pos.z);							// �v���C���[�̌��݈ʒu��\��

		if (ImGui::TreeNode("collider"))
		{
			ImGui::DragFloat3("ColliderSize", (float*)&size);											// �R���C�_�[�̑傫����ݒ�

			ImGui::DragFloat3("ColliderPos", (float*)&pos);

			ImGui::TreePop();																			// �I��
		}

		ImGui::TreePop();																			// �I��
	}

	//�f�o�b�O�������I��
	ImGui::End();
}
#endif