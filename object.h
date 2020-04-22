//=============================================================================
//
// オブジェクト処理 [object.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"
#include "sceneX.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define DEPTH	20											// 奥行き
#define WIDE 20												// 幅

//=============================================================================
// 構造体定義
//=============================================================================
typedef enum
{
	OBJTYPE_TREE_1 = 0,
	OBJTYPE_TREE_2,
	OBJTYPE_TREE_3,
	OBJTYPE_FENCE,
	OBJTYPE_HOUSE_1,
	OBJTYPE_HOUSE_2,
	OBJTYPE_HOUSE_3,
	OBJTYPE_HOUSE_4,
	OBJTYPE_ARROW,
	OBJTYPE_MAX
} OBJTYPE;

//=============================================================================
// 前方宣言
//=============================================================================
class CColliderBox;
class CColliderSphere;

//=============================================================================
// クラス定義
//=============================================================================
class CObject : public CSceneX
{
public:
	CObject(PRIORITY obj);							// コンストラクタ
	~CObject();										// デストラクタ
	HRESULT Init(void);								// 初期化処理
	void Uninit(void);								// 開放処理
	void Update(void);								// 更新処理
	void Draw(void);								// 描画処理

	static CObject *Create(OBJTYPE type);			// クリエイト処理
	static HRESULT Load(void);						// ロード処理
	static void Unload(void);						// テクスチャの開放処理

	void SetType(OBJTYPE type);						// モデルタイプの設定
	static void LoadModel(char *add);				// モデルのロード処理

	OBJTYPE GetType(void) { return m_Type; }		// タイプの取得

	void OnTriggerEnter(CCollider *col) {};
	void OnCollisionEnter(CCollider *col) {};

private:
#ifdef _DEBUG
	void Debug(void);								// デバッグ処理
#endif

	static LPDIRECT3DTEXTURE9	*m_pTexture[OBJTYPE_MAX];					// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;										// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;										// インデックスバッファへのポインタ

	static LPD3DXMESH		m_pMesh[OBJTYPE_MAX];							// メッシュ情報へのポインタ
	static DWORD			m_nNumMat[OBJTYPE_MAX];							// マテリアル情報の数
	static LPD3DXBUFFER		m_pBuffMat[OBJTYPE_MAX];						// マテリアル情報へのポインタ

	CColliderBox *m_ColliderBox;											// ボックスコライダーへのポインタ
	CColliderSphere *m_ColliderSphere;										// スフィアコライダーへのポインタ
	OBJTYPE m_Type;															// タイプ
};
#endif