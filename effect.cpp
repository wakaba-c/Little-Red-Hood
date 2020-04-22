//=============================================================================
//
// �G�t�F�N�g���� [effect.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "effect.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "meshCapsule.h"
#include "meshSphere.h"
#include "meshCube.h"
#include "camera.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
CEffect *CEffect::m_pEffect[MAX_EFFECT] = {};
LPDIRECT3DTEXTURE9 CEffect::m_pTexture[EFFECTTYPE_MAX] = {};
EFFECT	CEffect::m_aEffect[EFFECTTYPE_MAX] = {};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEffect::CEffect(CScene::PRIORITY obj = CScene::PRIORITY_EFFECT) : CScene(obj)
{
	SetObjType(CScene::PRIORITY_EFFECT);								// �I�u�W�F�N�g�^�C�v�̐ݒ�

	/* =============== �l�̏����� =============== */
	m_pVtxBuff = NULL;													// ���_�o�b�t�@�̏�����
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);								// �ʒu�̏�����
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);							// �J���[�̏�����
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);								// ��]�̏�����
	m_size = D3DXVECTOR3(5.0f, 5.0f, 0.0f);								// �T�C�Y�̏�����
	m_move = D3DXVECTOR3(0.0f, 2.0f, 0.0f);								// �ړ��ʂ̏�����
	m_nLife = 0;														// �ϋv�l�̏�����
	m_bUse = false;														// �g�p��Ԃ̏�����
	m_bGravity = false;													// �d�͂̏�����
	m_type = EFFECTTYPE_ROSE;											// �G�t�F�N�g�^�C�v�̏�����
	m_AnimCount = 0;													// �A�j���[�V�����J�E���^�[�̏�����
	m_AnimPage = 0;														// �A�j���[�V�����y�[�W�̏�����
	m_nInterval = 0;													// �C���^�[�o���̏�����
	m_fRotationSpeed = 0;												// ��]�X�s�[�h�̏�����
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEffect::~CEffect()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CEffect::Init(void)
{
	//�f�o�C�X���擾����
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	// �摜���
	m_aEffect[EFFECTTYPE_ROSE].sprite = D3DXVECTOR2(1.0f, 1.0f);		// ������
	m_aEffect[EFFECTTYPE_ROSE].bAlpha = false;							// �A���t�@�u�����f�B���O�̗L��

	m_aEffect[EFFECTTYPE_SMOKE].sprite = D3DXVECTOR2(8.0f, 1.0f);		// ������
	m_aEffect[EFFECTTYPE_SMOKE].bAlpha = false;							// �A���t�@�u�����f�B���O�̗L��

	m_aEffect[EFFECTTYPE_HALO].sprite = D3DXVECTOR2(1.0f, 1.0f);		// ������
	m_aEffect[EFFECTTYPE_HALO].bAlpha = false;							// �A���t�@�u�����f�B���O�̗L��

	m_aEffect[EFFECTTYPE_SANDSMOKE].sprite = D3DXVECTOR2(8.0f, 1.0f);	// ������
	m_aEffect[EFFECTTYPE_SANDSMOKE].bAlpha = false;						// �A���t�@�u�����f�B���O�̗L��

	m_aEffect[EFFECTTYPE_SHOCKWAVE].sprite = D3DXVECTOR2(1.0f, 1.0f);	// ������
	m_aEffect[EFFECTTYPE_SHOCKWAVE].bAlpha = false;						// �A���t�@�u�����f�B���O�̗L��

	m_aEffect[EFFECTTYPE_STAR].sprite = D3DXVECTOR2(1.0f, 1.0f);		// ������
	m_aEffect[EFFECTTYPE_STAR].bAlpha = false;							// �A���t�@�u�����f�B���O�̗L��

	m_aEffect[EFFECTTYPE_SLASH].sprite = D3DXVECTOR2(9.0f, 1.0f);		// ������
	m_aEffect[EFFECTTYPE_SLASH].bAlpha = false;							// �A���t�@�u�����f�B���O�̗L��

	m_aEffect[EFFECTTYPE_SPHERE].sprite = D3DXVECTOR2(1.0f, 1.0f);		// ������
	m_aEffect[EFFECTTYPE_SPHERE].bAlpha = true;							// �A���t�@�u�����f�B���O�̗L��

	m_aEffect[EFFECTTYPE_WING].sprite = D3DXVECTOR2(1.0f, 1.0f);		// ������
	m_aEffect[EFFECTTYPE_WING].bAlpha = true;							// �A���t�@�u�����f�B���O�̗L��

	m_aEffect[EFFECTTYPE_AURA].sprite = D3DXVECTOR2(1.0f, 1.0f);		// ������
	m_aEffect[EFFECTTYPE_AURA].bAlpha = true;							// �A���t�@�u�����f�B���O�̗L��

	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);								// �T�C�Y�̐ݒ�

	// ���_���̍X�V
	MakeVertex();

	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CEffect::Uninit(void)
{
	// ���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{// ���_�o�b�t�@�����݂��Ă����Ƃ�
		m_pVtxBuff->Release();		// �J��
		m_pVtxBuff = NULL;			// NULL����
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CEffect::Update(void)
{
	if (m_bUse)
	{
		float fSize;
		float fRate = (float)m_nLife / (float)m_nMaxLife;

		// �ʒu�X�V
		m_pos += m_move;

		// �T�C�Y��
		switch (m_easingType)
		{
		case EASINGTYPE_NONE:
			m_size += m_moveSize;
			break;
		case EASINGTYPE_IN:
			fSize = CManager::easeIn(fRate, 0, 1, 1);
			m_size += m_moveSize * fSize;
			break;
		case EASINGTYPE_OUT:
			fSize = CManager::easeOut(fRate);
			m_size += m_moveSize * fSize;
			break;
		default:
			m_size += m_moveSize;
			break;
		}

		// ���_���̍X�V
		MakeVertex();

		switch (m_type)
		{
		case EFFECTTYPE_HALO:
			fSize = CManager::easeIn(fRate, 0, 1, 1);
			m_size.x -= 2 * fSize;
			m_size.y -= 2 * fSize;
			break;
		case EFFECTTYPE_STAR:
			fSize = CManager::easeOut(fRate);
			m_size.x -= (50 / m_nMaxLife) * fSize;
			m_size.y -= (50 / m_nMaxLife) * fSize;
			break;
		case EFFECTTYPE_ROSE:
			m_rot.x += 0.02f;
			m_rot.y += 0.02f;
			break;
		case EFFECTTYPE_SANDSMOKE:
			fSize = CManager::easeIn(fRate, 0, 1, 1);

			m_size.x += 2 + 15 * fSize;
			m_size.y += 2 + 15 * fSize;

			m_col.a -= 0.02f;
			SpriteAnimation(m_aEffect[m_type].sprite, m_AnimPage, 0);
			break;
		case EFFECTTYPE_SLASH:
			SpriteAnimation(m_aEffect[m_type].sprite, m_AnimPage, 0);
			break;
		case EFFECTTYPE_SMOKE:
			SpriteAnimation(m_aEffect[m_type].sprite, m_AnimPage, 0);
			break;
		case EFFECTTYPE_WING:
			m_rot.x += 0.02f;
			m_rot.y += 0.02f;
			break;
		}

		// ����
		m_move.x += (0 - m_move.x) * m_fResistance;
		m_move.y += (0 - m_move.y) * m_fResistance;
		m_move.z += (0 - m_move.z) * m_fResistance;

		if (fabs(m_fDistance) >= 1.0f)
		{
			m_fAngle += m_fRotationSpeed;

			// �ʏ펞�͂������ǂ�
			m_pos.x = m_centerPos.x + sinf(D3DX_PI * m_fAngle) * m_fDistance;
			m_pos.z = m_centerPos.z + cosf(D3DX_PI * m_fAngle) * m_fDistance;
		}

		// �A�j���[�V��������
		if (m_nLife < m_nMaxLife)
		{
			Animation();
		}

		// �J������
		if (m_nLife < m_nMaxLife)
		{
			m_nLife++;					//���C�t�����

			if (m_bGravity)
			{
				//�d�͏���
				m_move.y -= 0.15f;
			}
		}
		else
		{
			if (m_type != EFFECTTYPE_STAR)
			{
				m_col.a -= 0.05f;

				if (m_col.a < 0.0f)
				{
					m_bUse = false;
				}
			}
			else
			{
				m_bUse = false;
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CEffect::Draw(void)
{
	if (m_bUse)
	{
		CRenderer *pRenderer = CManager::GetRenderer();
		LPDIRECT3DDEVICE9 pDevice;

		D3DXMATRIX	mtxRot, mtxTrans, mtxView;				//�v�Z�p�}�g���b�N�X

		//�f�o�C�X���擾����
		pDevice = pRenderer->GetDevice();

		if (m_aEffect[m_type].bAlpha)
		{// ���Z�������g�p�����Ƃ�
			// �����_�[�X�e�[�g(���Z��������)
			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		}

		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// ����(�����)���J�����O����
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		if (m_type != EFFECTTYPE_ROSE)
		{
			pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		}

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&m_mtxWorld);

		if (m_bBillboard)
		{
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			m_mtxWorld._11 = mtxView._11;
			m_mtxWorld._12 = mtxView._21;
			m_mtxWorld._13 = mtxView._31;
			m_mtxWorld._21 = mtxView._12;
			m_mtxWorld._22 = mtxView._22;
			m_mtxWorld._23 = mtxView._32;
			m_mtxWorld._31 = mtxView._13;
			m_mtxWorld._32 = mtxView._23;
			m_mtxWorld._33 = mtxView._33;
		}

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
		pDevice->SetTexture(0, m_pTexture[m_type]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		// �����_�[�X�e�[�g(�ʏ�u�����h����)
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

		// �����_�[�X�e�[�g(�ʏ�u�����h����)
		pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		if (m_aEffect[m_type].bAlpha)
		{// ���Z�������g�p�����Ƃ�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}

		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// ����(�����)���J�����O����

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetTexture(0, NULL);
	}
}

//=============================================================================
// �N���G�C�g�֐�
//=============================================================================
void CEffect::Create(void)
{
	// �G�t�F�N�g�̍쐬
	for (int nCount = 0; nCount < MAX_EFFECT; nCount++)
	{
		m_pEffect[nCount] = new CEffect(PRIORITY_EFFECT);

		if (m_pEffect[nCount] != NULL)
		{// �G�t�F�N�g�����݂��Ă����Ƃ�
			m_pEffect[nCount]->Init();					// ����������
			m_pEffect[nCount]->SetUse(false);			// �g�p��Ԃ̕ύX
		}
	}
}

//=============================================================================
// ���f���̃��[�h�֐�
//=============================================================================
HRESULT CEffect::Load(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X���擾����
	pDevice = pRenderer->GetDevice();

	m_pTexture[EFFECTTYPE_ROSE] = CManager::GetResource("data/tex/effect/rose_01.png");
	m_pTexture[EFFECTTYPE_SMOKE] = CManager::GetResource("data/tex/effect/explosion002.png");
	m_pTexture[EFFECTTYPE_SANDSMOKE] = CManager::GetResource("data/tex/effect/explosion001.png");
	m_pTexture[EFFECTTYPE_HALO] = CManager::GetResource("data/tex/effect/Halo.png");
	m_pTexture[EFFECTTYPE_SHOCKWAVE] = CManager::GetResource("data/tex/effect/Shockwave.png");
	m_pTexture[EFFECTTYPE_STAR] = CManager::GetResource("data/tex/effect/Star.png");
	m_pTexture[EFFECTTYPE_SLASH] = CManager::GetResource("data/tex/effect/Slashing.png");
	m_pTexture[EFFECTTYPE_SPHERE] = CManager::GetResource("data/tex/effect/sphere.jpg");
	m_pTexture[EFFECTTYPE_WING] = CManager::GetResource("data/tex/effect/wing.jpg");
	m_pTexture[EFFECTTYPE_AURA] = CManager::GetResource("data/tex/effect/aura.png");

	return S_OK;
}

//=============================================================================
// �G�t�F�N�g�̈ʒu��ݒ�
//=============================================================================
void CEffect::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// �G�t�F�N�g�̈ړ��ʂ�ݒ�
//=============================================================================
void CEffect::SetMove(D3DXVECTOR3 move)
{
	m_move = move;
}

//=============================================================================
// �g�p�����ǂ����ݒ肷��
//=============================================================================
void CEffect::SetUse(bool bValue)
{
	m_bUse = bValue;
}

//=============================================================================
// �e�N�X�`���`��ʒu�̍X�V
//=============================================================================
void CEffect::SpriteAnimation(D3DXVECTOR2 patternNo, int nPatternAnim, int nHeight)
{// �c�Ɖ��̃p�^�[�����ƌ��݂̃y�[�W�ƍs

	VERTEX_3D *pVtx;										// ���_���ւ̃|�C���^

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`�����W�̍X�V
	pVtx[0].tex = D3DXVECTOR2(0.0f + nPatternAnim * 1.0f / patternNo.x, 0.0f + (1.0f / patternNo.y) * nHeight);
	pVtx[1].tex = D3DXVECTOR2(1.0f / patternNo.x + nPatternAnim * 1.0f / patternNo.x, 0.0f + (1.0f / patternNo.y) * nHeight);
	pVtx[2].tex = D3DXVECTOR2(0.0f + nPatternAnim * 1.0f / patternNo.x, 1.0f / patternNo.y + (1.0f / patternNo.y) * nHeight);
	pVtx[3].tex = D3DXVECTOR2(1.0f / patternNo.x + nPatternAnim * 1.0f / patternNo.x, 1.0f / patternNo.y + (1.0f / patternNo.y) * nHeight);

	// ���_�f�[�^�̃A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �G�t�F�N�g�̎g�p
//=============================================================================
void CEffect::SetEffect(EFFECTTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move,
	D3DXVECTOR3 moveSize, EASINGTYPE easingType, D3DXVECTOR3 rot, D3DXCOLOR col, int nLife,
	bool bGravity, float fResistance, bool bBillboard, int nPatternAnim, int nHeight,
	D3DXVECTOR3 centerPos, float fAngle, float fDistance, float fRotationSpeed)
{
	for (int nCount = 0; nCount < MAX_EFFECT; nCount++)
	{
		if (!m_pEffect[nCount]->m_bUse)
		{
			m_pEffect[nCount]->m_pos = pos;															// �ʒu���
			m_pEffect[nCount]->m_rot = rot;															// ��]���
			m_pEffect[nCount]->m_move = move;														// �ړ����
			m_pEffect[nCount]->m_col = col;															// �J���[
			m_pEffect[nCount]->m_nMaxLife = nLife;													// �������Ԃ̍ő吔
			m_pEffect[nCount]->m_nLife = 0;															// ��������
			m_pEffect[nCount]->m_bGravity = bGravity;												// �d�͂̎g�p
			m_pEffect[nCount]->m_type = type;														// �G�t�F�N�g�^�C�v
			m_pEffect[nCount]->m_AnimPage = nPatternAnim;											// �y�[�W��
			m_pEffect[nCount]->m_size = size;														// �傫��
			m_pEffect[nCount]->MakeVertex();														// ���_���̍쐬
			m_pEffect[nCount]->SpriteAnimation(m_aEffect[type].sprite, nPatternAnim, nHeight);		// �`��̈�̐ݒ�
			m_pEffect[nCount]->m_fAngle = fAngle;													// �p�x
			m_pEffect[nCount]->m_fDistance = fDistance;												// ����
			m_pEffect[nCount]->m_centerPos = centerPos;												// ���S�ʒu�̐ݒ�
			m_pEffect[nCount]->m_fResistance = fResistance;											// ��R
			m_pEffect[nCount]->m_fRotationSpeed = fRotationSpeed;									// ��]���x
			m_pEffect[nCount]->m_moveSize = moveSize;												// �T�C�Y�̉��Z�l
			m_pEffect[nCount]->m_bBillboard = bBillboard;											// �r���{�[�h�̗L��
			m_pEffect[nCount]->m_easingType = easingType;											// �C�[�W���O�̐ݒ�
			m_pEffect[nCount]->m_bUse = true;														// �g�p���ɐݒ�
			break;
		}
	}
}

//=============================================================================
// �����U��Ԃт�̃G�t�F�N�g
//=============================================================================
void CEffect::FallingPetals(bool bGravity, D3DXVECTOR3 pos, int nLife)
{
	float fAngle = float(CManager::GetRand(314)) - float(CManager::GetRand(314));
	float fAngle_x = float(CManager::GetRand(314)) - float(CManager::GetRand(314));

	D3DXVECTOR3 particlePos;

	// �ʒu�̌v�Z
	particlePos.x = cosf(D3DX_PI + fAngle) * cosf(D3DX_PI + fAngle_x);
	particlePos.y = sinf(D3DX_PI + fAngle_x);
	particlePos.z = sinf(D3DX_PI + fAngle) * cosf(D3DX_PI + fAngle_x);

	// �p�x
	fAngle = float(CManager::GetRand(314)) / 100.0f - float(CManager::GetRand(314)) / 100.0f;		// �����_���Ɏ擾

	D3DXVECTOR3 rot;
	rot = D3DXVECTOR3(sinf(fAngle) * 10, cosf(fAngle) * 10, 0.0f);			// ��]�ʂ̐ݒ�

	CEffect::SetEffect(EFFECTTYPE_ROSE,							// �G�t�F�N�g�^�C�v
		pos,													// �����ʒu
		D3DXVECTOR3(10.0f, 10.0f, 0.0f),						// �T�C�Y
		particlePos * 5.0f,										// �ړ�����
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// �T�C�Y��1�t���[��������̉��Z�l
		EASINGTYPE_NONE,										// �C�[�W���O�^�C�v
		rot,													// �e�N�X�`���̉�]
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),						// �J���[
		nLife,													// ���C�t
		bGravity,												// �d��
		0.025f,													// ��R
		true,													// �r���{�[�h
		0,														// �\������ӏ�(��)
		0,														// �\������ӏ�(�c)
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// ���S�ʒu
		0,														// �p�x
		0,														// ����
		0);														// ��]���x
}

//=============================================================================
// �W�܂�Ԃт�̃G�t�F�N�g
//=============================================================================
void CEffect::PetalsGather(D3DXVECTOR3 pos)
{
	float fAngle = float(CManager::GetRand(314)) - float(CManager::GetRand(314));
	float fAngle_x = float(CManager::GetRand(314)) - float(CManager::GetRand(314));

	D3DXVECTOR3 particlePos;

	// �ʒu�̌v�Z
	particlePos.x = cosf(D3DX_PI + fAngle) * cosf(D3DX_PI + fAngle_x);
	particlePos.y = sinf(D3DX_PI + fAngle_x);
	particlePos.z = sinf(D3DX_PI + fAngle) * cosf(D3DX_PI + fAngle_x);

	// �p�x
	fAngle = float(CManager::GetRand(314)) / 100.0f - float(CManager::GetRand(314)) / 100.0f;		// �����_���Ɏ擾

	D3DXVECTOR3 rot;
	rot = D3DXVECTOR3(sinf(fAngle) * 10, cosf(fAngle) * 10, 0.0f);		// ��]�ʂ̐ݒ�

	CEffect::SetEffect(EFFECTTYPE_ROSE,							// �G�t�F�N�g�^�C�v
		pos + particlePos * 200,								// �����ʒu
		D3DXVECTOR3(10.0f, 10.0f, 0.0f),						// �T�C�Y
		particlePos * -7,										// �ړ�����
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// �T�C�Y��1�t���[��������̉��Z�l
		EASINGTYPE_NONE,										// �C�[�W���O�^�C�v
		rot,													// �e�N�X�`���̉�]
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),						// �J���[
		30,														// ���C�t
		false,													// �d��
		0.025f,													// ��R
		true,													// �r���{�[�h
		0,														// �\������ӏ�(��)
		0,														// �\������ӏ�(�c)
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// ���S�ʒu
		0,														// �p�x
		0,														// ����
		0);														// ��]���x
}

//=============================================================================
// �Ԃт炪�����オ��G�t�F�N�g
//=============================================================================
void CEffect::PetalsSoaring(D3DXVECTOR3 pos)
{
	float fAngle = float(CManager::GetRand(314)) - float(CManager::GetRand(314));
	float fAngle_x = float(CManager::GetRand(314)) - float(CManager::GetRand(314));

	D3DXVECTOR3 particlePos;

	// �p�x
	fAngle = float(CManager::GetRand(314)) / 100.0f - float(CManager::GetRand(314)) / 100.0f;	// �����_���Ɏ擾

	// ��]
	D3DXVECTOR3 rot;
	rot = D3DXVECTOR3(sinf(fAngle) * 10, cosf(fAngle) * 10, 0.0f);		// ��]�ʂ̐ݒ�

	// ����
	float fDistance = float(CManager::GetRand(200 * 100)) / 100;		// �����_���Ɏ擾
	fDistance = 50.0f + fDistance;										// 50���Z����

	// �ʒu
	particlePos = pos;
	particlePos.x += sinf(D3DX_PI * fAngle) * fDistance;
	particlePos.z += cosf(D3DX_PI * fAngle) * fDistance;

	CEffect::SetEffect(EFFECTTYPE_ROSE,							// �G�t�F�N�g�^�C�v
		particlePos,											// �����ʒu
		D3DXVECTOR3(10.0f, 10.0f, 0.0f),						// �T�C�Y
		D3DXVECTOR3(0.0f, 5.0f, 0.0f),							// �ړ�����
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// �T�C�Y�̃t���[��������̉��Z�l
		EASINGTYPE_NONE,										// �C�[�W���O�^�C�v
		rot,													// �e�N�X�`���̉�]
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),						// �J���[
		250,													// ���C�t
		false,													// �d��
		0.015f,													// ��R
		true,													// �r���{�[�h
		0,														// �\������ӏ�(��)
		0,														// �\������ӏ�(�c)
		pos,													// ���S�ʒu
		fAngle,													// �p�x
		fDistance,												// ����
		0.01f);													// ��]���x
}

//=============================================================================
// �H�������G�t�F�N�g
//=============================================================================
void CEffect::WingStay(D3DXVECTOR3 pos)
{
	float fAngle = float(CManager::GetRand(314)) - float(CManager::GetRand(314));
	float fAngle_x = float(CManager::GetRand(314)) - float(CManager::GetRand(314));

	D3DXVECTOR3 particlePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �ʒu�̌v�Z
	particlePos.x = cosf(D3DX_PI + fAngle) * cosf(D3DX_PI + fAngle_x);
	particlePos.y = sinf(D3DX_PI + fAngle_x);
	particlePos.z = sinf(D3DX_PI + fAngle) * cosf(D3DX_PI + fAngle_x);

	// �p�x
	fAngle = float(CManager::GetRand(314)) / 100.0f - float(CManager::GetRand(314)) / 100.0f;		// �����_���Ɏ擾

	// ��]
	D3DXVECTOR3 rot;
	rot = D3DXVECTOR3(sinf(fAngle) * 10, cosf(fAngle) * 10, 0.0f);		// �����_���Ɏ擾

	// ����
	float fDistance = float(CManager::GetRand((int)350 * 100)) / 100;	// �����_���Ɏ擾
	fDistance = 50.0f + fDistance;										// 50���Z����

	// �X�s�[�h
	float fSpeed = float(CManager::GetRand((int)350 * 100)) / 100;		// �����_���Ɏ擾
	fSpeed = 50 + fSpeed;												// 50���Z����

	CEffect::SetEffect(EFFECTTYPE_WING,							// �G�t�F�N�g�^�C�v
		pos + particlePos * fDistance,							// �����ʒu
		D3DXVECTOR3(5.0f, 5.0f, 0.0f),							// �T�C�Y
		particlePos,											// �ړ�����
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// �T�C�Y�̃t���[��������̉��Z�l
		EASINGTYPE_NONE,										// �C�[�W���O�^�C�v
		rot,													// �e�N�X�`���̉�]
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),						// �J���[
		250,													// ���C�t
		false,													// �d��
		0.025f,													// ��R
		true,													// �r���{�[�h
		0,														// �\������ӏ�(��)
		0,														// �\������ӏ�(�c)
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// ���S�ʒu
		0,														// �p�x
		0,														// ����
		0);														// ��]���x
}

//=============================================================================
// �����̃G�t�F�N�g
//=============================================================================
void CEffect::SandSmokeEffect(D3DXVECTOR3 pos)
{
	for (int nCount = 0; nCount < 15; nCount++)
	{
		float fAngle = float(CManager::GetRand(314)) / 100.0f - float(CManager::GetRand(314)) / 100.0f;
		float fRadius = float(CManager::GetRand(3 * 100)) / 100.0f - float(CManager::GetRand(3 * 100)) / 100.0f;

		D3DXVECTOR3 particlePos;

		// �ʒu�̌v�Z
		particlePos.x = sinf(D3DX_PI * fAngle) * (3.0f);
		particlePos.y = 1.0f;
		particlePos.z = cosf(D3DX_PI * fAngle) * (3.0f);

		D3DXMATRIX mtxMeshRot, mtxMeshTrans;					// �v�Z�p�}�g���b�N�X
		D3DXMATRIX mtx;											// ����̃}�g���b�N�X

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtx);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, 0.0f, 0.0f, 0.0f);
		D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxMeshTrans, particlePos.x, particlePos.y, particlePos.z);
		D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshTrans);

		// �T�C�Y
		float size = CManager::GetRand(100) / 10.0f;			// �����_���Ɏ擾

		CEffect::SetEffect(EFFECTTYPE_SANDSMOKE,				// �G�t�F�N�g�^�C�v
			pos,												// �����ʒu
			D3DXVECTOR3(size, size, 0.0f),						// �T�C�Y
			D3DXVECTOR3(mtx._41, mtx._42, mtx._43),				// �ړ�����
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),						// �T�C�Y�̃t���[��������̉��Z�l
			EASINGTYPE_NONE,									// �C�[�W���O�^�C�v
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),						// �e�N�X�`���̉�]
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f),					// �J���[
			100,												// ���C�t
			false,												// �d��
			0.025f,												// ��R
			true,												// �r���{�[�h�̎g�p
			0,													// �\������ӏ�(��)
			0,													// �\������ӏ�(�c)
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),						// ���S�ʒu
			0,													// �p�x
			0,													// ����
			0);													// ��]���x
	}
}

//=============================================================================
// ��������
//=============================================================================
void CEffect::SandSmokeExplosion(D3DXVECTOR3 pos)
{
	for (int nCount = 0; nCount < 100; nCount++)
	{
		float fAngle = float(CManager::GetRand(314)) / 100.0f - float(CManager::GetRand(314)) / 100.0f;
		float fRadius = float(CManager::GetRand(4 * 100)) / 100 - float(CManager::GetRand(4 * 100)) / 100;

		D3DXVECTOR3 particlePos;

		D3DXVECTOR3 vecPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXMATRIX	mtxMeshRot, mtxMeshTrans;				//�v�Z�p�}�g���b�N�X
		D3DXMATRIX mtx;			// ����̃}�g���b�N�X
		D3DXMATRIX mtxPlayer;

		// �X�s�[�h
		float fSpeed = float(CManager::GetRand(15 * 100)) / 100;		// �����_���Ɏ擾
		particlePos.y = 1 + fSpeed;
		particlePos.x = sinf(D3DX_PI * fAngle) * (fSpeed);
		particlePos.z = cosf(D3DX_PI * fAngle) * (fSpeed);

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtx);
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtxPlayer);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, 0.0f, 0.0f, 0.0f);
		D3DXMatrixMultiply(&mtxPlayer, &mtxPlayer, &mtxMeshRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxMeshTrans, vecPos.x, vecPos.y, vecPos.z);
		D3DXMatrixMultiply(&mtxPlayer, &mtxPlayer, &mtxMeshTrans);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, 0.0f, 0.0f, 0.0f);
		D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxMeshTrans, particlePos.x, particlePos.y, particlePos.z);
		D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshTrans);

		// �}�g���b�N�X�̍���
		D3DXMatrixMultiply(&mtx, &mtx, &mtxPlayer);

		D3DXVECTOR3 circlePos = pos;

		// �ʒu�v�Z
		circlePos.x += sinf(D3DX_PI * fAngle) * 30;
		circlePos.z += cosf(D3DX_PI * fAngle) * 30;

		CEffect::SetEffect(EFFECTTYPE_SANDSMOKE,				// �G�t�F�N�g�^�C�v
			circlePos,											// �����ʒu
			D3DXVECTOR3(15.0f, 15.0f, 0.0f),					// �T�C�Y
			D3DXVECTOR3(mtx._41, mtx._42, mtx._43),				// �ړ�����
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),						// �T�C�Y�̃t���[��������̉��Z�l
			EASINGTYPE_NONE,									// �C�[�W���O�^�C�v
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),						// �e�N�X�`���̉�]
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f),					// �J���[
			15,													// ���C�t
			false,												// �d��
			0.025f,												// ��R
			true,												// �r���{�[�h
			0,													// �\������ӏ�(��)
			0,													// �\������ӏ�(�c)
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),						// ���S�ʒu
			0,													// �p�x
			0,													// ����
			0);													// ��]���x
	}
}

//=============================================================================
// �V���b�N�E�E�F�[�u
//=============================================================================
void CEffect::Shockwave(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife)
{
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera != NULL)
	{// �J���������݂��Ă����Ƃ�
		CEffect::SetEffect(EFFECTTYPE_SHOCKWAVE,				// �G�t�F�N�g�^�C�v
			pos,												// �����ʒu
			D3DXVECTOR3(15.0f, 15.0f, 0.0f),					// �T�C�Y
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),						// �ړ�����
			D3DXVECTOR3(10.0f, 10.0f, 0.0f),					// �T�C�Y�̃t���[��������̉��Z�l
			EASINGTYPE_NONE,									// �C�[�W���O�^�C�v
			D3DXVECTOR3(0.0f, rot.y - pCamera->GetRotation().y, 0.0f),	// �e�N�X�`���̉�]
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f),					// �J���[
			nLife,												// ���C�t
			false,												// �d��
			0.025f,												// ��R
			false,												// �r���{�[�h
			0,													// �\������ӏ�(��)
			0,													// �\������ӏ�(�c)
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),						// ���S�ʒu
			0,													// �p�x
			0,													// ����
			0);													// ��]���x
	}
}

//=============================================================================
// �n���[����
//=============================================================================
void CEffect::Halo(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	float fAngle = float(CManager::GetRand(314)) / 100.0f - float(CManager::GetRand(314)) / 100.0f;

	D3DXVECTOR3 rot;

	// ��]
	rot = D3DXVECTOR3(0.0f, 0.0f, cosf(fAngle) * 10);			// �����_���Ɏ擾

	CEffect::SetEffect(EFFECTTYPE_HALO,							// �G�t�F�N�g�^�C�v
		pos,													// �����ʒu
		D3DXVECTOR3(2.0f * size.x, 2.0f * size.y, 0.0f),		// �T�C�Y
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// �ړ�����
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// �T�C�Y�̃t���[��������̉��Z�l
		EASINGTYPE_NONE,										// �C�[�W���O�^�C�v
		rot,													// �e�N�X�`���̉�]
		D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.8f),						// �J���[
		15,														// ���C�t
		false,													// �d��
		0.025f,													// ��R
		true,													// �r���{�[�h
		0,														// �\������ӏ�(��)
		0,														// �\������ӏ�(�c)
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// ���S�ʒu
		0,														// �p�x
		0,														// ����
		0);														// ��]���x
}

//=============================================================================
// �򉻕��U
//=============================================================================
void CEffect::Purification(D3DXVECTOR3 pos)
{
	float fAngle = float(CManager::GetRand(314)) / 100.0f - float(CManager::GetRand(314)) / 100.0f;

	D3DXVECTOR3 particlePos;

	D3DXMATRIX mtxMeshRot, mtxMeshTrans;				//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtx;										// ����̃}�g���b�N�X

	// ���x
	float fSpeed = float(CManager::GetRand(3 * 100)) / 100;
	particlePos.y = 1.0f + fSpeed;

	// �ʒu�̌v�Z
	particlePos.x = sinf(D3DX_PI * fAngle) * (fSpeed);
	particlePos.z = cosf(D3DX_PI * fAngle) * (fSpeed);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtx);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxMeshTrans, particlePos.x, particlePos.y, particlePos.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshTrans);

	D3DXVECTOR3 point = pos;

	// �����ʒu�̌v�Z
	point.x += sinf(D3DX_PI * fAngle) * 80;
	point.z += cosf(D3DX_PI * fAngle) * 80;

	CEffect::SetEffect(EFFECTTYPE_SMOKE,						// �G�t�F�N�g�^�C�v
		point,													// �����ʒu
		D3DXVECTOR3(20.0f, 20.0f, 0.0f),						// �T�C�Y
		D3DXVECTOR3(0.0f, mtx._42, 0.0f),						// �ړ�����
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// �T�C�Y�̃t���[��������̉��Z�l
		EASINGTYPE_NONE,										// �C�[�W���O�^�C�v
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// �e�N�X�`���̉�]
		D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f),						// �J���[
		50,														// ���C�t
		false,													// �d��
		0.025f,													// ��R
		true,													// �r���{�[�h
		0,														// �\������ӏ�(��)
		0,														// �\������ӏ�(�c)
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// ���S�ʒu
		0,														// �p�x
		0,														// ����
		0);														// ��]���x
}

//=============================================================================
// �X�^�[
//=============================================================================
void CEffect::Star(D3DXVECTOR3 pos)
{
	CEffect::SetEffect(EFFECTTYPE_STAR,							// �G�t�F�N�g�^�C�v
		pos,													// �����ʒu
		D3DXVECTOR3(50.0f, 50.0f, 0.0f),						// �T�C�Y
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// �ړ�����
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// �T�C�Y�̃t���[��������̉��Z�l
		EASINGTYPE_NONE,										// �C�[�W���O�^�C�v
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// �e�N�X�`���̉�]
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),						// �J���[
		15,														// ���C�t
		false,													// �d��
		0.025f,													// ��R
		true,													// �r���{�[�h
		0,														// �\������ӏ�(��)
		0,														// �\������ӏ�(�c)
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// ���S�ʒu
		0,														// �p�x
		0,														// ����
		0);														// ��]���x
}

//=============================================================================
// �a��
//=============================================================================
void CEffect::Slashing(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	D3DXMATRIX mtxMeshRot, mtxMeshTrans;				//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtx;										// ����̃}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtx);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxMeshTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshTrans);

	CEffect::SetEffect(EFFECTTYPE_SLASH,						// �G�t�F�N�g�^�C�v
		pos,													// �����ʒu
		D3DXVECTOR3(50.0f, 150.0f, 0.0f),						// �T�C�Y
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// �ړ�����
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// �T�C�Y�̃t���[��������̉��Z�l
		EASINGTYPE_NONE,										// �C�[�W���O�^�C�v
		rot,													// �e�N�X�`���̉�]
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),						// �J���[
		15,														// ���C�t
		false,													// �d��
		0.025f,													// ��R
		true,													// �r���{�[�h
		0,														// �\������ӏ�(��)
		0,														// �\������ӏ�(�c)
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// ���S�ʒu
		0,														// �p�x
		0,														// ����
		0);														// ��]���x
}

//=============================================================================
// �I�[��
//=============================================================================
void CEffect::Aura(int nCount, bool bJump, D3DXVECTOR3 pos)
{
	float fAngle = float(CManager::GetRand(314)) - float(CManager::GetRand(314));
	float fAngle_x = float(CManager::GetRand(314)) - float(CManager::GetRand(314));

	D3DXVECTOR3 pointPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �p�x
	fAngle = float(CManager::GetRand(314)) / 100.0f - float(CManager::GetRand(314)) / 100.0f;		// �����_���Ɏ擾

	// ��]
	D3DXVECTOR3 rot;
	rot = D3DXVECTOR3(sinf(fAngle) * 10, cosf(fAngle) * 10, 0.0f);			// �����_���Ɏ擾

	// ����
	float fDistance = float(CManager::GetRand(200 * 100)) / 100;			// �����_���Ɏ擾
	fDistance = 50.0f + fDistance;

	pointPos = pos;

	// �ʒu�̌v�Z
	pointPos.x += sinf(D3DX_PI * fAngle) * fDistance;
	pointPos.z += cosf(D3DX_PI * fAngle) * fDistance;

	CEffect::SetEffect(EFFECTTYPE_SPHERE,						// �G�t�F�N�g�^�C�v
		pointPos,												// �����ʒu
		D3DXVECTOR3(5.0f, 5.0f, 0.0f),							// �T�C�Y
		D3DXVECTOR3(0.0f, 5.0f, 0.0f),							// �ړ�����
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// �T�C�Y�̃t���[��������̉��Z�l
		EASINGTYPE_NONE,										// �C�[�W���O�^�C�v
		rot,													// �e�N�X�`���̉�]
		D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f),						// �J���[
		100,													// ���C�t
		false,													// �d��
		0.005f,													// ��R
		true,													// �r���{�[�h
		0,														// �\������ӏ�(��)
		0,														// �\������ӏ�(�c)
		pos,													// ���S�ʒu
		0,														// �p�x
		0,														// ����
		0);														// ��]���x

	// �ʒu�̃��Z�b�g
	pointPos = pos;

	// ��]
	rot = D3DXVECTOR3(sinf(fAngle) * 10, cosf(fAngle) * 10, 0.0f);		// �����_���Ɏ擾

	// ����
	fDistance = float(CManager::GetRand(50 * 100)) / 100;				// �����_���Ɏ擾

	// �ʒu�̌v�Z
	pointPos.x += sinf(D3DX_PI * fAngle) * fDistance;
	pointPos.z += cosf(D3DX_PI * fAngle) * fDistance;

	CEffect::SetEffect(EFFECTTYPE_AURA,							// �G�t�F�N�g�^�C�v
		pointPos,												// �����ʒu
		D3DXVECTOR3(50.0f, 50.0f, 0.0f),						// �T�C�Y
		D3DXVECTOR3(0.0f, 5.0f, 0.0f),							// �ړ�����
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// �T�C�Y�̃t���[��������̉��Z�l
		EASINGTYPE_NONE,										// �C�[�W���O�^�C�v
		rot,													// �e�N�X�`���̉�]
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f),						// �J���[
		5 + CManager::GetRand(50),								// ���C�t
		false,													// �d��
		0.005f,													// ��R
		true,													// �r���{�[�h
		0,														// �\������ӏ�(��)
		0,														// �\������ӏ�(�c)
		pos,													// ���S�ʒu
		0,														// �p�x
		0,														// ����
		0);														// ��]���x

	if (!bJump)
	{// �W�����v���Ă��Ȃ������Ƃ�
		if (nCount % 20 == 0)
		{// ���܂肪0�������Ƃ�
			CEffect::SetEffect(EFFECTTYPE_SPHERE,				// �G�t�F�N�g�^�C�v
				pos,											// �����ʒu
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),					// �T�C�Y
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),					// �ړ�����
				D3DXVECTOR3(5.0f, 5.0f, 0.0f),					// �T�C�Y�̃t���[��������̉��Z�l
				EASINGTYPE_OUT,									// �C�[�W���O�^�C�v
				D3DXVECTOR3(1.57f, 0.0f, 0.0f),					// �e�N�X�`���̉�]
				D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.8f),				// �J���[
				50,												// ���C�t
				false,											// �d��
				0.025f,											// ��R
				false,											// �r���{�[�h
				0,												// �\������ӏ�(��)
				0,												// �\������ӏ�(�c)
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),					// ���S�ʒu
				0,												// �p�x
				0,												// ����
				0);												// ��]���x
		}

		if (nCount % 15 == 0)
		{// ���܂肪0�������Ƃ�
			CEffect::SetEffect(EFFECTTYPE_SHOCKWAVE,			// �G�t�F�N�g�^�C�v
				pos,											// �����ʒu
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),					// �T�C�Y
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),					// �ړ�����
				D3DXVECTOR3(4.0f, 4.0f, 0.0f),					// �T�C�Y�̃t���[��������̉��Z�l
				EASINGTYPE_OUT,									// �C�[�W���O�^�C�v
				D3DXVECTOR3(1.57f, 0.0f, 0.0f),					// �e�N�X�`���̉�]
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f),				// �J���[
				50,												// ���C�t
				false,											// �d��
				0.025f,											// ��R
				false,											// �r���{�[�h
				0,												// �\������ӏ�(��)
				0,												// �\������ӏ�(�c)
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),					// ���S�ʒu
				0,												// �p�x
				0,												// ����
				0);												// ��]���x
		}
	}
}

//=============================================================================
// �Ԃт�̏W����
//=============================================================================
void CEffect::PetalCluster(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int nCount = 0; nCount < 150; nCount++)
	{
		float fAngle = float(CManager::GetRand(314)) / 100.0f - float(CManager::GetRand(314)) / 100.0f;
		float fRadius = float(CManager::GetRand(10 * 100)) / 100.0f - float(CManager::GetRand(10 * 100)) / 100.0f;

		D3DXVECTOR3 petalPos;

		// �ʒu�̌v�Z
		petalPos.x = sinf(D3DX_PI * fAngle) * (fRadius);
		petalPos.y = 15.0f;
		petalPos.z = cosf(D3DX_PI * fAngle) * (fRadius);

		D3DXVECTOR3 vecPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXMATRIX	mtxMeshRot, mtxMeshTrans;				//�v�Z�p�}�g���b�N�X
		D3DXMATRIX mtx;										// ����̃}�g���b�N�X
		D3DXMATRIX mtxPlayer;

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtx);

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtxPlayer);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, rot.y + D3DX_PI, 1.54f, rot.z);
		D3DXMatrixMultiply(&mtxPlayer, &mtxPlayer, &mtxMeshRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxMeshTrans, vecPos.x, vecPos.y, vecPos.z);
		D3DXMatrixMultiply(&mtxPlayer, &mtxPlayer, &mtxMeshTrans);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, 0.0f, 0.0f, 0.0f);
		D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxMeshTrans, petalPos.x, petalPos.y, petalPos.z);
		D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshTrans);

		D3DXMatrixMultiply(&mtx, &mtx, &mtxPlayer);

		D3DXVECTOR3 petalRot;
		petalRot = D3DXVECTOR3(sinf(fAngle) * 10, cosf(fAngle) * 10, 0.0f);

		CEffect::SetEffect(EFFECTTYPE_ROSE,							// �G�t�F�N�g�^�C�v
			pos,	// �����ʒu										// �����ʒu
			D3DXVECTOR3(10.0f, 10.0f, 0.0f),						// �T�C�Y
			D3DXVECTOR3(mtx._41, mtx._42, mtx._43),					// �ړ�����
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// �T�C�Y�̃t���[��������̉��Z�l
			EASINGTYPE_NONE,										// �C�[�W���O�^�C�v
			petalRot,												// �e�N�X�`���̉�]
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),						// �J���[
			50,														// ���C�t
			false,													// �d��
			0,														// ��R
			true,													// �r���{�[�h
			0,														// �\������ӏ�(��)
			0,														// �\������ӏ�(�c)
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// ���S�ʒu
			0,														// �p�x
			0,														// ����
			0);														// ��]���x
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void CEffect::MakeVertex(void)
{
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
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//�e�N�X�`���`�ʂ̈ʒu
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�f�[�^�̃A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �A�j���[�V��������
//=============================================================================
void CEffect::Animation(void)
{
	// �A�j���[�V����
	if (m_AnimCount >= m_nMaxLife / m_aEffect[m_type].sprite.x)
	{
		m_AnimPage++;		// �y�[�W��1���Z����

		SpriteAnimation(m_aEffect[m_type].sprite, m_AnimPage, 0);		// �`��͈�
		m_AnimCount = 0;		// ������
	}

	m_AnimCount++;				// �J�E���^��1�v���X����
}

#ifdef _DEBUG
//=============================================================================
// �f�o�b�O����
//=============================================================================
void CEffect::Debug(void)
{

}
#endif