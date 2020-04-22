//=============================================================================
//
// ���C�g���� [light.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "light.h"
#include "renderer.h"
#include "manager.h"
#include "inputKeyboard.h"
#include "player.h"
#include "game.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLight::CLight()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLight::~CLight()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CLight::Init(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X���擾����
	pDevice = pRenderer->GetDevice();
	D3DXVECTOR3 vecDir[MAX_LIGHT];						//�ݒ�p�x�N�g��

	// ���C�g���N���A����
	for (int nCount = 0; nCount < MAX_LIGHT; nCount++)
	{
		ZeroMemory(&m_aLight[nCount], sizeof(D3DLIGHT9));
	}

	// ���C�g�̎�ނ�ݒ�
	m_aLight[0].Type = D3DLIGHT_DIRECTIONAL;
	// ���C�g�̎�ނ�ݒ�
	m_aLight[1].Type = D3DLIGHT_DIRECTIONAL;
	// ���C�g�̎�ނ�ݒ�
	m_aLight[2].Type = D3DLIGHT_DIRECTIONAL;

	// ���C�g�̊g�U����ݒ�
	m_aLight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ���C�g�̊g�U����ݒ�
	m_aLight[1].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);

	// ���C�g�̊g�U����ݒ�
	m_aLight[2].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);

	// ���C�g�̕����̐ݒ�
	vecDir[0] = D3DXVECTOR3(0.22f, -0.87f, 0.44f);
	D3DXVec3Normalize(&vecDir[0], &vecDir[0]);			//���K������
	m_aLight[0].Direction = vecDir[0];

	// ���C�g�̕����̐ݒ�
	vecDir[1] = D3DXVECTOR3(-0.18f, 0.88f, -0.44f);
	D3DXVec3Normalize(&vecDir[1], &vecDir[1]);			//���K������
	m_aLight[1].Direction = vecDir[1];

	// ���C�g�̕����̐ݒ�
	vecDir[2] = D3DXVECTOR3(0.0f, -0.11f, -0.11f);
	D3DXVec3Normalize(&vecDir[2], &vecDir[2]);			//���K������
	m_aLight[2].Direction = vecDir[2];

	// ���C�g��ݒ肷��
	pDevice->SetLight(0, &m_aLight[0]);

	// ���C�g��ݒ肷��
	pDevice->SetLight(1, &m_aLight[1]);

	// ���C�g��ݒ肷��
	pDevice->SetLight(2, &m_aLight[2]);

	// ���C�g��L���ɂ���
	pDevice->LightEnable(0, TRUE);

	// ���C�g��L���ɂ���
	pDevice->LightEnable(1, TRUE);

	// ���C�g��L���ɂ���
	pDevice->LightEnable(2, TRUE);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CLight::Uninit(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void CLight::Update(void)
{
#ifdef _DEBUG
	Debug();
#endif
}

//=============================================================================
// ���C�g�̎擾
//=============================================================================
D3DLIGHT9 CLight::GetLight(const int nIndex)
{
	return m_aLight[nIndex];					//�w�肳�ꂽ���C�g��Ԃ�
}

#ifdef _DEBUG
//=============================================================================
// �f�o�b�O����
//=============================================================================
void CLight::Debug(void)
{

}
#endif