//=============================================================================
//
// ���b�V���X�t�B�A���� [meshSphere.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "meshSphere.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
PDIRECT3DTEXTURE9 CMeshSphere::m_pTexture = NULL;		// �e�N�X�`�����̏�����

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CMeshSphere::CMeshSphere(CScene::PRIORITY obj = CScene::PRIORITY_MESH) : CScene(obj)
{
	// �ʒu�E��]�̏����ݒ�
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// ��]�̃|�C���^
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// �F
	m_vecAxis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ��]��
	m_fRadius = 100.0;									// ���a
	m_fValueRot = 0.0f;									// ��]�p
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMeshSphere::~CMeshSphere()
{

}

//==============================================================================
// ����������
//==============================================================================
HRESULT CMeshSphere::Init(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();				// �`����̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();			// �f�o�C�X�̎擾

	// ================== �l�p�` ================== //
	m_nNumVertex = (DIVISION + 1) * (DIVISION + 1);				//�����_��
	m_nNumIndex = ((DIVISION + 1) * 2) * DIVISION + ((DIVISION - 1) * 2);		//�C���f�b�N�X��

	//���|���S����	�������� * ������ * 2 + �k�ރ|���S���� * ������(�Ō�̗�͏���)
	m_nNumPolygon = DIVISION * DIVISION * 2 + 4 * (DIVISION - 1);

	// ================== �~�` ================== //

	m_nNumVertexSphere = DIVISION + 2;								//�����_��
	m_nNumIndexSphere = DIVISION;									//�C���f�b�N�X��
	m_nNumPolygonSphere = DIVISION;									//���|���S����

	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * ((m_nNumVertexSphere * 2) + m_nNumVertex), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	// �C���f�b�N�X�o�b�t�@�𐶐�
	pDevice->CreateIndexBuffer(sizeof(WORD) * (((m_nNumIndexSphere + 2) * 2) + m_nNumIndex) + 1, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIdxBuff, NULL);

	// ���_���̊i�[
	MakeVertex();

	// �C���f�b�N�X�o�b�t�@�̐ݒ�
	MakeIndex();

	return S_OK;				// �� ��Ԃ�
}

//=============================================================================
// �J������
//=============================================================================
void CMeshSphere::Uninit(void)
{
	//
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

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
void CMeshSphere::Update(void)
{
#ifdef _DEBUG
	Debug();
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void CMeshSphere::Draw(void)
{
	if (GetActive())
	{
		CRenderer *pRenderer = CManager::GetRenderer();
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();	// �f�o�C�X�̎擾
		D3DXVECTOR3 pos = GetPosition();

		D3DXMATRIX	mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&m_mtxWorld);

		// �N�H�[�^�j�I��
		D3DXQuaternionRotationAxis(&m_quat, &m_vecAxis, D3DXToRadian(m_fValueRot));
		D3DXMatrixRotationQuaternion(&mtxRot, &m_quat);

		// ��]�𔽉f
		//D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
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

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetTexture(0, m_pTexture);

		// �|���S���̕`��		��1�ԏ�
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, m_nNumVertexSphere, 0, m_nNumPolygonSphere);

		// �|���S���̕`��		��2�Ԗڈȍ~�`�Ō��1����O�܂�
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVertex - m_nNumVertexSphere, m_nNumIndexSphere + 2, m_nNumPolygon - m_nNumPolygonSphere * 2);

		//�|���S���̕`��		���Ō�
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, m_nNumVertex + m_nNumVertexSphere, m_nNumIndex - m_nNumPolygonSphere + 2, m_nNumPolygonSphere);

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetTexture(0, NULL);
	}
}

//==============================================================================
// �N���G�C�g����
//==============================================================================
CMeshSphere *CMeshSphere::Create(float fRadius)
{
	CMeshSphere *pMeshSphere;

	pMeshSphere = new CMeshSphere(CScene::PRIORITY_MESH);

	if (pMeshSphere != NULL)
	{// ���b�V���X�t�B�A�����݂��Ă����Ƃ�
		pMeshSphere->SetRadius(fRadius);		// ���a�̐ݒ�
		pMeshSphere->Init();					// ����������
	}
	return pMeshSphere;
}

//==============================================================================
// ���[�h����
//==============================================================================
HRESULT CMeshSphere::Load(void)
{
	return S_OK;
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void CMeshSphere::MakeVertex(void)
{
	VERTEX_3D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ================= �~�̒��S =================== //
	pVtx[0].pos = D3DXVECTOR3(0.0f, -m_fRadius, 0.0f);

	// �@���x�N�g��
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// ���_�J���[
	pVtx[0].col = m_col;

	// �e�N�X�`���`�ʂ̈ʒu
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);

	// ���_�f�[�^�̃|�C���^��1�i�߂�
	pVtx++;

	// ================= �~���`�� =================== //
	for (int nDepth = 0; nDepth <= DIVISION; nDepth++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(sinf(D3DX_PI - ((D3DX_PI * 2 / DIVISION) * nDepth)) * sinf(D3DX_PI + ((D3DX_PI / (DIVISION + 1)) * 1)) * m_fRadius,
			cosf(D3DX_PI + ((D3DX_PI / (DIVISION)) * 1)) * m_fRadius,
			cosf(D3DX_PI - ((D3DX_PI * 2 / DIVISION) * nDepth)) * sinf(D3DX_PI + ((D3DX_PI / (DIVISION + 1)) * 1)) * m_fRadius);

		// �@���x�N�g��
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// ���_�J���[
		pVtx[0].col = m_col;

		// �e�N�X�`���`�ʂ̈ʒu
		pVtx[0].tex = D3DXVECTOR2((1.0f / DIVISION) * nDepth, (1.0f / DIVISION) * 1);

		// ���_�f�[�^�̃|�C���^��1���i�߂�
		pVtx++;
	}

	// �l�p�`�̃��b�V�����~�`�ɂ���
	for (int nDepth = 1; nDepth < DIVISION + 1; nDepth++)
	{
		for (int nWide = 0; nWide < DIVISION + 1; nWide++)
		{
			// ���������߂�
			float fWork = sinf(D3DX_PI + ((D3DX_PI / (DIVISION + 1)) * nDepth)) * m_fRadius;

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(sinf(D3DX_PI + ((D3DX_PI * 2 / DIVISION) * nWide)) * fWork,
				cosf(D3DX_PI + ((D3DX_PI / (DIVISION + 1)) * nDepth)) * m_fRadius,
				cosf(D3DX_PI + ((D3DX_PI * 2 / DIVISION) * nWide)) * fWork);

			// �@��
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// ���_�J���[
			pVtx[0].col = m_col;

			//�e�N�X�`���`�ʂ̈ʒu
			pVtx[0].tex = D3DXVECTOR2((1.0f / DIVISION) * nWide, (1.0f / DIVISION) * nDepth);

			// ���_�f�[�^�̃|�C���^��1���i�߂�
			pVtx++;
		}
	}

	// ================= �~�̒��S =================== //

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, m_fRadius, 0.0f);

	//�@���x�N�g��
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//���_�J���[
	pVtx[0].col = m_col;

	//�e�N�X�`���`�ʂ̈ʒu
	pVtx[0].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�f�[�^�̃|�C���^��1���i�߂�
	pVtx++;

	// ================= �~���`�� =================== //
	for (int nDepth = 0; nDepth <= DIVISION; nDepth++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(sinf(D3DX_PI + ((D3DX_PI * 2 / DIVISION) * nDepth)) * sinf(D3DX_PI + ((D3DX_PI / (DIVISION + 1)) * 1)) * m_fRadius,
			cosf(D3DX_PI + ((D3DX_PI / (DIVISION)) * 1)) * -m_fRadius,
			cosf(D3DX_PI - ((D3DX_PI * 2 / DIVISION) * nDepth)) * sinf(D3DX_PI + ((D3DX_PI / (DIVISION + 1)) * 1)) * m_fRadius);

		//�@���x�N�g��
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[
		pVtx[0].col = m_col;

		//�e�N�X�`���`�ʂ̈ʒu
		pVtx[0].tex = D3DXVECTOR2((1.0f / DIVISION) * nDepth, (1.0f / DIVISION) * 1);

		pVtx++;
	}

	// ���_�f�[�^�̃A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �C���f�b�N�X���蓖��
//=============================================================================
void CMeshSphere::MakeIndex(void)
{
	WORD *pIdx;
	int nCount = 0;

	// �C���f�b�N�X�o�b�t�@�����b�N���A�C���f�b�N�X�f�[�^�ւ̃|�C���^
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

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
	for (nCount = 1; nCount < DIVISION; nCount++)
	{
		// �k�ރ|���S��
		pIdx[0] = (DIVISION + 1) + nCount * (DIVISION + 1);
		pIdx++;

		for (int nCntX = 0; nCntX < DIVISION + 1; nCntX++)
		{
			pIdx[0] = (DIVISION + 1) + nCntX + nCount * (DIVISION + 1);
			pIdx[1] = nCntX + nCount * (DIVISION + 1) + 1;
			pIdx += 2;
		}

		// �k�ރ|���S��
		pIdx[0] = DIVISION + (nCount + 1) * (DIVISION + 1);
		pIdx++;
	}

	// �k�ރ|���S��
	pIdx[0] = (DIVISION + 1) + nCount * (DIVISION + 1);
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
	m_pIdxBuff->Unlock();
}

//=============================================================================
// ��]�����߂�֐�
//==============================================================================
void CMeshSphere::SetRotation(D3DXVECTOR3 rot)
{
	m_rot = rot;				// ��]�� �̑��
}

//=============================================================================
// ��]�������߂�֐�
//==============================================================================
void CMeshSphere::SetVecAxis(D3DXVECTOR3 vecAxis)
{
	m_vecAxis = vecAxis;		// ��]�� �̑��
}

//=============================================================================
// ��]�ʂ����߂�֐�
//==============================================================================
void CMeshSphere::SetValueRot(float fValue)
{
	m_fValueRot += fValue;		// ��]�� �̉��Z
}

//=============================================================================
// �F�����߂�֐�
//==============================================================================
void CMeshSphere::SetColor(D3DXCOLOR col)
{
	VERTEX_3D *pVtx;
	m_col = col;				// �F �̑��

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < m_nNumVertex + m_nNumVertexSphere; nCount++)
	{
		//���_�J���[
		pVtx[0].col = m_col;

		pVtx++;
	}

	// ���_�f�[�^�̃A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// ���a�����߂�֐�
//==============================================================================
void CMeshSphere::SetRadius(float fValue)
{
	m_fRadius = fValue;			// ���a �̑��
}

#ifdef _DEBUG
//=============================================================================
// �f�o�b�O����
//=============================================================================
void CMeshSphere::Debug(void)
{
	ImGui::Begin("System");

	ImGui::Text("move = %f, %f, %f", m_vecAxis.x, m_vecAxis.y, m_vecAxis.z);	// �v���C���[ �̌��݈ʒu ��\��

	//�f�o�b�O�������I��
	ImGui::End();
}
#endif