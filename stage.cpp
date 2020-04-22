//=============================================================================
//
// �X�e�[�W�Ǘ����� [stage.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "stage.h"
#include "manager.h"
#include "renderer.h"
#include "colliderBox.h"
#include "meshField.h"
#include "object.h"
#include "player.h"
#include "game.h"
#include "enemy.h"
#include "camera.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9		CStage::m_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 CStage::m_pVtxBuff = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
CStage::FADE			CStage::m_fade = CStage::FADE_NONE;					// �t�F�[�h���
D3DXCOLOR				CStage::m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);				// �t�F�[�h�F
D3DXVECTOR3				CStage::m_playerPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �v���C���[�̗����Ă����ꏊ
bool					CStage::m_bEvent = false;								// �C�x���g���ǂ���
CEnemy					*CStage::m_apEnemy[MAX_ENEMY] = {};

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CStage::CStage()
{
	// �l�̏�����
	m_bEvent = false;
	m_playerPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CStage::~CStage()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CStage::Init(void)
{
	CRenderer *Renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

	m_fade = FADE_NONE;											// �t�F�[�h�^�C�v�̏�����
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);					// ������ʁi�s�����j

	// ���_���̍X�V
	MakeVertexFade(pDevice);
	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CStage::Uninit(void)
{
	// �e�N�X�`���̊J��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	// ���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CStage::Update(void)
{
	VERTEX_2D *pVtx;

	if (m_fade != FADE_NONE)
	{
		if (m_fade == FADE_IN)
		{
			m_col.a -= 0.05f;				//��ʂ𓧖��ɂ��Ă���
			if (m_col.a <= 0.0f)
			{
				m_col.a = 0.0f;
				m_fade = FADE_NONE;
			}
		}
		else if (m_fade == FADE_OUT)
		{
			m_col.a += 0.05f;
			if (m_col.a > 1.5f)
			{
				//�t�F�[�h�C�������ɐ؂�ւ�
				m_col.a = 1.0f;
				m_fade = FADE_IN;

				//���[�h�ؑ�
				ChengeStage();
			}
		}
	}

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// ���_�f�[�^���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CStage::Draw(void)
{
	CRenderer *Renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// �N���G�C�g����
//=============================================================================
CStage *CStage::Create(void)
{
	CStage *pStage;
	pStage = new CStage();
	pStage->Init();
	return pStage;
}

//=============================================================================
// �G�̓ǂݍ���(phase)
//=============================================================================
void CStage::LoadPhase(char *add)
{
	FILE *pFile = NULL;																	// �t�@�C��
	char cReadText[128] = {};															// ����
	char cHeadText[128] = {};															// ��r
	float fData[3];																	// ����

	pFile = fopen(add, "r");									// �t�@�C�����J���܂��͍��

	if (pFile != NULL)																// �t�@�C�����ǂݍ��߂��ꍇ
	{
		fgets(cReadText, sizeof(cReadText), pFile);										// �s���΂�
		fgets(cReadText, sizeof(cReadText), pFile);										// �s���΂�
		fgets(cReadText, sizeof(cReadText), pFile);										// �s���΂�

		for (int nCount = 0; nCount < MAX_ENEMY; nCount++)
		{
			CManager::ConvertStringToFloat(cReadText, ",", fData);

			m_apEnemy[nCount] = CEnemy::Create();

			if (m_apEnemy[nCount] != NULL)
			{
				m_apEnemy[nCount]->SetPosition(D3DXVECTOR3(fData[0], fData[1], fData[2]));			// �|�W�V���������߂�
				m_apEnemy[nCount]->SetActive(false);
			}

			fgets(cReadText, sizeof(cReadText), pFile);									// �s���΂�
			sscanf(cReadText, "%s", &cHeadText);
		}

		fclose(pFile);																// �t�@�C�������
	}
	else
	{
		MessageBox(NULL, "���f�����̃A�N�Z�X���s�I", "WARNING", MB_ICONWARNING);	// ���b�Z�[�W�{�b�N�X�̐���
	}
}

//=============================================================================
// �t�F�[�h�̏�Ԑݒ�
//=============================================================================
void CStage::SetStage(void)
{
	m_fade = FADE_OUT;
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);		//�������(����)
}

//=============================================================================
// �t�F�[�Y�̊J�n����
//=============================================================================
void CStage::SetPhase(void)
{
	for (int nCount = 0; nCount < MAX_ENEMY; nCount++)
	{
		if (m_apEnemy[nCount] != NULL)
		{
			m_apEnemy[nCount]->SetActive(true);
			m_apEnemy[nCount]->SetTarget(TARGETTYPE_HOUSE);
		}
	}
}

//=============================================================================
// �X�e�[�W�̕ύX����
//=============================================================================
void CStage::ChengeStage(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	CPlayer *pPlayer = CGame::GetPlayer();
	CCamera *pCamera = CManager::GetCamera();

	if (m_bEvent)
	{
		pPlayer->SetPosition(m_playerPos);

		if (pCamera != NULL)
		{
			pCamera->SetPosCamera(m_playerPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
		m_bEvent = false;
	}
	else
	{
		m_playerPos = pPlayer->GetPosition();
		pPlayer->SetPosition(D3DXVECTOR3(540.88f, 165.00f, 4942.73f));

		if (pCamera != NULL)
		{
			pCamera->SetPosCamera(D3DXVECTOR3(540.88f, 165.00f, 4942.73f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
		m_bEvent = true;
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void CStage::MakeVertexFade(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�f�[�^���A�����b�N����
	m_pVtxBuff->Unlock();
}