//=============================================================================
//
// ���C������ [main.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "main.h"
#include "manager.h"
#include "resource.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define CLASS_NAME		"WindowClass"			// �E�C���h�E�̃N���X��
#define WINDOW_NAME		"�����Ԃ����񂪐킦����"		// �E�C���h�E�̃L���v�V������

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void Init(void);
void Uninit(void);
void Update(void);
void Draw(void);

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
int g_nCountFPS;

//=============================================================================
// ���C���֐�
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	CManager *pManager;

#ifdef _DEBUG
	// ���������[�N�̒�����ݒ�
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
#endif

	//�}�l�[�W���̐���
	pManager = new CManager;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WindowProc,
		0,
		0,
		hInstance,
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)),
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1))
	};
	HWND hWnd;
	MSG msg;
	RECT rect =	{0,	0, SCREEN_WIDTH, SCREEN_HEIGHT};

	DWORD dwCurrentTime;		// ���ݎ���
	DWORD dwExecLastTime;		// �Ō�ɏ�����������

	DWORD dwFrameCount;
	DWORD dwFPSLastTime;

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �w�肵���N���C�A���g�̈���m�ۂ��邽�߂ɕK�v�ȃE�B���h�E���W���v�Z
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// �E�C���h�E���쐬
	hWnd = CreateWindowEx(0,
						CLASS_NAME,
						WINDOW_NAME,
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						(rect.right - rect.left),
						(rect.bottom - rect.top),
						NULL,
						NULL,
						hInstance,
						NULL);

	// ����������(�E�B���h�E���쐬���Ă���s��)
	if (FAILED(pManager->Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	Init();

	// �E�C���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// ����\��ݒ�
	timeBeginPeriod(1);

	dwCurrentTime = 0;
	dwExecLastTime = timeGetTime();		// �V�X�e���������~���b�P�ʂŎ擾

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();		// �V�X�e���������~���b�P�ʂŎ擾

	// ���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)	// ���b�Z�[�W���擾���Ȃ������ꍇ 0 ��Ԃ�
		{// Windows�̏���
			if (msg.message == WM_QUIT)
			{// WM_QUIT���b�Z�[�W�������烋�[�v�𔲂���
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��Ƒ��o
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{// DirectX�̏���

			dwCurrentTime = timeGetTime();		// �V�X�e���������擾

			if ((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5�b���ƂɎ��s
			{
#ifdef _DEBUG
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);		// FPS�𑪒�
#endif

				dwFPSLastTime = dwCurrentTime;		// FPS�𑪒肵��������ۑ�
				dwFrameCount = 0;					// �J�E���g���N���A
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))		// 1/60�b���ƂɎ��s
			{
				dwExecLastTime = dwCurrentTime;		// ��������������ۑ�

				// �X�V����
				pManager->Update();

				// �`�揈��
				pManager->Draw();

				dwFrameCount++;			// �J�E���g�����Z
			}
		}
	}

	// �I������
	if (pManager != NULL)
	{
		pManager->Uninit();
		delete pManager;
		pManager = NULL;
	}

	// ����\��߂�
	timeEndPeriod(1);

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

// Win32 message handler
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//=============================================================================
// �E�C���h�E�v���V�[�W��
//=============================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
#ifdef _DEBUG
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
	{
		return true;
	}
#endif

	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=============================================================================
// ����������
//=============================================================================
void Init(void)
{

}

//=============================================================================
// �I������
//=============================================================================
void Uninit(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void Update(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void Draw(void)
{
}

//=============================================================================
// GetFPS�֐�
//=============================================================================
int GetFPS(void)
{
	return g_nCountFPS;
}