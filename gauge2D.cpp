//=============================================================================
//
// �Q�[�W���� [gauge2D.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "gauge2D.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CGauge2D::CGauge2D(CScene::PRIORITY obj = CScene::PRIORITY_UI) : CScene2D(obj)
{
	SetObjType(PRIORITY_UI);				//�I�u�W�F�N�g�^�C�v

	//�l�̏�����
	m_move = D3DXVECTOR3(0, 0, 0.0f);			// �ړ��ʂ̏�����
	m_size = D3DXVECTOR3(180.0f, 15.0f, 0.0f);	// �T�C�Y�̏�����
	m_nDamage = 1.0f;							// �_���[�W�̏�����
	m_fHP = 1;									// HP�̏�����

	for (int nCount = 0; nCount < DRAWINGTYPE_MAX; nCount++)
	{
		if (m_apScene2D[nCount] != NULL)
		{// �V�[�������݂��Ă����Ƃ�
			m_apScene2D[nCount] = NULL;			// NULL��������
		}
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGauge2D::~CGauge2D()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CGauge2D::Init(void)
{
	m_frameSize = D3DXVECTOR3(m_size.x + 20, m_size.y + 16, m_size.z);		// �Q�[�W�t���[���̃T�C�Y�ݒ�

	// �T�C�Y�̐ݒ�
	SetSize(m_size);

	m_apScene2D[DRAWINGTYPE_GLAS] = CScene2D::Create(PRIORITY_UI);			// ����

	if (m_apScene2D[DRAWINGTYPE_GLAS] != NULL)
	{// �Q�[�W�O���X�����݂��Ă����Ƃ�
		m_apScene2D[DRAWINGTYPE_GLAS]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f));		// �F�̐ݒ�
		m_apScene2D[DRAWINGTYPE_GLAS]->SetCenter(CScene2D::TEXTUREVTX_LEFT);			// ���S�ʒu�����ɐݒ�
		m_apScene2D[DRAWINGTYPE_GLAS]->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// �ʒu�̐ݒ�
		m_apScene2D[DRAWINGTYPE_GLAS]->SetSize(m_size);									// �T�C�Y�̐ݒ�
		m_apScene2D[DRAWINGTYPE_GLAS]->SetTransform();									// ���_���̍X�V
	}

	m_apScene2D[DRAWINGTYPE_DAMAGE] = CScene2D::Create(PRIORITY_UI);		// ����

	if (m_apScene2D[DRAWINGTYPE_DAMAGE] != NULL)
	{// �_���[�W�Q�[�W�����݂��Ă����Ƃ�
		m_apScene2D[DRAWINGTYPE_DAMAGE]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));	// �F�̐ݒ�
		m_apScene2D[DRAWINGTYPE_DAMAGE]->SetCenter(CScene2D::TEXTUREVTX_LEFT);			// ���S�ʒu�����ɐݒ�
		m_apScene2D[DRAWINGTYPE_DAMAGE]->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// �ʒu�̐ݒ�
		m_apScene2D[DRAWINGTYPE_DAMAGE]->SetSize(m_size);								// �T�C�Y�̐ݒ�
		m_apScene2D[DRAWINGTYPE_DAMAGE]->SetTransform();								// ���_���̍X�V
	}

	m_apScene2D[DRAWINGTYPE_BACK] = CScene2D::Create(PRIORITY_UI);			// ����

	if (m_apScene2D[DRAWINGTYPE_BACK] != NULL)
	{// �Q�[�W�w�i�����݂��Ă����Ƃ�
		m_apScene2D[DRAWINGTYPE_BACK]->SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));		// �F�̐ݒ�
		m_apScene2D[DRAWINGTYPE_BACK]->SetCenter(CScene2D::TEXTUREVTX_LEFT);			// ���S�ʒu�����ɐݒ�
		m_apScene2D[DRAWINGTYPE_BACK]->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// �ʒu�̐ݒ�
		m_apScene2D[DRAWINGTYPE_BACK]->SetSize(m_size);									// �T�C�Y�̐ݒ�
		m_apScene2D[DRAWINGTYPE_BACK]->SetTransform();									// ���_���̍X�V
	}

	m_apScene2D[DRAWINGTYPE_FRONT] = CScene2D::Create(PRIORITY_UI);			// ����

	if (m_apScene2D[DRAWINGTYPE_FRONT] != NULL)
	{// �Q�[�WUI�����݂��Ă����Ƃ�
		m_apScene2D[DRAWINGTYPE_FRONT]->BindTexture(CManager::GetResource("data/tex/Gauge.png"));	//UI�p�̉摜��ݒ�
		m_apScene2D[DRAWINGTYPE_FRONT]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));				// �F�̐ݒ�
		m_apScene2D[DRAWINGTYPE_FRONT]->SetCenter(CScene2D::TEXTUREVTX_LEFT);						// ���S�ʒu�����ɐݒ�
		m_apScene2D[DRAWINGTYPE_FRONT]->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));					// �ʒu�̐ݒ�
		m_apScene2D[DRAWINGTYPE_FRONT]->SetSize(m_frameSize);										// �T�C�Y�̐ݒ�
		m_apScene2D[DRAWINGTYPE_FRONT]->SetTransform();												// ���_���̍X�V
	}

	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CGauge2D::Uninit(void)
{
	//�I������
	for (int nCntLife = 0; nCntLife < DRAWINGTYPE_MAX; nCntLife++)
	{
		if (m_apScene2D[nCntLife] != NULL)
		{// �Q�[�W�����݂��Ă����Ƃ�
			m_apScene2D[nCntLife]->Release();			// �폜�\��
			m_apScene2D[nCntLife] = NULL;				// NULL����
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CGauge2D::Update(void)
{
	if (m_nDamage * m_size.x < m_fHP * m_size.x)
	{// ���݂̑傫�����_���[�W�p�̃Q�[�W���傫���ꍇ
		m_fHP -= 0.005f;			// ����
	}

	m_apScene2D[DRAWINGTYPE_DAMAGE]->SetSize(D3DXVECTOR3(m_fHP * m_size.x, m_size.y, 0.0f));		// �T�C�Y�̐ݒ�

	for (int nCount = 0; nCount < DRAWINGTYPE_FRONT; nCount++)
	{
		m_apScene2D[nCount]->SetPosition(D3DXVECTOR3(20.0f, -15.0f, 0.0f) + m_pos);			// �ʒu�̐ݒ�
		m_apScene2D[nCount]->SetTransform();												// ���_���̍X�V
	}

	m_apScene2D[DRAWINGTYPE_FRONT]->SetPosition(m_pos);				// �ʒu�̐ݒ�
	m_apScene2D[DRAWINGTYPE_FRONT]->SetTransform();					// ���_���̍X�V
}

//=============================================================================
// �`�揈��
//=============================================================================
void CGauge2D::Draw(void)
{
	for (int nCntLife = 0; nCntLife < DRAWINGTYPE_MAX; nCntLife++)
	{
		if (m_apScene2D[nCntLife] != NULL)
		{// �Q�[�W�����݂��Ă����Ƃ�
			m_apScene2D[nCntLife]->Draw();			// �`��
		}
	}
}

//=============================================================================
// �N���G�C�g����
//=============================================================================
CGauge2D *CGauge2D::Create(void)
{
	CGauge2D *pGauge;
	pGauge = new CGauge2D(PRIORITY_UI);

	if (pGauge != NULL)
	{// �Q�[�W�����݂��Ă����Ƃ�
		pGauge->Init();			// ����������
	}
	return pGauge;
}

//=============================================================================
// ���[�h����
//=============================================================================
HRESULT CGauge2D::Load(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X���擾����
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̍쐬
	CManager::Load("data/tex/Gauge.png");

	return S_OK;
}

//=============================================================================
// �|�W�V�����̐ݒ�
//=============================================================================
void CGauge2D::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// ���C�t�o�[�̕\�������̐ݒ�
//=============================================================================
void CGauge2D::SetLifeBar(bool bDie, float fValue)
{
	m_apScene2D[DRAWINGTYPE_BACK]->SetSize(D3DXVECTOR3(m_size.x * fValue, m_size.y, 0.0f));
	m_nDamage = fValue;
}

//=============================================================================
// �Q�[�W�T�C�Y�̐ݒ�
//=============================================================================
void CGauge2D::SetGaugeSize(D3DXVECTOR3	size, D3DXVECTOR3 frameSize)
{
	m_size = size;
	m_frameSize = frameSize;

	for (int nCount = 0; nCount < DRAWINGTYPE_FRONT; nCount++)
	{
		m_apScene2D[nCount]->SetSize(m_size);
	}

	m_apScene2D[DRAWINGTYPE_FRONT]->SetSize(m_frameSize);
}