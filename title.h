//=============================================================================
//
// タイトル処理 [title.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CTitlelogo;

//=============================================================================
// クラス定義
//=============================================================================
class CTitle
{
public:
	CTitle();							// コンストラクタ
	~CTitle();							// デストラクタ
	HRESULT Init(void);					// 初期化処理
	void Uninit(void);					// 開放処理
	void Update(void);					// 更新処理
	void Draw(void);					// 描画処理

	static void LoadAsset(void);		// アセットの読み込み処理

private:
	static CTitlelogo *m_pTitleLogo;			// タイトルロゴのポインタ
};
#endif