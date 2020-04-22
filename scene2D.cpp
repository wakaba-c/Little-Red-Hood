//=============================================================================
//
// scene2D���� [Scene2D.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "scene2D.h"
#include "manager.h"
#include "renderer.h"
#include "inputKeyboard.h"
#include "inputMouse.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CScene2D::CScene2D(CScene::PRIORITY obj) : CScene(obj)
{
	// �D��x �̐ݒ�
	SetObjType(obj);

	// �l�̏�����
	m_pTexture = NULL;								// �e�N�X�`���̏�����
	m_pVtxBuff = NULL;								// ���_�o�b�t�@�̏�����
	m_Vtx = TEXTUREVTX_CENTER;						// ���S�ʒu�̏�����
	m_size = D3DXVECTOR3(10.0f, 10.0f, 0.0f);		// �T�C�Y�̏�����
	m_DrawPos = D3DXVECTOR2(0.0f, 0.0f);			// �`��ʒu�̏�����
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ��]�̏�����
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// �F�̏�����
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScene2D::~CScene2D()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CScene2D::Init(void)
{
	CRenderer *Renderer = CManager::GetRenderer();
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = Renderer->GetDevice();

	D3DXVECTOR3 pos = GetPosition();				// �ʒu�̎擾

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	m_fAngle = atan2f(m_size.x, m_size.y);
	m_fLength = sqrtf((m_size.x * m_size.x) + (m_size.y * m_size.y)) / 2;


	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(pos.x - m_size.x / 2, pos.y - m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + m_size.x / 2, pos.y - m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x - m_size.x / 2, pos.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + m_size.x / 2, pos.y, 0.0f);

	// 1.0�ŌŒ�A�������W
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�f�[�^�̃A�����b�N
	m_pVtxBuff->Unlock();
	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CScene2D::Uninit(void)
{
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
void CScene2D::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CScene2D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	CRenderer *pRenderer = CManager::GetRenderer();

	//�f�o�C�X���擾����
	pDevice = pRenderer->GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�Ƀo�C���h
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//�e�N�X�`���̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// �N���G�C�g����
//=============================================================================
CScene2D *CScene2D::Create(CScene::PRIORITY obj)
{
	CScene2D *pScene2D;
	pScene2D = new CScene2D(obj);
	pScene2D->Init();
	return pScene2D;
}

//=============================================================================
// �e�N�X�`���̐ݒ�
//=============================================================================
void CScene2D::BindTexture(LPDIRECT3DTEXTURE9 tex)
{
	m_pTexture = tex;
}

//=============================================================================
// ���_���̍X�V
//=============================================================================
void CScene2D::SetTransform(void)
{
	VERTEX_2D *pVtx;										// ���_���ւ̃|�C���^
	D3DXVECTOR3 pos = GetPosition();						// �ʒu�̎擾

	m_fAngle = atan2f(m_size.x, m_size.y);
	m_fLength = sqrtf((m_size.x * m_size.x) + (m_size.y * m_size.y)) / 2;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	switch (m_Vtx)
	{
	case TEXTUREVTX_CENTER:
		//���_���W�̐ݒ�
		pVtx[0].pos.x = pos.x + sinf(-D3DX_PI + m_fAngle + (D3DX_PI * m_rot.z)) * m_fLength;
		pVtx[0].pos.y = pos.y + cosf(-D3DX_PI + m_fAngle + (D3DX_PI * m_rot.z)) * m_fLength;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pos.x + sinf(D3DX_PI - m_fAngle + (D3DX_PI * m_rot.z)) * m_fLength;
		pVtx[1].pos.y = pos.y + cosf(D3DX_PI - m_fAngle + (D3DX_PI * m_rot.z)) * m_fLength;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pos.x + sinf(-m_fAngle + (D3DX_PI * m_rot.z)) * m_fLength;
		pVtx[2].pos.y = pos.y + cosf(-m_fAngle + (D3DX_PI * m_rot.z)) * m_fLength;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pos.x + sinf(m_fAngle + (D3DX_PI * m_rot.z)) * m_fLength;
		pVtx[3].pos.y = pos.y + cosf(m_fAngle + (D3DX_PI * m_rot.z)) * m_fLength;
		pVtx[3].pos.z = 0.0f;
		break;

	case TEXTUREVTX_LEFT:
		//���_���W�̐ݒ�
		pVtx[0].pos.x = cosf(D3DX_PI * m_rot.z) - -m_size.y * sinf(D3DX_PI * m_rot.z) + pos.x;
		pVtx[0].pos.y = sinf(D3DX_PI * m_rot.z) + -m_size.y * cosf(D3DX_PI * m_rot.z) + pos.y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = m_size.x / 2 * cosf(D3DX_PI * m_rot.z) - -m_size.y * sinf(D3DX_PI * m_rot.z) + pos.x;
		pVtx[1].pos.y = m_size.x / 2 * sinf(D3DX_PI * m_rot.z) + -m_size.y * cosf(D3DX_PI * m_rot.z) + pos.y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = cosf(D3DX_PI * m_rot.z) - sinf(D3DX_PI * m_rot.z) + pos.x;
		pVtx[2].pos.y = sinf(D3DX_PI * m_rot.z) + cosf(D3DX_PI * m_rot.z) + pos.y;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = m_size.x / 2 * cosf(D3DX_PI * m_rot.z) - sinf(D3DX_PI * m_rot.z) + pos.x;
		pVtx[3].pos.y = m_size.x / 2 * sinf(D3DX_PI * m_rot.z) + cosf(D3DX_PI * m_rot.z) + pos.y;
		pVtx[3].pos.z = 0.0f;
		break;
	}

	//���_�f�[�^�̃A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �摜�̔��]
//=============================================================================
void CScene2D::SetTransformTurnOver(void)
{
	VERTEX_2D *pVtx;										//���_���ւ̃|�C���^
	D3DXVECTOR3 pos[4];

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W�̍X�V
	pos[0] = pVtx[0].pos;
	pos[1] = pVtx[1].pos;
	pos[2] = pVtx[2].pos;
	pos[3] = pVtx[3].pos;

	pVtx[0].pos = pos[1];
	pVtx[1].pos = pos[0];
	pVtx[2].pos = pos[3];
	pVtx[3].pos = pos[2];

	//���_�f�[�^�̃A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �T�C�Y�ݒ菈��
//=============================================================================
void CScene2D::SetSize(D3DXVECTOR3 size)
{
	m_size = size;
}

//=============================================================================
// �e�N�X�`���`��ʒu�̍X�V
//=============================================================================
void CScene2D::SetDrawPos(D3DXVECTOR2 tex)
{
	VERTEX_2D *pVtx;										//���_���ւ̃|�C���^

	m_DrawPos = tex;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W�̍X�V
	pVtx[0].tex = D3DXVECTOR2(0.0f + tex.x, 0.0f + tex.y);
	pVtx[1].tex = D3DXVECTOR2(1.0f + tex.x, 0.0f + tex.y);
	pVtx[2].tex = D3DXVECTOR2(0.0f + tex.x, 1.0f + tex.y);
	pVtx[3].tex = D3DXVECTOR2(1.0f + tex.x, 1.0f + tex.y);

	//���_�f�[�^�̃A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �e�N�X�`���`��ʒu�̍X�V
//=============================================================================
void CScene2D::SpriteAnimation(D3DXVECTOR2 patternNo, int nPatternAnim, int nHeight)
{//�c�Ɖ��̃p�^�[�����ƌ��݂̃y�[�W�ƍs

	VERTEX_2D *pVtx;										//���_���ւ̃|�C���^

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W�̍X�V
	pVtx[0].tex = D3DXVECTOR2(0.0f + nPatternAnim * 1.0f / patternNo.x, 0.0f + (1.0f / patternNo.y) * nHeight);
	pVtx[1].tex = D3DXVECTOR2(1.0f / patternNo.x + nPatternAnim * 1.0f / patternNo.x, 0.0f + (1.0f / patternNo.y) * nHeight);
	pVtx[2].tex = D3DXVECTOR2(0.0f + nPatternAnim * 1.0f / patternNo.x, 1.0f / patternNo.y + (1.0f / patternNo.y) * nHeight);
	pVtx[3].tex = D3DXVECTOR2(1.0f / patternNo.x + nPatternAnim * 1.0f / patternNo.x, 1.0f / patternNo.y + (1.0f / patternNo.y) * nHeight);

	//���_�f�[�^�̃A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �e�N�X�`���[�̒��_�J���[�̓K��
//=============================================================================
void CScene2D::SetColor(D3DXCOLOR col)
{
	VERTEX_2D *pVtx;										//���_���ւ̃|�C���^

	m_col = col;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W�̍X�V
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//���_�f�[�^�̃A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �e�N�X�`�����S�ʒu�̍X�V
//=============================================================================
void CScene2D::SetCenter(TEXTUREVTX vtx)
{
	//���S�ʒu�̐ݒ�
	m_Vtx = vtx;
}

//=============================================================================
// �e�N�X�`���`��ʒu�̍X�V
//=============================================================================
void CScene2D::SetRotation(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//=============================================================================
// �e�N�X�`���̓����蔻��(Hit)
//=============================================================================
bool CScene2D::HitCollision2D(CScene2D *obj, bool bTrigger)
{
	bool bJudge = false;
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 posOld = GetPosOld();

	D3DXVECTOR3 objPos = obj->GetPosition();
	D3DXVECTOR3 objPosOld = obj->GetPosOld();

	switch (m_Vtx)
	{
		//�摜�̒��S�������������Ƃ�
	case TEXTUREVTX_CENTER:
		//�㉺����
		if ((objPosOld.x + obj->m_size.x / 2 >= posOld.x - m_size.x / 2) &&
			(objPosOld.x - obj->m_size.x / 2 <= posOld.x + m_size.x / 2))
		{
			//��
			if ((objPos.y + obj->m_size.y / 2 > pos.y - m_size.y / 2) &&
				(objPosOld.y + obj->m_size.y / 2 <= posOld.y - m_size.y / 2))
			{
				bJudge = true;

				//�g���K�[�ł͖����Ƃ�
				if (!bTrigger)
				{
					objPos.y = pos.y - m_size.y / 2;
					obj->SetPosition(objPos);
				}
			}

			//��
			else if ((objPos.y - obj->m_size.y / 2 < pos.y + m_size.y / 2) &&
				(objPosOld.y - obj->m_size.y / 2 >= posOld.y + m_size.y / 2))
			{
				OutputDebugString("�����蔻��\n");
				bJudge = true;
				//�g���K�[�ł͖����Ƃ�
				if (!bTrigger)
				{
					objPos.y = pos.y - m_size.y / 2;
					obj->SetPosition(objPos);
				}
			}
		}

		if (!bJudge)
		{
			//���E����
			if ((objPosOld.y + obj->m_size.y / 2 > posOld.y - m_size.y / 2) &&
				(objPosOld.y - obj->m_size.y / 2 <= posOld.y + m_size.y / 2))
			{
				//�v���C���[�̔���
				if (objPos.x + obj->m_size.x / 2 > pos.x - m_size.x / 2 &&
					(objPosOld.x + obj->m_size.x / 2 <= posOld.x - m_size.x / 2))
				{
					bJudge = true;
					//�g���K�[�ł͖����Ƃ�
					if (!bTrigger)
					{
						objPos.y = pos.y - m_size.y / 2;
						obj->SetPosition(objPos);
					}
				}
				else if (objPos.x - obj->m_size.x / 2 < pos.x + m_size.x / 2 &&
					(objPos.x - obj->m_size.x / 2 >= posOld.x + m_size.x / 2))
				{
					bJudge = true;

					//�g���K�[�ł͖����Ƃ�
					if (!bTrigger)
					{
						objPos.y = pos.y - m_size.y / 2;
						obj->SetPosition(objPos);
					}
				}
			}
		}
		break;

		//�摜�̒��S�����l�߂������Ƃ�
	case TEXTUREVTX_LEFT:
		//�㉺����
		if ((objPosOld.x + obj->m_size.x / 2 >= posOld.x - m_size.x / 2) &&
			(objPosOld.x - obj->m_size.x / 2 <= posOld.x + m_size.x / 2))
		{
			//��
			if ((objPos.y + obj->m_size.y / 2 > pos.y - m_size.y) &&
				(objPosOld.y + obj->m_size.y / 2 <= posOld.y - m_size.y))
			{
				bJudge = true;

				//�g���K�[�ł͖����Ƃ�
				if (!bTrigger)
				{
					//�v���C���[�̈ʒu��␳
					objPos.y = pos.y - m_size.y - obj->m_size.y / 2;
					obj->SetPosition(objPos);
					objPosOld.y = pos.y - m_size.y - obj->m_size.y / 2;
					obj->SetPosOld(objPosOld);
				}
			}

			//��
			else if ((objPos.y - obj->m_size.y / 2 < pos.y) &&
				(objPosOld.y - obj->m_size.y / 2 >= posOld.y))
			{
				OutputDebugString("�����蔻��\n");
				bJudge = true;
				//�g���K�[�ł͖����Ƃ�
				if (!bTrigger)
				{
					//�v���C���[�̈ʒu��␳
					objPos.y = pos.y + obj->m_size.y / 2;
					obj->SetPosition(objPos);
					objPosOld.y = pos.y + obj->m_size.y / 2;
					SetPosOld(objPosOld);
				}
			}
		}

		if (!bJudge)
		{
			//���E����
			if ((objPosOld.y + obj->m_size.y / 2 > posOld.y - m_size.y) &&
				(objPosOld.y - obj->m_size.y / 2 <= posOld.y))
			{
				//�v���C���[�̔���
				if (objPos.x + obj->m_size.x / 2 > pos.x - m_size.x / 2 &&
					(objPosOld.x + obj->m_size.x / 2 <= posOld.x - m_size.x / 2))
				{
					bJudge = true;
					//�g���K�[�ł͖����Ƃ�
					if (!bTrigger)
					{
						objPos.x = pos.x - m_size.x / 2 - obj->m_size.x / 2;
						obj->SetPosition(objPos);
						objPosOld.x = pos.x - m_size.x / 2 - obj->m_size.x / 2;
						SetPosOld(objPosOld);
					}
				}
				else if (objPos.x - obj->m_size.x / 2 < pos.x + m_size.x / 2 &&
					(objPosOld.x - obj->m_size.x / 2 >= posOld.x + m_size.x / 2))
				{
					bJudge = true;

					//�g���K�[�ł͖����Ƃ�
					if (!bTrigger)
					{
						objPos.x = pos.x + m_size.x / 2 + obj->m_size.x / 2;
						obj->SetPosition(objPos);
						objPosOld.x = pos.x + m_size.x / 2 + obj->m_size.x / 2;
						SetPosOld(objPosOld);
					}
				}
			}
		}
		break;
	}

	//���ʂ�Ԃ�
	return bJudge;
}

//=============================================================================
// �e�N�X�`���̓����蔻��
//=============================================================================
bool CScene2D::InCollision2D(CScene2D *obj)
{
	bool bJudge = false;

	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 objPos = obj->GetPosition();

	//���̓����蔻�� (pos1�̉���)
	if (objPos.y - obj->m_size.y / 2 < pos.y + m_size.y / 2)
	{
		if (objPos.y - obj->m_size.y / 2 > pos.y - m_size.y / 2)
		{
			//���̓����蔻�� (pos1�̉E��)
			if (objPos.x + obj->m_size.x / 2 < pos.x + m_size.x / 2)
			{
				if (objPos.x + obj->m_size.x / 2 > pos.x - m_size.x / 2)
				{
					bJudge = true;
				}
			}
			//���̓����蔻�� (pos1�̍���)
			else if (objPos.x - obj->m_size.x / 2 < pos.x + m_size.x / 2)
			{
				if (objPos.x - obj->m_size.x / 2 > pos.x - m_size.x / 2)
				{
					bJudge = true;
				}
			}
		}
	}
	if (!bJudge)
	{//�܂������蔻�肪false�̏ꍇ
		//���̓����蔻�� (pos1�̏㑤)
		if (objPos.y + obj->m_size.y / 2 < pos.y + m_size.y / 2)
		{
			if (objPos.y + obj->m_size.y / 2 > pos.y - m_size.y / 2)
			{
				//���̓����蔻�� (pos1�̉E��)
				if (objPos.x + obj->m_size.x / 2 < pos.x + m_size.x / 2)
				{
					if (objPos.x + obj->m_size.x / 2 > pos.x - m_size.x / 2)
					{
						bJudge = true;
					}
				}
				//���̓����蔻�� (pos1�̍���)
				else if (objPos.x - obj->m_size.x / 2 < pos.x + m_size.x / 2)
				{
					if (objPos.x - obj->m_size.x / 2 > pos.x - m_size.x / 2)
					{
						bJudge = true;
					}
				}
			}
		}
	}

	//���ʂ�Ԃ�
	return bJudge;
}