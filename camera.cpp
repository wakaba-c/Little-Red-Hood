//=============================================================================
//
// �J�������� [camera.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "inputKeyboard.h"
#include "inputMouse.h"
#include "enemy.h"
#include "meshField.h"
#include "game.h"
#include "player.h"
#include "inputController.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCamera::CCamera()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCamera::~CCamera()
{

}

//==============================================================================
// ����������
//==============================================================================
HRESULT CCamera::Init(void)
{
	CRenderer *Renderer = CManager::GetRenderer();				// �����_���[�̎擾
	LPDIRECT3DDEVICE9 pDevice;									// �f�o�C�X
	D3DXVECTOR3		originPos;									// �ʒu

	pDevice = Renderer->GetDevice();							// �f�o�C�X�̎擾
	m_worldPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// �}�E�X�̃��[���h���W
	m_bStorker = true;											// �v���C���[�ǔ�
	m_bSmooth = true;											// �X���[�Y�ړ�
	m_bRotMove = false;											// ��]�^���̍X�V
	m_rot = D3DXVECTOR3(0.0f, -2.66f, 0.0f);					// ��]��
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// ��]�ʂ̖ړI�n
	m_posV = D3DXVECTOR3(0.0f, 500.0f, -316.0f);				// ���_
	m_posVDest = m_posV;										// ���_�̖ړI�n
	m_posR = D3DXVECTOR3(0.0f, 100.0f, 0.0f);					// �����_
	m_posRDest = m_posR;										// �����_�̖ړI�n
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);						// ������x�N�g��
	m_target = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// �ŏI���Z��
	m_currentRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// ���݂̉��Z�l
	m_rotMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// 1�t���[��������̉�]��
	m_originPos = D3DXVECTOR3(9276.95f, 130.0f, -9218.09f);		// �J�����̈ʒu
	originPos = m_posR - m_posV;								// �J�����ʒu �̑��
	m_fDistance = sqrtf(originPos.y * originPos.y + originPos.z * originPos.z + 50);	// ���� �̎Z�o

	// �`��̈� �̐ݒ�
	SetViewport(D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2),
		D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT));

#ifdef _DEBUG
	m_nType = 0;	// ���f���^�C�v

	// �f�o�b�O�\���p�t�H���g��ݒ�
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "�l�r �S�V�b�N", &m_pCameraFont);
#endif

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CCamera::Uninit(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void CCamera::Update(void)
{
	CPlayer *pPlayer = CGame::GetPlayer();		// �v���C���[�̎擾
	D3DXVECTOR3 Calculation;					// �v�Z�p

	Calculation.y = m_rotDest.y - m_rot.y;		// ���݂̒l�ƖړI�̒l�̍� �̎Z�o

	if (Calculation.y > D3DX_PI)
	{// ��]�� ������l�𒴂����Ƃ�
		m_rotDest.y -= D3DX_PI * 2;				// ��]�̕␳
	}
	else if (Calculation.y < -D3DX_PI)
	{// ��]�� ������l�𒴂����Ƃ�
		m_rotDest.y += D3DX_PI * 2;				// ��]�̕␳
	}

	if (fabsf(Calculation.y) < 0.0f)
	{// �v�Z�̒l ������l�𒴂����Ƃ�
		Calculation.y = m_rotDest.y;			// �v�Z�̒l��␳
	}

	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		if (m_bStorker)
		{// �v���C���[�ǔ���������Ă���Ƃ�
			if (pPlayer != NULL)
			{// �v���C���[�����݂��Ă����Ƃ�
				D3DXVECTOR3 pos = pPlayer->GetPosition();		// �v���C���[�̈ʒu���擾
				pos.y += 120.0f;								// �v���C���[�̈ʒu�ɃI�t�Z�b�g�����Z
				m_originPos = pos;								// �J�����̈ʒu �ɑ��
			}
		}
	}

	//�J�����̈ʒu�v�Z
	m_posVDest.x = m_originPos.x + sinf(D3DX_PI + m_rot.y) * cosf(D3DX_PI + m_rot.x) * m_fDistance;
	m_posVDest.y = m_originPos.y + sinf(D3DX_PI + m_rot.x) * m_fDistance;
	m_posVDest.z = m_originPos.z + cosf(D3DX_PI + m_rot.y) * cosf(D3DX_PI + m_rot.x) * m_fDistance;

	m_posRDest.x = m_originPos.x + cosf(D3DX_PI + m_rot.y) * cosf(D3DX_PI + m_rot.x);
	m_posRDest.y = m_originPos.y + sinf(D3DX_PI + m_rot.x);
	m_posRDest.z = m_originPos.z + sinf(D3DX_PI + m_rot.y) * cosf(D3DX_PI + m_rot.x);

	if (m_bSmooth)
	{
		//�J�����̈ʒu�K��
		m_posV += (m_posVDest - m_posV) * SPLIT;
		m_posR += (m_posRDest - m_posR) * SPLIT;

		m_posVDest = m_originPos + m_rot;
	}
	else
	{
		//�J�����̈ʒu�K��
		m_posV = m_posVDest;
		m_posR = m_posRDest;
	}

	//��]���̃��Z�b�g
	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2;
	}
	else if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2;
	}

	if (CManager::GetMode() == CManager::MODE_GAME)
	{// ���[�h���Q�[���������Ƃ�
		//�J��������
		CameraMove();
	}

	if (m_bRotMove)
	{// ��]�^�����L���������Ƃ�
		m_currentRot += m_rotMove;			// ���݂̉��Z�ʂɉ��Z����
		m_rot += m_rotMove;					// ���Z����

		if (m_target.y < m_currentRot.y)
		{
			m_bRotMove = false;			// ��]�^���̖�����
			CRenderer *pRenderer = CManager::GetRenderer();				// �����_���[�̎擾

			m_fDistance = 509.81f;				// �����̐ݒ�
			m_rot.y += D3DX_PI;					// �J�����𔼉�]������

			pRenderer->SetUpdate(true);			// �X�V������ݒ�
		}
	}

#ifdef _DEBUG
	//�f�o�b�O����
	Debug();
#endif
}

//=============================================================================
// �ݒ菈��
//=============================================================================
void CCamera::SetCamera(void)
{
	CRenderer *Renderer = CManager::GetRenderer();		// �����_���[�̎擾
	LPDIRECT3DDEVICE9 pDevice;							// �f�o�C�X
	pDevice = Renderer->GetDevice();					// �f�o�C�X�̎擾

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(
		&m_mtxProjection,
		D3DXToRadian(45.0f),							// ����p
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,		// �A�X�y�N�g��
		1.0f,											// NearZ�l
		150000.0f);										// FarZ�l

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	// �r���[�}�g���b�N�X���쐬
	D3DXMatrixLookAtLH(
		&m_mtxView,
		&m_posV,										// �J�����̎��_
		&m_posR,										// �J�����̒����_
		&m_vecU);										// �J�����̏�����x�N�g��

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//=============================================================================
// ��]�ʂ̐ݒ�
//=============================================================================
void CCamera::SetRotation(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//==============================================================================
// �J�����z�u����
//==============================================================================
void CCamera::SetPosition(D3DXVECTOR3 pos)
{
	m_originPos = pos;
}

//=============================================================================
// �����̐ݒ菈��
//=============================================================================
void CCamera::SetDistance(float fDistance)
{
	m_fDistance = fDistance;
}

//=============================================================================
// ������J�����O
//=============================================================================
bool CCamera::VFCulling(D3DXVECTOR3 pos, int nType, float fAngle, float fNearClip, float fFarClip)
{
	D3DXPLANE VFLeftPlane, VFRightPlane, VFTopPlane, VFBottomPlane;
	D3DXVECTOR3 Pos = pos;
	float fRadius = 0.0f;
	float fAspect = SCREEN_WIDTH / SCREEN_HEIGHT;				// �A�X�y�N�g��

	// �^�C�v�ʔ��a�̐ݒ�
	switch (nType)
	{
	case CScene::PRIORITY_ENEMY:
		fRadius = 50.0f;
		break;
	case CScene::PRIORITY_FLOOR:
		fRadius = 5000.0f;
		break;
	case CScene::PRIORITY_MESH:
		fRadius = 1800.0f;
		break;
	case CScene::PRIORITY_MODEL:
		fRadius = 2500.0f;
		break;
	case CScene::PRIORITY_ORBIT:
		fRadius = 1800.0f;
		break;
	case CScene::PRIORITY_PLAYER:
		fRadius = 200.0f;
		break;
	case CScene::PRIORITY_TREE:
		fRadius = 2500.0f;
		break;
	}

	// �W�I���g���̈ʒu�x�N�g�������[���h����r���[��Ԃɕϊ�
	D3DXVec3TransformCoord(&Pos, &Pos, &m_mtxView);

	// ���E�㉺�̕��ʂ��v�Z
	D3DXVECTOR3 p1, p2, p3;
	// ����
	p1 = D3DXVECTOR3(0, 0, 0);
	p2.x = -fFarClip * ((FLOAT)tan(fAngle / 2) * fAspect);
	p2.y = -fFarClip * (FLOAT)tan(fAngle / 2);
	p2.z = fFarClip;
	p3.x = p2.x;
	p3.y = -p2.y;
	p3.z = p2.z;
	D3DXPlaneFromPoints(&VFLeftPlane, &p1, &p2, &p3);
	// �E��
	p1 = D3DXVECTOR3(0, 0, 0);
	p2.x = fFarClip * ((FLOAT)tan(fAngle / 2) * fAspect);
	p2.y = fFarClip * (FLOAT)tan(fAngle / 2);
	p2.z = fFarClip;
	p3.x = p2.x;
	p3.y = -p2.y;
	p3.z = p2.z;
	D3DXPlaneFromPoints(&VFRightPlane, &p1, &p2, &p3);
	// ���
	p1 = D3DXVECTOR3(0, 0, 0);
	p2.x = -fFarClip * ((FLOAT)tan(fAngle / 2) * fAspect);
	p2.y = fFarClip * (FLOAT)tan(fAngle / 2);
	p2.z = fFarClip;
	p3.x = -p2.x;
	p3.y = p2.y;
	p3.z = p2.z;
	D3DXPlaneFromPoints(&VFTopPlane, &p1, &p2, &p3);
	// ����
	p1 = D3DXVECTOR3(0, 0, 0);
	p2.x = fFarClip * ((FLOAT)tan(fAngle / 2) * fAspect);
	p2.y = -fFarClip * (FLOAT)tan(fAngle / 2);
	p2.z = fFarClip;
	p3.x = -p2.x;
	p3.y = p2.y;
	p3.z = p2.z;
	D3DXPlaneFromPoints(&VFBottomPlane, &p1, &p2, &p3);

	// 6�̕��ʂƃW�I���g�����E�����`�F�b�N
	// �j�A�N���b�v��
	if ((Pos.z + fRadius) < fNearClip)
	{
		return false;
	}
	// �t�@�[�N���b�v��
	if ((Pos.z - fRadius) > fFarClip)
	{
		return false;
	}
	// ���N���b�v��
	FLOAT Distance = (Pos.x * VFLeftPlane.a) + (Pos.z * VFLeftPlane.c);
	if (Distance > fRadius)
	{
		return false;
	}
	// �E�N���b�v��
	Distance = (Pos.x * VFRightPlane.a) + (Pos.z * VFRightPlane.c);
	if (Distance > fRadius)
	{
		return false;
	}
	// ��N���b�v��
	Distance = (Pos.y * VFTopPlane.b) + (Pos.z * VFTopPlane.c);
	if (Distance > fRadius)
	{
		return false;
	}
	// ���N���b�v�ʂɂ���
	Distance = (Pos.y * VFBottomPlane.b) + (Pos.z * VFBottomPlane.c);
	if (Distance > fRadius)
	{
		return false;
	}

	return true;
}

//=============================================================================
// �J�����̎��_�ύX
//=============================================================================
void CCamera::SetPosCamera(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_originPos = pos;					// �ʒu��ݒ�
	m_originPos.y += 120;				// �v���C���[�̍������l��

	m_rotDest = rot;					// ��]�ڕW�l��ݒ�
	m_rot = rot;						// ��]�l��ݒ�

	// �J�����̈ʒu�v�Z
	m_posVDest.x = m_originPos.x + sinf(D3DX_PI + m_rot.y) * cosf(D3DX_PI + m_rot.x) * m_fDistance;
	m_posVDest.y = m_originPos.y + sinf(D3DX_PI + m_rot.x) * m_fDistance;
	m_posVDest.z = m_originPos.z + cosf(D3DX_PI + m_rot.y) * cosf(D3DX_PI + m_rot.x) * m_fDistance;

	m_posRDest.x = m_originPos.x + cosf(D3DX_PI + m_rot.y) * cosf(D3DX_PI + m_rot.x);
	m_posRDest.y = m_originPos.y + sinf(D3DX_PI + m_rot.x);
	m_posRDest.z = m_originPos.z + sinf(D3DX_PI + m_rot.y) * cosf(D3DX_PI + m_rot.x);

	// �J�����̈ʒu�K��
	m_posV = m_posVDest;
	m_posR = m_posRDest;
}

//=============================================================================
// ��]�^���̐ݒ�
//=============================================================================
void CCamera::SetRotMotion(D3DXVECTOR3 target, D3DXVECTOR3 move)
{
	if (!m_bRotMove)
	{
		m_target = target;		// ��]�ʂ̍ő�l��ݒ�
		m_rotMove = move;		// 1�t���[��������̉�]�ʂ̐ݒ�
		m_currentRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ڕW�ɑ΂��錻�݂̉��Z�l��������
		m_bRotMove = true;		// ��]�^���̗L����
	}
}

//==============================================================================
// �`��̈��ݒ�
//==============================================================================
HRESULT CCamera::SetViewport(D3DXVECTOR2 pos, D3DXVECTOR2 size)
{
	CRenderer *Renderer = CManager::GetRenderer();																// �����_���[�̎擾
	LPDIRECT3DDEVICE9 pDevice;																					// �f�o�C�X
	pDevice = Renderer->GetDevice();																			// �f�o�C�X�̎擾

	D3DVIEWPORT9 vp;																							// �`��̈�

	vp.X = (DWORD)(pos.x - size.x / 2);																			// X���̈ʒu�ݒ�
	vp.Y = (DWORD)(pos.y - size.y / 2);																			// Y���̈ʒu�ݒ�
	vp.Width = (DWORD)size.x;																					// X���̑傫���ݒ�
	vp.Height = (DWORD)size.y;																					// Y���̑傫���ݒ�
	vp.MinZ = 0;
	vp.MaxZ = 1;

	if (FAILED(pDevice->SetViewport(&vp)))
	{// �`��̈� �̐ݒ� �Ɏ��s�����Ƃ�
		MessageBox(NULL, "�r���[�|�[�g�؂�ւ����s", "�G���[", MB_OK);												// �G���[�E�B���h�E�̍쐬
		return E_FAIL;
	}
	return S_OK;
}

//==============================================================================
// XZ���ʂƃX�N���[�����W�̌�_�Z�o�֐�
//==============================================================================
D3DXVECTOR3 CCamera::CalcScreenToXZ(float fSx, float fSy, int nScreen_w, int nScreen_h, D3DXMATRIX* mView, D3DXMATRIX* mPrj)
{
	D3DXVECTOR3 Answer;
	D3DXVECTOR3 nearpos;
	D3DXVECTOR3 farpos;
	D3DXVECTOR3 ray;
	CalcScreenToWorld(&nearpos, fSx, fSy, 0.0f, nScreen_w, nScreen_h, mView, mPrj);
	CalcScreenToWorld(&farpos, fSx, fSy, 1.0f, nScreen_w, nScreen_h, mView, mPrj);
	ray = farpos - nearpos;
	D3DXVec3Normalize(&ray, &ray);

	if (ray.y <= 0)
	{// ���Ƃ̌������N���Ă���ꍇ�͌�_��
		// ����_
		float Lray = D3DXVec3Dot(&ray, &D3DXVECTOR3(0, 1, 0));
		float LP0 = D3DXVec3Dot(&(-nearpos), &D3DXVECTOR3(0, 1, 0));
		Answer = nearpos + (LP0 / Lray)*ray;
	}
	else
	{// �N���Ă��Ȃ��ꍇ�͉����̕ǂƂ̌�_���o��
		Answer = farpos;
	}

	return Answer;
}

//==============================================================================
// �X�N���[�����W�����[���h���W�ɕϊ�
//==============================================================================
D3DXVECTOR3* CCamera::CalcScreenToWorld(D3DXVECTOR3* pout, float fSx, float fSy, float fZ, int nScreen_w, int nScreen_h, D3DXMATRIX* mView, D3DXMATRIX* mPrj)
{
	// �e�s��̋t�s����Z�o
	D3DXMATRIX InvView, InvPrj, VP, InvViewport;
	D3DXMatrixInverse(&InvView, NULL, mView);
	D3DXMatrixInverse(&InvPrj, NULL, mPrj);
	D3DXMatrixIdentity(&VP);
	VP._11 = nScreen_w / 2.0f; VP._22 = -nScreen_h / 2.0f;
	VP._41 = nScreen_w / 2.0f; VP._42 = nScreen_h / 2.0f;
	D3DXMatrixInverse(&InvViewport, NULL, &VP);

	// �t�ϊ�
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;
	D3DXVec3TransformCoord(pout, &D3DXVECTOR3(fSx, fSy, fZ), &tmp);

	return pout;
}

//=============================================================================
// �J�����̑���
//=============================================================================
void CCamera::CameraMove(void)
{
	if (CManager::GetMode() == CManager::MODE_GAME)
	{// ���[�h���Q�[���������Ƃ�
		CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();		// �L�[�{�[�h�̎擾
		CInputMouse *pMouse = CManager::GetInputMouse();				// �}�E�X�̎擾
		CInputController *pGamepad = CManager::GetInputController();	// �Q�[���p�b�h�̎擾

		D3DXVECTOR2 mousePos;									// �}�E�X���W

		float nValueH = 0;									//�R���g���[���[
		float nValueV = 0;									//�R���g���[���[

		if (pGamepad != NULL)
		{// �Q�[���p�b�h�����݂��Ă����Ƃ�
			if (pGamepad->GetJoypadUse(0))
			{// �g�p�\�������Ƃ�
			 // �E�X�e�B�b�N�̊p�x���擾
				pGamepad->GetJoypadStickRight(0, &nValueH, &nValueV);

				// �J�������w��͈̔͊O�������Ƃ�
				if (m_rot.x < -0.7f)
				{
					// �ő�l�܂Ŗ߂�
					m_rot.x = -0.7f;
				}
				else if (m_rot.x > 0.1f)
				{
					// �ŏ��l�܂Ŗ߂�
					m_rot.x = 0.1f;
				}

				// �J�����̉�]
				// �J�����̏c�ɐ��������
				m_rot.x += nValueV * 0.05f;

				// �J���������ɐ��񂷂�
				m_rot.y -= nValueH * 0.05f;
			}
		}

		// �X�N���[�����W��XZ���ʂ̃��[���h���W��_�Z�o
		m_worldPos = CalcScreenToXZ((float)pMouse->GetMouseX(), (float)pMouse->GetMouseY(), SCREEN_WIDTH, SCREEN_HEIGHT, &m_mtxView, &m_mtxProjection);

		// ��Alt�L�[�������Ă���Ƃ�
		if (pKeyboard->GetPressKeyboard(DIK_LALT))
		{
			// ���{�^���������ꂽ�Ƃ�
			if (pMouse->GetTriggerMouse(MOUSE_LEFT))
			{
				//���݂̃}�E�X���W���擾
				m_mousePosOld.x = (float)pMouse->GetMouseX();
				m_mousePosOld.y = (float)pMouse->GetMouseY();
			}
			else if (pMouse->GetTriggerMouse(MOUSE_RIGHT))
			{// �E�{�^���������ꂽ�Ƃ�

			 // ���݂̃}�E�X���W���擾
				m_mousePosOld.y = (float)pMouse->GetMouseY();
			}
			else if (pMouse->GetTriggerMouse(MOUSE_CENTER))
			{// ���{�^���������ꂽ�Ƃ�
			 // ���݂̃}�E�X���W���擾
				m_mousePosOld.x = (float)pMouse->GetMouseX();
				m_mousePosOld.y = (float)pMouse->GetMouseY();
			}
			else if (pMouse->GetPressMouse(MOUSE_LEFT))
			{// ���{�^����������Ă����
			 // �}�E�X���W�̎擾
				mousePos.x = (float)pMouse->GetMouseX();
				mousePos.y = (float)pMouse->GetMouseY();

				// �O��̈ʒu�Ƃ̍��������߂�
				D3DXVECTOR2 mouseMove = mousePos - m_mousePosOld;

				// ������]������
				m_rot.x -= mouseMove.y * 0.01f;
				m_rot.y += mouseMove.x * 0.01f;

				// �Â������X�V����
				m_mousePosOld = mousePos;
			}
			else if (pMouse->GetPressMouse(MOUSE_RIGHT))
			{// �E�{�^����������Ă����
			 // �}�E�X���W�̎擾
				mousePos.y = (float)pMouse->GetMouseY();

				// �O��̈ʒu�Ƃ̍��������߂�
				D3DXVECTOR2 mouseMove = mousePos - m_mousePosOld;

				// ���������X�V����
				m_fDistance += mouseMove.y;

				// �Â������X�V����
				m_mousePosOld = mousePos;
			}
			else if (pMouse->GetPressMouse(MOUSE_CENTER))
			{// ���{�^����������Ă����
			 // �}�E�X���W�̎擾
				mousePos.x = (float)pMouse->GetMouseX();
				mousePos.y = (float)pMouse->GetMouseY();

				// �O��̈ʒu�Ƃ̍��������߂�
				D3DXVECTOR2 work = mousePos - m_mousePosOld;

				// �J������Y�����ړ�
				m_originPos.x -= sinf(D3DX_PI * 1.0f) * work.y;
				m_originPos.y -= cosf(D3DX_PI * 1.0f) * work.y;

				// �J������X�����ړ�
				m_originPos.x += sinf(D3DX_PI * 0.5f + m_rot.y) * work.x * 0.5f;
				m_originPos.y += cosf(D3DX_PI * 0.5f + m_rot.y) * work.x * 0.5f;

				// �Â������X�V����
				m_mousePosOld = mousePos;
			}
		}
	}
}

#ifdef _DEBUG
//=============================================================================
// �f�o�b�O����
//=============================================================================
void CCamera::Debug(void)
{
	ImGui::Begin("Camera");									// �ucamera�v�Ƃ����E�B���h�E���쐬���܂�����ɒǉ����܂��B

	ImGui::Text("Camera Setting");							// �������̃e�L�X�g��\�����܂��i�t�H�[�}�b�g��������g�p�ł��܂��j
	ImGui::Checkbox("playerStoker", &m_bStorker);

	ImGui::LabelText("", "CameraV Pos:(%.2f, %.2f, %.2f )", m_posV.x, m_posV.y, m_posV.z);
	ImGui::LabelText("", "CameraR Pos:(%.2f, %.2f, %.2f )", m_posR.x, m_posR.y, m_posR.z);
	ImGui::LabelText("", "Distance : %.2f", m_fDistance);

	if (ImGui::Button("Reset"))                            // �{�^���̓N���b�N�����true��Ԃ��܂��i�قƂ�ǂ̃E�B�W�F�b�g�͕ҏW/�A�N�e�B�u�������true��Ԃ��܂��j
	{
		m_posV = D3DXVECTOR3(-2061.0f, 1122.0f, 1904.0f);
	}

	if (ImGui::Button("Model"))                            // �{�^���̓N���b�N�����true��Ԃ��܂��i�قƂ�ǂ̃E�B�W�F�b�g�͕ҏW/�A�N�e�B�u�������true��Ԃ��܂��j
	{
		m_posV = D3DXVECTOR3(0.0f, 349.0f, 560.0f);
	}

	if (ImGui::Button("Debug"))                            // �{�^���̓N���b�N�����true��Ԃ��܂��i�قƂ�ǂ̃E�B�W�F�b�g�͕ҏW/�A�N�e�B�u�������true��Ԃ��܂��j
	{
		m_posV = D3DXVECTOR3(92.99f, 31153.32f, -967.5f);
		m_posR = D3DXVECTOR3(99.52f, 51.00f, 375.68f);
		m_rot = D3DXVECTOR3(-0.67f, 3.14f, 0.0f);
		m_fDistance = 31132.31f;
	}
	if (ImGui::Button("Player"))
	{
		m_posV = D3DXVECTOR3(100.52f, 50.00f, -227.31f);
		m_posR = D3DXVECTOR3(99.00f, 50.00f, 100.00f);
		m_rot = D3DXVECTOR3(-0.67f, 3.14f, 0.0f);
		m_fDistance = 327.31f;
	}

	ImGui::End();											//�Ō�ɂ���
}
#endif