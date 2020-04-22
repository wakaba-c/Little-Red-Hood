//=============================================================================
//
// �I�u�W�F�N�g���� [object.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "meshField.h"
#include "colliderBox.h"
#include "colliderSphere.h"
#include "camera.h"
#include "house.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
PDIRECT3DTEXTURE9 *CObject::m_pTexture[OBJTYPE_MAX] = {};
LPD3DXMESH		CObject::m_pMesh[OBJTYPE_MAX] = {};
DWORD			CObject::m_nNumMat[OBJTYPE_MAX] = {};
LPD3DXBUFFER		CObject::m_pBuffMat[OBJTYPE_MAX] = {};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CObject::CObject(CScene::PRIORITY obj = CScene::PRIORITY_MODEL) : CSceneX(obj)
{
	SetObjType(CScene::PRIORITY_MODEL);

	m_ColliderBox = NULL;
	m_ColliderSphere = NULL;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CObject::~CObject()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CObject::Init(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();	// �f�o�C�X�̎擾

	// ����������
	CSceneX::Init();

	// ���f���̏����Z�b�g
	BindModel(m_pVtxBuff, m_pMesh[m_Type], m_nNumMat[m_Type], m_pBuffMat[m_Type], m_pTexture[m_Type]);

	D3DXVECTOR3 pos = D3DXVECTOR3(500.0f, 0.0f, 0.0f);

	switch (m_Type)
	{
	case OBJTYPE_FENCE:
		m_ColliderBox = CColliderBox::Create(false, D3DXVECTOR3(550.0f, 120.0f, 50.0f));		// ����
		break;
	}

	if (m_ColliderBox != NULL)
	{// �{�b�N�X�R���C�_�[�����݂��Ă����Ƃ�
		switch (m_Type)
		{
		case OBJTYPE_FENCE:
			m_ColliderBox->SetScene(this);									// �������ݒ�
			m_ColliderBox->SetTag("fence");									// fence��ݒ�
			m_ColliderBox->SetPosition(pos);								// �ʒu�̐ݒ�
			m_ColliderBox->SetOffset(D3DXVECTOR3(0.0f, 60.0f, 0.0f));		// �I�t�Z�b�g�l�̐ݒ�
			break;
		}
	}

	if (m_ColliderSphere != NULL)
	{// �����蔻�肪���݂��Ă����Ƃ�
		m_ColliderSphere->SetScene(this);									// �������ݒ�
		m_ColliderSphere->SetTag("house");									// house��ݒ�
		m_ColliderSphere->SetPosition(pos);									// �ʒu�̐ݒ�
		m_ColliderSphere->SetOffset(D3DXVECTOR3(0.0f, 85.0f, -10.0f));		// �I�t�Z�b�g�l�̐ݒ�
	}

	SetPosition(pos);			// �ʒu�̐ݒ�
	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CObject::Uninit(void)
{
	if (m_ColliderBox != NULL)
	{// �{�b�N�X�R���C�_�[�����݂��Ă����Ƃ�
		m_ColliderBox->Release();			// �폜�\��
		m_ColliderBox = NULL;				// NULL����
	}

	// �J������
	CSceneX::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CObject::Update(void)
{
	D3DXVECTOR3 pos = GetPosition();				// �ʒu�̎擾

	if (m_ColliderBox != NULL)
	{// �{�b�N�X�R���C�_�[�����݂��Ă����Ƃ�
		m_ColliderBox->SetPosition(pos);			// �ʒu�̐ݒ�
	}
	if (m_ColliderSphere != NULL)
	{// �X�t�B�A�R���C�_�[�����݂��Ă����Ƃ�
		m_ColliderSphere->SetPosition(pos);			// �ʒu�̐ݒ�
	}

	SetPosition(pos);		// �ʒu�̐ݒ�
#ifdef _DEBUG
	Debug();				// �f�o�b�O����
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void CObject::Draw(void)
{
	if (m_Type != OBJTYPE_HOUSE_1 && m_Type != OBJTYPE_HOUSE_2 && m_Type != OBJTYPE_HOUSE_3 && m_Type != OBJTYPE_HOUSE_4)
	{
		CSceneX::Draw();
	}
}

//=============================================================================
// �N���G�C�g�֐�
//=============================================================================
CObject *CObject::Create(OBJTYPE type)
{
	CObject *pObject;
	pObject = new CObject(CScene::PRIORITY_MODEL);
	if (pObject != NULL)
	{
		pObject->SetType(type);
		pObject->Init();
	}
	return pObject;
}

//=============================================================================
// ���[�h����
//=============================================================================
HRESULT CObject::Load(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X���擾����
	pDevice = pRenderer->GetDevice();

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data/model/house/house.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_pBuffMat[OBJTYPE_HOUSE_1],
		NULL,
		&m_nNumMat[OBJTYPE_HOUSE_1],
		&m_pMesh[OBJTYPE_HOUSE_1]);

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data/model/house/MedievalHouse_1.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_pBuffMat[OBJTYPE_HOUSE_2],
		NULL,
		&m_nNumMat[OBJTYPE_HOUSE_2],
		&m_pMesh[OBJTYPE_HOUSE_2]);

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data/model/house/MedievalHouse_2.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_pBuffMat[OBJTYPE_HOUSE_3],
		NULL,
		&m_nNumMat[OBJTYPE_HOUSE_3],
		&m_pMesh[OBJTYPE_HOUSE_3]);

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data/model/house/MedievalHouse_3.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_pBuffMat[OBJTYPE_HOUSE_4],
		NULL,
		&m_nNumMat[OBJTYPE_HOUSE_4],
		&m_pMesh[OBJTYPE_HOUSE_4]);

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data/model/wood1.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_pBuffMat[OBJTYPE_TREE_1],
		NULL,
		&m_nNumMat[OBJTYPE_TREE_1],
		&m_pMesh[OBJTYPE_TREE_1]);

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data/model/wood2.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_pBuffMat[OBJTYPE_TREE_2],
		NULL,
		&m_nNumMat[OBJTYPE_TREE_2],
		&m_pMesh[OBJTYPE_TREE_2]);

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data/model/wood3.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_pBuffMat[OBJTYPE_TREE_3],
		NULL,
		&m_nNumMat[OBJTYPE_TREE_3],
		&m_pMesh[OBJTYPE_TREE_3]);

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data/model/fence/fence.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_pBuffMat[OBJTYPE_FENCE],
		NULL,
		&m_nNumMat[OBJTYPE_FENCE],
		&m_pMesh[OBJTYPE_FENCE]);

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data/model/Arrow.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_pBuffMat[OBJTYPE_ARROW],
		NULL,
		&m_nNumMat[OBJTYPE_ARROW],
		&m_pMesh[OBJTYPE_ARROW]);

	D3DXMATERIAL	*pMat;							//���݂̃}�e���A���ۑ��p

	for (int nCount = 0; nCount < OBJTYPE_MAX; nCount++)
	{
		if (nCount != OBJTYPE_HOUSE_1 && nCount != OBJTYPE_HOUSE_2 && nCount != OBJTYPE_HOUSE_3 && nCount != OBJTYPE_HOUSE_4)
		{
			// �}�e���A�����ɑ΂���|�C���^���擾
			pMat = (D3DXMATERIAL*)m_pBuffMat[nCount]->GetBufferPointer();

			// �}�e���A���̃e�N�X�`���ǂݍ���
			for (int nCntMat = 0; nCntMat < (int)m_nNumMat[nCount]; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{// �e�N�X�`�����������Ƃ�
					char *pFileName = NULL;
					pFileName = pMat[nCntMat].pTextureFilename;			// �e�N�X�`���̃A�h���X���擾
					CManager::Load(pFileName);		// �e�N�X�`���̓ǂݍ���
				}
			}
		}
	}
	return S_OK;
}

//=============================================================================
// �e�N�X�`���̉���֐�
//=============================================================================
void CObject::Unload(void)
{
	// �e�N�X�`���̊J��
	for (int nCntTex = 0; nCntTex < OBJTYPE_MAX; nCntTex++)
	{
		if (m_pTexture[nCntTex] != NULL)
		{
			for (int nCount = 0; nCount < (int)m_nNumMat[nCntTex]; nCount++)
			{
				if (m_pTexture[nCntTex][nCount] != NULL)
				{
					m_pTexture[nCntTex][nCount]->Release();
				}
			}
			delete[] m_pTexture[nCntTex];
			m_pTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
// ���f���^�C�v�̐ݒ�
//=============================================================================
void CObject::SetType(OBJTYPE type)
{
	m_Type = type;
}

//=============================================================================
// ���f���̓Ǎ�
//=============================================================================
void CObject::LoadModel(char *add)
{
	FILE *pFile = NULL;											// �t�@�C��
	char cReadText[128] = {};									// ����
	char cHeadText[128] = {};									// ��r
	float fData[7];												// ����
	CObject *pObject = NULL;

	pFile = fopen(add, "r");									// �t�@�C�����J���܂��͍��

	if (pFile != NULL)											// �t�@�C�����ǂݍ��߂��ꍇ
	{
		fgets(cReadText, sizeof(cReadText), pFile);				// �s���΂�
		fgets(cReadText, sizeof(cReadText), pFile);				// �s���΂�
		fgets(cReadText, sizeof(cReadText), pFile);				// �s���΂�

		while (strcmp(cHeadText, "End") != 0)
		{
			CManager::ConvertStringToFloat(cReadText, ",", fData);

			pObject = CObject::Create((OBJTYPE)(int)fData[6]);
			pObject->SetPosition(D3DXVECTOR3(fData[0], fData[1], fData[2]));			// �ʒu�̐ݒ�
			pObject->SetRotation(D3DXVECTOR3(fData[3], fData[4], fData[5]));			// ��]�l��ݒ�

			fgets(cReadText, sizeof(cReadText), pFile);									// �s���΂�
			sscanf(cReadText, "%s", &cHeadText);
		}

		fclose(pFile);					// �t�@�C�������
	}
	else
	{
		MessageBox(NULL, "���f�����̃A�N�Z�X���s�I", "WARNING", MB_ICONWARNING);	// ���b�Z�[�W�{�b�N�X�̐���
	}
}

#ifdef _DEBUG
//=============================================================================
// �f�o�b�O����
//=============================================================================
void CObject::Debug(void)
{

}
#endif