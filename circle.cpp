//=============================================================================
//
// �~���� [circle.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "circle.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
PDIRECT3DTEXTURE9 CCircle::m_pTexture = NULL;		// �e�N�X�`�����̏�����

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCircle::CCircle()
{
	// �����ݒ�
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ��]
	m_col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.5f);		// �F
	m_fRadius = 20.0;								// ���a
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCircle::~CCircle()
{

}

//==============================================================================
// ����������
//==============================================================================
HRESULT CCircle::Init(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();				// �����_���[�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();			// �f�o�C�X�̎擾

	m_nNumVertex = DIVISION_CIRCLE + 2;							//�����_��
	m_nNumIndex = DIVISION_CIRCLE;								//�C���f�b�N�X��
	m_nNumPolygon = DIVISION_CIRCLE;							//���|���S����

	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVertex, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	//�C���f�b�N�X�o�b�t�@�𐶐�
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIndex, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIdxBuff, NULL);

	// ���_���̍쐬
	MakeVertex();

	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CCircle::Uninit(void)
{
	// �e�N�X�`���̊J��
	if (m_pTexture != NULL)
	{// �e�N�X�`�������݂��Ă����Ƃ�
	 // �J������
		m_pTexture->Release();
		m_pTexture = NULL;		// NULL�̑��
	}

	// �C���f�b�N�X�o�b�t�@�̊J��
	if (m_pIdxBuff != NULL)
	{// �C���f�b�N�X�o�b�t�@�����݂��Ă����Ƃ�
	 // �J������
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;		// NULL�̑��
	}

	// ���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{// ���_�o�b�t�@�����݂��Ă����Ƃ�
	 // �J������
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;		// NULL�̑��
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CCircle::Update(void)
{
#ifdef _DEBUG
	Debug();	// �f�o�b�O����
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void CCircle::Draw(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();			// �����_���[�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();		// �f�o�C�X�̎擾
	D3DXMATRIX	mtxRot, mtxTrans;							//�v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

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

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, (DIVISION_CIRCLE + 1), 0, (DIVISION_CIRCLE + 1));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetTexture(0, NULL);
}

//==============================================================================
// �N���G�C�g����
//==============================================================================
CCircle *CCircle::Create(void)
{
	CCircle *pCircle;
	pCircle = new CCircle();

	if (pCircle != NULL)
	{// �T�[�N�������݂��Ă����Ƃ�
		pCircle->Init();				// ������
	}
	return pCircle;
}

//==============================================================================
// �e�N�X�`���̃��[�h�֐�
//==============================================================================
HRESULT CCircle::Load(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();		// �����_���[�̎擾
	LPDIRECT3DDEVICE9 pDevice;							// �f�o�C�X

	//�f�o�C�X���擾����
	pDevice = pRenderer->GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/tex/skydome.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���̉���֐�
//=============================================================================
void CCircle::Unload(void)
{
	// �e�N�X�`���̊J��
	if (m_pTexture != NULL)
	{// �e�N�X�`����񂪑��݂��Ă����Ƃ�
		// �J������
		m_pTexture->Release();
		m_pTexture = NULL;			// NULL�̑��
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void CCircle::MakeVertex(void)
{
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^
	WORD *pIdx;			// �C���f�b�N�X�f�[�^�ւ�

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//================= �~�̒��S ===================//
	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, m_pos.y, 0.0f);

	//�@���x�N�g���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//���_�J���[�̐ݒ�
	pVtx[0].col = m_col;

	//�e�N�X�`���`�ʂ̈ʒu��ݒ�
	pVtx[0].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�f�[�^�̃|�C���^��1���i�߂�
	pVtx++;

	//================= �~���`�� ===================//
	for (int nDepth = 0; nDepth < DIVISION_CIRCLE - 1; nDepth++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(sinf(D3DX_PI + 6.28f + ((6.28f / (DIVISION_CIRCLE - 2)) * nDepth)) * m_fRadius,
								m_pos.y,
								cosf(D3DX_PI + 6.28f + ((6.28f / (DIVISION_CIRCLE - 2)) * nDepth)) * m_fRadius);

		//�@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

		//�e�N�X�`���`�ʂ̈ʒu��ݒ�
		pVtx[0].tex = D3DXVECTOR2(1.0f, 1.0f * nDepth);

		// ���_�f�[�^�̃|�C���^��1���i�߂�
		pVtx++;
	}

	//���_�f�[�^�̃A�����b�N
	m_pVtxBuff->Unlock();

	//�C���f�b�N�X�o�b�t�@�����b�N���A�C���f�b�N�X�f�[�^�ւ̃|�C���^
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntY = 0; nCntY < m_nNumVertex; nCntY++)
	{
		// �C���f�b�N�X�o�b�t�@ �̐ݒ�
		pIdx[0] = nCntY;
		pIdx++;		// �C���f�b�N�X�f�[�^�̃|�C���^��1���i�߂�
	}

	//�C���f�b�N�X�o�b�t�@�����b�N���A�C���f�b�N�X�f�[�^�ւ̃|�C���^
	m_pIdxBuff->Unlock();
}

//==============================================================================
// �ʒu�����߂�֐�
//==============================================================================
void CCircle::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//==============================================================================
// ��]�����߂�֐�
//==============================================================================
void CCircle::SetRotation(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//==============================================================================
// �F�����߂�֐�
//==============================================================================
void CCircle::SetColor(D3DXCOLOR col)
{
	m_col = col;
}

//==============================================================================
// ���a�����߂�֐�
//==============================================================================
void CCircle::SetRadius(float fValue)
{
	m_fRadius = fValue;
}

//==============================================================================
// �T�C�Y�����߂�֐�
//==============================================================================
void CCircle::SetSize(D3DXVECTOR3 size)
{
	m_size = size;
}

#ifdef _DEBUG
//=============================================================================
// �f�o�b�O����
//=============================================================================
void CCircle::Debug(void)
{
	ImGui::Begin("System");

	//ImGui::Text("move = %f, %f, %f", m_vecAxis.x, m_vecAxis.y, m_vecAxis.z);										// �v���C���[�̌��݈ʒu��\��

	//�f�o�b�O�������I��
	ImGui::End();
}
#endif