#ifdef _DEBUG
//=============================================================================
//
// デバッグ処理 [debug.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _DEBUG_H_
#define _DEBUG_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"
#include "imgui\imgui.h"
#include "imgui\imgui_impl_dx9.h"
#include "imgui\imgui_impl_win32.h"

//=============================================================================
// 構造体定義
//=============================================================================
typedef enum
{
	DEBUGMODE_NONE = 0,
	DEBUGMODE_RAND,
	DEBUGMODE_MANY,
	DEBUGMODE_INDIVIDUAL,
	DEBUGMODE_DELETE,
	DEBUGMODE_PAINT,
	DEBUGMODE_ENEMY,
	DEBUGMODE_MAX
} DEBUGMODE;

//=============================================================================
// 前方宣言
//=============================================================================
class CCircle;
class CObject;
class CEnemy;

//=============================================================================
// クラス定義
//=============================================================================
class CDebugProc
{
public:
	CDebugProc();						// コンストラクタ
	~CDebugProc();						// デストラクタ
	HRESULT Init(HWND hWnd);			// 初期化処理
	void Uninit(void);					// 開放処理
	void Update(void);					// 更新処理
	void Draw(void);					// 描画処理

	static void Log(char* fmt, ...);	// デバッグログの処理

	static CCircle *GetCircle(void) { return m_pCircle; }	// 円の範囲内にいるか
	static bool GetDebugState(void) { return m_bDebug; }	// デバッグが使用中か

private:
	static void Debug(void);								// デバッグ処理
	static void CreateRand(D3DXVECTOR3 &worldPos);			// 地形編集処理
	static void Paint(D3DXVECTOR3 &worldPos);				// ペイント処理
	static void CreateEnemy(D3DXVECTOR3 &worldPos);			// エネミー配置処理
	static void CreateObject(D3DXVECTOR3 &worldPos);		// 木配置処理
	static void CreateIndividual(D3DXVECTOR3 &worldPos);	// 単体配置処理
	static void	DeleteObject(D3DXVECTOR3 &worldPos);		// オブジェクト範囲内削除処理

	LPD3DXFONT	m_pFont;									// フォントへのポインタ
	static char m_aStr[1024];								// 文字列
	static CObject *m_pObject;								// 見本用オブジェクトポインタ
	static CEnemy *m_pEnemy;								// 見本用敵ポインタ
	static float m_fPaintSize;								// 地形編集ブラシの大きさ
	static int m_nCreateIndex;								// 1フレームの生成数
	static bool m_bDebug;									// デバッグモードの切り替え
	static int m_nCntGeneration;							// 生成数
	static int m_nMode;										// モード選択
	static int m_nType;										// モデルタイプ
	static D3DXVECTOR3 m_mouseOld;							// マウスの前のワールド座標
	static int m_nCntContinue;								// 再演算回数
	static CCircle *m_pCircle;								// 円のポインタ
};
#endif
#endif