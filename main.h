//=============================================================================
//
// ���C������ [main.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include <windows.h>
#include <stdio.h>
#include <XInput.h>
#include <map>
#include "d3dx9.h"
#include <time.h>
#include <fstream>
#include <iostream>
#include <stdarg.h>
#include <string>

#define DIRECTINPUT_VERSION (0x0800)	// �x���Ώ��p
#include "dinput.h"
#include "debug.h"

#include <XAudio2.h>
#include <X3DAudio.h>
#include <x3daudio.h>
#include <xaudio2fx.h>
#define _USE_VOICECALLBACK_

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

//=============================================================================
// ���C�u�����̃����N
//=============================================================================
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment	(lib, "xinput.lib")
#pragma comment (lib, "x3daudio.lib")

//=============================================================================
// �}�N����`
//=============================================================================
#define SCREEN_WIDTH	(1280)	// �E�C���h�E�̕�
#define SCREEN_HEIGHT	(720)	// �E�C���h�E�̍���

//=============================================================================
// �O���錾
//=============================================================================
class CRenderer;
class CScene;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
int GetFPS(void);
#endif