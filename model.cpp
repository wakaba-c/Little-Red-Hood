//=============================================================================
//
// ���f������ [model.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "light.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CModel::CModel()
{
	m_pBuffMat = NULL;
	m_pMesh = NULL;
	m_nNumMat = 0;
	m_pTexture = NULL;
	m_bUse = true;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CModel::Init(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X���擾����
	pDevice = pRenderer->GetDevice();

	//�l�̏�����
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	return S_OK;
}

//=============================================================================
// �X�V����
//=============================================================================
void CModel::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CModel::Draw(D3DXMATRIX *mtxWorld)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;

	D3DXMATRIX		mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X
	D3DXMATERIAL	*pMat;							//���݂̃}�e���A���ۑ��p
	D3DMATERIAL9	matDef;							//�}�e���A���f�[�^�ւ̃|�C���^

	//�f�o�C�X���擾����
	pDevice = pRenderer->GetDevice();

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//�e�q�֌W���������Ă���ꍇ
	if (m_pParent == NULL)
	{
		//�}�g���b�N�X�̍���
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, mtxWorld);
	}
	else
	{
		//�}�g���b�N�X�̍���
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_pParent->m_mtxWorld);
	}

	CScene::SetShadow(pDevice, m_mtxWorld, m_pBuffMat, m_nNumMat, m_pMesh, m_pos);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);					//���ׂẴf�[�^���^

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	if (m_bUse)
	{
		for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
		{
			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, CManager::GetResource(pMat[nCntMat].pTextureFilename));
			}

			// �`��
			m_pMesh->DrawSubset(nCntMat);
		}
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);

	// �e�N�X�`�����f�t�H���g�ɖ߂�
	pDevice->SetTexture(0, NULL);
}

//=============================================================================
// �J������
//=============================================================================
void CModel::Uninit(void)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CModel::~CModel()
{

}

//=============================================================================
// �N���G�C�g����
//=============================================================================
CModel *CModel::Create(char sAdd[64])
{
	CModel *pModel;
	pModel = new CModel();

	if (pModel != NULL)
	{// ���f�������݂��Ă����Ƃ�
		pModel->Load(sAdd);				// ���f���̃��[�h
		pModel->Init();					// ����������

	}
	return pModel;
}

//=============================================================================
// ���f���̃��[�h�֐�
//=============================================================================
HRESULT CModel::Load(char sAdd[64])
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = NULL;

	//�f�o�C�X���擾����
	pDevice = pRenderer->GetDevice();

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(sAdd, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_nNumMat, &m_pMesh);

	D3DXMATERIAL	*pMat;							//���݂̃}�e���A���ۑ��p

		// �}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// �}�e���A���̃e�N�X�`���ǂݍ���
	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{// �e�N�X�`�����������Ƃ�
			char *pFileName = NULL;
			pFileName = pMat[nCntMat].pTextureFilename;			// �e�N�X�`���̃A�h���X���擾
			CManager::GetResource(pFileName);		// �e�N�X�`���̓ǂݍ���
		}
	}

	return S_OK;
}

//=============================================================================
// �e�̐ݒ�
//=============================================================================
void CModel::SetParent(CModel *pModel)
{
	m_pParent = pModel;							//�e�̃|�C���^���i�[
}

//=============================================================================
// ID�̐ݒ�
//=============================================================================
void CModel::SetIndex(int nValue)
{
	m_nIndex = nValue;							//�l�̊i�[
}

//=============================================================================
// �|�W�V�����Z�b�g
//=============================================================================
void CModel::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;		//�|�W�V�����̒l���X�V
}

//=============================================================================
// �|�W�V�����v���Z�b�g�ݒ�
//=============================================================================
void CModel::SetPosPreset(D3DXVECTOR3 pos)
{
	m_originPos = pos;		//�|�W�V�����̒l���X�V
}

//=============================================================================
// ��]�ʂ̐ݒ�
//=============================================================================
void CModel::SetRotation(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//=============================================================================
// ��]�ʂ̃v���Z�b�g��ݒ�
//=============================================================================
void CModel::SetRotPreset(D3DXVECTOR3 rot)
{
	m_originRot = rot;
}

//=============================================================================
// �X�V�`��Ώېݒ�
//=============================================================================
void CModel::SetActive(bool bValue)
{
	m_bUse = bValue;
}