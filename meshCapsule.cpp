//=============================================================================
//
// ���b�V���J�v�Z������ [meshCapsule.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "meshCapsule.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
PDIRECT3DTEXTURE9 CMeshCapsule::m_pTexture = NULL;				// �e�N�X�`�����̃|�C���^

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMeshCapsule::CMeshCapsule(CScene::PRIORITY obj = CScene::PRIORITY_BG) : CScene(obj)
{
	SetObjType(CScene::PRIORITY_BG);							// �I�u�W�F�N�g�^�C�v�̐ݒ�

	// �ʒu�E��]�̏����ݒ�
	m_size = D3DXVECTOR3(0.0f, 100.0f, 0.0f);					// �T�C�Y
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// ��]
	m_col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.3f);					// �F
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMeshCapsule::~CMeshCapsule()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CMeshCapsule::Init(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();				// �`����̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();			// �f�o�C�X�̎擾

	// ================== �l�p�` ================== //

	m_nNumVertex = (CAPSULE_DIVISION + 1) * (CAPSULE_DIVISION + 1);				//�����_��
	m_nNumIndex = ((CAPSULE_DIVISION + 1) * 2) * CAPSULE_DIVISION + ((CAPSULE_DIVISION - 1) * 2);		//�C���f�b�N�X��

	//���|���S����	�������� * ������ * 2 + �k�ރ|���S���� * ������(�Ō�̗�͏���)
	m_nNumPolygon = CAPSULE_DIVISION * CAPSULE_DIVISION * 2 + 4 * (CAPSULE_DIVISION - 1);

	// ================== �~�` ================== //

	m_nNumVertexSphere = CAPSULE_DIVISION + 2;								//�����_��
	m_nNumIndexSphere = CAPSULE_DIVISION;									//�C���f�b�N�X��
	m_nNumPolygonSphere = CAPSULE_DIVISION;									//���|���S����

	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * ((m_nNumVertexSphere * 2) + m_nNumVertex), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffSphere, NULL);

	// �C���f�b�N�X�o�b�t�@�𐶐�
	pDevice->CreateIndexBuffer(sizeof(WORD) * (((m_nNumIndexSphere + 2) * 2) + m_nNumIndex) + 1, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIdxBuffSphere, NULL);

	// ���_���̊i�[
	MakeVertex();

	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CMeshCapsule::Uninit(void)
{
	// �e�N�X�`���̊J��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	// �C���f�b�N�X�o�b�t�@�̊J��
	if (m_pIdxBuffSphere != NULL)
	{
		m_pIdxBuffSphere->Release();
		m_pIdxBuffSphere = NULL;
	}

	// ���_�o�b�t�@�̊J��
	if (m_pVtxBuffSphere != NULL)
	{
		m_pVtxBuffSphere->Release();
		m_pVtxBuffSphere = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CMeshCapsule::Update(void)
{
#ifdef _DEBUG
	Debug();
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void CMeshCapsule::Draw(void)
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
	pDevice->SetStreamSource(0, m_pVtxBuffSphere, 0, sizeof(VERTEX_3D));

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuffSphere);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��		��1�ԏ�
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, m_nNumVertexSphere, 0, m_nNumPolygonSphere);

	// �|���S���̕`��		��2�Ԗڈȍ~�`�Ō��1����O�܂�
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVertex - m_nNumVertexSphere, m_nNumIndexSphere + 2, m_nNumPolygon - m_nNumPolygonSphere * 2);

	//�|���S���̕`��		���Ō�
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, m_nNumVertex + m_nNumVertexSphere, m_nNumIndex - m_nNumPolygonSphere + 2, m_nNumPolygonSphere);
}

//=============================================================================
// �N���G�C�g����
//=============================================================================
CMeshCapsule *CMeshCapsule::Create(void)
{
	CMeshCapsule *pMeshCapsule;
	pMeshCapsule = new CMeshCapsule(CScene::PRIORITY_BG);

	if (pMeshCapsule != NULL)
	{// ���b�V���J�v�Z�������݂��Ă����Ƃ�
		pMeshCapsule->Init();									// ����������
	}
	return pMeshCapsule;
}

//=============================================================================
// ���[�h����
//=============================================================================
HRESULT CMeshCapsule::Load(void)
{
	return S_OK;
}

//=============================================================================
// �F�����߂�֐�
//=============================================================================
void CMeshCapsule::SetColor(D3DXCOLOR col)
{
	m_col = col;								// �F�̑��
}

//=============================================================================
// �T�C�Y�����߂�֐�
//=============================================================================
void CMeshCapsule::SetSize(D3DXVECTOR3 size)
{
	m_size = size;								// �T�C�Y�̑��
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void CMeshCapsule::MakeVertex(void)
{
	D3DXVECTOR3 pos = GetPosition();
	VERTEX_3D *pVtx;							// ���_���ւ̃|�C���^
	WORD *pIdx;									// �C���f�b�N�X�f�[�^�ւ̃|�C���^

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuffSphere->Lock(0, 0, (void**)&pVtx, 0);

	// ================= �~�̒��S =================== //
	pVtx[0].pos = D3DXVECTOR3(pos.x, (pos.y - m_size.y / 2) - RADIUS / 2, pos.z);

	// �@���x�N�g��
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// ���_�J���[
	pVtx[0].col = m_col;

	// �e�N�X�`���`�ʂ̈ʒu
	pVtx[0].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx++;

	// ================= �~���`�� =================== //
	for (int nDepth = 0; nDepth <= CAPSULE_DIVISION; nDepth++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(pos.x / 2 + sinf(D3DX_PI - ((D3DX_PI * 2 / CAPSULE_DIVISION) * nDepth)) * sinf(D3DX_PI + ((D3DX_PI / (CAPSULE_DIVISION + 1)) * 1)) * RADIUS / 2,
			(pos.y - m_size.y / 2) - RADIUS - cosf(D3DX_PI + ((D3DX_PI / (CAPSULE_DIVISION)) * 1)) * RADIUS / 2,
			pos.z / 2 + cosf(D3DX_PI - ((D3DX_PI * 2 / CAPSULE_DIVISION) * nDepth)) * sinf(D3DX_PI + ((D3DX_PI / (CAPSULE_DIVISION + 1)) * 1)) * RADIUS / 2);

		// �@���x�N�g��
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// ���_�J���[
		pVtx[0].col = m_col;

		// �e�N�X�`���`�ʂ̈ʒu
		pVtx[0].tex = D3DXVECTOR2(1.0f, 1.0f * nDepth);

		// ���_�f�[�^�̃|�C���^��1���i�߂�
		pVtx++;
	}

	// �l�p�`�̃��b�V�����~�`�ɂ���
	for (int nDepth = 1; nDepth < CAPSULE_DIVISION + 1; nDepth++)
	{
		for (int nWide = 0; nWide < CAPSULE_DIVISION + 1; nWide++)
		{
			// ���������߂�
			float fWork = sinf(D3DX_PI + ((D3DX_PI / (CAPSULE_DIVISION + 1)) * nDepth)) * RADIUS / 2;

			if (nDepth < (CAPSULE_DIVISION + 1) / 2)
			{
				// ���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(pos.x / 2 + sinf(D3DX_PI + ((D3DX_PI * 2 / CAPSULE_DIVISION) * nWide)) * fWork,
					(pos.y - m_size.y / 2) + cosf(D3DX_PI + ((D3DX_PI / (CAPSULE_DIVISION + 1)) * nDepth)) * RADIUS / 2,
					pos.z / 2 + cosf(D3DX_PI + ((D3DX_PI * 2 / CAPSULE_DIVISION) * nWide)) * fWork);
			}

			else if (nDepth >= (CAPSULE_DIVISION + 1) / 2)
			{
				// ���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(pos.x / 2 + sinf(D3DX_PI + ((D3DX_PI * 2 / CAPSULE_DIVISION) * nWide)) * fWork,
					(pos.y + m_size.y / 2) + cosf(D3DX_PI + ((D3DX_PI / (CAPSULE_DIVISION + 1)) * nDepth)) * RADIUS / 2,
					pos.z / 2 + cosf(D3DX_PI + ((D3DX_PI * 2 / CAPSULE_DIVISION) * nWide)) * fWork);
			}

			// �@��
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// ���_�J���[
			pVtx[0].col = m_col;

			//�e�N�X�`���`�ʂ̈ʒu
			pVtx[0].tex = D3DXVECTOR2(1.0f * nWide, 1.0f * nDepth);

			// ���_�f�[�^�̃|�C���^��1���i�߂�
			pVtx++;
		}
	}

	// ================= �~�̒��S =================== //

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(pos.x, (pos.y + m_size.y / 2) + RADIUS / 2, pos.z);

	//�@���x�N�g��
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//���_�J���[
	pVtx[0].col = m_col;

	//�e�N�X�`���`�ʂ̈ʒu
	pVtx[0].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�f�[�^�̃|�C���^��1���i�߂�
	pVtx++;

	// ================= �~���`�� =================== //
	for (int nDepth = 0; nDepth <= CAPSULE_DIVISION; nDepth++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(pos.x / 2 + sinf(D3DX_PI + ((D3DX_PI * 2 / CAPSULE_DIVISION) * nDepth)) * sinf(D3DX_PI + ((D3DX_PI / (CAPSULE_DIVISION + 1)) * 1)) * RADIUS / 2,
			(pos.y + m_size.y / 2) + cosf(D3DX_PI + ((D3DX_PI / (CAPSULE_DIVISION)) * 1)) * -RADIUS / 2,
			pos.z / 2 + cosf(D3DX_PI - ((D3DX_PI * 2 / CAPSULE_DIVISION) * nDepth)) * sinf(D3DX_PI + ((D3DX_PI / (CAPSULE_DIVISION + 1)) * 1)) * RADIUS / 2);

		//�@���x�N�g��
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[
		pVtx[0].col = m_col;

		//�e�N�X�`���`�ʂ̈ʒu
		pVtx[0].tex = D3DXVECTOR2(1.0f, 1.0f * nDepth);

		pVtx++;
	}

	// 2�i�ڈȍ~�̂��Ƃł��B

	// ���_�f�[�^�̃A�����b�N
	m_pVtxBuffSphere->Unlock();

	// �C���f�b�N�X�o�b�t�@�����b�N���A�C���f�b�N�X�f�[�^�ւ̃|�C���^
	m_pIdxBuffSphere->Lock(0, 0, (void**)&pIdx, 0);
	int nCount = 0;

	// ���_,1�i�ځ`
	for (nCount = 0; nCount < m_nNumVertexSphere; nCount++)
	{
		pIdx[0] = nCount;
		pIdx++;
	}

	// �k�ރ|���S��
	pIdx[0] = nCount;
	pIdx++;

	// 2�i�ڈȍ~
	for (nCount = 1; nCount < CAPSULE_DIVISION; nCount++)
	{
		// �k�ރ|���S��
		pIdx[0] = (CAPSULE_DIVISION + 1) + nCount * (CAPSULE_DIVISION + 1);
		pIdx++;

		for (int nCntX = 0; nCntX < CAPSULE_DIVISION + 1; nCntX++)
		{
			pIdx[0] = (CAPSULE_DIVISION + 1) + nCntX + nCount * (CAPSULE_DIVISION + 1);
			pIdx[1] = nCntX + nCount * (CAPSULE_DIVISION + 1) + 1;
			pIdx += 2;
		}

		// �k�ރ|���S��
		pIdx[0] = CAPSULE_DIVISION + (nCount + 1) * (CAPSULE_DIVISION + 1);
		pIdx++;
	}

	// �k�ރ|���S��
	pIdx[0] = (CAPSULE_DIVISION + 1) + nCount * (CAPSULE_DIVISION + 1);
	pIdx++;

	// �l�p�`�̑����_����1�v���X����
	nCount = m_nNumVertex + 1;

	// �k�ރ|���S��
	pIdx[0] = nCount;
	pIdx++;

	// �Ōォ��1�Ԏ�O�ƍŌ�̒��_
	for (nCount += 1; nCount < m_nNumVertex + m_nNumVertexSphere + 1; nCount++)
	{
		// ���݂̔ԍ���1�v���X����
		pIdx[0] = nCount;
		pIdx++;
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N
	m_pIdxBuffSphere->Unlock();
}

#ifdef _DEBUG
//=============================================================================
// �f�o�b�O����
//=============================================================================
void CMeshCapsule::Debug(void)
{

}
#endif