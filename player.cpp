//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "player.h"
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "inputKeyboard.h"
#include "inputController.h"
#include "scene3D.h"
#include "meshField.h"
#include "model.h"
#include "meshCapsule.h"
#include "colliderSphere.h"
#include "meshCube.h"
#include "enemy.h"
#include "meshOrbit.h"
#include "camera.h"
#include "colliderBox.h"
#include "stage.h"
#include "sound.h"
#include "scene2D.h"
#include "effect.h"
#include "playerUi.h"
#include "gauge2D.h"
#include "purpose.h"
#include "messageWindow.h"
#include "result.h"
#include "fade.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define	SCRIPT_PLAYER "data/animation/girl.txt"		// �Ԃ�����̃��f�����A�h���X
#define COUNTER_COMBO 30							// �h���U����t�J�E���^
#define JUMP_MAX 10									// �W�����v�̉����x

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPlayer::CPlayer(CScene::PRIORITY obj = CScene::PRIORITY_PLAYER) : CScene(obj)
{
	// �D��x�̐ݒ�
	SetObjType(CScene::PRIORITY_PLAYER);				// �I�u�W�F�N�g�^�C�v

	m_nLife = 100;										// �̗͂̏�����
	m_fSpeed = NORMAL_SPEED;							// �X�s�[�h�̏�����
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// ��]�̏�����
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �ړ��ʂ̏�����
	m_dest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �ړ���̏�����
	m_difference = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ���̏�����
	m_nNumParts = 0;									// �p�[�c�����̏�����
	m_nCurrentFrame = 0;								// �t���[�����̏�����
	m_nCurrentKey = 0;									// �L�[���̏�����
	m_pColPlayerSphere = NULL;							// �v���C���[�����蔻��|�C���^�̏�����
	m_pColWeaponSphere = NULL;							// �n�����蔻��|�C���^�̏�����
	m_pColHandSphere = NULL;							// ��̓����蔻��̏�����
	m_pBox = NULL;
	m_bAnimSwitch = false;								// �A�j���[�V�����X�C�b�`�̏�����
	m_bJump = false;									// �W�����v�t���O�̏�����
	m_nCntAttacCombo = COUNTER_COMBO;					// �U���h���J�E���^�̏�����
	m_AttackTypeOld = ANIMATIONTYPE_ATTACK_1;			// �O�̍U���^�C�v��������
	m_AnimationTypeOld = ANIMATIONTYPE_NONE;			// �O�̃A�j���[�V�����^�C�v��������
	m_bAnimation = true;								// �A�j���[�V�����t���O�̏�����
	m_state = PLAYERSTATE_NORMAL;						// �v���C���[�X�e�[�^�X�̏�����
	m_nActionCount = 0;									// �A�N�V�����J�E���^�̏�����
	m_nParticleCount = 0;								// �p�[�e�B�N���J�E���^�̏�����
	m_fDeathblow = 0.0f;								// �K�E�Z�|�C���g�̏�����
	m_bEvent = false;									// �C�x���g�����t���O�̏�����

	// �A�j���[�V�������̏�����
	for (int nCount = 0; nCount < ANIMATIONTYPE_MAX; nCount++)
	{
		m_pAnimation[nCount].apKeyInfo = NULL;			// �A�j���[�V�������̏�����
	}

	m_apCurrentPos = NULL;								// ���݈ʒu �̏�����
	m_apCurrentRot = NULL;								// ���݉�] �̏�����
	m_pModel = NULL;									// ���f���|�C���^ �̏�����
	m_pPlayerUi = NULL;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPlayer::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	CRenderer *pRenderer = CManager::GetRenderer();

	//�f�o�C�X���擾����
	pDevice = pRenderer->GetDevice();

	CCamera *pCamera = CManager::GetCamera();
	D3DXVECTOR3 pos = GetPosition();						//�v���C���[�̈ʒu�擾

	pos = D3DXVECTOR3(9276.96f, 10.0f, -9218.10f);			// �v���C���[�̈ʒu�ݒ�

	if (pCamera != NULL)
	{
		pCamera->SetPosCamera(pos, D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	}

	m_AnimationType = ANIMATIONTYPE_NEUTRAL;				// �A�j���[�V�����^�C�v �̏�����

	// �v���C���[���f�����̓ǂݍ���
	LoadScript();

	// ����̓����蔻��𐶐�
	m_pColWeaponSphere = CColliderSphere::Create(true, 30.0f);

	if (m_pColWeaponSphere != NULL)
	{ // ���̂̃|�C���^��NULL�ł͂Ȃ��Ƃ�
		m_pColWeaponSphere->SetScene(this);											// �Ăяo���� �̐ݒ�
		m_pColWeaponSphere->SetTag("weapon");										// �^�O �̐ݒ�
	}

	// ����̓����蔻��𐶐�
	m_pColHandSphere = CColliderSphere::Create(true, 50.0f);

	if (m_pColHandSphere != NULL)
	{ // ���̂̃|�C���^��NULL�ł͂Ȃ��Ƃ�
		m_pColHandSphere->SetScene(this);											// �Ăяo���� �̐ݒ�
		m_pColHandSphere->SetTag("weapon");											// �^�O �̐ݒ�
	}

	// �v���C���[�̓����蔻��𐶐�
	m_pColPlayerSphere = CColliderSphere::Create(false, 20.0f);

	if (m_pColPlayerSphere != NULL)
	{ //���̂̃|�C���^��NULL�ł͂Ȃ��Ƃ�
		m_pColPlayerSphere->SetScene(this);
		m_pColPlayerSphere->SetTag("player");										// �^�O �̐ݒ�
		m_pColPlayerSphere->SetPosition(pos);										// �ʒu �̐ݒ�
		m_pColPlayerSphere->SetOffset(D3DXVECTOR3(0.0f, 100.0f, 0.0f));
	}

	SetPosition(pos);

	// �O�Ղ̐���
	m_pMeshOrbit = CMeshOrbit::Create();

	m_pPlayerUi = CPlayerUi::Create();

	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CPlayer::Uninit(void)
{
	if (m_pColWeaponSphere != NULL)
	{// ����̓����蔻�肪���݂��Ă����Ƃ�
		if (m_pColWeaponSphere != NULL)
		{
			m_pColWeaponSphere->Release();
		}
	}

	if (m_pColPlayerSphere != NULL)
	{// ����̓����蔻�肪���݂��Ă����Ƃ�
		m_pColPlayerSphere->Release();
	}

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
void CPlayer::Update(void)
{
	D3DXVECTOR3 pos;
	CSound *pSound = CManager::GetSound();
	float fHeight = 0.0f;

	if (!m_bEvent)
	{// �C�x���g���������Ă��Ȃ������Ƃ�
		// ���͏���
		Input();
	}

	pos = GetPosition();				// �ʒu�̎擾

	//���̍������擾����
	CScene *pSceneNext = NULL;														// ������
	CScene *pSceneNow = GetScene(PRIORITY_FLOOR);									// �V�[���̐擪�A�h���X���擾

	CScene *pScene = NowFloor(pos);													// ���݂���t�B�[���h���擾

	if (pScene != NULL)
	{// �������Ă��鏰�����݂����Ƃ�
		CMeshField *pFloor = (CMeshField*)pScene;									// �L���X�g
		fHeight = pFloor->GetHeight(pos);

		RANDTYPE Type = pFloor->GetRandType();

		if (m_AnimationType == ANIMATIONTYPE_WALK || m_AnimationType == ANIMATIONTYPE_RUN)
		{
			if (m_nCurrentKey == 5 || m_nCurrentKey == 0)
			{
				if (m_nCurrentFrame == 0)
				{
					if (Type == RANDTYPE_GRASS)
					{
						pSound->PlaySoundA((SOUND_LABEL)(CManager::GetRand(3) + (int)SOUND_LABEL_SE_GRASS_1));
					}
					else if (Type == RANDTYPE_SAND)
					{
						pSound->PlaySoundA((SOUND_LABEL)(CManager::GetRand(3) + (int)SOUND_LABEL_SE_SAND_1));
					}
				}
			}
		}
	}

	if (m_bEvent)
	{
		if (m_AnimationTypeOld == ANIMATIONTYPE_SWITCHWEAPON)
		{
			if (m_state == PLAYERSTATE_FLOWER)
			{
				if (m_nActionCount == 30)
				{
					CRenderer *pRenderer = CManager::GetRenderer();
					CCamera *pCamera = CManager::GetCamera();
					pRenderer->SetUpdate(false);
					pCamera->SetRotMotion(D3DXVECTOR3(0.0f, D3DX_PI * 2, 0.0f), D3DXVECTOR3(0.0f, 0.1f, 0.0f));
					m_nActionCount++;
				}
				else if (m_nActionCount == 31)
				{
					m_bEvent = false;
				}
				else
				{
					m_nActionCount++;
				}
			}
		}
	}
	else
	{
		// ���̏�Ԃ��\����Ԃ������Ƃ�
		if (m_state == PLAYERSTATE_FLOWER)
		{// �\����Ԃ������Ƃ�
			if (m_nParticleCount > 15)
			{// �J�E���^��15�ȏゾ�����Ƃ�
				for (int nCount = 0; nCount < 3; nCount++)
				{
					// �G�t�F�N�g�̐�������
					CEffect::WingStay(D3DXVECTOR3(pos.x, pos.y + 100.0f, pos.z));
					CEffect::PetalsSoaring(pos);
				}

				m_nParticleCount = 0;			// �J�E���^�̃��Z�b�g
				m_nActionCount = 0;				// �J�E���^�̃��Z�b�g
			}
			else
			{
				m_nParticleCount++;				// �J�E���^��1�v���X����
			}

			CEffect::Aura(m_nActionCount, m_bJump, pos);		// �I�[���G�t�F�N�g�̐���
			m_nActionCount++;					// �J�E���^��1�v���X����

			m_fDeathblow -= 0.1f;
			CGauge2D *pGauge2D = m_pPlayerUi->GetGauge(GAUGETYPE_DEATHBLOW);
			pGauge2D->SetLifeBar(false, (float)m_fDeathblow / 50.0f);				// �Q�[�W�̍X�V

			if (m_fDeathblow <= 0.0f)
			{// �K�E�Z�|�C���g��0�ȉ��������Ƃ�
				if (m_state == PLAYERSTATE_FLOWER)
				{// �C�x���g�����t���O�������Ă��Ȃ������Ƃ�
					if (!m_bEvent)
					{
						AnimationSwitch(ANIMATIONTYPE_SWITCHWEAPON);			// �A�j���[�V�����̕ύX
						m_nActionCount = 0;			// �J�E���^�̏�����
						m_bEvent = true;
					}
				}
			}
		}
	}

	if (m_bAnimation)
	{// �A�j���[�V�����Đ�
		// �A�j���[�V�����X�V����
		Animation();
	}

	//�p�x
	if (m_AnimationType != ANIMATIONTYPE_ATTACK_4)
	{
		if (!m_bJump)
		{
			m_dest.y = atan2f(-m_move.x, -m_move.z);
		}
	}

	// �����蔻��Ǘ�����
	Collision();

	// �ʒu�X�V
	pos += m_move;

	if (!m_bJump)
	{
		// ����
		m_move.x += (0 - m_move.x) * 0.12f;
		m_move.z += (0 - m_move.z) * 0.12f;
	}

	//�d�͏���
	if (m_bJump)
	{// �W�����v���Ă����Ƃ�
		if (m_AnimationType != ANIMATIONTYPE_JUMP_1)
		{
			if (pos.y > fHeight)
			{// ���݂̍���������荂�������Ƃ�
				m_move.y += -0.7f;
			}
			else
			{// ���݂̍����������Ⴉ������
				pos.y = fHeight;											// ���̍��������߂�
				m_move.y = 0.0f;
				if (m_bJump)
				{
					// �����̃G�t�F�N�g�\��
					CEffect::SandSmokeEffect(pos);

					m_bJump = false;										// �W�����v�����ς���
					m_bAnimation = true;									// �A�j���[�V�����̍ĊJ
					AnimationSwitch(ANIMATIONTYPE_JUMP_5);					// �A�j���[�V�����̕ύX
					pSound->PlaySoundA(SOUND_LABEL_SE_LANDING);				// ���n���̍Đ�
				}
			}
		}
	}
	else
	{
		pos.y = fHeight;											// ���̍��������߂�
	}

	// �G�t�F�N�g�̐���
	if (m_AnimationType == ANIMATIONTYPE_ATTACK_5)
	{
		if (m_nCurrentKey == 1)
		{
			if (m_nCurrentFrame > 0)
			{
				CEffect::PetalCluster(D3DXVECTOR3(m_pModel[4].GetMtxWorld()._41, m_pModel[4].GetMtxWorld()._42, m_pModel[4].GetMtxWorld()._43), m_rot);
			}
		}
	}

	//���̂̃|�C���^��NULL�ł͂Ȃ��Ƃ�
	if (m_pColWeaponSphere != NULL)
	{
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 40.0f, -80.0f);
		D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXMATRIX	mtxRot, mtxTrans, mtxView, mtxMeshRot, mtxMeshTrans;				//�v�Z�p�}�g���b�N�X
		D3DXMATRIX mtx;			// ����̃}�g���b�N�X

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtx);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, rot.y, rot.x, rot.z);
		D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxMeshTrans, pos.x, pos.y, pos.z);
		D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshTrans);

		D3DXMatrixMultiply(&mtx, &mtx, &m_pModel[14].GetMtxWorld());

		m_pColWeaponSphere->SetPosition(D3DXVECTOR3(mtx._41, mtx._42, mtx._43));
	}

	if (m_pColHandSphere != NULL)
	{// ��̓����蔻�肪���݂��Ă����Ƃ�
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, -50.0f, 0.0f);
		D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXMATRIX	mtxRot, mtxTrans, mtxView, mtxMeshRot, mtxMeshTrans;				//�v�Z�p�}�g���b�N�X
		D3DXMATRIX mtx;			// ����̃}�g���b�N�X

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtx);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, rot.y, rot.x, rot.z);
		D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxMeshTrans, pos.x, pos.y, pos.z);
		D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshTrans);

		D3DXMatrixMultiply(&mtx, &mtx, &m_pModel[4].GetMtxWorld());

		m_pColHandSphere->SetPosition(D3DXVECTOR3(mtx._41, mtx._42, mtx._43));
	}

	//���̂̃|�C���^��NULL�ł͂Ȃ��Ƃ�
	if (m_pColPlayerSphere != NULL)
	{
		m_pColPlayerSphere->SetPosition(pos);		// ���݈ʒu �̍X�V
	}

	SetPosition(pos);

	if (m_pBox != NULL)
	{
		m_pBox->SetPosition(pos);
	}

	//�L�����N�^�[�̐U�����
	m_difference.y = m_rot.y - m_dest.y;

	//�f�B�t�@�����X�̒l���w�肵���l
	if (m_difference.y > D3DX_PI)
	{
		m_difference.y -= D3DX_PI * 2;
	}
	else if (m_difference.y < -D3DX_PI)
	{
		m_difference.y += D3DX_PI * 2;
	}

	// 1�t���[��������̉�]�ʂ���
	m_rot.y -= m_difference.y * 0.1f;

	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2;
	}
	else if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2;
	}

	// ����o���G�t�F�N�g����
	if (m_AnimationType == ANIMATIONTYPE_SWITCHWEAPON)
	{// �O�̃A�j���[�V����������̐؂�ւ��������Ƃ�
		if (!m_bAnimation)
		{// ���݃A�j���[�V���������Ă��Ȃ������Ƃ�
			m_nActionCount++;				// �J�E���^��+1����

			if (m_nActionCount > 50)
			{// �J�E���^��50�ȏゾ�����Ƃ�
				m_pModel[14].SetActive(true);		// ����̍X�V��������
				m_bAnimation = true;				// �A�j���[�V�����̍ĊJ
				AnimationSwitch(ANIMATIONTYPE_NEUTRAL);	// �ҋ@���[�V�����ɐݒ�
				m_nActionCount = 0;					// �J�E���^�����Z�b�g

				int nNumVertices;					//���_��
				DWORD sizeFVF;						//���_�t�H�[�}�b�g
				BYTE	*pVertexBuffer;				//���_�o�b�t�@�ւ̃|�C���^
				LPD3DXMESH pMesh = m_pModel[14].GetMesh();		// ���b�V�����̎擾

				//���_�����擾
				nNumVertices = pMesh->GetNumVertices();

				//���_�t�H�[�}�b�g�̃T�C�Y���擾
				sizeFVF = D3DXGetFVFVertexSize(pMesh->GetFVF());

				//���_�o�b�t�@�����b�N
				pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertexBuffer);

				// �p�[�e�B�N������
				for (int nCount = 0; nCount < nNumVertices; nCount++)
				{
					D3DXVECTOR3	vtx = *(D3DXVECTOR3*)pVertexBuffer;

					if (nCount % 50 == 0)
					{// 50���1�񐶐�����
						D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						D3DXMATRIX	mtxRot, mtxTrans, mtxView, mtxMeshRot, mtxMeshTrans;				//�v�Z�p�}�g���b�N�X
						D3DXMATRIX mtx;			// ����̃}�g���b�N�X
						D3DXVECTOR3 pos = GetPosition();

						// ���[���h�}�g���b�N�X�̏�����
						D3DXMatrixIdentity(&mtx);

						// ��]�𔽉f
						D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, rot.y, rot.x, rot.z);
						D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshRot);

						// �ʒu�𔽉f
						D3DXMatrixTranslation(&mtxMeshTrans, vtx.x, vtx.y, vtx.z);
						D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshTrans);

						// �}�g���b�N�X���̍���
						D3DXMatrixMultiply(&mtx, &mtx, &m_pModel[14].GetMtxWorld());

						// �G�t�F�N�g�̐���
						CEffect::FallingPetals(false, D3DXVECTOR3(mtx._41, mtx._42, mtx._43), 50);
					}

					pVertexBuffer += sizeFVF;			//�T�C�Y���̃|�C���^��i�߂�
				}

				//���_�o�b�t�@���A�����b�N
				pMesh->UnlockVertexBuffer();
			}
		}
	}

#ifdef _DEBUG
	Debug();
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPlayer::Draw(void)
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
			m_pModel[nCount].Draw(&m_mtxWorld);							//�`��
		}
	}
}

//=============================================================================
// �N���G�C�g����
//=============================================================================
CPlayer *CPlayer::Create(void)
{
	CPlayer *pPlayer;
	pPlayer = new CPlayer(CScene::PRIORITY_PLAYER);
	pPlayer->Init();
	return pPlayer;
}

//=============================================================================
// ���[�h����
//=============================================================================
HRESULT CPlayer::Load(void)
{
	return S_OK;
}

//=============================================================================
// �K�E�Z�|�C���g���̐ݒ�
//=============================================================================
void CPlayer::SetDeathblow(float nValue)
{
	m_fDeathblow = nValue;
}

//=============================================================================
// �����蔻��(trigger)
//=============================================================================
void CPlayer::OnTriggerEnter(CCollider *col)
{
	std::string sTag = col->GetTag();

	if (col->GetScene()->GetObjType() == PRIORITY_ENEMY)
	{
		if (sTag == "weapon")
		{
			CSound *pSound = CManager::GetSound();				// �T�E���h�̎擾
			pSound->PlaySoundA(SOUND_LABEL_SE_PUNCH);			// �_���[�W���̍Đ�
			m_nLife -= 5;										// �̗͂����
			AnimationSwitch(ANIMATIONTYPE_DAMAGE);				// �A�j���[�V������ύX

			D3DXVECTOR3 vec;

			vec = GetPosition() - col->GetPosition();			//���������߂�(���������߂邽��)
			D3DXVec3Normalize(&vec, &vec);						//���K������

			m_move.x = vec.x * 25;
			m_move.z = vec.z * 25;

			if (m_pPlayerUi != NULL)
			{
				CGauge2D *pHitPoint = m_pPlayerUi->GetGauge(GAUGETYPE_HP);
				pHitPoint->SetLifeBar(false, (float)m_nLife / 100.0f);
			}

			if (m_nLife < 0)
			{
				CResult::SetIdxKill(CEnemy::GetEnemyKill());			// Kill�������U���g�ɓn��
				CFade::SetFade(CManager::MODE_RESULT);					// ���U���g�ɑJ��
			}
		}

		if (sTag == "enemy")
		{
			if (m_AnimationType != ANIMATIONTYPE_ATTACK_5)
			{
				D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 40.0f, -80.0f);
				D3DXMATRIX	mtxRot, mtxTrans, mtxView, mtxMeshRot, mtxMeshTrans;				//�v�Z�p�}�g���b�N�X
				D3DXMATRIX mtx;			// ����̃}�g���b�N�X

										// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&mtx);

				// �ʒu�𔽉f
				D3DXMatrixTranslation(&mtxMeshTrans, pos.x, pos.y, pos.z);
				D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshTrans);

				D3DXMatrixMultiply(&mtx, &mtx, &m_pModel[14].GetMtxWorld());

				for (int nCount = 0; nCount < 20; nCount++)
				{
					CEffect::FallingPetals(true, D3DXVECTOR3(mtx._41, mtx._42, mtx._43), 150);
				}
			}
		}
	}
	if (sTag == "house")
	{
		CPurpose *pPurpose = CGame::GetPurpose();				// �ړI�p�Ŕ̎擾

		if (pPurpose->GetPurpose() != MESSAGETYPE_HOUSEGUARD)
		{// ���݂̖ړI��[�h�q]�ł͂Ȃ��Ƃ�
			pPurpose->ChangePurpose(MESSAGETYPE_HOUSEGUARD);	// ���b�Z�[�W�̐ݒ�
			CStage::SetPhase();									// ���b�Z�[�W�̕ύX
		}
	}
}

//=============================================================================
// �����蔻��(collider)
//=============================================================================
void CPlayer::OnCollisionEnter(CCollider *col)
{
	std::string sTag = col->GetTag();
}

//========================================================================================
// �p�[�c�̃��[�h
//========================================================================================
void CPlayer::LoadScript(void)
{
	FILE *pFile;
	char cReadText[128];		//����
	char cHeadText[128];		//��r
	char cDie[128];
	int nCntPointer = 0;			//�|�C���^�[�̐��l

	char sAdd[64];											//���f���̃A�h���X
	int nIndex;
	int nParent;

	int nCntMotion = 0;												//�Q�Ƃ���|�C���^�̒l��������
	int nCntKey = 0;

	int nMaxModel = 0;

	int nCntMaya = 0;				//maya�̃f�[�^�Q��

	//�e�L�X�g�f�[�^���[�h
	pFile = fopen(SCRIPT_PLAYER, "r");

	if (pFile != NULL)
	{
		//�|�C���^�[�̃��Z�b�g
		nCntPointer = 0;

		//�X�N���v�g������܂Ń��[�v
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			fgets(cReadText, sizeof(cReadText), pFile);
			sscanf(cReadText, "%s", &cHeadText);
		}

		//�X�N���v�g��������
		if (strcmp(cHeadText, "SCRIPT") == 0)
		{
			//�G���h�X�N���v�g������܂�
			while (strcmp(cHeadText, "END_SCRIPT") != 0)
			{
				fgets(cReadText, sizeof(cReadText), pFile);
				sscanf(cReadText, "%s", &cHeadText);

				//���s
				if (strcmp(cReadText, "\n") != 0)
				{
					if (strcmp(cHeadText, "NUM_MODEL") == 0)
					{//�p�[�c�����̂Ƃ�
						if (m_pModel == NULL)
						{
							sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nMaxModel);					//�p�[�c�̐����擾

							const int nNumParts = nMaxModel;											//�p�[�c�̐���萔�Ƃ��Ē�`

							if (m_pModel == NULL)
							{//���f�����i�[����ϐ���NULL�������Ƃ�
								m_pModel = new CModel[nNumParts];											//�p�[�c�̑��������������擾����
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
									m_pAnimation[nCount].apKeyInfo = new KEY_INFO[nNumParts];										//�p�[�c�̑��������������擾����
								}
							}

							m_nNumParts = nNumParts;													//�p�[�c�̑������i�[����
						}
					}
					else if (strcmp(cHeadText, "MODEL_FILENAME") == 0)
					{//�p�[�c���f���̃A�h���X���̂Ƃ�
						sscanf(cReadText, "%s %s %s", &cDie, &cDie, &sAdd[0]);						//�A�h���X�̎擾

						if (m_pModel != NULL)
						{
							m_pModel[nCntPointer].Load(sAdd);										//�p�[�c���f���̃��[�h
							m_pModel[nCntPointer].Init();											//�p�[�c���f���̏�����
							nCntPointer++;																//�|�C���^�[��4B�v���X
						}
					}

					else if (strcmp(cHeadText, "CHARACTERSET") == 0)
					{//�L�����N�^�[�̏������̂Ƃ�
						nCntPointer = 0;															//�Q�Ƃ���|�C���^�̒l��������

						//�G���h�L�����N�^�[�Z�b�g������܂Ń��[�v
						while (strcmp(cHeadText, "END_CHARACTERSET") != 0)
						{
							fgets(cReadText, sizeof(cReadText), pFile);
							sscanf(cReadText, "%s", &cHeadText);

							//�p�[�c�Z�b�g��������
							if (strcmp(cHeadText, "PARTSSET") == 0)
							{
								//�G���h�p�[�c�Z�b�g������܂Ń��[�v
								while (strcmp(cHeadText, "END_PARTSSET") != 0)
								{
									fgets(cReadText, sizeof(cReadText), pFile);
									sscanf(cReadText, "%s", &cHeadText);

									//�C���f�b�N�X
									if (strcmp(cHeadText, "INDEX") == 0)
									{
										sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nIndex);			//�C���f�b�N�X�̒l���擾
										m_pModel[nCntPointer].SetIndex(nIndex);							//�C���f�b�N�X�̒l�����ݎQ�ƒ��̃p�[�c���f���Ɋi�[
									}

									//�e
									if (strcmp(cHeadText, "PARENT") == 0)
									{
										sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nParent);			//�e�̒l���擾

										if (nParent == -1)
										{//�e�̒l��-1�������Ƃ�
											m_pModel[nCntPointer].SetParent(NULL);				//NULL���i�[����
										}
										else
										{//�e�̒l��-1�ł͂Ȃ��Ƃ�
											if (m_pModel != NULL)
											{//���f���̒��g��NULL�ł͂Ȃ��Ƃ�
												//���݃��[�h����Ă��郂�f�����̒�����Ώۂ�������܂ŉ�
												for (int nCount = 0; nCount < nMaxModel; nCount++)
												{
													if (nParent == m_pModel[nCount].GetIndex())
													{//�p�[�c���f���̃C���f�b�N�l�Ǝ����̐e�̒l����v�����Ƃ�
														m_pModel[nCntPointer].SetParent(&m_pModel[nCount]);				//�e�̃|�C���^���i�[����
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
					{//�L�����N�^�[�Z�b�g��������

						//�G���h�L�����N�^�[�Z�b�g������܂Ń��[�v
						while (strcmp(cHeadText, "END_MOTIONSET") != 0)
						{
							fgets(cReadText, sizeof(cReadText), pFile);
							sscanf(cReadText, "%s", &cHeadText);

							if (strcmp(cHeadText, "LOOP") == 0)
							{//���[�v���邩�ǂ���
								sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_pAnimation[nCntMotion].nLoop);
							}
							else if (strcmp(cHeadText, "NUM_KEY") == 0)
							{//�L�[��
								sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_pAnimation[nCntMotion].nMaxKey);
							}
							else if (strcmp(cHeadText, "SWITCHFRAME") == 0)
							{// �t���[���Ԃ̈ړ��t���[����
								sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_pAnimation[nCntMotion].nSwitchFrame);
							}

							if (strcmp(cHeadText, "KEYSET") == 0)
							{//�L�[�Z�b�g��������

								//�Q�Ƃ���|�C���^�̒l��������
								nCntPointer = 0;
								//�G���h�L�[�Z�b�g������܂Ń��[�v
								while (strcmp(cHeadText, "END_KEYSET") != 0)
								{
									fgets(cReadText, sizeof(cReadText), pFile);
									sscanf(cReadText, "%s", &cHeadText);

									//�t���[����
									if (strcmp(cHeadText, "FRAME") == 0)
									{
										sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_pAnimation[nCntMotion].apKeyInfo[nCntKey].nFrame);
									}

									//�L�[��������
									if (strcmp(cHeadText, "KEY") == 0)
									{
										//�G���h�L�[������܂Ń��[�v
										while (strcmp(cHeadText, "END_KEY") != 0)
										{
											fgets(cReadText, sizeof(cReadText), pFile);
											sscanf(cReadText, "%s", &cHeadText);

											//�ʒu
											if (strcmp(cHeadText, "POS") == 0)
											{
												sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
													&m_pAnimation[nCntMotion].apKeyInfo[nCntPointer].aKey[nCntKey].pos.x,
													&m_pAnimation[nCntMotion].apKeyInfo[nCntPointer].aKey[nCntKey].pos.y,
													&m_pAnimation[nCntMotion].apKeyInfo[nCntPointer].aKey[nCntKey].pos.z);
											}

											//��]
											if (strcmp(cHeadText, "ROT") == 0)
											{
												sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
													&m_pAnimation[nCntMotion].apKeyInfo[nCntPointer].aKey[nCntKey].rot.x,
													&m_pAnimation[nCntMotion].apKeyInfo[nCntPointer].aKey[nCntKey].rot.y,
													&m_pAnimation[nCntMotion].apKeyInfo[nCntPointer].aKey[nCntKey].rot.z);
											}
										}

										//�p�[�c�̃L�[�����i�[����ꏊ��i�߂�
										nCntPointer++;
									}
								}

								//key�̃J�E���^�[��1�v���X
								nCntKey++;
								nCntPointer = 0;
							}
						}

						//�A�j���[�V�����̎�ނ�1�v���X����
						nCntMotion++;
						nCntKey = 0;
					}
				}
			}
		}

		//�t�@�C����
		fclose(pFile);
	}
	else
	{

	}
}

//=============================================================================
// �߂��ɂ���G�̋߂��܂ňړ����鏈��
//=============================================================================
void CPlayer::MoveNearEnemy(void)
{
	bool bTarget = false;				// �͈͓��ɓG�����݂��Ă����Ƃ�
	float fMinDistance = 750000.0f;		// ���݂̍ŒZ����
	float fDistance = 0.0f;				// ���݂̋���
	D3DXVECTOR3 enemyPos;				// �ŒZ�����ɂ���G�̈ʒu
	CScene *pSceneNext = NULL;														// ������
	CScene *pSceneNow = CScene::GetScene(CScene::PRIORITY_ENEMY);					// �擪�A�h���X�̎擾

																					// �����Ȃ��Ȃ�܂ŌJ��Ԃ�
	while (pSceneNow != NULL)
	{
		pSceneNext = CScene::GetSceneNext(pSceneNow, CScene::PRIORITY_ENEMY);		//����A�b�v�f�[�g�Ώۂ��T����
		fDistance = CManager::GetDistance(GetPosition(), pSceneNow->GetPosition());		// ���������߂�

		if (fDistance < fMinDistance)
		{
			if (!bTarget)
			{// �^�[�Q�b�g�̑��݂��m�F����Ă��Ȃ������Ƃ�
				bTarget = true;				// �ʒu�̈ړ���������
			}

			enemyPos = pSceneNow->GetPosition();			// �G�̈ʒu���L�^����
		}

		pSceneNow = pSceneNext;								//����A�b�v�f�[�g�Ώۂ��i�[
	}

	if (bTarget)
	{// �ړ��̋����o�Ă����Ƃ�
		SetPosition(enemyPos);			// �ʒu�̈ړ�
	}
}

//=============================================================================
// �����蔻�菈��
//=============================================================================
void CPlayer::Collision(void)
{
	switch (m_AnimationType)
	{
	case ANIMATIONTYPE_NEUTRAL:					// �ҋ@���[�V�����̂Ƃ�
		if (m_pColWeaponSphere != NULL)
		{// ����̓����蔻�肪���݂��Ă����Ƃ�
			if (m_pColWeaponSphere->GetUse())
			{// �����蔻��̑Ώۂ������Ƃ�
				m_pColWeaponSphere->SetUse(false);		// �Ώۂ���O��
			}
			if (m_pColHandSphere->GetUse())
			{
				m_pColHandSphere->SetUse(false);
			}
		}
		if (m_pMeshOrbit != NULL)
		{// �O�Ղ����݂��Ă����Ƃ�
			if (m_pMeshOrbit->GetOrbit())
			{
				m_pMeshOrbit->SetOrbit(false);
			}
		}
		break;
	case ANIMATIONTYPE_WALK:						// �������[�V�����̂Ƃ�
		if (m_pColWeaponSphere != NULL)
		{// ����̓����蔻�肪���݂��Ă����Ƃ�
			if (m_pColWeaponSphere->GetUse())
			{// �����蔻��̑Ώۂ������Ƃ�
				m_pColWeaponSphere->SetUse(false);		// �Ώۂ���O��
			}
			if (m_pColHandSphere->GetUse())
			{
				m_pColHandSphere->SetUse(false);
			}
		}
		if (m_pMeshOrbit != NULL)
		{// �O�Ղ����݂��Ă����Ƃ�
			if (m_pMeshOrbit->GetOrbit())
			{
				m_pMeshOrbit->SetOrbit(false);
			}
		}
		break;
	case ANIMATIONTYPE_ATTACK_1:				// �U�����[�V�����̂Ƃ�
		if (m_pColWeaponSphere != NULL)
		{// ����̓����蔻�肪���݂��Ă����Ƃ�
			if (!m_pColWeaponSphere->GetUse())
			{// �����蔻��̑ΏۊO�������Ƃ�
				m_pColWeaponSphere->SetUse(true);		// �Ώۂɂ���
			}
		}
		if (m_pMeshOrbit != NULL)
		{// �O�Ղ����݂��Ă����Ƃ�
			if (!m_pMeshOrbit->GetOrbit())
			{
				m_pMeshOrbit->SetOrbit(true);
			}
		}
		break;
	case ANIMATIONTYPE_ATTACK_2:				// �U�����[�V�����̂Ƃ�
		if (m_pColWeaponSphere != NULL)
		{// ����̓����蔻�肪���݂��Ă����Ƃ�
			if (!m_pColWeaponSphere->GetUse())
			{// �����蔻��̑ΏۊO�������Ƃ�
				m_pColWeaponSphere->SetUse(true);		// �Ώۂɂ���
			}
		}
		if (m_pMeshOrbit != NULL)
		{// �O�Ղ����݂��Ă����Ƃ�
			if (!m_pMeshOrbit->GetOrbit())
			{// �O�Ղ̍X�V����~���Ă����Ƃ�
				m_pMeshOrbit->SetOrbit(true);
			}
		}
		break;
	case ANIMATIONTYPE_ATTACK_3:				// �U�����[�V�����̂Ƃ�
		if (m_pColWeaponSphere != NULL)
		{// ����̓����蔻�肪���݂��Ă����Ƃ�
			if (!m_pColWeaponSphere->GetUse())
			{// �����蔻��̑ΏۊO�������Ƃ�
				m_pColWeaponSphere->SetUse(true);		// �Ώۂɂ���
			}
		}
		if (m_pMeshOrbit != NULL)
		{// �O�Ղ����݂��Ă����Ƃ�
			if (!m_pMeshOrbit->GetOrbit())
			{// �O�Ղ̍X�V����~���Ă����Ƃ�
				m_pMeshOrbit->SetOrbit(true);
			}
		}
	case ANIMATIONTYPE_ATTACK_4:				// �U�����[�V�����̂Ƃ�
		if (m_pColWeaponSphere != NULL)
		{// ����̓����蔻�肪���݂��Ă����Ƃ�
			if (!m_pColWeaponSphere->GetUse())
			{// �����蔻��̑ΏۊO�������Ƃ�
				m_pColWeaponSphere->SetUse(true);		// �Ώۂɂ���
			}
		}
		if (m_pMeshOrbit != NULL)
		{// �O�Ղ����݂��Ă����Ƃ�
			if (!m_pMeshOrbit->GetOrbit())
			{// �O�Ղ̍X�V����~���Ă����Ƃ�
				m_pMeshOrbit->SetOrbit(true);
			}
		}
		break;
	case ANIMATIONTYPE_ATTACK_6:				// �U�����[�V�����̂Ƃ�
		if (m_pColWeaponSphere != NULL)
		{// ����̓����蔻�肪���݂��Ă����Ƃ�
			if (!m_pColWeaponSphere->GetUse())
			{// �����蔻��̑ΏۊO�������Ƃ�
				m_pColWeaponSphere->SetUse(true);		// �Ώۂɂ���
			}
		}
		if (m_pMeshOrbit != NULL)
		{// �O�Ղ����݂��Ă����Ƃ�
			if (!m_pMeshOrbit->GetOrbit())
			{// �O�Ղ̍X�V����~���Ă����Ƃ�
				m_pMeshOrbit->SetOrbit(true);
			}
		}
		break;
	}
}

//=============================================================================
// �L�[���͏�񏈗�
//=============================================================================
void CPlayer::Input(void)
{
	// �L�[�{�[�h�̎擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	// �Q�[���p�b�h�̎擾
	CInputController *pGamepad = CManager::GetInputController();

	// �J�����̎擾
	CCamera *pCamera = CManager::GetCamera();

	// �T�E���h�̎擾
	CSound *pSound = CManager::GetSound();

	D3DXVECTOR3 rot = pCamera->GetRotation();
	D3DXVECTOR3 nor;
	float nValueH = 0;									//�R���g���[���[
	float nValueV = 0;									//�R���g���[���[

	if (m_nCntAttacCombo <= COUNTER_COMBO)
	{// �U���h����t�J�E���^��15�J�E���g�����������Ƃ�
		m_nCntAttacCombo++;				// �J�E���^��1�v���X
	}

	// ====================== �R���g���[���[ ====================== //

	// �U��
	if (m_AnimationType != ANIMATIONTYPE_ATTACK_1 && m_AnimationType != ANIMATIONTYPE_ATTACK_2 &&
		m_AnimationType != ANIMATIONTYPE_ATTACK_3 && m_AnimationType != ANIMATIONTYPE_ATTACK_4 &&
		m_AnimationType != ANIMATIONTYPE_ATTACK_5 && m_AnimationType != ANIMATIONTYPE_ATTACK_6 &&
		m_AnimationType != ANIMATIONTYPE_RUNATTACK && m_AnimationType != ANIMATIONTYPE_DAMAGE &&
		m_AnimationType != ANIMATIONTYPE_SWITCHWEAPON && m_AnimationType != ANIMATIONTYPE_JUMP_1 &&
		m_AnimationType != ANIMATIONTYPE_JUMP_2 && m_AnimationType != ANIMATIONTYPE_JUMP_3 &&
		m_AnimationType != ANIMATIONTYPE_JUMP_4 && m_AnimationType != ANIMATIONTYPE_JUMP_5)
	{
		if (pGamepad != NULL)
		{// �Q�[���p�b�h�����݂����Ƃ�
			if (pGamepad->GetJoypadUse(0))
			{// �g�p�\�������Ƃ�
				if (pGamepad->GetControllerTrigger(0, JOYPADKEY_X))
				{// X�{�^���������ꂽ�Ƃ�
					pSound->PlaySoundA(SOUND_LABEL_SE_SLASHING);

					if (m_nCntAttacCombo < COUNTER_COMBO)
					{
						if (m_AttackTypeOld == ANIMATIONTYPE_ATTACK_1)
						{
							AnimationSwitch(ANIMATIONTYPE_ATTACK_2);								// �A�j���[�V�����̐؂�ւ�
							m_AttackTypeOld = ANIMATIONTYPE_ATTACK_2;
						}
						else if (m_AttackTypeOld == ANIMATIONTYPE_ATTACK_2)
						{
							AnimationSwitch(ANIMATIONTYPE_ATTACK_3);
							m_AttackTypeOld = ANIMATIONTYPE_ATTACK_3;
						}
						else if (m_AttackTypeOld == ANIMATIONTYPE_ATTACK_3)
						{
							AnimationSwitch(ANIMATIONTYPE_ATTACK_4);
							m_AttackTypeOld = ANIMATIONTYPE_ATTACK_4;
						}

						m_nCntAttacCombo = COUNTER_COMBO;										// �J�E���^��t�I��
					}
					else
					{
						AnimationSwitch(ANIMATIONTYPE_ATTACK_1);								// �A�j���[�V�����̐؂�ւ�
						m_AttackTypeOld = ANIMATIONTYPE_ATTACK_1;							// �O�̍U���^�C�v��ݒ�
					}
				}
				if (pGamepad->GetControllerTrigger(0, JOYPADKEY_Y))
				{// Y�{�^���������ꂽ�Ƃ�
					if (m_nCntAttacCombo < COUNTER_COMBO)
					{
						if (m_AttackTypeOld == ANIMATIONTYPE_ATTACK_1)
						{
							AnimationSwitch(ANIMATIONTYPE_ATTACK_6);							// �A�j���[�V�����̐؂�ւ�
							m_AttackTypeOld = ANIMATIONTYPE_ATTACK_6;
						}

						m_nCntAttacCombo = COUNTER_COMBO;										// �J�E���^��t�I��
					}
					else
					{
						AnimationSwitch(ANIMATIONTYPE_ATTACK_5);								// �A�j���[�V�����̐؂�ւ�
						m_AttackTypeOld = ANIMATIONTYPE_ATTACK_5;							// �O�̍U���^�C�v��ݒ�

						if (pSound != NULL)
						{// �T�E���h�����݂��Ă����Ƃ�
							pSound->PlaySoundA(SOUND_LABEL_SE_CHARGE);							// �`���[�W���̍Đ�
						}
					}
				}
				if (pGamepad->GetControllerTrigger(0, JOYPADKEY_A))
				{
					if (!m_bJump)
					{// �W�����v���Ă��Ȃ��Ƃ�
						AnimationSwitch(ANIMATIONTYPE_JUMP_1);		// �W�����v���[�V�����ɐ؂�ւ�
						m_bJump = true;					// �W�����v����
						pSound->PlaySoundA(SOUND_LABEL_SE_JUMP);		// �W�����v���̍Đ�
					}
				}
				if (pGamepad->GetControllerTrigger(0, JOYPADKEY_RIGHT_TRIGGER))
				{
					D3DXVECTOR3 pos = GetPosition();
					pos.y += 100.0f;
					AnimationSwitch(ANIMATIONTYPE_RUNATTACK);
					CEffect::Star(pos);
					CEffect::Halo(pos, D3DXVECTOR3(30.0f, 30.0f, 0.0f));
				}
				if (pGamepad->GetControllerTrigger(0, JOYPADKEY_LEFT_THUMB))
				{// �{�C�̗͊J��
					if (m_state != PLAYERSTATE_FLOWER)
					{// ���݂̃X�e�[�^�X���\����Ԃł͂Ȃ��Ƃ�
						if (m_fDeathblow > 25.0f)
						{
							AnimationSwitch(ANIMATIONTYPE_SWITCHWEAPON);			// �A�j���[�V�����̕ύX
							m_nActionCount = 0;			// �J�E���^�̏�����

							D3DXVECTOR3 cameraRot = m_rot;				// ��]�ʂ��i�[
							cameraRot.y += D3DX_PI;						// ��]�ʂ�3.14�����Z
							pCamera->SetDistance(300.0f);				// ���_�����_�̋�����ݒ�
							pCamera->SetPosCamera(GetPosition(), cameraRot);		// �J�����̏ꏊ��ݒ�
							m_bEvent = true;
						}
					}
				}
			}
		}

		if (pGamepad != NULL)
		{// �Q�[���p�b�h�����݂����Ƃ�
			if (pGamepad->GetJoypadUse(0))
			{// �g�p�\�������Ƃ�
				pGamepad->GetJoypadStickLeft(0, &nValueH, &nValueV);

				//float fRot = fabs(nValueH) + (nValueV);

				//float fAngle = atan2f(nValueH, nValueV);
				//float fDistance = D3DXVec2Length(&D3DXVECTOR2(nValueH, nValueV));

				//�ړ�
				m_move += D3DXVECTOR3(sinf(D3DX_PI * 1.0f + rot.y) * (m_fSpeed * nValueV), 0, cosf(D3DX_PI * 1.0f + rot.y) * (m_fSpeed * nValueV));
				m_move += D3DXVECTOR3(sinf(D3DX_PI * 0.5f + rot.y) * (m_fSpeed * nValueH), 0, cosf(D3DX_PI * 0.5f + rot.y) * (m_fSpeed * nValueH));

				//fDistance /= 1;

				//m_move.x += sinf(fAngle - (rot.y + D3DX_PI)) * -fDistance;
				//m_move.z += cosf(fAngle + (rot.y - D3DX_PI)) * fDistance;


#ifdef _DEBUG
				//CDebugProc::Log("���� : %.2f\n", fAngle);
				//CDebugProc::Log("�����o�Y�̓��� : %.2f\n",(rot.y - D3DX_PI));

				//CDebugProc::Log("�v�Z : %f\n", fDistance);

				CDebugProc::Log("�ړ��� : %.2f %.2f %.2f", m_move.x, m_move.y, m_move.z);

				if (pGamepad->GetControllerPress(0, JOYPADKEY_A))
				{
					CDebugProc::Log("�R���g���[���[ : 0��\n");
				}
				if (pGamepad->GetControllerPress(1, JOYPADKEY_A))
				{
					CDebugProc::Log("�R���g���[���[ : 1��\n");
				}

				CDebugProc::Log("�R���g���[���[H : %f\n", nValueH);
				CDebugProc::Log("�R���g���[���[V : %f\n", nValueV);
#endif
			}
		}

		// ====================== �L�[�{�[�h ====================== //

		if (pKeyboard->GetTriggerKeyboard(DIK_1))
		{
			for (int nCount = 0; nCount < 20; nCount++)
			{
				float fAngle = float(CManager::GetRand(314)) - float(CManager::GetRand(314));
				float fAngle_x = float(CManager::GetRand(314)) - float(CManager::GetRand(314));

				D3DXVECTOR3 particlePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				particlePos.x = cosf(D3DX_PI + fAngle) * cosf(D3DX_PI + fAngle_x);
				particlePos.y = sinf(D3DX_PI + fAngle_x);
				particlePos.z = sinf(D3DX_PI + fAngle) * cosf(D3DX_PI + fAngle_x);

				fAngle = float(CManager::GetRand(314)) / 100.0f - float(CManager::GetRand(314)) / 100.0f;
				D3DXVECTOR3 rot;
				rot = D3DXVECTOR3(sinf(fAngle) * 10, cosf(fAngle) * 10, 0.0f);

				CEffect::SetEffect(EFFECTTYPE_ROSE,										// �p�[�e�B�N���̃^�C�v
					GetPosition(),											// �����ʒu
					D3DXVECTOR3(8.0f, 8.0f, 0.0f),							// �T�C�Y
					particlePos * 5.0f,										// �����x�N�g��
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					EASINGTYPE_NONE,
					rot,													// �e�N�X�`���̉�]
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),						// �J���[
					200,													// �p�[�e�B�N���̐����J�E���g��
					true,													// �d��
					0,														// ��R
					true,													// �r���{�[�h
					0,														// �\������ӏ�(��)
					0,														// �\������ӏ�(�c)
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					0,
					0,
					0);
			}
		}

		if (pKeyboard->GetTriggerKeyboard(DIK_LSHIFT))
		{
			D3DXVECTOR3 pos = GetPosition();
			pos.y += 100.0f;
			AnimationSwitch(ANIMATIONTYPE_RUNATTACK);
			CEffect::Star(pos);
			CEffect::Halo(pos, D3DXVECTOR3(30.0f, 30.0f, 0.0f));
		}

		if (pKeyboard->GetTriggerKeyboard(DIK_X))
		{// X�{�^���������ꂽ�Ƃ�
			switch (m_state)
			{
			case PLAYERSTATE_NORMAL:
				if (m_nCntAttacCombo < COUNTER_COMBO)
				{
					if (m_AttackTypeOld == ANIMATIONTYPE_ATTACK_1)
					{
						AnimationSwitch(ANIMATIONTYPE_ATTACK_2);								// �A�j���[�V�����̐؂�ւ�
						m_AttackTypeOld = ANIMATIONTYPE_ATTACK_2;
					}
					else if (m_AttackTypeOld == ANIMATIONTYPE_ATTACK_2)
					{
						AnimationSwitch(ANIMATIONTYPE_ATTACK_3);
						m_AttackTypeOld = ANIMATIONTYPE_ATTACK_3;
					}
					else if (m_AttackTypeOld == ANIMATIONTYPE_ATTACK_3)
					{
						AnimationSwitch(ANIMATIONTYPE_ATTACK_4);
						m_AttackTypeOld = ANIMATIONTYPE_ATTACK_4;
					}

					m_nCntAttacCombo = COUNTER_COMBO;										// �J�E���^��t�I��
				}
				else
				{
					AnimationSwitch(ANIMATIONTYPE_ATTACK_1);								// �A�j���[�V�����̐؂�ւ�
					m_AttackTypeOld = ANIMATIONTYPE_ATTACK_1;							// �O�̍U���^�C�v��ݒ�
				}
				break;
			case PLAYERSTATE_FLOWER:
				MoveNearEnemy();					// �G�Ƀ��[�v����
				break;
			}
		}
		if (pKeyboard->GetTriggerKeyboard(DIK_C))
		{// C�{�^���������ꂽ�Ƃ�
			if (m_nCntAttacCombo < COUNTER_COMBO)
			{
				if (m_AttackTypeOld == ANIMATIONTYPE_ATTACK_1)
				{
					AnimationSwitch(ANIMATIONTYPE_ATTACK_6);							// �A�j���[�V�����̐؂�ւ�
					m_AttackTypeOld = ANIMATIONTYPE_ATTACK_6;
				}

				m_nCntAttacCombo = COUNTER_COMBO;										// �J�E���^��t�I��
			}
			else
			{
				AnimationSwitch(ANIMATIONTYPE_ATTACK_5);								// �U�����[�V����5�ɐ؂�ւ�
				m_AttackTypeOld = ANIMATIONTYPE_ATTACK_5;							// �O�̍U���^�C�v��ݒ�
			}
		}

		if (pKeyboard->GetTriggerKeyboard(DIK_SPACE))
		{// �X�y�[�X�L�[�������ꂽ�Ƃ�
			if (!m_bJump)
			{
				AnimationSwitch(ANIMATIONTYPE_JUMP_1);		// �W�����v���[�V�����ɐ؂�ւ�
				m_bJump = true;			// �W�����v���Ă���
			}
		}

		//���E����
		if (pKeyboard->GetPressKeyboard(DIK_LEFTARROW))
		{
			if (pKeyboard->GetPressKeyboard(DIK_UPARROW))
			{
				m_move.x += sinf(D3DX_PI * 0.75f + rot.y) * m_fSpeed;
				m_move.z += cosf(D3DX_PI * 0.75f + rot.y) * m_fSpeed;

				D3DXVec3Normalize(&nor, &D3DXVECTOR3(m_move.z, m_move.y, -m_move.x));
				m_dest.y = -D3DX_PI * 0.25f + rot.y;
			}
			else if (pKeyboard->GetPressKeyboard(DIK_DOWNARROW))
			{
				m_move.x += sinf(D3DX_PI * 0.25f + rot.y) * m_fSpeed;
				m_move.z += cosf(D3DX_PI * 0.25f + rot.y) * m_fSpeed;

				D3DXVec3Normalize(&nor, &D3DXVECTOR3(m_move.z, m_move.y, -m_move.x));
				m_dest.y = -D3DX_PI * 0.75f + rot.y;
			}
			else
			{
				m_move.x += sinf(D3DX_PI * 0.5f + rot.y) * m_fSpeed;
				m_move.z += cosf(D3DX_PI * 0.5f + rot.y) * m_fSpeed;

				D3DXVec3Normalize(&nor, &D3DXVECTOR3(m_move.z, m_move.y, -m_move.x));
				m_dest.y = -D3DX_PI * 0.5f + rot.y;
			}
		}

		else if (pKeyboard->GetPressKeyboard(DIK_RIGHTARROW))
		{
			//�㉺����
			if (pKeyboard->GetPressKeyboard(DIK_UPARROW))
			{
				m_move.x += sinf(-D3DX_PI * 0.75f + rot.y) * m_fSpeed;
				m_move.z += cosf(-D3DX_PI * 0.75f + rot.y) * m_fSpeed;

				D3DXVec3Normalize(&nor, &D3DXVECTOR3(m_move.z, m_move.y, -m_move.x));
				m_dest.y = D3DX_PI * 0.25f + rot.y;
			}
			else if (pKeyboard->GetPressKeyboard(DIK_DOWNARROW))
			{
				m_move.x += sinf(-D3DX_PI * 0.25f + rot.y) * m_fSpeed;
				m_move.z += cosf(-D3DX_PI * 0.25f + rot.y) * m_fSpeed;

				D3DXVec3Normalize(&nor, &D3DXVECTOR3(m_move.z, m_move.y, -m_move.x));
				m_dest.y = D3DX_PI * 0.75f + rot.y;
			}
			else
			{
				m_move.x += sinf(-D3DX_PI * 0.5f + rot.y) * m_fSpeed;
				m_move.z += cosf(-D3DX_PI * 0.5f + rot.y) * m_fSpeed;

				D3DXVec3Normalize(&nor, &D3DXVECTOR3(m_move.z, m_move.y, -m_move.x));
				m_dest.y = D3DX_PI * 0.5f + rot.y;
			}
		}

		//�㉺����
		else if (pKeyboard->GetPressKeyboard(DIK_UPARROW))
		{
			m_move.x += sinf(D3DX_PI * 1.0f + rot.y) * m_fSpeed;
			m_move.z += cosf(D3DX_PI * 1.0f + rot.y) * m_fSpeed;

			D3DXVec3Normalize(&nor, &D3DXVECTOR3(m_move.z, m_move.y, -m_move.x));
			m_dest.y = D3DX_PI * 0.0f + rot.y;
		}
		else if (pKeyboard->GetPressKeyboard(DIK_DOWNARROW))
		{
			D3DXVec3Normalize(&nor, &D3DXVECTOR3(m_move.z, m_move.y, -m_move.x));

			m_move.x += sinf(D3DX_PI * 0.0f + rot.y) * m_fSpeed;
			m_move.z += cosf(D3DX_PI * 0.0f + rot.y) * m_fSpeed;
			m_dest.y = D3DX_PI * 1.0f + rot.y;
		}
	}

	if (pGamepad != NULL)
	{// �Q�[���p�b�h�����݂����Ƃ�
		if (pGamepad->GetJoypadUse(0))
		{// �g�p�\�������Ƃ�
			// ���ߍU��
			if (pGamepad->GetControllerRelease(0, JOYPADKEY_Y))
			{
				if (m_AnimationType == ANIMATIONTYPE_ATTACK_5)
				{
					if (!m_bAnimation)
					{// �A�j���[�V�������~�܂��Ă���Ƃ�
						if (m_pColHandSphere != NULL)
						{// ����̓����蔻�肪���݂��Ă����Ƃ�
							if (!m_pColHandSphere->GetUse())
							{// �����蔻��̑ΏۊO�������Ƃ�
								m_pColHandSphere->SetUse(true);		// �Ώۂɂ���
							}
						}

						m_nCurrentKey = 1;				// ���݂̃L�[����[1]�ɐݒ�
						m_nCurrentFrame = 0;			// ���݂̃t���[������[0]�ɐݒ�
						m_bAnimation = true;			// �A�j���[�V�������ĊJ
						pSound->PlaySoundA(SOUND_LABEL_SE_BOMB);	// �������̍Đ�
					}
					else
					{
						AnimationSwitch(ANIMATIONTYPE_NEUTRAL);
					}
				}
			}
			else if (pGamepad->GetControllerPress(0, JOYPADKEY_Y))
			{// Y�{�^����������Ă���Ƃ�
				if (m_AnimationType == ANIMATIONTYPE_ATTACK_5)
				{// ���ݍU��5�̍Œ��������Ƃ�
					if (m_bAnimation)
					{// �A�j���[�V�������X�V���������Ƃ�
						D3DXMATRIX mtx = m_pModel[4].GetMtxWorld();				// ���f���̃}�g���b�N�X�擾
						CEffect::Halo(D3DXVECTOR3(mtx._41, mtx._42, mtx._43), D3DXVECTOR3(15.0f, 15.0f, 0.0f));	// ���f���̃}�g���b�N�X��񂩂�ʒu����n��
					}
				}
			}
		}
	}

	if (pKeyboard->GetReleaseKeyboard(DIK_C))
	{
		if (m_AnimationType == ANIMATIONTYPE_ATTACK_5)
		{
			if (!m_bAnimation)
			{// �A�j���[�V�������~�܂��Ă���Ƃ�
				if (m_pColHandSphere != NULL)
				{// ����̓����蔻�肪���݂��Ă����Ƃ�
					if (!m_pColHandSphere->GetUse())
					{// �����蔻��̑ΏۊO�������Ƃ�
						m_pColHandSphere->SetUse(true);		// �Ώۂɂ���
					}
				}

				m_nCurrentKey = 1;				// ���݂̃L�[����[1]�ɐݒ�
				m_nCurrentFrame = 0;			// ���݂̃t���[������[0]�ɐݒ�
				m_bAnimation = true;			// �A�j���[�V�������ĊJ
			}
			else
			{
				AnimationSwitch(ANIMATIONTYPE_NEUTRAL);
			}
		}
	}
	if (pKeyboard->GetPressKeyboard(DIK_C))
	{
		if (m_AnimationType == ANIMATIONTYPE_ATTACK_5)
		{
			if (m_pColHandSphere != NULL)
			{// ����̓����蔻�肪���݂��Ă����Ƃ�
				if (m_bAnimation)
				{
					D3DXMATRIX mtx = m_pModel[4].GetMtxWorld();
					CEffect::Halo(D3DXVECTOR3(mtx._41, mtx._42, mtx._43), D3DXVECTOR3(15.0f, 15.0f, 0.0f));
				}
			}
		}
	}

#ifdef _DEBUG
	if (pKeyboard->GetTriggerKeyboard(DIK_K))
	{// K�������ꂽ�Ƃ�
		m_fDeathblow = 50.0f;				// �K�E�Z�|�C���g���ő�l�܂ŏグ��
	}
	if (pKeyboard->GetTriggerKeyboard(DIK_F8))
	{// F8�������ꂽ�Ƃ�
		switch (m_state)
		{
		case PLAYERSTATE_NORMAL:
			m_state = PLAYERSTATE_FLOWER;
			m_fSpeed = SP_SPEED;
			break;
		case PLAYERSTATE_FLOWER:
			m_state = PLAYERSTATE_NORMAL;
			m_fSpeed = NORMAL_SPEED;
			break;
		}
	}
	if (pKeyboard->GetTriggerKeyboard(DIK_L))
	{
		AnimationSwitch(ANIMATIONTYPE_SWITCHWEAPON);
	}
#endif
}

//=============================================================================
// �A�j���[�V��������
//=============================================================================
void CPlayer::Animation(void)
{
	//�ŏ��̃t���[���̎�
	if (m_nCurrentFrame == 0)
	{
		for (int nCount = 0; nCount < m_nNumParts; nCount++)
		{
			D3DXVECTOR3 pos = m_pModel[nCount].GetPosition();
			D3DXVECTOR3 rot = m_pModel[nCount].GetRotation();

			if (m_bAnimSwitch)
			{
				//pos�̌v�Z((�ڕW��key + �v���Z�b�g�̔z�u) - ���݂̃L�[)
				m_apCurrentPos[nCount] = ((m_pAnimation[m_AnimationType].apKeyInfo[nCount].aKey[m_nCurrentKey].pos + m_pModel[nCount].GetPosPreset()) - pos) / 0.1f * (float)m_pAnimation[m_AnimationType].nSwitchFrame;

				//rot�̌v�Z((�ڕW��key + �v���Z�b�g�̔z�u) - ���݂̃L�[)
				m_apCurrentRot[nCount] = ((m_pAnimation[m_AnimationType].apKeyInfo[nCount].aKey[m_nCurrentKey].rot + m_pModel[nCount].GetRotPreset()) - rot) / 0.1f * (float)m_pAnimation[m_AnimationType].nSwitchFrame;
			}
			else
			{
				//pos�̌v�Z((�ڕW��key + �v���Z�b�g�̔z�u) - ���݂̃L�[)
				m_apCurrentPos[nCount] = ((m_pAnimation[m_AnimationType].apKeyInfo[nCount].aKey[m_nCurrentKey].pos + m_pModel[nCount].GetPosPreset()) - pos) / (float)m_pAnimation[m_AnimationType].apKeyInfo[m_nCurrentKey].nFrame;

				//rot�̌v�Z((�ڕW��key + �v���Z�b�g�̔z�u) - ���݂̃L�[)
				m_apCurrentRot[nCount] = ((m_pAnimation[m_AnimationType].apKeyInfo[nCount].aKey[m_nCurrentKey].rot + m_pModel[nCount].GetRotPreset()) - rot) / (float)m_pAnimation[m_AnimationType].apKeyInfo[m_nCurrentKey].nFrame;
			}
		}
	}
	else
	{//����ȊO�̃t���[��
		for (int nCount = 0; nCount < m_nNumParts; nCount++)
		{
			D3DXVECTOR3 pos = m_pModel[nCount].GetPosition();
			D3DXVECTOR3 rot = m_pModel[nCount].GetRotation();

			//rot�̈ړ�
			m_pModel[nCount].SetRotation(rot + m_apCurrentRot[nCount]);

			//pos�̈ړ�
			m_pModel[nCount].SetPosition(pos + m_apCurrentPos[nCount]);
		}
	}

	if (m_state == PLAYERSTATE_NORMAL)
	{
		if (m_AnimationType == ANIMATIONTYPE_SWITCHWEAPON)
		{
			if (m_state == PLAYERSTATE_NORMAL)
				if (m_nCurrentKey == 0)
				{
					CCamera *pCamera = CManager::GetCamera();
					float fDistance = pCamera->GetDistance();
					fDistance -= 10;
					pCamera->SetDistance(fDistance);
				}
		}
	}

	if (m_pAnimation[m_AnimationType].apKeyInfo != NULL)
	{// �L�[���NULL�ł͂Ȃ��Ƃ�
		//�t���[���̍ő吔�ɖ����Ȃ��ꍇ
		if (m_nCurrentFrame < m_pAnimation[m_AnimationType].apKeyInfo[m_nCurrentKey].nFrame)
		{
			m_nCurrentFrame++;
		}
		//�t���[�����̍ő�l�ɒB�����ꍇ
		else if (m_nCurrentFrame >= m_pAnimation[m_AnimationType].apKeyInfo[m_nCurrentKey].nFrame)
		{
			if (m_AnimationType == ANIMATIONTYPE_ATTACK_4)
			{// ���[�V�������U��4�������Ƃ�
				if (m_nCurrentKey == 4)
				{// ���݂̃L�[����4�������Ƃ�
					D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 40.0f, -80.0f);
					D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					D3DXMATRIX	mtxRot, mtxTrans, mtxView, mtxMeshRot, mtxMeshTrans;				//�v�Z�p�}�g���b�N�X
					D3DXMATRIX mtx;			// ����̃}�g���b�N�X

					// ���[���h�}�g���b�N�X�̏�����
					D3DXMatrixIdentity(&mtx);

					// ��]�𔽉f
					D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, rot.y, rot.x, rot.z);
					D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshRot);

					// �ʒu�𔽉f
					D3DXMatrixTranslation(&mtxMeshTrans, pos.x, pos.y, pos.z);
					D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshTrans);

					D3DXMatrixMultiply(&mtx, &mtx, &m_pModel[14].GetMtxWorld());

					CEffect::SandSmokeExplosion(D3DXVECTOR3(mtx._41, mtx._42, mtx._43));
					CSound *pSound = CManager::GetSound();			// �T�E���h�̎擾
					pSound->PlaySoundA(SOUND_LABEL_SE_BOMB);		// �������̍Đ�

					m_move.y += 20;			// ������ɗ͂�������
					m_bJump = true;			// �W�����v���ɐݒ�
				}
			}
			else if (m_AnimationType == ANIMATIONTYPE_ATTACK_5)
			{// ���[�V�������U��5�������Ƃ�
				if (m_nCurrentKey == 0)
				{// ���݂̃L�[����0�������Ƃ�
					m_bAnimation = false;
				}
				else if (m_nCurrentKey == 1)
				{
					D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					D3DXMATRIX	mtxRot, mtxTrans, mtxView, mtxMeshRot, mtxMeshTrans;				//�v�Z�p�}�g���b�N�X
					D3DXMATRIX mtx;			// ����̃}�g���b�N�X

					// ���[���h�}�g���b�N�X�̏�����
					D3DXMatrixIdentity(&mtx);

					// ��]�𔽉f
					D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, rot.y, rot.x, rot.z);
					D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshRot);

					// �ʒu�𔽉f
					D3DXMatrixTranslation(&mtxMeshTrans, 0.0f, 0.0f, 0.0f);
					D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshTrans);

					D3DXMatrixMultiply(&mtx, &mtx, &m_pModel[4].GetMtxWorld());

					CEffect::Shockwave(D3DXVECTOR3(mtx._41, mtx._42, mtx._43),
						m_rot,
						15);
				}
			}
			else if (m_AnimationType == ANIMATIONTYPE_JUMP_1)
			{// �W�����v_1�������Ƃ�
				if (m_nCurrentKey == 1)
				{
					m_move.y += JUMP_MAX;				// ������ɗ͂�������
				}
			}
			m_nCurrentKey++;			// �L�[�̒l��1�v���X
			m_nCurrentFrame = 0;		// �t���[���̃��Z�b�g

			if (m_AnimationType == ANIMATIONTYPE_RUNATTACK)
			{
				if (m_nCurrentKey == 1)
				{
					if (m_nCurrentFrame == 0)
					{

						// �J���� �̎擾
						CCamera *pCamera = CManager::GetCamera();
						D3DXVECTOR3 rot = pCamera->GetRotation();

						if (m_bJump)
						{
							m_move.x += sinf(D3DX_PI * 1.0f + m_rot.y) * 50.0f;
							m_move.z += cosf(D3DX_PI * 1.0f + m_rot.y) * 50.0f;
						}
						else
						{
							m_move.x += sinf(D3DX_PI * 1.0f + m_rot.y) * 25.0f;
							m_move.z += cosf(D3DX_PI * 1.0f + m_rot.y) * 25.0f;
						}

						m_dest.y = D3DX_PI * 0.0f + rot.y;
					}
				}
			}
		}
	}

	//�L�[���̍ő�l�ɒB�����ꍇ
	if (m_nCurrentKey >= m_pAnimation[m_AnimationType].nMaxKey)
	{
		D3DXVECTOR3 rot = m_pModel[0].GetRotation();		// ��]�ʎ擾

		switch (m_AnimationType)
		{
		case ANIMATIONTYPE_ATTACK_3:
			rot.y = 1.5f;										// Y���̉�]�ʕύX
			m_pModel[0].SetRotation(rot);						// ��]�ʂ̐ݒ�
			break;
		case ANIMATIONTYPE_ATTACK_4:
			rot.x = 0.0f;										// X���̉�]�ʕύX
			m_pModel[0].SetRotation(rot);						// ��]�ʂ̐ݒ�
			break;
		case ANIMATIONTYPE_RUNATTACK:
			rot.x = 0.72f;										// X���̉�]�ʕύX
			m_pModel[0].SetRotation(rot);						// ��]�ʂ̐ݒ�
			break;
		case ANIMATIONTYPE_JUMP_1:
			AnimationSwitch(ANIMATIONTYPE_JUMP_2);
			break;
		case ANIMATIONTYPE_JUMP_2:
			AnimationSwitch(ANIMATIONTYPE_JUMP_3);
			break;
		case ANIMATIONTYPE_JUMP_3:
			AnimationSwitch(ANIMATIONTYPE_JUMP_4);
			break;
		case ANIMATIONTYPE_JUMP_4:
			m_bAnimation = false;
			break;
		case ANIMATIONTYPE_SWITCHWEAPON:
			int nNumVertices;			//���_��
			DWORD sizeFVF;				//���_�t�H�[�}�b�g
			BYTE	*pVertexBuffer;		//���_�o�b�t�@�ւ̃|�C���^
			LPD3DXMESH pMesh = m_pModel[14].GetMesh();

			//���_�����擾
			nNumVertices = pMesh->GetNumVertices();

			//���_�t�H�[�}�b�g�̃T�C�Y���擾
			sizeFVF = D3DXGetFVFVertexSize(pMesh->GetFVF());

			//���_�o�b�t�@�����b�N
			pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertexBuffer);

			// �p�[�e�B�N������
			for (int nCount = 0; nCount < nNumVertices; nCount++)
			{
				D3DXVECTOR3	vtx = *(D3DXVECTOR3*)pVertexBuffer;

				if (nCount % 50 == 0)
				{// 50���1�񐶐�����
					D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					D3DXMATRIX	mtxRot, mtxTrans, mtxView, mtxMeshRot, mtxMeshTrans;				//�v�Z�p�}�g���b�N�X
					D3DXMATRIX mtx;			// ����̃}�g���b�N�X
					D3DXVECTOR3 pos = GetPosition();

					// ���[���h�}�g���b�N�X�̏�����
					D3DXMatrixIdentity(&mtx);

					// ��]�𔽉f
					D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, rot.y, rot.x, rot.z);
					D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshRot);

					// �ʒu�𔽉f
					D3DXMatrixTranslation(&mtxMeshTrans, vtx.x, vtx.y, vtx.z);
					D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshTrans);

					D3DXMatrixMultiply(&mtx, &mtx, &m_pModel[14].GetMtxWorld());

					switch (m_state)
					{
					case PLAYERSTATE_NORMAL:
						CEffect::FallingPetals(false, D3DXVECTOR3(mtx._41, mtx._42, mtx._43), 50);
						break;
					case PLAYERSTATE_FLOWER:
						CEffect::PetalsGather(D3DXVECTOR3(mtx._41, mtx._42, mtx._43));
						break;
					}
				}

				pVertexBuffer += sizeFVF;			//�T�C�Y���̃|�C���^��i�߂�
			}

			//���_�o�b�t�@���A�����b�N
			pMesh->UnlockVertexBuffer();

			CCamera *pCamera = CManager::GetCamera();

			switch (m_state)
			{
			case PLAYERSTATE_NORMAL:
				pCamera->SetDistance(350.0f);			// ���_�ƒ����_�̋�����ς���
				m_pModel[14].SetActive(false);			// ����̕`����~
				m_state = PLAYERSTATE_FLOWER;			// �X�e�[�^�X��\���ɕύX
				m_fSpeed = SP_SPEED;					// �X�s�[�h��ς���
				m_nActionCount = 0;						// �J�E���^�̏�����
				break;
			case PLAYERSTATE_FLOWER:
				m_state = PLAYERSTATE_NORMAL;			// �X�e�[�^�X��ʏ�ɖ߂�
				m_fSpeed = NORMAL_SPEED;				// �X�s�[�h��ʏ�ɖ߂�
				m_bAnimation = false;					// �A�j���[�V�����̒�~
				m_bEvent = false;						// �C�x���g�t���O������
				return;
				break;
			}
			break;
		}

		switch (m_state)
		{
		case PLAYERSTATE_NORMAL:
			//�U���̔h���J�E���^�̃��Z�b�g
			if (m_AnimationType == ANIMATIONTYPE_ATTACK_1 || m_AnimationType == ANIMATIONTYPE_ATTACK_2 ||
				m_AnimationType == ANIMATIONTYPE_ATTACK_3)
			{// ���[�V�������U�����[�V�����������Ƃ�
				m_nCntAttacCombo = 0;
			}
			break;
		case PLAYERSTATE_FLOWER:
			//�U���̔h���J�E���^�̃��Z�b�g
			if (m_AnimationType == ANIMATIONTYPE_ATTACK_1)
			{// ���[�V�������U�����[�V�����������Ƃ�
				m_nCntAttacCombo = 0;
			}
			break;
		}

		if (m_AnimationType != ANIMATIONTYPE_JUMP_2 && m_AnimationType != ANIMATIONTYPE_JUMP_3 && m_AnimationType != ANIMATIONTYPE_JUMP_4)
		{
			//���[�v���邩�ǂ���
			if (m_pAnimation[m_AnimationType].nLoop)
			{
				//�L�[�̃��Z�b�g
				m_nCurrentKey = 0;
				m_nCurrentFrame = 0;
			}
			else
			{
				//�j���[�g�������[�V����
				AnimationSwitch(ANIMATIONTYPE_NEUTRAL);
			}
		}
	}

	if (m_AnimationType != ANIMATIONTYPE_ATTACK_1 && m_AnimationType != ANIMATIONTYPE_ATTACK_2 &&
		m_AnimationType != ANIMATIONTYPE_ATTACK_3 && m_AnimationType != ANIMATIONTYPE_ATTACK_4 &&
		m_AnimationType != ANIMATIONTYPE_ATTACK_5 && m_AnimationType != ANIMATIONTYPE_ATTACK_6 &&
		m_AnimationType != ANIMATIONTYPE_RUNATTACK && m_AnimationType != ANIMATIONTYPE_DAMAGE &&
		m_AnimationType != ANIMATIONTYPE_SWITCHWEAPON && m_AnimationType != ANIMATIONTYPE_JUMP_1 &&
		m_AnimationType != ANIMATIONTYPE_JUMP_2 && m_AnimationType != ANIMATIONTYPE_JUMP_3 &&
		m_AnimationType != ANIMATIONTYPE_JUMP_4 && m_AnimationType != ANIMATIONTYPE_JUMP_5)
	{
		if (fabs(m_move.x) > 2 || fabs(m_move.z) > 2)
		{
			switch (m_state)
			{
			case PLAYERSTATE_NORMAL:
				if (m_AnimationType != ANIMATIONTYPE_WALK)
				{
					AnimationSwitch(ANIMATIONTYPE_WALK);
				}
				break;
			case PLAYERSTATE_FLOWER:
				if (m_AnimationType != ANIMATIONTYPE_RUN)
				{
					AnimationSwitch(ANIMATIONTYPE_RUN);
				}
			}
		}
		else
		{
			if (m_AnimationType != ANIMATIONTYPE_NEUTRAL)
			{
				AnimationSwitch(ANIMATIONTYPE_NEUTRAL);
			}
		}
	}

	if (m_AnimationType == ANIMATIONTYPE_ATTACK_1 || m_AnimationType == ANIMATIONTYPE_ATTACK_2)
	{
		if (m_nCurrentKey == 1)
		{
			if (m_nCurrentFrame == 0)
			{// ���ɂ��U�����̉��S�͂ɂ��ړ�����
				m_move.x += sinf(D3DX_PI * 1.0f + m_rot.y) * 10.0f;
				m_move.z += cosf(D3DX_PI * 1.0f + m_rot.y) * 10.0f;
			}
		}
	}
	else if (m_AnimationType == ANIMATIONTYPE_ATTACK_4)
	{
		if (m_nCurrentKey == 5)
		{
			if (m_nCurrentFrame == 0)
			{// ���̍U���ɂ��Ռ��g�Ō��ɔ�΂�����
				m_move.x += sinf(D3DX_PI * 0.0f + m_rot.y) * 15.0f;
				m_move.z += cosf(D3DX_PI * 0.0f + m_rot.y) * 15.0f;
			}
		}
	}
}

//=============================================================================
// �A�j���[�V�����̐؂�ւ�
//=============================================================================
void CPlayer::AnimationSwitch(ANIMATIONTYPE Type)
{
	m_AnimationTypeOld = m_AnimationType;								// �O�̃A�j���[�V�����ɍ��̃A�j���[�V����������
	m_AnimationType = Type;												// �A�j���[�V�����̐؂�ւ�
	m_nCurrentFrame = 0;												// ���݂̃t���[���������Z�b�g
	m_nCurrentKey = 0;													// ���݂̃L�[�������Z�b�g

	if (m_pMeshOrbit != NULL)
	{// ���g������Ƃ�
		if (m_AnimationType == ANIMATIONTYPE_ATTACK_1 || m_AnimationType == ANIMATIONTYPE_ATTACK_2)
		{// �U�����[�V�����������Ƃ�
			if (!m_pMeshOrbit->GetOrbit())
			{// �O�Ղ��`�悳��Ă��Ȃ��Ƃ�
				m_pMeshOrbit->SetOrbit(true);
			}
		}
		else
		{// �w�肵�����[�V�����ȊO�������Ƃ�
			if (m_pMeshOrbit->GetOrbit())
			{// �O�Ղ��`�悳��Ă�����
				m_pMeshOrbit->SetOrbit(false);
			}
		}
	}

	if (m_AnimationType == ANIMATIONTYPE_ATTACK_1 || m_AnimationType == ANIMATIONTYPE_ATTACK_2 ||
		m_AnimationType == ANIMATIONTYPE_ATTACK_3 || m_AnimationType == ANIMATIONTYPE_ATTACK_4)
	{
		CEffect::SandSmokeEffect(GetPosition());
	}
}

#ifdef _DEBUG
//=============================================================================
// �f�o�b�O����
//=============================================================================
void CPlayer::Debug(void)
{
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 posOld = GetPosOld();
	ImGui::Begin("System");													// �uSystem�v�E�B���h�E�ɒǉ����܂��B�Ȃ���΍쐬���܂��B

	if (ImGui::CollapsingHeader("player"))
	{
		ImGui::Text("pos = %.2f, %.2f, %.2f", pos.x, pos.y, pos.z);								// �v���C���[�̌��݈ʒu��\��
		ImGui::Text("posOld = %.2f, %.2f, %.2f", posOld.x, posOld.y, posOld.z);								// �v���C���[�̌��݈ʒu��\��
		ImGui::Text("move = %.2f, %.2f, %.2f", m_move.x, m_move.y, m_move.z);								// �v���C���[�̌��݈ʒu��\��
		ImGui::Text("HP = %d", m_nLife);				// �v���C���[�̗̑͂�\��

		switch (m_state)
		{
		case PLAYERSTATE_NORMAL:
			ImGui::Text("normal");
			break;
		case PLAYERSTATE_FLOWER:
			ImGui::Text("flower");
			break;
		}

		if (ImGui::Button("BOSS"))
		{
			pos = D3DXVECTOR3(5972.14f, 100.0f, 8000.62f);
			SetPosition(pos);
		}

		if (ImGui::TreeNode("model"))
		{//�v���C���[�̃c���[�m�[�h��T���܂��B�Ȃ���΍쐬���܂��B
			for (int nCount = 0; nCount < m_nNumParts; nCount++)
			{
				if (m_pModel != NULL)
				{
					D3DXVECTOR3 pos = m_pModel[nCount].GetPosition();													// �ʒu���擾
					ImGui::Text("pos%d = %.2f, %.2f, %.2f", nCount, pos.x, pos.y, pos.z);								// �v���C���[�̌��݈ʒu��\��
				}
			}

			ImGui::TreePop();																		// �I��
		}
		if (ImGui::TreeNode("animation"))
		{//�A�j���[�V�����̃c���[�m�[�h��T���܂��B�Ȃ���΍쐬���܂��B
			ImGui::Text("frame = %d", m_nCurrentFrame);
			ImGui::Text("Key = %d", m_nCurrentKey);
			ImGui::Text("animationType = %d", m_AnimationType);

			ImGui::TreePop();																		// �I��
}
	}

	//�f�o�b�O�������I��
	ImGui::End();
}
#endif