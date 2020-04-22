//=============================================================================
//
// �����_���[���� [renderer.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "renderer.h"
#include "inputKeyboard.h"
#include "fade.h"
#include "game.h"
#include "scene.h"
#include "scene2D.h"
#include "fade.h"
#include "camera.h"
#include "light.h"
#include "circle.h"
#include "collider.h"
#include "stage.h"
#include "player.h"
#include "ranking.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
#ifdef _DEBUG
CDebugProc *CRenderer::m_pDebug = NULL;
#endif

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CRenderer::CRenderer()
{
	m_bWireFrame = false;
	m_bUpdate = true;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CRenderer::~CRenderer()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CRenderer::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	D3DDISPLAYMODE d3ddm;			// �f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;	// �v���[���e�[�V�����p�����[�^

	// Direct3D�I�u�W�F�N�g�̐���
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ���[�N���[���N���A
	d3dpp.BackBufferWidth = SCREEN_WIDTH;						// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;						// �o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1;									// �o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// �_�u���o�b�t�@�̐؂�ւ�(�f���M���ɓ���)
	d3dpp.EnableAutoDepthStencil = TRUE;						// �f�v�X�o�b�t�@(�y�o�b�t�@)�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// �f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindow;									// �E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// ���t���b�V�����[�g(���݂̑��x�ɍ��킹��)
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// �N���C�A���g�̈�𒼂��ɍX�V����

	// Direct3D�f�o�C�X�̐���
	// [�f�o�C�X�쐬����]<�`��>��<���_����>���n�[�h�E�F�A�ōs�Ȃ�
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,					// �f�B�X�v���C�A�_�v�^
		D3DDEVTYPE_HAL,						// �f�o�C�X�^�C�v
		hWnd,								// �t�H�[�J�X����E�C���h�E�ւ̃n���h��
		D3DCREATE_HARDWARE_VERTEXPROCESSING,// �f�o�C�X�쐬����̑g�ݍ��킹
		&d3dpp,								// �f�o�C�X�̃v���[���e�[�V�����p�����[�^
		&m_pD3DDevice)))					// �f�o�C�X�C���^�[�t�F�[�X�ւ̃|�C���^
	{
		// ��L�̐ݒ肪���s������
		// [�f�o�C�X�쐬����]<�`��>���n�[�h�E�F�A�ōs���A<���_����>��CPU�ōs�Ȃ�
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// [�f�o�C�X�쐬����]<�`��>��<���_����>��CPU�ōs�Ȃ�
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pD3DDevice)))
			{
				// ���������s
				return E_FAIL;
			}
		}
	}

#ifdef _DEBUG
	m_pDebug = new CDebugProc;
	m_pDebug->Init(hWnd);
#endif

	// �����_�[�X�e�[�g�p�����[�^�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// ����(�����)���J�����O����
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Z�o�b�t�@���g�p
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);						// ���C�e�B���O���[�h�L��
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��

	// �T���v���[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �A���t�@�u�����f�B���O����
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �ŏ��̃A���t�@����
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// �Q�Ԗڂ̃A���t�@����

	//�}�E�X�J�[�\��������
	ShowCursor(false);
#ifdef _DEBUG
	ShowCursor(true);
#endif

	//�t�F�[�h
	m_pFade = CFade::Create();

	// �X�e�[�W�̍쐬
	m_pStage = CStage::Create();
	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CRenderer::Uninit(void)
{
#ifdef _DEBUG
	//�f�o�b�O�����̊J��
	if (m_pDebug != NULL)
	{
		m_pDebug->Uninit();			// �f�o�b�O�̏I������
		delete m_pDebug;			// �f�o�b�O�̃��������
		m_pDebug = NULL;			// �f�o�b�O�̃|�C���^��NULL�Ɏw��
	}
#endif

	// Direct3D�f�o�C�X�̊J��
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// Direct3D�I�u�W�F�N�g�̊J��
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}

	//�t�F�[�h�̊J��
	if (m_pStage != NULL)
	{
		m_pStage->Uninit();			// �X�e�[�W�̊J������
		delete m_pStage;			// �X�e�[�W�̃��������
		m_pStage = NULL;			// �X�e�[�W�̃|�C���^��NULL����
	}

	//�t�F�[�h�̊J��
	if (m_pFade != NULL)
	{
		m_pFade->Uninit();			// �t�F�[�h�̊J������
		delete m_pFade;				// �t�F�[�h�̃��������
		m_pFade = NULL;				// �t�F�[�h�̃|�C���^��NULL����
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CRenderer::Update(void)
{
	CCamera *pCamera = CManager::GetCamera();
	CLight *pLight = CManager::GetLight();

#ifdef _DEBUG
	m_pDebug->Update();
	Debug();
#endif

	if (m_bUpdate)
	{// �X�V���L���������Ƃ�
		CScene::UpdateAll();
	}

	// �e��X�V
	pCamera->Update();
	pLight->Update();
	m_pFade->Update();
	m_pStage->Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CRenderer::Draw(void)
{
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// �`��̊J�n
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		CCamera *pCamera = CManager::GetCamera();

		CScene::DrawAll();

#ifdef _DEBUG
		CDebugProc::Log("FPS : %d", GetFPS());									// FPS

		if (m_bWireFrame)
		{// �@���C���[�t���[�����L���������Ƃ�
			m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			m_pDebug->Draw();
			m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		}
		else
		{
			m_pDebug->Draw();
		}

		// �����蔻��̉���
		CCollider::DrawAll();
#endif

		m_pStage->Draw();
		m_pFade->Draw();
		pCamera->SetCamera();

		// �`��̏I��
		m_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// �X�V�����̗L��
//=============================================================================
void CRenderer::SetUpdate(bool bValue)
{
	m_bUpdate = bValue;
}

#ifdef _DEBUG
//=============================================================================
// �f�o�b�O����
//=============================================================================
void CRenderer::Debug(void)
{
	bool bWork = m_bWireFrame;

	ImGui::Begin("System");

	ImGui::Checkbox("Wire frame", &m_bWireFrame);

	ImGui::End();

	if (bWork != m_bWireFrame)
	{
		if (m_bWireFrame)
		{
			m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		}
		else
		{
			m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}
	}
}
#endif