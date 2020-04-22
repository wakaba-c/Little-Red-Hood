//=============================================================================
//
// ���b�V���L���[�u���� [meshCube.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "meshCube.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
PDIRECT3DTEXTURE9 CMeshCube::m_pTexture = NULL;				// �e�N�X�`�����̃|�C���^

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMeshCube::CMeshCube(CScene::PRIORITY obj = CScene::PRIORITY_MESH) : CScene(obj)
{
	SetObjType(CScene::PRIORITY_MESH);						// �I�u�W�F�N�g�^�C�v

	// �ʒu�E��]�̏����ݒ�
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// ��]�̏�����
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);				// �F�̏�����
	m_size = D3DXVECTOR3(100.0f, 100.0f, 100.0f);			// �T�C�Y�̏�����
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMeshCube::~CMeshCube()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CMeshCube::Init(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();	// �f�o�C�X�̎擾

	//�����_��
	m_nNumVertex = (DEPTH + 1) * (WIDE + 1);

	//�C���f�b�N�X��
	m_nNumIndex = ((DEPTH + 1) * 2) * DEPTH + ((WIDE - 1) * 2);

	//���|���S����
	m_nNumPolygon = DEPTH * WIDE * 2 + 4 * (DEPTH - 1);

	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVertex, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	//�C���f�b�N�X�o�b�t�@�𐶐�
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIndex, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIdxBuff, NULL);

	int nPolygon = DEPTH * WIDE * 2;

	//���_���̍쐬
	MakeVertex();

	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CMeshCube::Uninit(void)
{
	// �e�N�X�`���̊J��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	// �C���f�b�N�X�o�b�t�@�̊J��
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

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
void CMeshCube::Update(void)
{
#ifdef _DEBUG
	Debug();
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void CMeshCube::Draw(void)
{
	D3DXVECTOR3 pos = GetPosition();
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX	mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

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

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVertex, 0, m_nNumPolygon);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetTexture(0, NULL);
}

//=============================================================================
// �N���G�C�g����
//=============================================================================
CMeshCube *CMeshCube::Create(void)
{
	CMeshCube *pMeshCube;

	pMeshCube = new CMeshCube(CScene::PRIORITY_MESH);
	pMeshCube->Init();
	return pMeshCube;
}

//=============================================================================
// ���[�h�֐�
//=============================================================================
HRESULT CMeshCube::Load(void)
{
	return S_OK;
}

//=============================================================================
// �e�N�X�`���̉���֐�
//=============================================================================
void CMeshCube::MakeVertex(void)
{
	VERTEX_3D *pVtx;
	WORD *pIdx;									//�C���f�b�N�X�f�[�^�փ|�C���^

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-m_size.x / 2, -m_size.y / 2,  -m_size.z / 2);
	pVtx[1].pos = D3DXVECTOR3(m_size.x / 2, -m_size.y / 2,  -m_size.z / 2);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x / 2, -m_size.y / 2,  m_size.z / 2);
	pVtx[3].pos = D3DXVECTOR3(m_size.x / 2, -m_size.y / 2,  m_size.z / 2);

	pVtx[4].pos = D3DXVECTOR3(-m_size.x / 2, m_size.y / 2,  m_size.z / 2);
	pVtx[5].pos = D3DXVECTOR3(m_size.x / 2, m_size.y / 2,  m_size.z / 2);

	pVtx[6].pos = D3DXVECTOR3(-m_size.x / 2, m_size.y / 2,  -m_size.z / 2);
	pVtx[7].pos = D3DXVECTOR3(m_size.x / 2, m_size.y / 2,  -m_size.z / 2);

	pVtx[8].pos = D3DXVECTOR3(-m_size.x / 2, -m_size.y / 2,  -m_size.z / 2);
	pVtx[9].pos = D3DXVECTOR3(m_size.x / 2, -m_size.y / 2,  -m_size.z / 2);

	//�@��
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	pVtx[4].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[5].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	pVtx[6].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[7].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	pVtx[8].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[9].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//���_�J���[
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	pVtx[4].col = m_col;
	pVtx[5].col = m_col;

	pVtx[6].col = m_col;
	pVtx[7].col = m_col;

	pVtx[8].col = m_col;
	pVtx[9].col = m_col;

	//�e�N�X�`���`�ʂ̈ʒu
	pVtx[0].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx[4].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx[6].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx[8].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[9].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx++;

	//���_�f�[�^�̃A�����b�N
	m_pVtxBuff->Unlock();

	int nCnt = 0;

	//�C���f�b�N�X�o�b�t�@�����b�N���A�C���f�b�N�X�f�[�^�ւ̃|�C���^
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntY = 0; nCntY < 5; nCntY++)
	{
		pIdx[0] = 2 * nCntY;
		pIdx[1] = 2 * nCntY + 1;
		pIdx += 2;
		nCnt += 2;
	}

	pIdx[0] = 9;
	pIdx[1] = 7;
	pIdx += 2;

	pIdx[0] = 7;
	pIdx[1] = 5;
	pIdx[2] = 1;
	pIdx[3] = 3;
	pIdx += 4;

	pIdx[0] = 3;
	pIdx[1] = 4;
	pIdx += 2;

	pIdx[0] = 4;
	pIdx[1] = 6;
	pIdx[2] = 2;
	pIdx[3] = 0;

	//�C���f�b�N�X�o�b�t�@�����b�N���A�C���f�b�N�X�f�[�^�ւ̃|�C���^
	m_pIdxBuff->Unlock();
}

//=============================================================================
// �F�����߂�֐�
//=============================================================================
void CMeshCube::SetColor(D3DXCOLOR col)
{
	m_col = col;								// �F�̑��
}

//=============================================================================
// �T�C�Y�����߂�֐�
//=============================================================================
void CMeshCube::SetSize(D3DXVECTOR3 size)
{
	m_size = size;								// �T�C�Y�̑��
}

#ifdef _DEBUG
//=============================================================================
// �f�o�b�O����
//=============================================================================
void CMeshCube::Debug(void)
{

}
#endif