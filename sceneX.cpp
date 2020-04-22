//=============================================================================
//
// sceneX���� [sceneX.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "sceneX.h"
#include "manager.h"
#include "renderer.h"
#include "light.h"
#include "object.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CSceneX::CSceneX(CScene::PRIORITY obj = CScene::PRIORITY_MODEL) : CScene(obj)
{
	SetObjType(CScene::PRIORITY_MODEL);

	m_pBuffMat = NULL;
	m_pMesh = NULL;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_size = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CSceneX::~CSceneX()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CSceneX::Init(void)
{
	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CSceneX::Uninit(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void CSceneX::Update(void)
{
#ifdef _DEBUG
	Debug();
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void CSceneX::Draw(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	D3DXVECTOR3 pos = GetPosition();

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
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);					//���ׂẴf�[�^���^

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

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

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// �N���G�C�g�֐�
//=============================================================================
CSceneX *CSceneX::Create(void)
{
	CSceneX *pSceneX;
	pSceneX = new CSceneX(CScene::PRIORITY_MODEL);

	if (pSceneX != NULL)
	{// �V�[�������݂��Ă����Ƃ�
		pSceneX->Init();				// ����������
	}
	return pSceneX;
}

//=============================================================================
// ���f�����̐ݒ�
//=============================================================================
void CSceneX::BindModel(LPDIRECT3DVERTEXBUFFER9 pVtxBuff, LPD3DXMESH pMesh, DWORD nNumMat, LPD3DXBUFFER pBuffMat, LPDIRECT3DTEXTURE9 *tex)
{
	m_pVtxBuff = pVtxBuff;
	m_pMesh = pMesh;
	m_nNumMat = nNumMat;
	m_pBuffMat = pBuffMat;
	m_pTexture = tex;
}

//=============================================================================
// �J���[�̐ݒ�
//=============================================================================
void CSceneX::SetColor(D3DXCOLOR col)
{
	m_col = col;
}

//=============================================================================
// �T�C�Y�̐ݒ�
//=============================================================================
void CSceneX::SetSize(D3DXVECTOR3 size)
{
	m_size = size;
}

//=============================================================================
// ��]�l�̐ݒ�
//=============================================================================
void CSceneX::SetRotation(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

#ifdef _DEBUG
//=============================================================================
// �f�o�b�O����
//=============================================================================
void CSceneX::Debug(void)
{

}
#endif