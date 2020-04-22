//=============================================================================
//
// ���b�V���t�B�[���h���� [meshField.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "meshField.h"
#include "manager.h"
#include "renderer.h"
#include "inputMouse.h"
#include "inputKeyboard.h"
#include "camera.h"
#include "circle.h"
#include "tree.h"
#include "collider.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMeshField::CMeshField(CScene::PRIORITY obj = CScene::PRIORITY_FLOOR) : CScene(obj)
{
	SetObjType(CScene::PRIORITY_FLOOR);

	// �ʒu�E��]�̏����ݒ�
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 1.0f, 0.4f, 1.0f);

	for (int nCount = 0; nCount < DEPTH_FIELD * WIDE_FIELD * 2; nCount++)
	{
		m_randType[nCount] = RANDTYPE_NONE;
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMeshField::~CMeshField()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CMeshField::Init(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();	// �f�o�C�X�̎擾

   //�����_��
	m_nNumVertex = (DEPTH_FIELD + 1) * (WIDE_FIELD + 1);

	//�C���f�b�N�X��
	m_nNumIndex = ((DEPTH_FIELD + 1) * 2) * DEPTH_FIELD + ((WIDE_FIELD - 1) * 2);

	//���|���S����
	m_nNumPolygon = DEPTH_FIELD * WIDE_FIELD * 2 + 4 * (DEPTH_FIELD - 1);

	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVertex, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	//�C���f�b�N�X�o�b�t�@�𐶐�
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIndex, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIdxBuff, NULL);

	int nPolygon = DEPTH_FIELD * WIDE_FIELD * 2;

	// ���_���̍쐬
	MakeVertex(NULL, false);

	// ���_�C���f�b�N�X�̍쐬
	MakeIndex();

	// �@���̌v�Z
	CalculationNormalize();

	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CMeshField::Uninit(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void CMeshField::Update(void)
{
#ifdef _DEBUG
	// �f�o�b�O����
	Debug();
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void CMeshField::Draw(void)
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
	pDevice->SetTexture(0, NULL);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVertex, 0, m_nNumPolygon);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetTexture(0, NULL);
}

//=============================================================================
// �N���G�C�g����
//=============================================================================
CMeshField *CMeshField::Create(void)
{
	CMeshField *pMeshField;
	pMeshField = new CMeshField(CScene::PRIORITY_FLOOR);

	if (pMeshField != NULL)
	{// ���b�V���t�B�[���h�����݂��Ă����Ƃ�
		pMeshField->Init();			// ����������
	}
	return pMeshField;
}

//=============================================================================
// ���[�h����
//=============================================================================
HRESULT CMeshField::Load(void)
{
	return S_OK;
}

//=============================================================================
// �n�`����Ǎ��֐�
//=============================================================================
void CMeshField::LoadRand(char *add, bool bDebug)
{
	FILE *pFile = NULL;																	// �t�@�C��
	char cReadText[128];															// ����
	char cHeadText[128];															// ��r
	float aData[7];																	// ����

	CScene *pScene = CScene::GetScene(PRIORITY_FLOOR);					// ���̐擪�A�h���X�̎擾
	CScene *pSceneNext = NULL;														// ������
	CMeshField *pField = NULL;

	pFile = fopen(add, "r");				// �t�@�C�����J���܂��͍��

	if (pFile != NULL)						//�t�@�C�����ǂݍ��߂��ꍇ
	{
		while (strcmp(cHeadText, "End") != 0)
		{
			fgets(cReadText, sizeof(cReadText), pFile);									//�ŏ��̍s���΂�
			sscanf(cReadText, "%s", &cHeadText);

			if (pScene != NULL)
			{
				pSceneNext = CScene::GetSceneNext(pScene, CScene::PRIORITY_FLOOR);		//����A�b�v�f�[�g�Ώۂ��T����
				pField = (CMeshField*)pScene;			// �L���X�g
			}

			if (strcmp(cHeadText, "pos") == 0)
			{
				if (pField == NULL)
				{// ����Ă��Ȃ��Ƃ�
					pField = CMeshField::Create();										// ���̍쐬
				}

				fgets(cReadText, sizeof(cReadText), pFile);								//�ŏ��̍s���΂�
				sscanf(cReadText, "%s", &cHeadText);

				CManager::ConvertStringToFloat(cReadText, ",", aData);

				pField->SetPosition(D3DXVECTOR3(aData[0], aData[1], aData[2]));			// �|�W�V���������߂�
				fgets(cReadText, sizeof(cReadText), pFile);								//�s���΂�

				pField->MakeVertex(pFile, bDebug);										// ���_���̍쐬(�t�@�C������)
				pField->CalculationNormalize();									// �@���̌v�Z
			}

			pField = NULL;
		}

		fclose(pFile);																// �t�@�C�������
	}
	else
	{
		MessageBox(NULL, "�n�ʏ��̃A�N�Z�X���s�I", "WARNING", MB_ICONWARNING);	// ���b�Z�[�W�{�b�N�X�̐���
	}
}

//=============================================================================
// �n�ʂ̎�ނ�ύX
//=============================================================================
void CMeshField::SetTexType(int nValue, RANDTYPE Type)
{
	m_randType[nValue] = Type;
}

//=============================================================================
// �|���S���̏��̍��������߂�
//=============================================================================
float CMeshField::GetHeight(D3DXVECTOR3 pos)
{
	VERTEX_3D *pVtx;										//���_���ւ̃|�C���^

	D3DXVECTOR3 FieldPos = GetPosition();

	D3DXVECTOR3 AB;
	D3DXVECTOR3 BC;

	D3DXVECTOR3 point1;
	D3DXVECTOR3 point2;
	D3DXVECTOR3 point3;

	D3DXVECTOR3 aWork[3];
	D3DXVECTOR3 aPlayer[3];
	float fHeight = 0;
	float fAnswer[3];

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�@�����߂�
	for (int nDepth = 0; nDepth < DEPTH_FIELD; nDepth++)
	{
		for (int nWide = 0; nWide < WIDE_FIELD; nWide++)
		{
			point1 = pVtx[WIDE_FIELD + nWide + 1 + ((WIDE_FIELD + 1) * nDepth)].pos + FieldPos;
			point1.y = 0.0f;
			point2 = pVtx[nWide + ((WIDE_FIELD + 1) * nDepth)].pos + FieldPos;
			point2.y = 0.0f;
			point3 = pVtx[WIDE_FIELD + nWide + 2 + ((WIDE_FIELD + 1) * nDepth)].pos + FieldPos;
			point3.y = 0.0f;

			if (CManager::GetDistance(point1, D3DXVECTOR3(pos.x, 0.0f, pos.z)) <= 4000.0f ||
				CManager::GetDistance(point2, D3DXVECTOR3(pos.x, 0.0f, pos.z)) <= 4000.0f ||
				CManager::GetDistance(point3, D3DXVECTOR3(pos.x, 0.0f, pos.z)) <= 4000.0f)
			{
				//�����̌v�Z
				aWork[0] = (pVtx[WIDE_FIELD + nWide + 1 + ((WIDE_FIELD + 1) * nDepth)].pos + FieldPos) - (pVtx[nWide + ((WIDE_FIELD + 1) * nDepth)].pos + FieldPos);
				aPlayer[0] = pos - (pVtx[WIDE_FIELD + nWide + 1 + ((WIDE_FIELD + 1) * nDepth)].pos + FieldPos);

				aWork[1] = (pVtx[WIDE_FIELD + nWide + 2 + ((WIDE_FIELD + 1) * nDepth)].pos + FieldPos) - (pVtx[WIDE_FIELD + nWide + 1 + ((WIDE_FIELD + 1) * nDepth)].pos + FieldPos);
				aPlayer[1] = pos - (pVtx[WIDE_FIELD + nWide + 2 + ((WIDE_FIELD + 1) * nDepth)].pos + FieldPos);

				aWork[2] = (pVtx[nWide + ((WIDE_FIELD + 1) * nDepth)].pos + FieldPos) - (pVtx[WIDE_FIELD + nWide + 2 + ((WIDE_FIELD + 1) * nDepth)].pos + FieldPos);
				aPlayer[2] = pos - (pVtx[nWide + ((WIDE_FIELD + 1) * nDepth)].pos + FieldPos);

				//�@��
				fAnswer[0] = aWork[0].x * aPlayer[0].z - aWork[0].z * aPlayer[0].x;
				fAnswer[1] = aWork[1].x * aPlayer[1].z - aWork[1].z * aPlayer[1].x;
				fAnswer[2] = aWork[2].x * aPlayer[2].z - aWork[2].z * aPlayer[2].x;

				if ((fAnswer[0] > 0 && fAnswer[1] > 0 && fAnswer[2] > 0) || (fAnswer[0] <= 0 && fAnswer[1] <= 0 && fAnswer[2] <= 0))
				{
					//�R�_���ʖ@
					m_type = m_randType[((WIDE_FIELD * 2) * nDepth) + (2 * nWide)];
					return fHeight = (pVtx[nWide + ((WIDE_FIELD + 1) * nDepth)].pos.y + FieldPos.y) - (1 / apNor[((WIDE_FIELD * 2) * nDepth) + (2 * nWide)].y) * (apNor[((WIDE_FIELD * 2) * nDepth) + (2 * nWide)].x * (pos.x - (pVtx[nWide + ((WIDE_FIELD + 1) * nDepth)].pos.x + FieldPos.x)) + apNor[((WIDE_FIELD * 2) * nDepth) + (2 * nWide)].z * (pos.z - (pVtx[nWide + ((WIDE_FIELD + 1) * nDepth)].pos.z + FieldPos.z)));
				}

				//�����̌v�Z
				aWork[0] = (pVtx[nWide + ((WIDE_FIELD + 1) * nDepth)].pos + FieldPos) - (pVtx[1 + nWide + ((WIDE_FIELD + 1) * nDepth)].pos + FieldPos);
				aPlayer[0] = pos - (pVtx[1 + nWide + ((WIDE_FIELD + 1) * nDepth)].pos + FieldPos);

				aWork[1] = (pVtx[1 + nWide + ((WIDE_FIELD + 1) * nDepth)].pos + FieldPos) - (pVtx[WIDE_FIELD + nWide + 2 + ((WIDE_FIELD + 1) * nDepth)].pos + FieldPos);
				aPlayer[1] = pos - (pVtx[WIDE_FIELD + nWide + 2 + ((WIDE_FIELD + 1) * nDepth)].pos + FieldPos);

				aWork[2] = (pVtx[WIDE_FIELD + nWide + 2 + ((WIDE_FIELD + 1) * nDepth)].pos + FieldPos) - (pVtx[nWide + ((WIDE_FIELD + 1) * nDepth)].pos + FieldPos);
				aPlayer[2] = pos - (pVtx[nWide + ((WIDE_FIELD + 1) * nDepth)].pos + FieldPos);

				//�O��
				fAnswer[0] = aWork[0].x * aPlayer[0].z - aWork[0].z * aPlayer[0].x;
				fAnswer[1] = aWork[1].x * aPlayer[1].z - aWork[1].z * aPlayer[1].x;
				fAnswer[2] = aWork[2].x * aPlayer[2].z - aWork[2].z * aPlayer[2].x;

				//��������v�����Ƃ�
				if ((fAnswer[0] >= 0 && fAnswer[1] >= 0 && fAnswer[2] >= 0) || (fAnswer[0] < 0 && fAnswer[1] < 0 && fAnswer[2] < 0))
				{
					//�R�_���ʖ@
					m_type = m_randType[((WIDE_FIELD * 2) * nDepth) + (2 * nWide) + 1];
					return fHeight = (pVtx[nWide + ((WIDE_FIELD + 1) * nDepth)].pos.y + FieldPos.y) - (1 / apNor[((WIDE_FIELD * 2) * nDepth) + (2 * nWide) + 1].y) * (apNor[((WIDE_FIELD * 2) * nDepth) + (2 * nWide) + 1].x * (pos.x - (pVtx[nWide + ((WIDE_FIELD + 1) * nDepth)].pos.x + FieldPos.x)) + apNor[((WIDE_FIELD * 2) * nDepth) + (2 * nWide) + 1].z * (pos.z - (pVtx[nWide + ((WIDE_FIELD + 1) * nDepth)].pos.z + FieldPos.z)));
				}
			}
		}
	}
	//���_�f�[�^�̃A�����b�N
	m_pVtxBuff->Unlock();

	m_type = RANDTYPE_NONE;
	return fHeight;
}

//=============================================================================
// X����Z���̓����蔻��
//=============================================================================
bool CMeshField::SphereModel(D3DXVECTOR3 C1, D3DXVECTOR3 C2, float R1)
{
	bool bDetection = false;
	//D3DXVECTOR3 C3;							//�v�Z�p
	D3DXVECTOR3 save = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						//�ۑ��p
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if ((C1.x - C2.x) * (C1.x - C2.x) + (C1.z - C2.z) * (C1.z - C2.z) <= R1 * R1)
	{
		bDetection = true;
	}
	return bDetection;
}

//====================================================================
// �t�B�[���h�͈͓����ǂ���
//====================================================================
bool CMeshField::CollisionRange(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 FieldPos = GetPosition();
	bool bIn = false;

	if (FieldPos.x - (WIDE_FIELD * SIZE) / 2 < pos.x)
	{
		if (FieldPos.x + (WIDE_FIELD * SIZE) / 2 > pos.x)
		{
			if (FieldPos.z - (WIDE_FIELD * SIZE) / 2 < pos.z)
			{
				if (FieldPos.z + (WIDE_FIELD * SIZE) / 2 > pos.z)
				{
					bIn = true;
				}
			}
		}
	}

	return bIn;
}

//=============================================================================
// �~�͈͓̔�����
//=============================================================================
bool CMeshField::CollisionRangeToCircle(D3DXVECTOR3 pos, float fRadius)
{
	D3DXVECTOR3 FieldPos = GetPosition();
	bool bIn = false;																// �����蔻�� �̏�����

	// ���E�̔��a���̓����蔻��
	if ((FieldPos.x - (WIDE_FIELD * SIZE) / 2) - fRadius < pos.x)
	{// ���Ɠ_
		if ((FieldPos.x + (WIDE_FIELD * SIZE) / 2) + fRadius > pos.x)
		{// �E�Ɠ_
			if (FieldPos.z - (WIDE_FIELD * SIZE) / 2 < pos.z)
			{// ���Ɠ_
				if (FieldPos.z + (WIDE_FIELD * SIZE) / 2 > pos.z)
				{// ��O�Ɠ_
					bIn = true;														// �����蔻�� �̃t���O�𗧂Ă�
				}
			}
		}
	}
	if (!bIn)
	{// �����蔻�� �̃t���O�������Ă��Ȃ������Ƃ�
		// ���s�����a���̓����蔻��
		if (FieldPos.x - (WIDE_FIELD * SIZE) / 2 < pos.x)
		{// ���Ɠ_
			if (FieldPos.x + (WIDE_FIELD * SIZE) / 2 > pos.x)
			{// �E�Ɠ_
				if ((FieldPos.z - (WIDE_FIELD * SIZE) / 2) - fRadius < pos.z)
				{// ���Ɠ_
					if ((FieldPos.z + (WIDE_FIELD * SIZE) / 2) + fRadius > pos.z)
					{// ��O�Ɠ_
						bIn = true;													// �����蔻�� �̃t���O�𗧂Ă�
					}
				}
			}
		}
	}
	if (!bIn)
	{// �����蔻�� �̃t���O�������Ă��Ȃ������Ƃ�
		// �p�̉~�Ɠ_�ɂ�铖���蔻��
		if (CollisionCircle(pos, fRadius))
		{// �����蔻�� �̃t���O���������Ƃ�
			bIn = true;
		}
	}

	return bIn;																		// �����蔻�� �̃t���O��Ԃ�Ԃ�
}

//=============================================================================
// �_�Ɖ~�̓����蔻��
//=============================================================================
bool CMeshField::CollisionCircle(D3DXVECTOR3 pos, float fRadius)
{
	D3DXVECTOR3 FieldPos = GetPosition();
	bool bIn = false;		// �����蔻��̏�����

	if (((FieldPos.x - (WIDE_FIELD * SIZE) / 2 - pos.x) * (FieldPos.x - (WIDE_FIELD * SIZE) / 2 - pos.x)) +
		((FieldPos.z - (DEPTH_FIELD * SIZE) / 2 - pos.z) * (FieldPos.z - (DEPTH_FIELD * SIZE) / 2 - pos.z)) <= fRadius * fRadius)
	{// ����Ɠ_
		bIn = true;			// �����蔻�� �̃t���O�𗧂Ă�
	}
	else if (((FieldPos.x + (WIDE_FIELD * SIZE) / 2 - pos.x) * (FieldPos.x + (WIDE_FIELD * SIZE) / 2 - pos.x)) +
		((FieldPos.z - (DEPTH_FIELD * SIZE) / 2 - pos.z) * (FieldPos.z - (DEPTH_FIELD * SIZE) / 2 - pos.z)) <= fRadius * fRadius)
	{// �E��Ɠ_
		bIn = true;			// �����蔻�� �̃t���O�𗧂Ă�
	}
	else if (((FieldPos.x - (WIDE_FIELD * SIZE) / 2 - pos.x) * (FieldPos.x - (WIDE_FIELD * SIZE) / 2 - pos.x)) +
		((FieldPos.z + (DEPTH_FIELD * SIZE) / 2 - pos.z) * (FieldPos.z + (DEPTH_FIELD * SIZE) / 2 - pos.z)) <= fRadius * fRadius)
	{// �����Ɠ_
		bIn = true;			// �����蔻�� �̃t���O�𗧂Ă�
	}
	else if (((FieldPos.x + (WIDE_FIELD * SIZE) / 2 - pos.x) * (FieldPos.x + (WIDE_FIELD * SIZE) / 2 - pos.x)) +
		((FieldPos.z + (DEPTH_FIELD * SIZE) / 2 - pos.z) * (FieldPos.z + (DEPTH_FIELD * SIZE) / 2 - pos.z)) <= fRadius * fRadius)
	{// �E���Ɠ_
		bIn = true;			// �����蔻�� �̃t���O�𗧂Ă�
	}

	return bIn;			// �����蔻�� �t���O�̏�Ԃ�Ԃ�
}

//=============================================================================
// �n�`�̍�������Z�b�g
//=============================================================================
void CMeshField::Reset(void)
{
	MakeVertex(NULL, false);									// ���_���̍č쐬
	CalculationNormalize();						// �@���x�N�g���̌v�Z
}

//=============================================================================
// �@���̌v�Z
//=============================================================================
void CMeshField::CalculationNormalize(void)
{
	VERTEX_3D *pVtx;
	int nSand = 0;						// ���J�E���^�[
	int nGrass = 0;						// ���J�E���^�[

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �@�����߂�
	for (int nDepth = 0; nDepth < DEPTH_FIELD; nDepth++)
	{
		for (int nWide = 0; nWide < WIDE_FIELD; nWide++)
		{
			D3DXVECTOR3 AB = pVtx[nWide + ((WIDE_FIELD + 1) * nDepth)].pos - pVtx[WIDE_FIELD + 1 + nWide + ((WIDE_FIELD + 1) * nDepth)].pos;
			D3DXVECTOR3 BC = pVtx[nWide + ((WIDE_FIELD + 1) * nDepth)].pos - pVtx[WIDE_FIELD + nWide + 2 + ((WIDE_FIELD + 1) * nDepth)].pos;

			D3DXVec3Cross(&apNor[((WIDE_FIELD * 2) * nDepth) + (2 * nWide)], &BC, &AB);
			D3DXVec3Normalize(&apNor[((WIDE_FIELD * 2) * nDepth) + (2 * nWide)], &apNor[((WIDE_FIELD * 2) * nDepth) + (2 * nWide)]);

			AB = pVtx[nWide + ((WIDE_FIELD + 1) * nDepth)].pos - pVtx[WIDE_FIELD + nWide + 2 + ((WIDE_FIELD + 1) * nDepth)].pos;
			BC = pVtx[nWide + ((WIDE_FIELD + 1) * nDepth)].pos - pVtx[1 + nWide + ((WIDE_FIELD + 1) * nDepth)].pos;

			D3DXVec3Cross(&apNor[((WIDE_FIELD * 2) * nDepth) + (2 * nWide) + 1], &BC, &AB);
			D3DXVec3Normalize(&apNor[((WIDE_FIELD * 2) * nDepth) + (2 * nWide) + 1], &apNor[((WIDE_FIELD * 2) * nDepth) + (2 * nWide) + 1]);

			// �n�ʂ̃^�C�v��I��
			if (pVtx[nWide + ((WIDE_FIELD + 1) * nDepth)].col == D3DXCOLOR(1.0f, 1.0f, 0.4f, 1.0f))
			{
				nSand++;
			}
			else
			{
				nGrass++;
			}

			if (pVtx[WIDE_FIELD + 1 + nWide + ((WIDE_FIELD + 1) * nDepth)].col == D3DXCOLOR(1.0f, 1.0f, 0.4f, 1.0f))
			{
				nSand++;
			}
			else
			{
				nGrass++;
			}

			if (pVtx[WIDE_FIELD + nWide + 2 + ((WIDE_FIELD + 1) * nDepth)].col == D3DXCOLOR(1.0f, 1.0f, 0.4f, 1.0f))
			{
				nSand++;
			}
			else
			{
				nGrass++;
			}

			if (nSand > nGrass)
			{
				m_randType[((WIDE_FIELD * 2) * nDepth) + (2 * nWide)] = RANDTYPE_SAND;
			}
			else
			{
				m_randType[((WIDE_FIELD * 2) * nDepth) + (2 * nWide)] = RANDTYPE_GRASS;
			}

			nSand = 0;
			nGrass = 0;

			// �n�ʂ̃^�C�v��I��
			if (pVtx[nWide + ((WIDE_FIELD + 1) * nDepth)].col == D3DXCOLOR(1.0f, 1.0f, 0.4f, 1.0f))
			{
				nSand++;
			}
			else
			{
				nGrass++;
			}

			if (pVtx[WIDE_FIELD + nWide + 2 + ((WIDE_FIELD + 1) * nDepth)].col == D3DXCOLOR(1.0f, 1.0f, 0.4f, 1.0f))
			{
				nSand++;
			}
			else
			{
				nGrass++;
			}

			if (pVtx[1 + nWide + ((WIDE_FIELD + 1) * nDepth)].col == D3DXCOLOR(1.0f, 1.0f, 0.4f, 1.0f))
			{
				nSand++;
			}
			else
			{
				nGrass++;
			}

			if (nSand > nGrass)
			{
				m_randType[((WIDE_FIELD * 2) * nDepth) + (2 * nWide) + 1] = RANDTYPE_SAND;
			}
			else
			{
				m_randType[((WIDE_FIELD * 2) * nDepth) + (2 * nWide) + 1] = RANDTYPE_GRASS;
			}

			nSand = 0;
			nGrass = 0;
		}
	}

	// ���_�f�[�^�̃A�����b�N
	m_pVtxBuff->Unlock();

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �@�����ϋ��߂�
	for (int nDepth = 0; nDepth < DEPTH_FIELD + 1; nDepth++)
	{
		for (int nWide = 0; nWide < WIDE_FIELD + 1; nWide++)
		{
			// ��ԏ�̒i
			if (nDepth == 0)
			{
				if (nWide != WIDE_FIELD)
				{//�Ō���ł͂Ȃ��ꍇ
					if (nWide == 0)
					{// ��ԍŏ�
					 // �悱
						pVtx[(WIDE_FIELD + 1) * nDepth + nWide].nor =
							(apNor[((WIDE_FIELD * 2) * nDepth) + (2 * nWide)] +
								apNor[((WIDE_FIELD * 2) * nDepth) + (2 * nWide) + 1])
							/ 2;
					}
					else if (nWide != WIDE_FIELD)
					{
						pVtx[(WIDE_FIELD + 1) * nDepth + nWide].nor =
							(apNor[((WIDE_FIELD * 2) * nDepth) + (2 * nWide) - 1] +
								apNor[((WIDE_FIELD * 2) * nDepth) + (2 * nWide)] +
								apNor[((WIDE_FIELD * 2) * nDepth) + (2 * nWide) + 1])
							/ 3;
					}
				}
			}
			else if (nDepth != DEPTH_FIELD)
			{// ��ԉ��̒i�ł͂Ȃ��ꍇ
				if (nWide == 0)
				{// ��Ԓ[�����������Ƃ�
					pVtx[(WIDE_FIELD + 1) * nDepth + nWide].nor =
						(apNor[(nDepth - 1) * 2 * WIDE_FIELD + (2 * nWide)] +
							apNor[((WIDE_FIELD * 2) * nDepth) + (2 * nWide)] +
							apNor[((WIDE_FIELD * 2) * nDepth) + (2 * nWide) + 1])
						/ 3;
				}
				else if (nWide == WIDE_FIELD)
				{// ���Ō�
					pVtx[(WIDE_FIELD + 1) * nDepth + nWide].nor =
						(apNor[(nDepth - 1) * 2 * WIDE_FIELD + (2 * nWide) - 2] +
							apNor[(nDepth - 1) * 2 * WIDE_FIELD + (2 * nWide) - 1] +
							apNor[((WIDE_FIELD * 2) * nDepth) + (2 * nWide) - 1])
						/ 3;
				}
				else
				{// �^��
					pVtx[(WIDE_FIELD + 1) * nDepth + nWide].nor =
						(apNor[(nDepth - 1) * 2 * WIDE_FIELD + (2 * nWide) - 2] +
							apNor[(nDepth - 1) * 2 * WIDE_FIELD + (2 * nWide) - 1] +
							apNor[(nDepth - 1) * 2 * WIDE_FIELD + (2 * nWide)] +
							apNor[((WIDE_FIELD * 2) * nDepth) + (2 * nWide) - 1] +
							apNor[((WIDE_FIELD * 2) * nDepth) + (2 * nWide)] +
							apNor[((WIDE_FIELD * 2) * nDepth) + (2 * nWide) + 1])
						/ 6;
				}
			}
			else
			{
				if (nWide == 0)
				{// ��ԍŏ��ł͂Ȃ��ꍇ

				}
				else if (nWide == WIDE_FIELD)
				{
					pVtx[(WIDE_FIELD + 1) * nDepth + nWide].nor =
						(apNor[(nDepth - 1) * 2 * WIDE_FIELD + (2 * nWide) - 2] +
							apNor[(nDepth - 1) * 2 * WIDE_FIELD + (2 * nWide) - 1])
						/ 2;
				}
				else
				{
					pVtx[(WIDE_FIELD + 1) * nDepth + nWide].nor =
						(apNor[(nDepth - 1) * 2 * WIDE_FIELD + (2 * nWide) - 2] +
							apNor[(nDepth - 1) * 2 * WIDE_FIELD + (2 * nWide) - 1] +
							apNor[(nDepth - 1) * 2 * WIDE_FIELD + (2 * nWide)])
						/ 3;
				}
			}
		}
	}

	// ���_�f�[�^�̃A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �ؔz�u�Ǎ��֐�
//=============================================================================
void CMeshField::LoadTree(void)
{
	FILE *pFile;																	// �t�@�C��
	char cReadText[128];															// ����
	char cHeadText[128];															// ��r
	float aData[4];																	// ����

	pFile = fopen("data/stage/object.csv", "r");										// �t�@�C�����J���܂��͍��

	if (pFile != NULL)																//�t�@�C�����ǂݍ��߂��ꍇ
	{
		fgets(cReadText, sizeof(cReadText), pFile);									//�ŏ��̍s���΂�
		fgets(cReadText, sizeof(cReadText), pFile);									//2�s�ڂ��΂�

		while (fgets(cReadText, sizeof(cReadText), pFile) != NULL)
		{
			//fgets(cReadText, sizeof(cReadText), pFile);							//�ŏ��̍s���΂�
			sscanf(cReadText, "%s", &cHeadText);

			CManager::ConvertStringToFloat(cReadText, ",", aData);

			CTree *pTree = CTree::Create(TREETYPE_NORMAL);
			pTree->SetPosition(D3DXVECTOR3(aData[0], aData[1], aData[2]));
		}

		fclose(pFile);																// �t�@�C�������
	}
	else
	{
		MessageBox(NULL, "�I�u�W�F�N�g���̃A�N�Z�X���s�I", "WARNING", MB_ICONWARNING);	// ���b�Z�[�W�{�b�N�X�̐���
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void CMeshField::MakeVertex(FILE *pFile, bool bDebug)
{
	VERTEX_3D *pVtx;
	char cReadText[128];															// ����
	char cHeadText[128];															// ��r
	float aData[7];																	// ����

																					//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nDepth = 0; nDepth < DEPTH_FIELD + 1; nDepth++)
	{
		for (int nWide = 0; nWide < WIDE_FIELD + 1; nWide++)
		{
			if (pFile != NULL)
			{

				fgets(cReadText, sizeof(cReadText), pFile);							// �f�[�^����1�s�擾
				sscanf(cReadText, "%s", &cHeadText);								// �s��ǂݍ���

																					// �f�[�^��������`���ɕ���
				CManager::ConvertStringToFloat(cReadText, ",", aData);

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(aData[0], aData[1], aData[2]);			// �f�[�^�𓖂Ă͂߂�

				D3DXCOLOR col = D3DXCOLOR(aData[3], aData[4], aData[5], aData[6]);

				//���_�J���[
				pVtx[0].col = col;	// �f�[�^�𓖂Ă͂߂�

				if (bDebug)
				{
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �f�[�^�𓖂Ă͂߂�
				}

			}
			else
			{
				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3((-SIZE * WIDE_FIELD) / 2 + SIZE * nWide, 0.0f, (SIZE * DEPTH_FIELD) / 2 - SIZE * nDepth);

				//���_�J���[
				pVtx[0].col = m_col;
			}

			//�@��
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//�e�N�X�`���`�ʂ̈ʒu
			pVtx[0].tex = D3DXVECTOR2(1.0f * nWide, 1.0f * nDepth);

			pVtx++;
		}
	}

	//���_�f�[�^�̃A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �C���f�b�N�X���蓖��
//=============================================================================
void CMeshField::MakeIndex(void)
{
	WORD *pIdx;									//�C���f�b�N�X�f�[�^�փ|�C���^
	int nCount = 0;

	//�C���f�b�N�X�o�b�t�@�����b�N���A�C���f�b�N�X�f�[�^�ւ̃|�C���^
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntY = 0; nCntY < DEPTH_FIELD; nCntY++)
	{
		if (nCntY != 0)
		{
			pIdx[0] = (WIDE_FIELD + 1) + nCntY * (WIDE_FIELD + 1);
			pIdx++;
			nCount++;
		}

		for (int nCntX = 0; nCntX < WIDE_FIELD + 1; nCntX++)
		{
			pIdx[0] = (WIDE_FIELD + 1) + nCntX + nCntY * (WIDE_FIELD + 1);
			pIdx[1] = nCntX + nCntY * (WIDE_FIELD + 1);
			pIdx += 2;
			nCount += 2;
		}

		if (DEPTH_FIELD > nCntY + 1)
		{
			pIdx[0] = WIDE_FIELD + nCntY * (WIDE_FIELD + 1);
			pIdx++;
			nCount++;
		}
	}

	//�C���f�b�N�X�o�b�t�@�����b�N���A�C���f�b�N�X�f�[�^�ւ̃|�C���^
	m_pIdxBuff->Unlock();
}

#ifdef _DEBUG
//=============================================================================
// �f�o�b�O����
//=============================================================================
void CMeshField::Debug(void)
{

}

//=============================================================================
// �n�`�ҏW�֐�
//=============================================================================
void CMeshField::CreateRand(D3DXVECTOR3 &worldPos, float fRadius)
{
	VERTEX_3D *pVtx;																// ���_���ւ̃|�C���^
	CInputMouse *pMouse = CManager::GetInputMouse();								// �}�E�X�̎擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();						// �L�[�{�[�h�̎擾
	D3DXVECTOR3 pos = GetPosition();

	if (!pKeyboard->GetPressKeyboard(DIK_LALT))
	{// ��Alt�L�[��������Ă��Ȃ��Ƃ�
		if (pMouse->GetReleaseMouse(MOUSE_LEFT))
		{// �}�E�X�̍��{�^���������ꂽ�Ƃ�
			CalculationNormalize();														// �@���̌v�Z
		}
		else if (pMouse->GetPressMouse(MOUSE_LEFT))
		{// �}�E�X�̍��{�^����������Ă���Ƃ�
				// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			//���̒��_�S�����Q��
			for (int nCount = 0; nCount < m_nNumVertex; nCount++)
			{
				if (SphereModel(worldPos, pVtx[nCount].pos + pos, fRadius))
				{// �}�E�X�̃��[���h���W����w�肵�����a�܂łœ��Ă͂܂钸�_���������Ƃ�
					//pVtx[nCount].pos.y += 5.0f;
					pVtx[nCount].pos.y = 80.0f;
				}
			}

			//���_�f�[�^�̃A�����b�N
			m_pVtxBuff->Unlock();
		}
		else if (pMouse->GetPressMouse(MOUSE_RIGHT))
		{// �}�E�X�̉E�{�^����������Ă���Ƃ�
			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// ���̒��_�S�����Q��
			for (int nCount = 0; nCount < m_nNumVertex; nCount++)
			{
				if (SphereModel(worldPos, pVtx[nCount].pos + pos, fRadius))
				{// �}�E�X�̃��[���h���W����w�肵�����a�܂łœ��Ă͂܂钸�_���������Ƃ�
					pVtx[nCount].pos.y -= 5.0f;									// ���_�̍�����������
				}
			}

			//// ���_�f�[�^�̃A�����b�N
			m_pVtxBuff->Unlock();
		}
	}
}

//=============================================================================
// ���_�F�ύX�֐�
//=============================================================================
void CMeshField::Paint(D3DXVECTOR3 &worldPos, float fRadius)
{
	VERTEX_3D *pVtx;																// ���_���ւ̃|�C���^
	CInputMouse *pMouse = CManager::GetInputMouse();								// �}�E�X�̎擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();						// �L�[�{�[�h�̎擾
	D3DXVECTOR3 pos = GetPosition();

	if (!pKeyboard->GetPressKeyboard(DIK_LALT))
	{// ��Alt�L�[��������Ă��Ȃ��Ƃ�
		if (pMouse->GetPressMouse(MOUSE_LEFT))
		{// �}�E�X�̍��{�^����������Ă���Ƃ�
		 // ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			//���̒��_�S�����Q��
			for (int nCount = 0; nCount < m_nNumVertex; nCount++)
			{
				if (SphereModel(worldPos, pVtx[nCount].pos + pos, fRadius))
				{// �}�E�X�̃��[���h���W����w�肵�����a�܂łœ��Ă͂܂钸�_���������Ƃ�
					pVtx[nCount].col = D3DXCOLOR(1.0f, 1.0f, 0.4f, 1.0f);
				}
			}

			//���_�f�[�^�̃A�����b�N
			m_pVtxBuff->Unlock();
		}
		else if (pMouse->GetPressMouse(MOUSE_RIGHT))
		{// �}�E�X�̉E�{�^����������Ă���Ƃ�
		 // ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// ���̒��_�S�����Q��
			for (int nCount = 0; nCount < m_nNumVertex; nCount++)
			{
				if (SphereModel(worldPos, pVtx[nCount].pos + pos, fRadius))
				{// �}�E�X�̃��[���h���W����w�肵�����a�܂łœ��Ă͂܂钸�_���������Ƃ�
					pVtx[nCount].col = D3DXCOLOR(0.0f, 1.0f, 0.4f, 1.0f);
				}
			}

			// ���_�f�[�^�̃A�����b�N
			m_pVtxBuff->Unlock();
		}
	}
}

//=============================================================================
// �n�`����ۑ��֐�
//=============================================================================
void CMeshField::SaveRand(FILE *pFile)
{
	VERTEX_3D *pVtx;																// ���_���ւ̃|�C���^
	char cWriteText[128];															// �����Ƃ��ď������ݗp

	if (pFile != NULL)																// �t�@�C�����ǂݍ��߂��ꍇ
	{
		sprintf(cWriteText, "X��, Y��, Z��, Red, Green, Blue, Alpha\n");
		fputs(cWriteText, pFile);													// ��������

		//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nDepth = 0; nDepth < DEPTH_FIELD + 1; nDepth++)
		{
			for (int nWide = 0; nWide < WIDE_FIELD + 1; nWide++)
			{
				//���_���W�̐ݒ�
				pVtx[0].pos;
				D3DXCOLOR col = pVtx[0].col;
				sprintf(cWriteText, "%.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f\n", pVtx[0].pos.x, pVtx[0].pos.y, pVtx[0].pos.z, col.r, col.g, col.b, col.a);
				fputs(cWriteText, pFile);											// ��������

				pVtx++;																// �|�C���^���V�t�g
			}
		}

		//���_�f�[�^�̃A�����b�N
		m_pVtxBuff->Unlock();
	}
}
#endif