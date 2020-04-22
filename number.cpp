//=============================================================================
//
// �������� [number.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "number.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CNumber::CNumber(CScene::PRIORITY obj = CScene::PRIORITY_UI) : CScene2D(obj)
{
	//�l�̏�����
	m_nNumOld = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CNumber::~CNumber()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CNumber::Init(void)
{
	// ���_���̍쐬
	MakeVertex();
	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CNumber::Uninit(void)
{
	//���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{// ���_�o�b�t�@�����݂��Ă����Ƃ�
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CNumber::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CNumber::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	CRenderer *pRenderer = CManager::GetRenderer();

	//�f�o�C�X���擾����
	pDevice = pRenderer->GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�Ƀo�C���h
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//�e�N�X�`���̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetTexture(0, CManager::GetResource("data/tex/number001.png"));

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// �N���G�C�g����
//=============================================================================
CNumber *CNumber::Create(void)
{
	CNumber *pNumber;
	pNumber = new CNumber(PRIORITY_UI);

	if (pNumber != NULL)
	{// ���������݂��Ă����Ƃ�
		pNumber->Init();			// ����������
	}
	return pNumber;
}

//=============================================================================
// ���[�h����
//=============================================================================
HRESULT CNumber::Load(void)
{
	// �e�N�X�`���̎擾
	CManager::Load("data/tex/number001.png");

	return S_OK;
}

//=============================================================================
// �����̐ݒ�
//=============================================================================
void CNumber::SetNumber(int nNum)
{
	VERTEX_2D *pVtx;

	//�O��̐����ƍ���̐������Ⴄ�ꍇ
	if (m_nNumOld != nNum)
	{
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		m_nNumOld = nNum;

		//�e�N�X�`���`�ʂ̈ʒu
		pVtx[0].tex = D3DXVECTOR2(nNum * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(nNum * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(nNum * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(nNum * 0.1f + 0.1f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
void CNumber::MakeVertex(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	CRenderer *pRenderer = CManager::GetRenderer();
	D3DXVECTOR3 pos = GetPosition();					// �ʒu�̎擾
	D3DXVECTOR3 size = GetSize();						// �T�C�Y�̎擾

	pDevice = pRenderer->GetDevice();

	VERTEX_2D *pVtx;

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���̐ݒ�
	//���_���W�̐ݒ�(���x���W + �Ԋu * nCntScore (+ ��), ���y���W)
	pVtx[0].pos = D3DXVECTOR3(pos.x - size.x / 2, pos.y - size.y / 2, pos.z);
	pVtx[1].pos = D3DXVECTOR3(pos.x + size.x / 2, pos.y - size.y / 2, pos.z);
	pVtx[2].pos = D3DXVECTOR3(pos.x - size.x / 2, pos.y + size.y / 2, pos.z);
	pVtx[3].pos = D3DXVECTOR3(pos.x + size.x / 2, pos.y + size.y / 2, pos.z);

	//1.0�ŌŒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//�J���[�`���[�g�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`���`�ʂ̈ʒu
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

	// ���_�f�[�^���A�����b�N����
	m_pVtxBuff->Unlock();
}