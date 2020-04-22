//=============================================================================
//
// �^�C�g�����S���� [titleLogo.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "titleLogo.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CTitlelogo::CTitlelogo()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTitlelogo::~CTitlelogo()
{

}

//==============================================================================
// ����������
//==============================================================================
HRESULT CTitlelogo::Init(void)
{
	m_apScene2D[LOGOTYPE_IF] = CScene2D::Create(CScene2D::PRIORITY_UI);		// �w�i �̐���

	if (m_apScene2D[LOGOTYPE_IF] != NULL)
	{// ���݂��Ă����Ƃ�
		m_apScene2D[LOGOTYPE_IF]->BindTexture(CManager::GetResource("data/tex/if.png"));			// �w�i�e�N�X�`�� �̃|�C���^ ��n��
		m_apScene2D[LOGOTYPE_IF]->SetCenter(CScene2D::TEXTUREVTX_CENTER);		// ���S �̐ݒ�
		m_apScene2D[LOGOTYPE_IF]->SetSize(D3DXVECTOR3(100.0f, 70.0f, 0.0f));	// �傫�� �̐ݒ�
		m_apScene2D[LOGOTYPE_IF]->SetRotation(D3DXVECTOR3(-0.79f, -0.79f, 0.0f));
		m_apScene2D[LOGOTYPE_IF]->SetPosition(D3DXVECTOR3(320.0f, 150.0f, 0.0f));	// �ʒu �̐ݒ�
		m_apScene2D[LOGOTYPE_IF]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// �F �̐ݒ�
		m_apScene2D[LOGOTYPE_IF]->SetTransform();		// ���_��� �̐ݒ�
	}

	m_apScene2D[LOGOTYPE_AKAZUKIN] = CScene2D::Create(CScene2D::PRIORITY_UI);		// �w�i �̐���

	if (m_apScene2D[LOGOTYPE_AKAZUKIN] != NULL)
	{// ���݂��Ă����Ƃ�
		m_apScene2D[LOGOTYPE_AKAZUKIN]->BindTexture(CManager::GetResource("data/tex/akazukin.png"));			// �w�i�e�N�X�`�� �̃|�C���^ ��n��
		m_apScene2D[LOGOTYPE_AKAZUKIN]->SetCenter(CScene2D::TEXTUREVTX_CENTER);		// ���S �̐ݒ�
		m_apScene2D[LOGOTYPE_AKAZUKIN]->SetSize(D3DXVECTOR3(500.0f, 150.0f, 0.0f));	// �傫�� �̐ݒ�
		m_apScene2D[LOGOTYPE_AKAZUKIN]->SetPosition(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4, 0.0f));	// �ʒu �̐ݒ�
		m_apScene2D[LOGOTYPE_AKAZUKIN]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// �F �̐ݒ�
		m_apScene2D[LOGOTYPE_AKAZUKIN]->SetTransform();		// ���_��� �̐ݒ�
	}

	m_apScene2D[LOGOTYPE_ATTACK] = CScene2D::Create(CScene2D::PRIORITY_UI);		// �w�i �̐���

	if (m_apScene2D[LOGOTYPE_ATTACK] != NULL)
	{// ���݂��Ă����Ƃ�
		m_apScene2D[LOGOTYPE_ATTACK]->BindTexture(CManager::GetResource("data/tex/attack.png"));			// �w�i�e�N�X�`�� �̃|�C���^ ��n��
		m_apScene2D[LOGOTYPE_ATTACK]->SetCenter(CScene2D::TEXTUREVTX_CENTER);		// ���S �̐ݒ�
		m_apScene2D[LOGOTYPE_ATTACK]->SetSize(D3DXVECTOR3(250.0f, 100.0f, 0.0f));	// �傫�� �̐ݒ�
		m_apScene2D[LOGOTYPE_ATTACK]->SetPosition(D3DXVECTOR3(1000.0f, SCREEN_HEIGHT / 4 + 50, 0.0f));	// �ʒu �̐ݒ�
		m_apScene2D[LOGOTYPE_ATTACK]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// �F �̐ݒ�
		m_apScene2D[LOGOTYPE_ATTACK]->SetTransform();		// ���_��� �̐ݒ�
	}

	m_apScene2D[LOGOTYPE_PRESS] = CScene2D::Create(CScene2D::PRIORITY_UI);		// �w�i �̐���

	if (m_apScene2D[LOGOTYPE_PRESS] != NULL)
	{// ���݂��Ă����Ƃ�
		m_apScene2D[LOGOTYPE_PRESS]->BindTexture(CManager::GetResource("data/tex/press_enter.png"));			// �w�i�e�N�X�`�� �̃|�C���^ ��n��
		m_apScene2D[LOGOTYPE_PRESS]->SetCenter(CScene2D::TEXTUREVTX_CENTER);		// ���S �̐ݒ�
		m_apScene2D[LOGOTYPE_PRESS]->SetSize(D3DXVECTOR3(250.0f, 100.0f, 0.0f));	// �傫�� �̐ݒ�
		m_apScene2D[LOGOTYPE_PRESS]->SetPosition(D3DXVECTOR3(SCREEN_WIDTH / 2, 640, 0.0f));	// �ʒu �̐ݒ�
		m_apScene2D[LOGOTYPE_PRESS]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// �F �̐ݒ�
		m_apScene2D[LOGOTYPE_PRESS]->SetTransform();		// ���_��� �̐ݒ�
	}

	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CTitlelogo::Uninit(void)
{
	for (int nCount = 0; nCount < LOGOTYPE_MAX; nCount++)
	{
		if (m_apScene2D[nCount] != NULL)
		{// �`���񂪂������Ƃ�
			m_apScene2D[nCount]->Release();		// �폜�\��
			m_apScene2D[nCount] = NULL;			// NULL ����
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CTitlelogo::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CTitlelogo::Draw(void)
{

}

//==============================================================================
// �N���G�C�g����
//==============================================================================
CTitlelogo *CTitlelogo::Create(void)
{
	CTitlelogo *pTitleLogo;		// �w�i �̃|�C���^

	// �w�i �̐���
	pTitleLogo = new CTitlelogo;

	// �w�i �̏�����
	pTitleLogo->Init();

	return pTitleLogo;
}

//==============================================================================
// �A�Z�b�g�̐�������
//==============================================================================
HRESULT CTitlelogo::Load(void)
{
	//�e�N�X�`���̓ǂݍ���
	CManager::Load("data/tex/back.png");
	CManager::Load("data/tex/if.png");
	CManager::Load("data/tex/akazukin.png");
	CManager::Load("data/tex/attack.png");
	CManager::Load("data/tex/press_enter.png");

	return S_OK;
}