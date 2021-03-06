//=============================================================================
//
// メイン処理 [main.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルードファイル
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

#define DIRECTINPUT_VERSION (0x0800)	// 警告対処用
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
// ライブラリのリンク
//=============================================================================
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment	(lib, "xinput.lib")
#pragma comment (lib, "x3daudio.lib")

//=============================================================================
// マクロ定義
//=============================================================================
#define SCREEN_WIDTH	(1280)	// ウインドウの幅
#define SCREEN_HEIGHT	(720)	// ウインドウの高さ

//=============================================================================
// 前方宣言
//=============================================================================
class CRenderer;
class CScene;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
int GetFPS(void);
#endif