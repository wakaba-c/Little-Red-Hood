//=============================================================================
//
// �ړI�\������ [purpose.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "purpose.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPurpose::CPurpose(CScene::PRIORITY obj = CScene::PRIORITY_UI) : CScene2D(obj)
{
	SetObjType(PRIORITY_UI);				//�I�u�W�F�N�g�^�C�v

	//�l�̏�����
	for (int nCount = 0; nCount < DRAWINGTYPE_MAX; nCount++)
	{
		m_apScene2D[nCount] = NULL;
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPurpose::~CPurpose()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPurpose::Init(void)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(1000.0f, 50.0f, 0.0f);
	m_size = D3DXVECTOR3(500.0f, 150.0f, 0.0f);				// �T�C�Y�̒�`
	SetSize(m_size);			// �T�C�Y�̐ݒ�

	// �Ŕ̐���
	m_apScene2D[DRAWINGTYPE_SIGNBORAD] = CScene2D::Create(PRIORITY_UI);

	if (m_apScene2D[DRAWINGTYPE_SIGNBORAD] != NULL)
	{// �|�C���^��NULL�ł͂Ȃ��Ƃ�
		m_apScene2D[DRAWINGTYPE_SIGNBORAD]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));		// �F�̐ݒ�
		m_apScene2D[DRAWINGTYPE_SIGNBORAD]->SetPosition(pos);			// �ʒu�̐ݒ�
		m_apScene2D[DRAWINGTYPE_SIGNBORAD]->SetSize(m_size);			// �傫���̐ݒ�
		m_apScene2D[DRAWINGTYPE_SIGNBORAD]->BindTexture(CManager::GetResource("data/tex/signeBoard.png"));		//�e�N�X�`���̐ݒ�
		m_apScene2D[DRAWINGTYPE_SIGNBORAD]->SetTransform();				// ���_���̐ݒ�
	}

	// ���b�Z�[�W�̍쐬
	m_apScene2D[DRAWINGTYPE_MESSAGE] = CScene2D::Create(PRIORITY_UI);

	if (m_apScene2D[DRAWINGTYPE_MESSAGE] != NULL)
	{// �|�C���^��NULL�ł͂Ȃ��Ƃ�
		m_apScene2D[DRAWINGTYPE_MESSAGE]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));			// �F�̐ݒ�
		m_apScene2D[DRAWINGTYPE_MESSAGE]->SpriteAnimation(D3DXVECTOR2(2.0f, 2.0f), 0, 0);			// �ʒu�̐ݒ�
		m_apScene2D[DRAWINGTYPE_MESSAGE]->SetPosition(D3DXVECTOR3(1000.0f, 70.0f, 0.0f));			// �ʒu�̐ݒ�
		m_apScene2D[DRAWINGTYPE_MESSAGE]->SetSize(D3DXVECTOR3(350.0f, 150.0f, 0.0f));				// �傫���̐ݒ�
		m_apScene2D[DRAWINGTYPE_MESSAGE]->BindTexture(CManager::GetResource("data/tex/message.png"));		//�e�N�X�`���̐ݒ�
		m_apScene2D[DRAWINGTYPE_MESSAGE]->SetTransform();				// ���_���̐ݒ�
	}

	return S_OK;
}

//=============================================================================
// �X�V����
//=============================================================================
void CPurpose::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CPurpose::Draw(void)
{

}

//=============================================================================
// �J������
//=============================================================================
void CPurpose::Uninit(void)
{
	//�I������
	for (int nCntLife = 0; nCntLife < DRAWINGTYPE_MAX; nCntLife++)
	{
		if (m_apScene2D[nCntLife] != NULL)
		{// �V�[�������݂��Ă����Ƃ�
			m_apScene2D[nCntLife]->Release();			// �I������
			m_apScene2D[nCntLife] = NULL;				// NULL��������
		}
	}
}

//=============================================================================
// �N���G�C�g����
//=============================================================================
CPurpose *CPurpose::Create(void)
{
	CPurpose *pPurpose;
	pPurpose = new CPurpose(PRIORITY_UI);
	pPurpose->Init();
	return pPurpose;
}

//=============================================================================
// ���[�h����
//=============================================================================
HRESULT CPurpose::Load(void)
{
	CManager::Load("data/tex/signeBoard.png");
	CManager::Load("data/tex/message.png");
	return S_OK;
}

//=============================================================================
// �ړI�ύX����
//=============================================================================
void CPurpose::ChangePurpose(MESSAGETYPE type)
{
	// �ړI��ݒ�
	m_type = type;

	// �ړI�ɍ��킹��
	m_apScene2D[DRAWINGTYPE_MESSAGE]->SpriteAnimation(D3DXVECTOR2(2.0f, 2.0f), type, 0.0f);
}