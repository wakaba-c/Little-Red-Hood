//=============================================================================
//
// �ړI�\������ [messageWindow.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "messageWindow.h"
#include "manager.h"
#include "renderer.h"
#include "fade.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMessageWindow::CMessageWindow(CScene::PRIORITY obj = CScene::PRIORITY_UI) : CScene2D(obj)
{
	(CScene::PRIORITY_UI);				//�I�u�W�F�N�g�^�C�v

	// �l�̏�����
	m_fade = FADE_OUT;			// �t�F�[�h�^�C�v��ݒ�
	m_nPage = 0;				// �y�[�W
	m_nCount = 0;				// �J�E���^�[
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);		// �J���[
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMessageWindow::~CMessageWindow()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CMessageWindow::Init(void)
{
	// ����������
	CScene2D::Init();

	// �w�ʃe�N�X�`���̐ݒ�
	BindTexture(CManager::GetResource("data/tex/MessageWindow.png"));

	SetPosition(D3DXVECTOR3(640.0f, 500.0f, 0.0f));		// �w�ʂ̈ʒu�ݒ�
	SetSize(D3DXVECTOR3(SCREEN_WIDTH, 150.0f, 0.0f));		// �w�ʂ̑傫���ݒ�
	SetColor(m_col);
	SetTransform();					// ���_���̍X�V

	m_pMessage = CScene2D::Create(PRIORITY_UI);

	if (m_pMessage != NULL)
	{// ���b�Z�[�W�����݂��Ă����Ƃ�
		m_pMessage->BindTexture(CManager::GetResource("data/tex/message.png"));		// �e�N�X�`���̐ݒ�

		m_pMessage->SetPosition(D3DXVECTOR3(640.0f, 500.0f, 0.0f));		// ���b�Z�[�W�̈ʒu�ݒ�
		m_pMessage->SetSize(D3DXVECTOR3(1000.0f, 150.0f, 0.0f));		// ���b�Z�[�W�̑傫���ݒ�
		m_pMessage->SetColor(m_col);				// ���b�Z�[�W�̃J���[�ݒ�
		m_pMessage->SpriteAnimation(D3DXVECTOR2(2.0f, 2.0f), m_type, 1.0f);
		m_pMessage->SetTransform();					// ���_���̍X�V
	}
	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CMessageWindow::Uninit(void)
{
	if (m_pMessage != NULL)
	{// ���b�Z�[�W�����݂��Ă����Ƃ�
		m_pMessage->Release();
	}

	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CMessageWindow::Update(void)
{
	VERTEX_2D *pVtx;

	if (m_fade != FADE_NONE)
	{
		if (m_fade == FADE_IN)
		{
			m_col.a -= 0.05f;				//��ʂ𓧖��ɂ��Ă���

			if (m_col.a <= 0.0f)
			{// ���l��0�ȉ��������Ƃ�
				m_col.a = 0.0f;				// ���l��0�ɂ���

				if (m_type == MESSAGETYPE_CLEAR)
				{// �N���A�e���b�v�������Ƃ�
					CFade::SetFade(CManager::MODE_RESULT);		// ���U���g�ɑJ��
				}

				Release();			// �폜�\��
			}

			SetColor(m_col);		// �J���[�̍X�V

			if (m_pMessage != NULL)
			{// ���b�Z�[�W�����݂��Ă����Ƃ�
				m_pMessage->SetColor(m_col);	// ���b�Z�[�W�J���[�̍X�V
			}
		}
		else if (m_fade == FADE_OUT)
		{
			m_col.a += 0.05f;

			if (m_col.a > 1.5f)
			{// ���l��1.5���傫���Ƃ�
				//�e���b�v�����ɐ؂�ւ�
				m_col.a = 1.0f;				// ���l��1�ɃZ�b�g
				m_fade = FADE_TELOP;		// �e���b�v�\���ɐ؂�ւ���
			}

			SetColor(m_col);				// �F��ݒ�

			if (m_pMessage != NULL)
			{// ���b�Z�[�W�����݂��Ă���Ƃ�
				m_pMessage->SetColor(m_col);		// �F��ݒ�
			}
		}
		else if (m_fade == FADE_TELOP)
		{// �e���b�v�����������Ƃ�
			m_nCount++;		// �J�E���^��1�v���X

			if (m_nCount > 15)
			{// �J�E���^�[��15���傫���Ƃ�
				//�t�F�[�h�C�������ɐ؂�ւ�
				m_fade = FADE_IN;
			}
		}
	}
	CScene2D::Update();			// �X�V����
}

//=============================================================================
// �`�揈��
//=============================================================================
void CMessageWindow::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// �N���G�C�g����
//=============================================================================
CMessageWindow *CMessageWindow::Create(MESSAGETYPE type)
{
	CMessageWindow *pMessage;
	pMessage = new CMessageWindow(PRIORITY_UI);

	if (pMessage != NULL)
	{// ���b�Z�[�W�����݂��Ă����Ƃ�
		pMessage->m_type = type;				// �^�C�v�̐ݒ�
		pMessage->Init();						// ����������
	}
	return pMessage;
}

//=============================================================================
// ���[�h����
//=============================================================================
HRESULT CMessageWindow::Load(void)
{
	//�e�N�X�`���̓ǂݍ���
	CManager::Load("data/tex/MessageWindow.png");
	CManager::Load("data/tex/message.png");
	return S_OK;
}