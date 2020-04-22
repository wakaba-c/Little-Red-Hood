//=============================================================================
//
// �v���C���[��UI�Ǘ����� [playerUi.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "playerUi.h"
#include "manager.h"
#include "renderer.h"
#include "inputKeyboard.h"
#include "gauge2D.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPlayerUi::CPlayerUi(CScene::PRIORITY obj = CScene::PRIORITY_UI) : CScene2D(obj)
{
	(CScene::PRIORITY_UI);				//�I�u�W�F�N�g�^�C�v

	//�l�̏�����
	m_move = D3DXVECTOR3(0, 0, 0.0f);	// �ړ��ʂ̏�����
	m_nPage = 0;						// �y�[�W���̏�����

	for (int nCount = 0; nCount < GAUGETYPE_MAX; nCount++)
	{
		if (m_apGauge2D[nCount] != NULL)
		{// HP�ƕK�E���U�Q�[�W�̃|�C���^�����݂����Ƃ�
			m_apGauge2D[nCount] = NULL;		// NULL�ɂ���
		}
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPlayerUi::~CPlayerUi()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPlayerUi::Init(void)
{
	// ����������
	CScene2D::Init();

	// �w�ʃe�N�X�`���̐ݒ�
	BindTexture(CManager::GetResource("data/tex/status_back.png"));

	SetPosition(D3DXVECTOR3(230.0f, 600.0f, 0.0f));		// �w�ʂ̈ʒu�ݒ�
	SetSize(D3DXVECTOR3(420.0f, 150.0f, 0.0f));			// �w�ʂ̑傫���ݒ�
	SetTransform();										// ���_���̍X�V

	m_apGauge2D[GAUGETYPE_HP] = CGauge2D::Create();		// HP�Q�[�W�̍쐬

	if (m_apGauge2D[GAUGETYPE_HP] != NULL)
	{// HP�Q�[�W�����݂��Ă����Ƃ�
		m_apGauge2D[GAUGETYPE_HP]->SetPosition(D3DXVECTOR3(130.0f, 650.0f, 0.0f));											// HP�Q�[�W�̈ʒu�ݒ�
		m_apGauge2D[GAUGETYPE_HP]->SetGaugeSize(D3DXVECTOR3(520.0f, 15.0f, 0.0f), D3DXVECTOR3(600.0f, 50.0f, 0.0f));		// HP�Q�[�W�̑傫���ݒ�
	}

	m_apGauge2D[GAUGETYPE_DEATHBLOW] = CGauge2D::Create();				// �K�E�Z�Q�[�W�̍쐬

	if (m_apGauge2D[GAUGETYPE_DEATHBLOW] != NULL)
	{// �K�E�Z�Q�[�W�����݂��Ă����Ƃ�
		m_apGauge2D[GAUGETYPE_DEATHBLOW]->SetPosition(D3DXVECTOR3(130.0f, 600.0f, 0.0f));										// �K�E�Z�Q�[�W�̈ʒu�ݒ�
		m_apGauge2D[GAUGETYPE_DEATHBLOW]->SetGaugeSize(D3DXVECTOR3(520.0f, 15.0f, 0.0f), D3DXVECTOR3(600.0f, 50.0f, 0.0f));		// �K�E�Z�Q�[�W�̑傫���ݒ�
		m_apGauge2D[GAUGETYPE_DEATHBLOW]->SetLifeBar(false, 0.0f);
	}

	m_apScene2D[DRAWINGTYPE_HP] = CScene2D::Create(PRIORITY_UI);		// HP�^�O�̍쐬

	if (m_apScene2D[DRAWINGTYPE_HP] != NULL)
	{// HP�^�O�����݂��Ă����Ƃ�
		m_apScene2D[DRAWINGTYPE_HP]->BindTexture(CManager::GetResource("data/tex/HP.png"));			// �e�N�X�`���̐ݒ�
		m_apScene2D[DRAWINGTYPE_HP]->SetPosition(D3DXVECTOR3(190.0f, 600.0f, 0.0f));				// �ʒu�ݒ�
		m_apScene2D[DRAWINGTYPE_HP]->SetSize(D3DXVECTOR3(100.0f, 30.0f, 0.0f));						// �傫���ݒ�
		m_apScene2D[DRAWINGTYPE_HP]->SetTransform();												// ���_���̍X�V
	}

	m_apScene2D[DRAWINGTYPE_DEATHBLOW] = CScene2D::Create(PRIORITY_UI);	// �K�E�Z�^�O�̍쐬

	if (m_apScene2D[DRAWINGTYPE_DEATHBLOW] != NULL)
	{// �K�E�Z�^�O�����݂��Ă����Ƃ�
		m_apScene2D[DRAWINGTYPE_DEATHBLOW]->BindTexture(CManager::GetResource("data/tex/end.png"));	// �e�N�X�`���̐ݒ�
		m_apScene2D[DRAWINGTYPE_DEATHBLOW]->SetPosition(D3DXVECTOR3(190.0f, 550.0f, 0.0f));			// �ʒu�ݒ�
		m_apScene2D[DRAWINGTYPE_DEATHBLOW]->SetSize(D3DXVECTOR3(70.0f, 30.0f, 0.0f));				// �傫���ݒ�
		m_apScene2D[DRAWINGTYPE_DEATHBLOW]->SetTransform();											// ���_���̍X�V
	}

	m_apScene2D[DRAWINGTYPE_BACK] = CScene2D::Create(PRIORITY_UI);		// �w�i�̍쐬

	if (m_apScene2D[DRAWINGTYPE_BACK] != NULL)
	{// �w�i�����݂��Ă����Ƃ�
		m_apScene2D[DRAWINGTYPE_BACK]->BindTexture(CManager::GetResource("data/tex/circle.png"));	// �e�N�X�`���̐ݒ�
		m_apScene2D[DRAWINGTYPE_BACK]->SetPosition(D3DXVECTOR3(100.0f, 600.0f, 0.0f));				// �ʒu�ݒ�
		m_apScene2D[DRAWINGTYPE_BACK]->SetSize(D3DXVECTOR3(120.0f, 120.0f, 0.0f));					// �傫���ݒ�
		m_apScene2D[DRAWINGTYPE_BACK]->SetTransform();												// ���_���̍X�V
	}

	m_apScene2D[DRAWINGTYPE_CHARACTER] = CScene2D::Create(PRIORITY_UI);	// �Ԃ�����̍쐬

	if (m_apScene2D[DRAWINGTYPE_CHARACTER] != NULL)
	{// �Ԃ����񂪑��݂��Ă����Ƃ�
		m_apScene2D[DRAWINGTYPE_CHARACTER]->BindTexture(CManager::GetResource("data/tex/silhouette.png"));	// �e�N�X�`���̐ݒ�
		m_apScene2D[DRAWINGTYPE_CHARACTER]->SetPosition(D3DXVECTOR3(100.0f, 600.0f, 0.0f));			// �ʒu�̐ݒ�
		m_apScene2D[DRAWINGTYPE_CHARACTER]->SetSize(D3DXVECTOR3(100.0f, 100.0f, 0.0f));				// �傫���ݒ�
		m_apScene2D[DRAWINGTYPE_CHARACTER]->SetTransform();											// ���_���̍X�V
	}
	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CPlayerUi::Uninit(void)
{
	//�I������
	for (int nCntLife = 0; nCntLife < DRAWINGTYPE_MAX; nCntLife++)
	{
		if (m_apScene2D[nCntLife] != NULL)
		{
			m_apScene2D[nCntLife]->Uninit();
			m_apScene2D[nCntLife]->Release();
			m_apScene2D[nCntLife] = NULL;
		}
	}

	// �J������
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPlayerUi::Update(void)
{
	if (m_nDamage * m_size.x < m_fHP * m_size.x)
	{// ���݂̑傫�����_���[�W�p�̃Q�[�W���傫���ꍇ
		m_fHP -= 0.005f;
	}

	// �X�V����
	CScene2D::Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPlayerUi::Draw(void)
{
	// �`�揈��
	CScene2D::Draw();
}

//=============================================================================
// �N���G�C�g����
//=============================================================================
CPlayerUi *CPlayerUi::Create(void)
{
	CPlayerUi *pPlayerUi;
	pPlayerUi = new CPlayerUi(PRIORITY_UI);

	if (pPlayerUi != NULL)
	{// �v���C���[Ui�����݂��Ă����Ƃ�
		pPlayerUi->Init();
	}
	return pPlayerUi;
}

//=============================================================================
// ���[�h����
//=============================================================================
HRESULT CPlayerUi::Load(void)
{
	//�e�N�X�`���̓ǂݍ���
	CManager::Load("data/tex/status_back.png");
	CManager::Load("data/tex/rose.png");
	CManager::Load("data/tex/HP.png");
	CManager::Load("data/tex/end.png");
	CManager::Load("data/tex/circle.png");
	CManager::Load("data/tex/silhouette.png");

	return S_OK;
}