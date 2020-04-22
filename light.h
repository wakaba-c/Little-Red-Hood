//=============================================================================
//
// ライト処理 [light.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_LIGHT 3

//=============================================================================
// クラス定義
//=============================================================================
class CLight
{
public:
	CLight();				// コンストラクタ
	~CLight();				// デストラクタ
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 開放処理
	void Update(void);		// 更新処理

	D3DLIGHT9 GetLight(const int nIndex);		// ライトの取得

private:
#ifdef _DEBUG
	void Debug(void);							// デバッグ処理
#endif
	D3DLIGHT9 m_aLight[MAX_LIGHT];				// ライト情報
};
#endif
