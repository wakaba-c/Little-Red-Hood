//=============================================================================
//
// �؏��� [tree.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "tree.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
PDIRECT3DTEXTURE9 CTree::m_pTexture = NULL;				// �e�N�X�`���̃|�C���^

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTree::CTree(CScene::PRIORITY obj = CScene::PRIORITY_TREE) : CScene(obj)
{
	SetObjType(CScene::PRIORITY_TREE);								// �I�u�W�F�N�g�^�C�v�̐ݒ�

	/* =============== �l�̏����� =============== */
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);								// �ʒu�̏�����
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);								// ��]�̏�����
	m_size = D3DXVECTOR3(120.0f, 200.0f, 0.0f);							// �T�C�Y�̏�����
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTree::~CTree()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTree::Init(void)
{
	m_pTexture = CManager::GetResource("data/tex/tree.png");			// �e�N�X�`���̐ݒ�
	MakeVertex();														// ���_���̍X�V
	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CTree::Uninit(void)
{
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
void CTree::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CTree::Draw(void)
{
		CRenderer *pRenderer = CManager::GetRenderer();
		LPDIRECT3DDEVICE9 pDevice;

		D3DXMATRIX	mtxRot, mtxTrans, mtxView;				//�v�Z�p�}�g���b�N�X

		//�f�o�C�X���擾����
		pDevice = pRenderer->GetDevice();

		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// ����(�����)���J�����O����
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&m_mtxWorld);

		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		m_mtxWorld._11 = mtxView._11;
		m_mtxWorld._12 = mtxView._21;
		m_mtxWorld._13 = mtxView._31;
		m_mtxWorld._31 = mtxView._13;
		m_mtxWorld._32 = mtxView._23;
		m_mtxWorld._33 = mtxView._33;

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetTexture(0, m_pTexture);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetTexture(0, NULL);

		// �����_�[�X�e�[�g(�ʏ�u�����h����)
		pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// ����(�����)���J�����O����


		// �����_�[�X�e�[�g(�ʏ�u�����h����)
		pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// ����(�����)���J�����O����
}

//=============================================================================
// �N���G�C�g����
//=============================================================================
CTree *CTree::Create(TREETYPE type)
{
	CTree *pTree = new CTree(PRIORITY_TREE);
	pTree->SetType(type);
	pTree->Init();
	return pTree;
}

//=============================================================================
// ���[�h����
//=============================================================================
HRESULT CTree::Load(void)
{
	CManager::Load("data/tex/tree.png");
	return S_OK;
}

//=============================================================================
// �e�N�X�`���̉������
//=============================================================================
void CTree::Unload(void)
{
	// �e�N�X�`���̊J��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// ���f���̓Ǎ�
//=============================================================================
void CTree::LoadModel(char *add)
{
	FILE *pFile = NULL;							// �t�@�C��
	char cReadText[128] = {};					// ����
	char cHeadText[128] = {};					// ��r
	float aData[4];								// ����
	CTree *pTree = NULL;

	pFile = fopen(add, "r");					// �t�@�C�����J���܂��͍��

	if (pFile != NULL)													// �t�@�C�����ǂݍ��߂��ꍇ
	{
		fgets(cReadText, sizeof(cReadText), pFile);						// �s���΂�
		fgets(cReadText, sizeof(cReadText), pFile);						// �s���΂�
		fgets(cReadText, sizeof(cReadText), pFile);						// �s���΂�

		while (strcmp(cHeadText, "End") != 0)
		{
			CManager::ConvertStringToFloat(cReadText, ",", aData);

			pTree = CTree::Create(TREETYPE_NORMAL);
			pTree->SetPosition(D3DXVECTOR3(aData[0] - 15.0f, aData[1] + 200.0f, aData[2]));			// �|�W�V���������߂�

			fgets(cReadText, sizeof(cReadText), pFile);				// �s���΂�
			sscanf(cReadText, "%s", &cHeadText);
		}

		fclose(pFile);				// �t�@�C�������
	}
	else
	{
		MessageBox(NULL, "���f�����̃A�N�Z�X���s�I", "WARNING", MB_ICONWARNING);	// ���b�Z�[�W�{�b�N�X�̐���
	}
}

//=============================================================================
// ��ނ̐ݒ�֐�
//=============================================================================
void CTree::SetType(TREETYPE type)
{
	m_type = type;
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void CTree::MakeVertex(void)
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
	pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	//�e�N�X�`���`�ʂ̈ʒu
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�f�[�^�̃A�����b�N
	m_pVtxBuff->Unlock();
}

#ifdef _DEBUG
//=============================================================================
// �f�o�b�O����
//=============================================================================
void CTree::Debug(void)
{

}
#endif