#ifdef _DEBUG
//=============================================================================
//
// �f�o�b�O���� [debug.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "inputMouse.h"
#include "camera.h"
#include "scene.h"
#include "circle.h"
#include "inputKeyboard.h"
#include "meshField.h"
#include "game.h"
#include "object.h"
#include "sceneX.h"
#include "collider.h"
#include "enemy.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define CONTINUE_MAX 10													// �Čv�Z��

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
char CDebugProc::m_aStr[1024] = {};										// ������
D3DXVECTOR3 CDebugProc::m_mouseOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �}�E�X�̑O�̃��[���h���W
CCircle	*CDebugProc::m_pCircle = NULL;									// �~�̃|�C���^
CObject	*CDebugProc::m_pObject = NULL;									// �؂̃|�C���^
CEnemy *CDebugProc::m_pEnemy = NULL;									// �G�̃|�C���^
float CDebugProc::m_fPaintSize = 1;										// �n�`�ҏW�u���V�̑傫��
int CDebugProc::m_nCreateIndex = 0;										// 1�t���[���̐�����
bool CDebugProc::m_bDebug = false;										// �f�o�b�O���[�h�̐؂�ւ�
int CDebugProc::m_nCntGeneration = 0;									// ������
int	CDebugProc::m_nType = 0;											// �I�u�W�F�N�g�^�C�v
int	CDebugProc::m_nMode = 0;											// ���[�h�I��
int	CDebugProc::m_nCntContinue = 0;										// �Ċm�F��

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CDebugProc::CDebugProc()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CDebugProc::~CDebugProc()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CDebugProc::Init(HWND hWnd)
{
	CRenderer *Renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = Renderer->GetDevice();

	// �f�o�b�O�\���p�t�H���g��ݒ�
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "�l�r �S�V�b�N", &m_pFont);

	//IMGUI�̏�����
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(pDevice);

	// �~�̍쐬
	m_pCircle = CCircle::Create();
	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CDebugProc::Uninit(void)
{
	if (m_pCircle != NULL)
	{// �~�̃|�C���^���J������Ă��Ȃ��Ƃ�
		m_pCircle->Uninit();														// �J������
		delete m_pCircle;															// �~�̊J��
		m_pCircle = NULL;															// �~�̃|�C���^��NULL��
	}

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	if (m_pFont != NULL)
	{// �f�o�b�O�\���p�t�H���g�̊J��
		m_pFont->Release();
		m_pFont = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CDebugProc::Update(void)
{
	// �t���[���J�n
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// �����ō쐬�����ȒP�ȃE�B���h�E��\�����܂��B Begin / End�y�A���g�p���āA���O�t���E�B���h�E���쐬���܂��B
	ImGui::Begin("System");                          // �uSystem�v�Ƃ����E�B���h�E���쐬���܂�����ɒǉ����܂��B

	ImGui::SameLine();
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::Text("rand = %d", CManager::GetRand(10));								// �v���C���[�̌��݈ʒu��\��
	ImGui::End();

	Debug();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CDebugProc::Draw(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();	// �f�o�C�X�̎擾

	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	int nTime = 0;

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	// �~�̕`��
	if (m_bDebug)
	{
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// ����(�����)���J�����O����
		m_pCircle->Draw();
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// ����(�����)���J�����O����
	}

	//ImGui�̕`��
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	// �e�L�X�g�`��
	m_pFont->DrawText(NULL, &m_aStr[0], -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	*m_aStr = NULL;
}

//=============================================================================
// ���O�\������
//=============================================================================
void CDebugProc::Log(char* frm, ...)
{
	va_list args;			// ���X�g�̎擾
	char* c = "\n";			// ���s�p

	va_start(args, frm);		// ���X�g�̐擪���擾

	vsprintf(&m_aStr[strlen(m_aStr)], frm, args);		// �����ɓ��Ă͂߂�

	va_end(args);						// ���X�g���J������
}

//=============================================================================
// �f�o�b�O����
//=============================================================================
void CDebugProc::Debug(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();		// �L�[�{�[�h�̎擾
	CCamera *pCamera = CManager::GetCamera();		// �J���� �̎擾

	ImGui::Begin("System");			// System�E�B���h�E �̐����܂��̓A�N�Z�X

	ImGui::Checkbox("DebugMode", &m_bDebug);		// �n�`�ҏW���[�h�؂�ւ�

	//�f�o�b�O�������I��
	ImGui::End();

	// �f�o�b�O�E�B���h�E�̐���
	if (m_bDebug)
	{
		ImGui::Begin("Debug", &m_bDebug, ImGuiWindowFlags_MenuBar);   // �f�o�b�O�E�B���h�E����

		CRenderer *pRenderer = CManager::GetRenderer();
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();	// �f�o�C�X�̎擾

		pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);

		if (ImGui::BeginMenuBar())
		{// ���j���[�o�[�̐���
			if (ImGui::BeginMenu("File"))
			{// �t�@�C���^�u�̐���
				if (ImGui::MenuItem("Load"))
				{// ���[�h
					// �����̓ǂݍ���
					CMeshField::LoadRand("data/stage/center/rand.csv", false);

					// ���f�����̓ǂݍ���
					CObject::LoadModel("data/stage/center/object.csv");

					// �G�̔z�u���
					CEnemy::LoadEnemy("data/stage/center/enemy.csv");
				}
				if (ImGui::MenuItem("Save"))
				{// �Z�[�u
					// �����̏�������
					CScene::SaveRand();

					// ���f�����̏�������
					CScene::SaveModel();

					// �G���̏�������
					CScene::SaveEnemy();
				}

				ImGui::EndMenu();			// ���j���[�̍X�V�I��
			}
			ImGui::EndMenuBar();		// ���j���[�o�[�̍X�V�I��
		}
		ImGui::Text("[R]Press is Create");			// �f�o�b�O���[�h�e�L�X�g�\��

		if (pKeyboard->GetTriggerKeyboard(DIK_RCONTROL))
		{
			m_nMode = DEBUGMODE_NONE;
		}

		ImGui::Text("Rand DebugMode");			// �f�o�b�O���[�h�e�L�X�g�\��

		D3DXVECTOR3 worldPos = pCamera->GetWorldPos();			// �}�E�X�̃��[���h���W���擾
		m_pCircle->SetPosition(worldPos);						// �����͈͂̈ʒu��ݒ�
		m_pCircle->SetRadius(m_fPaintSize);						// �����͈͂̑傫����ݒ�
		m_pCircle->MakeVertex();								// �~�̒��_����

		CDebugProc::Log("�}�E�X�̃��[���h���W : %f, %f, %f\n",			// �}�E�X�̃��[���h���W���o��
			worldPos.x,
			worldPos.y,
			worldPos.z);

		if (m_nMode != DEBUGMODE_INDIVIDUAL)
		{// �X�z�u���[�h����Ȃ��Ƃ�
			ImGui::SliderFloat("paintSize", &m_fPaintSize, 1.0f, 10000.0f);			// �X���C�_�[���g�p����1�̃t���[�g��ҏW���܂�
		}

		if (m_nMode != DEBUGMODE_RAND)
		{// �n�`�ҏW���[�h����Ȃ��Ƃ�
			ImGui::InputInt("Type", &m_nType);

			if (m_nType >= OBJTYPE_MAX)
			{// �^�C�v���p�ӂ������f���̌��𒴂����Ƃ�
				m_nType = (int)OBJTYPE_TREE_1;										// ���f���^�C�v���m�[�}���ɐݒ�
			}
			else if (m_nType < OBJTYPE_TREE_1)
			{// �^�C�v��0�ȉ��ɐݒ肳��Ă���Ƃ�
				m_nType = (int)OBJTYPE_MAX - 1;										// �^�C�v�����f���̍ő�l�ɐݒ�
			}
		}

		ImGui::RadioButton("none", &m_nMode, DEBUGMODE_NONE);						// �I���� �Ȃ� ��ǉ�
		ImGui::SameLine();															// ���s���
		ImGui::RadioButton("rand", &m_nMode, DEBUGMODE_RAND);						// �I���� �n�ʕό`���[�h ��ǉ�
		ImGui::SameLine();															// ���s���
		ImGui::RadioButton("many", &m_nMode, DEBUGMODE_MANY);						// �I���� �͈͓������������[�h ��ǉ�
		ImGui::SameLine();															// ���s���
		if (ImGui::RadioButton("individual", &m_nMode, DEBUGMODE_INDIVIDUAL))		// �I���� �X�z�u���[�h ��ǉ�
		{
			if (m_pObject == NULL)
			{
				m_pObject = CObject::Create((OBJTYPE)m_nType);						// ���{�p�I�u�W�F�N�g���쐬

				if (m_pObject != NULL)
				{
					m_pObject->SetPosition(worldPos);								// �ʒu���}�E�X�̃��[���h���W�ɐݒ�
					m_pObject->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));			// �F�̕ύX
				}
			}
		}
		ImGui::RadioButton("paint", &m_nMode, DEBUGMODE_PAINT);
		ImGui::SameLine();															// ���s���
		if (ImGui::RadioButton("enemy", &m_nMode, DEBUGMODE_ENEMY))
		{
			if (m_pEnemy == NULL)
			{
				m_pEnemy = CEnemy::Create();			// ���{�p�I�u�W�F�N�g���쐬

				if (m_pEnemy != NULL)
				{
					m_pEnemy->SetPosition(worldPos);								// �ʒu���}�E�X�̃��[���h���W�ɐݒ�
				}
			}
		}
		ImGui::SameLine();															// ���s���
		ImGui::RadioButton("delete", &m_nMode, DEBUGMODE_DELETE);					// �I���� �͈͓������������[�h ��ǉ�

		if (m_nMode != DEBUGMODE_INDIVIDUAL)
		{// �X�z�u���[�h�ł͂Ȃ������Ƃ�
			if (m_pObject != NULL)
			{// �|�C���^�����݂��Ă����Ƃ�
				m_pObject->Release();				// �J���\��
				m_pObject = NULL;
			}
		}

		if (m_nMode != DEBUGMODE_ENEMY)
		{// �G�l�~�[�z�u���[�h�ł͂Ȃ������Ƃ�
			if (m_pEnemy != NULL)
			{// �|�C���^�����݂��Ă����Ƃ�
				m_pEnemy->Release();
				m_pEnemy = NULL;
			}
		}

		if (ImGui::Button("Reset"))
		{// Reset �{�^���������ꂽ�Ƃ�
			CScene::ResetFloor();					// ���̍�������Z�b�g
		}

		if (m_nMode == DEBUGMODE_NONE)
		{// �������Ȃ����[�h�������Ƃ�
			ImGui::Text("None");

			// �f�o�b�O�I���{�^��
			if (ImGui::Button("Debug End"))
			{
				m_bDebug = false;
				m_nType = 0;
				for (int nCount = 0; nCount < OBJTYPE_MAX; nCount++)
				{
					if (m_pObject != NULL)
					{// ���{�p�̃��f�����������Ƃ�
						m_pObject->Uninit();		// �I������
						m_pObject->Release();		// �������̊J��
						m_pObject = NULL;			// NULL����
					}
				}
			}
		}
		else if (m_nMode == DEBUGMODE_RAND)
		{// �n�`�ҏW���[�h�������Ƃ�
			CreateRand(worldPos);

			ImGui::Text("Rand Debug");				// ���݂̃f�o�b�O���[�h�̕\��
		}
		else if (m_nMode == DEBUGMODE_MANY)
		{// �͈͓������������[�h�������Ƃ�
			ImGui::SliderInt("Generation", &m_nCntGeneration, 0, 50);
			CreateObject(worldPos);					// �����z�u���[�h�̎��s

			ImGui::Text("Many Debug");
		}
		else if (m_nMode == DEBUGMODE_INDIVIDUAL)
		{// �X�z�u���[�h�������Ƃ�
			CreateIndividual(worldPos);				// �X�z�u���[�h�̎��s

			// ���݂̃f�o�b�O�^�C�v��\��
			ImGui::Text("individual Debug");
		}
		else if (m_nMode == DEBUGMODE_DELETE)
		{// �X�z�u���[�h�������Ƃ�
			DeleteObject(worldPos);					// �X�z�u���[�h�̎��s

			// ���݂̃f�o�b�O�^�C�v��\��
			ImGui::Text("individual Debug");
		}
		else if (m_nMode == DEBUGMODE_PAINT)
		{// �n�`�ҏW���[�h�������Ƃ�
			Paint(worldPos);

			ImGui::Text("Paint Debug");				// ���݂̃f�o�b�O���[�h�̕\��
		}
		else if (m_nMode == DEBUGMODE_ENEMY)
		{
			CreateEnemy(worldPos);
			ImGui::Text("Enemy Debug");				// ���݂̃f�o�b�O���[�h�̕\��
		}
		// �X�V�I��
		ImGui::End();
	}
}

//=============================================================================
// �n�`�ҏW����
//=============================================================================
void CDebugProc::CreateRand(D3DXVECTOR3 &worldPos)
{
	CInputMouse *pMouse = CManager::GetInputMouse();								// �}�E�X�̎擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();						// �L�[�{�[�h�̎擾

	// ���݃}�E�X�̍��W�͈͓̔��ɂ��鏰���擾����
	CScene *pSceneNext = NULL;														// ������
	CScene *pSceneNow = CScene::GetScene(CScene::PRIORITY_FLOOR);					// ���̐擪�A�h���X�̎擾

	//�����Ȃ��Ȃ�܂ŌJ��Ԃ�
	while (pSceneNow != NULL)
	{
		pSceneNext = CScene::GetSceneNext(pSceneNow, CScene::PRIORITY_FLOOR);		//����A�b�v�f�[�g�Ώۂ��T����
		CMeshField *pField = (CMeshField*)pSceneNow;
		if (pField->CollisionRangeToCircle(worldPos, m_fPaintSize))					// �~�����͈͓̔��ɓ����Ă��邩�ǂ���
		{
			if (m_mouseOld != worldPos)
			{// �O�̍��W�ƍ���̍��W���Ⴄ�Ƃ�
				pField->CreateRand(worldPos, m_fPaintSize);
			}
		}
		pSceneNow = pSceneNext;														//����A�b�v�f�[�g�Ώۂ��i�[
	}

	if (!pKeyboard->GetPressKeyboard(DIK_LALT))
	{
		if (pKeyboard->GetPressKeyboard(DIK_R))
		{
			if (pMouse->GetReleaseMouse(MOUSE_RIGHT))
			{// �}�E�X�̉E�{�^���������ꂽ�Ƃ�
				pSceneNow = CScene::GetScene(CScene::PRIORITY_FLOOR);					// ���̐擪�A�h���X�̎擾

				//�����Ȃ��Ȃ�܂ŌJ��Ԃ�
				while (pSceneNow != NULL)
				{
					pSceneNext = CScene::GetSceneNext(pSceneNow, CScene::PRIORITY_FLOOR);		//����A�b�v�f�[�g�Ώۂ��T����
					CMeshField *pField = (CMeshField*)pSceneNow;
					pField->CalculationNormalize();
					pSceneNow = pSceneNext;														//����A�b�v�f�[�g�Ώۂ��i�[
				}
			}
			else if (pMouse->GetReleaseMouse(MOUSE_LEFT))
			{// �}�E�X�̍��{�^���������ꂽ�Ƃ�
				pSceneNow = CScene::GetScene(CScene::PRIORITY_FLOOR);					// ���̐擪�A�h���X�̎擾

				//�����Ȃ��Ȃ�܂ŌJ��Ԃ�
				while (pSceneNow != NULL)
				{
					pSceneNext = CScene::GetSceneNext(pSceneNow, CScene::PRIORITY_FLOOR);		//����A�b�v�f�[�g�Ώۂ��T����
					CMeshField *pField = (CMeshField*)pSceneNow;
					pField->CalculationNormalize();
					pSceneNow = pSceneNext;														//����A�b�v�f�[�g�Ώۂ��i�[
				}
			}
		}
	}

	m_mouseOld = worldPos;
}

//=============================================================================
// ���_�F�ύX����
//=============================================================================
void CDebugProc::Paint(D3DXVECTOR3 & worldPos)
{
	CInputMouse *pMouse = CManager::GetInputMouse();								// �}�E�X�̎擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();						// �L�[�{�[�h�̎擾

	// ���݃}�E�X�̍��W�͈͓̔��ɂ��鏰���擾����
	CScene *pSceneNext = NULL;														// ������
	CScene *pSceneNow = CScene::GetScene(CScene::PRIORITY_FLOOR);					// ���̐擪�A�h���X�̎擾

	// �����Ȃ��Ȃ�܂ŌJ��Ԃ�
	while (pSceneNow != NULL)
	{
		pSceneNext = CScene::GetSceneNext(pSceneNow, CScene::PRIORITY_FLOOR);		// ����A�b�v�f�[�g�Ώۂ��T����
		CMeshField *pField = (CMeshField*)pSceneNow;
		if (pField->CollisionRangeToCircle(worldPos, m_fPaintSize))					// �~�����͈͓̔��ɓ����Ă��邩�ǂ���
		{
			if (m_mouseOld != worldPos)
			{// �O�̍��W�ƍ���̍��W���Ⴄ�Ƃ�
				pField->Paint(worldPos, m_fPaintSize);
			}
		}
		pSceneNow = pSceneNext;														// ����A�b�v�f�[�g�Ώۂ��i�[
	}

	m_mouseOld = worldPos;
}

//=============================================================================
// �G�z�u����
//=============================================================================
void CDebugProc::CreateEnemy(D3DXVECTOR3 &worldPos)
{
	CInputMouse *pMouse = CManager::GetInputMouse();			// �}�E�X�̎擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̎擾
	CScene *pScene = CScene::NowFloor(worldPos);				// ���݂���ꏊ�̃t�B�[���h���擾

	D3DXVECTOR3 pos = worldPos;

	if (pScene != NULL)
	{// �������݂��Ă����Ƃ�
		CMeshField *pMeshField = (CMeshField*)pScene;			// ���̎擾
		pos.y = pMeshField->GetHeight(worldPos);				// ���̍������擾
	}

	if (m_pEnemy != NULL)
	{
		m_pEnemy->SetPosition(pos);								// ���{�p���f���̈ʒu�����݂̃}�E�X���W�ɐݒ�
	}

	if (!pKeyboard->GetPressKeyboard(DIK_LALT))
	{// Alt�L�[ ��������Ă��Ȃ��Ƃ�
		if (pKeyboard->GetPressKeyboard(DIK_R))
		{
			if (pMouse->GetTriggerMouse(MOUSE_LEFT))
			{// �}�E�X�̍��{�^���������ꂽ�Ƃ�
				CEnemy *pEnemy = CEnemy::Create();				// ���݂̌��{���쐬
				pEnemy->SetPosition(worldPos);					// ���{�̏ꏊ�ɐݒu
			}
		}
	}
}

//=============================================================================
// �����z�u����
//=============================================================================
void CDebugProc::CreateObject(D3DXVECTOR3 &worldPos)
{
	CInputMouse *pMouse = CManager::GetInputMouse();				// �}�E�X�̎擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();		// �L�[�{�[�h�̎擾
	CCamera *pCamera = CManager::GetCamera();						// �J�����̎擾
	CMeshField *pMeshField = CGame::GetMesh();
	CObject *pObject = NULL;

	if (!pKeyboard->GetPressKeyboard(DIK_LALT))
	{// ��Alt�L�[��������Ă��Ȃ��Ƃ�
		if (pKeyboard->GetPressKeyboard(DIK_R))
		{
			if (pMouse->GetPressMouse(MOUSE_LEFT))
			{// �}�E�X�̍��{�^����������Ă���Ƃ�
				D3DXVECTOR3 Difference;				// ����
				float fDistance;					// ����

				// �O�̈ʒu�ƌ��݂̈ʒu�Ƃ̋������Z�o
				Difference.x = m_mouseOld.x - worldPos.x;
				Difference.z = m_mouseOld.z - worldPos.z;
				fDistance = (float)sqrt(Difference.x * Difference.x + Difference.z * Difference.z);

				if (fDistance > m_fPaintSize)
				{// �O�̍��W�ƍ���̍��W���Ⴄ�Ƃ�
					// ���f���͈͔̔z�u
					for (int nCount = 0; nCount < m_nCntGeneration; nCount++)
					{
						float fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						D3DXVECTOR3 pos = D3DXVECTOR3(sinf(D3DX_PI + 6.28f * fAngle) * ((float)CManager::GetRand((int)m_fPaintSize * 10) * 0.1f) + worldPos.x,
							0.0f,
							cosf(D3DX_PI + 6.28f * fAngle) * ((float)CManager::GetRand((int)m_fPaintSize * 10) * 0.1f) + worldPos.z);

						//���̍������擾����
						CScene *pSceneNext = NULL;			// ������
						CScene *pSceneNow = CScene::GetScene(CScene::PRIORITY_MODEL);			// �V�[���̐擪�A�h���X���擾

						//�����Ȃ��Ȃ�܂ŌJ��Ԃ�
						while (pSceneNow != NULL)
						{
							pSceneNext = CScene::GetSceneNext(pSceneNow, CScene::PRIORITY_MODEL);		// ����A�b�v�f�[�g�Ώۂ��T����
							CMeshField *pField = (CMeshField*)pSceneNow;								// �N���X�`�F���W(��)

							if (m_nCntContinue > CONTINUE_MAX)
							{// �Čv�Z�񐔂�10��𒴂����Ƃ�
								break;
							}
							else if (50.0f > CManager::GetDistance(pSceneNow->GetPosition(), pos))
							{
								// �l�̍Ď擾
								float fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
								D3DXVECTOR3 pos = D3DXVECTOR3(sinf(D3DX_PI + 6.28f * fAngle) * ((float)CManager::GetRand((int)m_fPaintSize * 10) * 0.1f) + worldPos.x,
									0.0f,
									cosf(D3DX_PI + 6.28f * fAngle) * ((float)CManager::GetRand((int)m_fPaintSize * 10) * 0.1f) + worldPos.z);

								pSceneNext = CScene::GetScene(CScene::PRIORITY_MODEL);			// �V�[���̐擪�A�h���X���擾
								m_nCntContinue++;					// �Ċm�F�񐔂Ƀv���X����
							}

							pSceneNow = pSceneNext;				//����A�b�v�f�[�g�Ώۂ��i�[
						}

						if (m_nCntContinue < CONTINUE_MAX)
						{
							// �I�u�W�F�N�g�̍쐬
							pObject = CObject::Create((OBJTYPE)m_nType);
						}

						m_nCntContinue = 0;

						if (pObject != NULL)
						{
							//���̍������擾����
							CScene *pSceneNext = NULL;			// ������
							CScene *pSceneNow = CScene::GetScene(CScene::PRIORITY_FLOOR);			// �V�[���̐擪�A�h���X���擾

							//�����Ȃ��Ȃ�܂ŌJ��Ԃ�
							while (pSceneNow != NULL)
							{
								pSceneNext = CScene::GetSceneNext(pSceneNow, CScene::PRIORITY_FLOOR);		// ����A�b�v�f�[�g�Ώۂ��T����
								CMeshField *pField = (CMeshField*)pSceneNow;								// �N���X�`�F���W(��)

								if (pField->CollisionRange(pos))
								{// �I�u�W�F�N�g �����ɏ���Ă����Ƃ�
									pos.y = pField->GetHeight(pos);										// ���̍��������߂�
									pObject->SetPosition(pos);
									break;
								}

								pSceneNow = pSceneNext;				//����A�b�v�f�[�g�Ώۂ��i�[
							}
						}
					}

					m_mouseOld = worldPos;					// �Ō�̉��H�ʒu��ۑ�
				}
			}
		}
	}
}

//=============================================================================
// �P�̔z�u����
//=============================================================================
void CDebugProc::CreateIndividual(D3DXVECTOR3 &worldPos)
{
	CInputMouse *pMouse = CManager::GetInputMouse();								// �}�E�X�̎擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();						// �L�[�{�[�h�̎擾
	CScene *pScene = CScene::NowFloor(worldPos);									// ���݂���ꏊ�̃t�B�[���h���擾

	D3DXVECTOR3 pos = worldPos;
	D3DXVECTOR3 rot;

	if (m_pObject != NULL)
	{// ���{�p�I�u�W�F�N�g�����݂��Ă����Ƃ�
		rot = m_pObject->GetRotation();					// ��]�l�̎擾
	}

	ImGui::DragFloat3("rot", (float*)&rot, 0.01f);

	if (pScene != NULL)
	{// �������݂��Ă����Ƃ�
		CMeshField *pMeshField = (CMeshField*)pScene;								// ���̎擾
		pos.y = pMeshField->GetHeight(worldPos);									// ���̍������擾
	}

	if (m_pObject != NULL)
	{
		if (m_pObject->GetType() != (OBJTYPE)m_nType)
		{// ���݂̃��f���^�C�v���O��̃��f���ƈႤ�Ƃ�
			m_pObject->Uninit();															// �O�̃��f�����J��
			m_pObject->Release();															// �O�̃��f���̊J���t���O�𗧂Ă�
			m_pObject = NULL;																// �O�̃��f���̃A�h���X���̂Ă�

			if (m_pObject == NULL)
			{// ���f���̃A�h���X�� NULL �������Ƃ�
				m_pObject = CObject::Create((OBJTYPE)m_nType);								// �V�������f���𐶐�
			}
		}
	}

	if (m_pObject != NULL)
	{// ���{�p�I�u�W�F�N�g�����݂��Ă����Ƃ�
		m_pObject->SetPosition(pos);														// ���{�p���f���̈ʒu�����݂̃}�E�X���W�ɐݒ�
		m_pObject->SetRotation(rot);					// ��]�l�̎擾
	}

	if (!pKeyboard->GetPressKeyboard(DIK_LALT))
	{// Alt�L�[ ��������Ă��Ȃ��Ƃ�
		if (pKeyboard->GetPressKeyboard(DIK_R))
		{
			if (pMouse->GetTriggerMouse(MOUSE_LEFT))
			{// �}�E�X�̍��{�^���������ꂽ�Ƃ�
				CObject *pObject = CObject::Create((OBJTYPE)m_nType);							// ���݂̌��{���쐬
				pObject->SetPosition(worldPos);												// ���{�̏ꏊ�ɐݒu
				pObject->SetRotation(rot);
			}
		}
	}
}

//=============================================================================
// �I�u�W�F�N�g�͈͓��폜����
//=============================================================================
void CDebugProc::DeleteObject(D3DXVECTOR3 &worldPos)
{
	CInputMouse *pMouse = CManager::GetInputMouse();								// �}�E�X�̎擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();						// �L�[�{�[�h�̎擾

	// ���݃}�E�X�̍��W�͈͓̔��ɂ��鏰���擾����
	CScene *pSceneNext = NULL;														// ������
	CScene *pSceneNow = CScene::GetScene(CScene::PRIORITY_MODEL);					// ���̐擪�A�h���X�̎擾

	if (pKeyboard->GetPressKeyboard(DIK_R))
	{
		if (pMouse->GetPressMouse(MOUSE_LEFT))
		{// �}�E�X�̉E�{�^���������ꂽ�Ƃ�
		// �����Ȃ��Ȃ�܂ŌJ��Ԃ�
			while (pSceneNow != NULL)
			{
				pSceneNext = CScene::GetSceneNext(pSceneNow, CScene::PRIORITY_MODEL);		//����A�b�v�f�[�g�Ώۂ��T����
				CSceneX *pObject = (CSceneX*)pSceneNow;

				if (CMeshField::SphereModel(pObject->GetPosition(), worldPos, m_fPaintSize))					// �~�����͈͓̔��ɓ����Ă��邩�ǂ���
				{
					pObject->Release();
				}
				pSceneNow = pSceneNext;														//����A�b�v�f�[�g�Ώۂ��i�[
			}
		}
	}
}
#endif