//=============================================================================
//
// �̗̓Q�[�W���� [life.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "life.h"
#include "manager.h"
#include "renderer.h"
#include "scene3D.h"
#include "inputKeyboard.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLife::CLife(CScene::PRIORITY obj = PRIORITY_LIFE) : CScene(obj)
{
	SetObjType(PRIORITY_LIFE);				//�I�u�W�F�N�g�^�C�v

	// �l�̏�����
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ��]�̏�����
	m_size = D3DXVECTOR3(50.0f, 10.0f, 0.0f);	// �T�C�Y�̏�����
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLife::~CLife()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CLife::Init(void)
{
	m_apScene3D[DRAWINGTYPE_BACK] = CScene3D::Create(PRIORITY_UI);		// ����

	if (m_apScene3D[DRAWINGTYPE_BACK] != NULL)
	{// �V�[�������݂��Ă����Ƃ�
		m_apScene3D[DRAWINGTYPE_BACK]->SetCenter(CScene3D::TEXTUREVTX_LEFT);			// ���S�ʒu�����ɐݒ�
		m_apScene3D[DRAWINGTYPE_BACK]->SetSize(m_size);									// �T�C�Y�̐ݒ�
		m_apScene3D[DRAWINGTYPE_BACK]->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// �ʒu�̐ݒ�
		m_apScene3D[DRAWINGTYPE_BACK]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f));		// �F�̐ݒ�
		m_apScene3D[DRAWINGTYPE_BACK]->MakeVertex();									// ���_���̍X�V
	}

	m_apScene3D[DRAWINGTYPE_FRONT] = CScene3D::Create(PRIORITY_UI);		// ����

	if (m_apScene3D[DRAWINGTYPE_FRONT] != NULL)
	{// �V�[�������݂��Ă����Ƃ�
		m_apScene3D[DRAWINGTYPE_FRONT]->SetCenter(CScene3D::TEXTUREVTX_LEFT);			// ���S�ʒu�����ɐݒ�
		m_apScene3D[DRAWINGTYPE_FRONT]->SetSize(m_size);								// �T�C�Y�̐ݒ�
		m_apScene3D[DRAWINGTYPE_FRONT]->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// �ʒu�̐ݒ�
		m_apScene3D[DRAWINGTYPE_FRONT]->SetColor(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));	// �F�̐ݒ�
		m_apScene3D[DRAWINGTYPE_FRONT]->MakeVertex();									// ���_���̍X�V
	}

	m_apScene3D[DRAWINGTYPE_FRAME] = CScene3D::Create(PRIORITY_UI);		// ����

	if (m_apScene3D[DRAWINGTYPE_FRAME] != NULL)
	{// �V�[�������݂��Ă����Ƃ�
		m_apScene3D[DRAWINGTYPE_FRAME]->SetCenter(CScene3D::TEXTUREVTX_LEFT);								// ���S�ʒu�����ɐݒ�
		m_apScene3D[DRAWINGTYPE_FRAME]->SetSize(D3DXVECTOR3(m_size.x + 5.0f, m_size.y * 2, 0.0f));			// �T�C�Y�̐ݒ�
		m_apScene3D[DRAWINGTYPE_FRAME]->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));							// �ʒu�̐ݒ�
		m_apScene3D[DRAWINGTYPE_FRAME]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));						// �F�̐ݒ�
		m_apScene3D[DRAWINGTYPE_FRAME]->BindTexture(CManager::GetResource("data/tex/HitPointGauge.png"));	// �e�N�X�`����ݒ�
		m_apScene3D[DRAWINGTYPE_FRAME]->MakeVertex();														// ���_���̍X�V
	}
	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CLife::Uninit(void)
{
	for (int nCount = 0; nCount < DRAWINGTYPE_MAX; nCount++)
	{
		if (m_apScene3D[nCount] != NULL)
		{// �V�[�������݂��Ă����Ƃ�
			m_apScene3D[nCount]->Uninit();			// �J������
			m_apScene3D[nCount]->Release();			// �폜�\��
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CLife::Update(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	D3DXVECTOR3 pos = GetPosition();

	m_apScene3D[DRAWINGTYPE_BACK]->SetPosition(pos + D3DXVECTOR3(3.0f, 0.0f, 0.0f));
	m_apScene3D[DRAWINGTYPE_FRONT]->SetPosition(pos + D3DXVECTOR3(3.0f, 0.0f, 0.0f));
	m_apScene3D[DRAWINGTYPE_FRAME]->SetPosition(pos + D3DXVECTOR3(0.0f, 2.5f, 0.0f));
}

//=============================================================================
// �`�揈��
//=============================================================================
void CLife::Draw(void)
{

}

//=============================================================================
// �N���G�C�g����
//=============================================================================
CLife *CLife::Create(void)
{
	CLife *pLife;
	pLife = new CLife(PRIORITY_LIFE);

	if (pLife != NULL)
	{// ���C�t�����݂��Ă����Ƃ�
		pLife->Init();				// ����������
	}
	return pLife;
}

//=============================================================================
// �e�N�X�`���̃��[�h�֐�
//=============================================================================
HRESULT CLife::Load(void)
{
	// �e�N�X�`���̓ǂݍ���
	CManager::Load("data/tex/HitPointGauge.png");
	return S_OK;
}

//=============================================================================
// ���C�t�o�[�̕\�������̐ݒ�
//=============================================================================
void CLife::SetLifeBar(float fValue)
{
	if (m_apScene3D[DRAWINGTYPE_FRONT] != NULL)
	{// �V�[�������݂��Ă����Ƃ�
		m_apScene3D[DRAWINGTYPE_FRONT]->SetSize(D3DXVECTOR3(m_size.x * fValue, m_size.y, 0.0f));	// �T�C�Y�̐ݒ�
		m_apScene3D[DRAWINGTYPE_FRONT]->MakeVertex();												// ���_���̍X�V
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void CLife::MakeVertex(void)
{
	//�f�o�C�X���擾����
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	VERTEX_3D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_size.x, m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_size.x, -m_size.y, 0.0f);

	//�@���x�N�g��
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//���_�J���[
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`���`�ʂ̈ʒu
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�f�[�^�̃A�����b�N
	m_pVtxBuff->Unlock();
}