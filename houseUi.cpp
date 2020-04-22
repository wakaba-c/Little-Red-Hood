//=============================================================================
//
// ���΂������Ƃ�UI�Ǘ����� [houseUi.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "houseUi.h"
#include "manager.h"
#include "renderer.h"
#include "inputKeyboard.h"
#include "gauge2D.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CHouseUi::CHouseUi(CScene::PRIORITY obj = CScene::PRIORITY_UI) : CScene2D(obj)
{
	(CScene::PRIORITY_UI);				//�I�u�W�F�N�g�^�C�v

	//�l�̏�����
	m_move = D3DXVECTOR3(0, 0, 0.0f);		// �ړ��ʂ̏�����
	m_nPage = 0;							// �y�[�W���̏�����

	if (m_pGauge2D != NULL)
	{// �ϋv�l�Q�[�W�̃|�C���^�����݂����Ƃ�
		m_pGauge2D = NULL;					// NULL����
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CHouseUi::~CHouseUi()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CHouseUi::Init(void)
{
	// ����������
	CScene2D::Init();

	// �w�ʃe�N�X�`���̐ݒ�
	BindTexture(CManager::GetResource("data/tex/SignBoard2.png"));

	SetPosition(D3DXVECTOR3(230.0f, 100.0f, 0.0f));		// �w�ʂ̈ʒu�ݒ�
	SetSize(D3DXVECTOR3(420.0f, 150.0f, 0.0f));			// �w�ʂ̑傫���ݒ�
	SetTransform();										// ���_���̍X�V

	m_pGauge2D = CGauge2D::Create();					// HP�Q�[�W�̍쐬

	if (m_pGauge2D != NULL)
	{// HP�Q�[�W�����݂��Ă����Ƃ�
		m_pGauge2D->SetPosition(D3DXVECTOR3(490.0f, 90.0f, 0.0f));			// HP�Q�[�W�̈ʒu�ݒ�
		m_pGauge2D->SetGaugeSize(D3DXVECTOR3(520.0f, 15.0f, 0.0f), D3DXVECTOR3(600.0f, 50.0f, 0.0f));		// HP�Q�[�W�̑傫���ݒ�
	}

	m_apScene2D[DRAWINGTYPE_HP] = CScene2D::Create(PRIORITY_UI);		// HP�^�O�̍쐬

	if (m_apScene2D[DRAWINGTYPE_HP] != NULL)
	{// HP�^�O�����݂��Ă����Ƃ�
		m_apScene2D[DRAWINGTYPE_HP]->BindTexture(CManager::GetResource("data/tex/Grandma's house_Tag.png"));			// �e�N�X�`���̎擾
		m_apScene2D[DRAWINGTYPE_HP]->SetPosition(D3DXVECTOR3(SCREEN_WIDTH / 2, 35.0f, 0.0f));		// �ʒu�ݒ�
		m_apScene2D[DRAWINGTYPE_HP]->SetSize(D3DXVECTOR3(300.0f, 90.0f, 0.0f));					// �傫���ݒ�
		m_apScene2D[DRAWINGTYPE_HP]->SetTransform();				// ���_���̍X�V
	}
	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CHouseUi::Uninit(void)
{
	//�I������
	for (int nCntLife = 0; nCntLife < DRAWINGTYPE_MAX; nCntLife++)
	{
		if (m_apScene2D[nCntLife] != NULL)
		{// �V�[�������݂��Ă����Ƃ�
			m_apScene2D[nCntLife]->Release();		// �폜�\��
			m_apScene2D[nCntLife] = NULL;			// NULL����
		}
	}

	// �I������
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CHouseUi::Update(void)
{
	if (m_nDamage * m_size.x < m_fHP * m_size.x)
	{// ���݂̑傫�����_���[�W�p�̃Q�[�W���傫���ꍇ
		m_fHP -= 0.005f;			// ����
	}

	// �X�V����
	CScene2D::Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CHouseUi::Draw(void)
{
	// �`�揈��
	CScene2D::Draw();
}

//=============================================================================
// �N���G�C�g����
//=============================================================================
CHouseUi *CHouseUi::Create(void)
{
	CHouseUi *pHouseUi;
	pHouseUi = new CHouseUi(PRIORITY_UI);

	if (pHouseUi != NULL)
	{// �Ƃ�UI�����݂��Ă����Ƃ�
		pHouseUi->Init();			// ����������
	}
	return pHouseUi;
}

//=============================================================================
// ���[�h�֐�
//=============================================================================
HRESULT CHouseUi::Load(void)
{
	//�e�N�X�`���̓ǂݍ���
	CManager::Load("data/tex/SignBoard2.png");
	CManager::Load("data/tex/rose.png");
	return S_OK;
}