//=============================================================================
//
// �O�Տ��� [meshOrbit.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "meshOrbit.h"
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "model.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
PDIRECT3DTEXTURE9 CMeshOrbit::m_pTexture = NULL;				// �e�N�X�`�����̏�����
D3DXVECTOR3 point[2 * (ORBIT_WIDE + 1)];						// �|�C���g

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMeshOrbit::CMeshOrbit(CScene::PRIORITY obj = CScene::PRIORITY_ORBIT) : CScene(obj)
{
	SetObjType(CScene::PRIORITY_ORBIT);								// �I�u�W�F�N�g�^�C�v�̐ݒ�
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMeshOrbit::~CMeshOrbit()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CMeshOrbit::Init(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();	// �f�o�C�X�̎擾

	// �ʒu�E��]�̏����ݒ�
	m_aOrbit.pVtxBuffOrbit = NULL;
	m_aOrbit.pIdxBuffMeshOrbit = NULL;
	m_aOrbit.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_aOrbit.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bUse = false;

	for (int nCount = 0; nCount < ORBIT_WIDE; nCount++)
	{
		point[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		point[(ORBIT_WIDE - 1) + nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	//�����_��
	m_aOrbit.nNumVertexMeshOrbit = (ORBIT_DEPTH + 1) * (ORBIT_DEPTH + 1);

	//�C���f�b�N�X��
	m_nNumIndexOrbit = ((ORBIT_DEPTH + 1) * 2) * ORBIT_DEPTH + ((ORBIT_WIDE - 1) * 2);

	//���|���S����
	m_aOrbit.nNumPolygonMeshOrbit = ORBIT_DEPTH * ORBIT_WIDE * 2 + 4 * (ORBIT_DEPTH - 1);

	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (ORBIT_DEPTH + 1) * (ORBIT_WIDE + 1), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_aOrbit.pVtxBuffOrbit, NULL);

	//�C���f�b�N�X�o�b�t�@�𐶐�
	pDevice->CreateIndexBuffer(sizeof(WORD) * ORBIT_DEPTH * ORBIT_WIDE * (2 + 4), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_aOrbit.pIdxBuffMeshOrbit, NULL);

	m_aMtxMeshOrbit.pos = D3DXVECTOR3(0.0f, 130.0f, 0.0f);

	// ���_���̍쐬
	MakeVertex();

	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CMeshOrbit::Uninit(void)
{
	//�C���f�b�N�X�o�b�t�@�̊J��
	if (m_aOrbit.pIdxBuffMeshOrbit != NULL)
	{
		m_aOrbit.pIdxBuffMeshOrbit->Release();
		m_aOrbit.pIdxBuffMeshOrbit = NULL;
	}

	// ���_�o�b�t�@�̊J��
	if (m_aOrbit.pVtxBuffOrbit != NULL)
	{
		m_aOrbit.pVtxBuffOrbit->Release();
		m_aOrbit.pVtxBuffOrbit = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CMeshOrbit::Update(void)
{
	VERTEX_3D *pVtx;																// ���_���̃|�C���^
	CPlayer *pPlayer;																// �v���C���[�̃|�C���^

	pPlayer = CGame::GetPlayer();													// �v���C���[�̎擾
	CModel *pModel = pPlayer->GetModel();											// ���f���̎擾

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_aOrbit.pVtxBuffOrbit->Lock(0, 0, (void**)&pVtx, 0);

	// �ŐV�̋O�Օ`��ʒu���擾
	pVtx[0].pos = D3DXVECTOR3(m_mtxWorldWeapon._41, m_mtxWorldWeapon._42, m_mtxWorldWeapon._43);
	pVtx[ORBIT_WIDE + 1].pos = D3DXVECTOR3(pModel[14].GetMtxWorld()._41, pModel[14].GetMtxWorld()._42, pModel[14].GetMtxWorld()._43);

	point[0] = D3DXVECTOR3(m_mtxWorldWeapon._41, m_mtxWorldWeapon._42, m_mtxWorldWeapon._43);
	point[ORBIT_WIDE + 1] = D3DXVECTOR3(pModel[14].GetMtxWorld()._41, pModel[14].GetMtxWorld()._42, pModel[14].GetMtxWorld()._43);

	// �O�Ղ̒l������炷
	for (int nCntOrbit = 0; nCntOrbit < ORBIT_WIDE; nCntOrbit++)
	{
		pVtx[nCntOrbit + 1].pos = D3DXVECTOR3(
			m_mtxWorldWeaponOld[nCntOrbit]._41,
			m_mtxWorldWeaponOld[nCntOrbit]._42,
			m_mtxWorldWeaponOld[nCntOrbit]._43);

		pVtx[nCntOrbit + ORBIT_WIDE + 2].pos = D3DXVECTOR3(
			m_mtxWorldMeshOrbitOld[nCntOrbit]._41,
			m_mtxWorldMeshOrbitOld[nCntOrbit]._42,
			m_mtxWorldMeshOrbitOld[nCntOrbit]._43);

		//�T��
		point[nCntOrbit + 1] = D3DXVECTOR3(m_mtxWorldWeaponOld[nCntOrbit]._41, m_mtxWorldWeaponOld[nCntOrbit]._42, m_mtxWorldWeaponOld[nCntOrbit]._43);

		point[nCntOrbit + ORBIT_WIDE + 2] = D3DXVECTOR3(m_mtxWorldMeshOrbitOld[nCntOrbit]._41,
			m_mtxWorldMeshOrbitOld[nCntOrbit]._42,
			m_mtxWorldMeshOrbitOld[nCntOrbit]._43);
	}

	// �ߋ��̃��[���h�}�g���b�N�X�������炷
	for (int nCntObit = ORBIT_WIDE - 1; nCntObit > 0; nCntObit--)
	{
		m_mtxWorldMeshOrbitOld[nCntObit] = m_mtxWorldMeshOrbitOld[nCntObit - 1];
		m_mtxWorldWeaponOld[nCntObit] = m_mtxWorldWeaponOld[nCntObit - 1];
	}

	// �Ō�ɍŐV�̈ʒu������
	m_mtxWorldMeshOrbitOld[0] = pModel[14].GetMtxWorld();

	m_mtxWorldWeaponOld[0] = m_mtxWorldWeapon;

	//���_�f�[�^�̃A�����b�N
	m_aOrbit.pVtxBuffOrbit->Unlock();

#ifdef _DEBUG
	Debug();
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void CMeshOrbit::Draw(void)
{
	// ���[���h�}�g���b�N�X�̏�����
	CPlayer *pPlayer = CGame::GetPlayer();
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();								// �f�o�C�X�̎擾
	CModel *pModel = pPlayer->GetModel();

	D3DXMATRIX	mtxRot, mtxTrans, mtxView, mtxMeshRot, mtxMeshTrans;				//�v�Z�p�}�g���b�N�X

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);							// ����(�����)���J�����O����

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// �����_�[�X�e�[�g(���Z��������)
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorldMeshOrbit);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorldWeapon);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_aOrbit.rot.y, m_aOrbit.rot.x, m_aOrbit.rot.z);
	D3DXMatrixMultiply(&m_mtxWorldMeshOrbit, &m_mtxWorldMeshOrbit, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_aOrbit.pos.x, m_aOrbit.pos.y, m_aOrbit.pos.z);
	D3DXMatrixMultiply(&m_mtxWorldMeshOrbit, &m_mtxWorldMeshOrbit, &mtxTrans);

	//D3DXMatrixMultiply(&m_mtxWorldMeshOrbit, &m_mtxWorldMeshOrbit, &pModel[14].GetMtxWorld());

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, m_aMtxMeshOrbit.rot.y, m_aMtxMeshOrbit.rot.x, m_aMtxMeshOrbit.rot.z);
	D3DXMatrixMultiply(&m_mtxWorldWeapon, &m_mtxWorldWeapon, &mtxMeshRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxMeshTrans, m_aMtxMeshOrbit.pos.x, m_aMtxMeshOrbit.pos.y, m_aMtxMeshOrbit.pos.z);
	D3DXMatrixMultiply(&m_mtxWorldWeapon, &m_mtxWorldWeapon, &mtxMeshTrans);

	D3DXMatrixMultiply(&m_mtxWorldWeapon, &m_mtxWorldWeapon, &pModel[14].GetMtxWorld());

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorldMeshOrbit);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_aOrbit.pVtxBuffOrbit, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_aOrbit.pIdxBuffMeshOrbit);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetTexture(0, m_pTextureOrbit);

	if(m_bUse)
	{
		// �|���S���̕`��
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_aOrbit.nNumVertexMeshOrbit, 0, m_aOrbit.nNumPolygonMeshOrbit);
	}


	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetTexture(0, NULL);

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
CMeshOrbit *CMeshOrbit::Create(void)
{
	CMeshOrbit *pMeshOrbit;									// ���b�V���X�t�B�A�̃|�C���^
	pMeshOrbit = new CMeshOrbit(CScene::PRIORITY_ORBIT);	// �������̓��I�m��
	pMeshOrbit->Init();										// ���b�V���X�t�B�A�̏�����
	return pMeshOrbit;										// ���b�V���X�t�B�A�̃|�C���^��Ԃ�
}

//=============================================================================
// ���[�h����
//=============================================================================
HRESULT CMeshOrbit::Load(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X���擾����
	pDevice = pRenderer->GetDevice();

	//�e�N�X�`���̓ǂݍ���
	m_pTexture = CManager::GetResource("data/tex/orbit.png");

	return S_OK;
}

//=============================================================================
// �O�Ղ�`�悷�邩�ǂ���
//=============================================================================
void CMeshOrbit::SetOrbit(bool bValue)
{
	m_bUse = bValue;							// �l���i�[����
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void CMeshOrbit::MakeVertex(void)
{
	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	WORD *pIdx;									// �C���f�b�N�X�f�[�^�փ|�C���^

		//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_aOrbit.pVtxBuffOrbit->Lock(0, 0, (void**)&pVtx, 0);

	for (int nDepth = 0; nDepth < ORBIT_DEPTH + 1; nDepth++)
	{
		for (int nWide = 0; nWide < ORBIT_WIDE + 1; nWide++)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3((-100.0f * ORBIT_WIDE) / 2 + 100 * nWide, (100.0f * ORBIT_DEPTH) - 100 * nDepth, 0.0f);

			//�@���x�N�g��
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			//���_�J���[
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f - nWide * (1.0f / ORBIT_WIDE));

			//�e�N�X�`���`�ʂ̈ʒu
			pVtx[0].tex = D3DXVECTOR2(1.0f * nWide, 1.0f * nDepth);

			pVtx++;
		}
	}

	//���_�f�[�^�̃A�����b�N
	m_aOrbit.pVtxBuffOrbit->Unlock();

	//�C���f�b�N�X�o�b�t�@�����b�N���A�C���f�b�N�X�f�[�^�ւ̃|�C���^
	m_aOrbit.pIdxBuffMeshOrbit->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntY = 0; nCntY < ORBIT_DEPTH; nCntY++)
	{
		if (nCntY != 0)
		{
			pIdx[0] = (ORBIT_WIDE + 1) + nCntY * (ORBIT_WIDE + 1);
			pIdx++;
		}

		for (int nCntX = 0; nCntX < ORBIT_WIDE + 1; nCntX++)
		{
			pIdx[0] = (ORBIT_WIDE + 1) + nCntX + nCntY * (ORBIT_WIDE + 1);
			pIdx[1] = nCntX + nCntY * (ORBIT_WIDE + 1);
			pIdx += 2;
		}

		if (ORBIT_DEPTH > nCntY + 1)
		{
			pIdx[0] = ORBIT_WIDE + nCntY * (ORBIT_WIDE + 1);
			pIdx++;

		}
	}

	//���_�f�[�^�̃A�����b�N
	m_aOrbit.pIdxBuffMeshOrbit->Unlock();
}

#ifdef _DEBUG
//=============================================================================
// �f�o�b�O����
//=============================================================================
void CMeshOrbit::Debug(void)
{

}
#endif