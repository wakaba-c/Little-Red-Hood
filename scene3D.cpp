//=============================================================================
//
// scene3D���� [scene3D.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "scene3D.h"
#include "manager.h"
#include "renderer.h"
#include "inputKeyboard.h"
#include "inputMouse.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CScene3D::CScene3D(CScene::PRIORITY obj) : CScene(obj)
{
	// �l�̏�����
	m_pTexture = NULL;
	m_size = D3DXVECTOR3(100.0f, 100.0f, 100.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScene3D::~CScene3D()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CScene3D::Init(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// �ʒu�̐ݒ�

	// ���_���̍쐬
	MakeVertex();
	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CScene3D::Uninit(void)
{
	//���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CScene3D::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CScene3D::Draw(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;

	D3DXVECTOR3 pos = GetPosition();					// �ʒu���擾

	D3DXMATRIX	mtxRot, mtxTrans, mtxView;				// �v�Z�p�}�g���b�N�X

	// �f�o�C�X���擾����
	pDevice = pRenderer->GetDevice();

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	m_mtxWorld._11 = mtxView._11;
	m_mtxWorld._12 = mtxView._21;
	m_mtxWorld._13 = mtxView._31;
	m_mtxWorld._21 = mtxView._12;
	m_mtxWorld._22 = mtxView._22;
	m_mtxWorld._23 = mtxView._32;
	m_mtxWorld._31 = mtxView._13;
	m_mtxWorld._32 = mtxView._23;
	m_mtxWorld._33 = mtxView._33;

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// �����_�[�X�e�[�g(�ʏ�u�����h����)
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetTexture(0, NULL);

}

//=============================================================================
// �N���G�C�g�֐�
//=============================================================================
CScene3D *CScene3D::Create(CScene::PRIORITY obj)
{
	CScene3D *pScene3D;
	pScene3D = new CScene3D(obj);
	pScene3D->Init();
	return pScene3D;
}

//=============================================================================
// �Z�b�g�e�N�X�`��
//=============================================================================
void CScene3D::BindTexture(LPDIRECT3DTEXTURE9 tex)
{
	m_pTexture = tex;
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void CScene3D::MakeVertex(void)
{
	VERTEX_3D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	switch (m_Vtx)
	{
	case TEXTUREVTX_CENTER:
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-m_size.x / 2, m_size.y / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_size.x / 2, m_size.y / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-m_size.x / 2, -m_size.y / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_size.x / 2, -m_size.y / 2, 0.0f);
		break;
	case TEXTUREVTX_LEFT:
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, m_size.y / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_size.x, m_size.y / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, -m_size.y / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_size.x, -m_size.y / 2, 0.0f);
		break;
	}

	//�@���x�N�g��
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//���_�J���[
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//�e�N�X�`���`�ʂ̈ʒu
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�f�[�^�̃A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �����̐ݒ�֐�
//=============================================================================
void CScene3D::SetCenter(TEXTUREVTX vtx)
{
	m_Vtx = vtx;
}

//=============================================================================
// �T�C�Y�̐ݒ�֐�
//=============================================================================
void CScene3D::SetSize(D3DXVECTOR3 size)
{
	m_size = size;
}

//=============================================================================
// �F�̐ݒ�
//=============================================================================
void CScene3D::SetColor(D3DXCOLOR col)
{
	m_col = col;
}