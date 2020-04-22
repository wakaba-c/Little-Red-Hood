//=============================================================================
//
// メッシュフィールド処理 [meshField.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define DEPTH_FIELD	50									// 奥行き
#define WIDE_FIELD 50									// 幅
#define SIZE		50.0f								// サイズ

//=============================================================================
// 構造体定義
//=============================================================================
//地面の種類
typedef enum
{
	RANDTYPE_NONE = -1,
	RANDTYPE_GRASS = 0,
	RANDTYPE_SAND,
	RANDTYPE_MAX
} RANDTYPE;

//=============================================================================
// 前方宣言
//=============================================================================
class CCircle;

//=============================================================================
// クラス定義
//=============================================================================
class CMeshField : public CScene
{
public:
	CMeshField(PRIORITY obj);														// コンストラクタ
	~CMeshField();																	// デストラクタ
	HRESULT Init(void);																// 初期化処理
	void Uninit(void);																// 開放処理
	void Update(void);																// 更新処理
	void Draw(void);																// 描画処理

	static CMeshField *Create(void);												// クリエイト処理
	static HRESULT Load(void);														// ロード処理

	static void LoadRand(char *add, bool bDebug);									// 地面の高さ読み込み
	void SetTexType(int nValue, RANDTYPE Type);										// 地面の種類を変更
	float GetHeight(D3DXVECTOR3 pos);												// ポリゴンの高さを求める

	static bool SphereModel(D3DXVECTOR3 C1, D3DXVECTOR3 C2, float R1);				// X軸とZ軸の当たり判定
	bool CollisionRange(D3DXVECTOR3 pos);											// 自分の範囲内にいるかどうか
	bool CollisionRangeToCircle(D3DXVECTOR3 pos, float fRadius);					// 円が自分の範囲内にいるかどうか
	bool CollisionCircle(D3DXVECTOR3 pos, float fRadius);							// 円と点の当たり判定
	void Reset(void);																// 地形の高低リセット
	void CalculationNormalize(void);												// 法線の計算

	RANDTYPE GetTexType(int nValue) { return m_randType[nValue]; }					// 地面のタイプを取得
	RANDTYPE GetRandType(void) { return	 m_type; }									// 地面のタイプを取得

	void OnTriggerEnter(CCollider *col) {};
	void OnCollisionEnter(CCollider *col) {};

#ifdef _DEBUG
	void Debug(void);
	void SaveRand(FILE *pFile);														// 地面の高さ書き込み
	void CreateRand(D3DXVECTOR3 &worldPos, float fRadius);							// 地形編集関数
	void Paint(D3DXVECTOR3 &worldPos, float fRadius);								// ペイント
#endif

private:
	void LoadTree(void);															// 木配置の書き込み

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;												// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;												// インデックスバッファへのポインタ

	void MakeVertex(FILE *pFile, bool bDebug);										// 頂点情報の作成
	void MakeIndex(void);															// 頂点インデックスの作成

	D3DXVECTOR3 apNor[DEPTH_FIELD * WIDE_FIELD * 2];								// 1面ごとの法線ベクトルの配列
	RANDTYPE	m_randType[DEPTH_FIELD * WIDE_FIELD * 2];							// 床の種類

	D3DXVECTOR3				m_rot;													// ポリゴンの向き(回転)
	D3DXCOLOR				m_col;													// 頂点カラー
	D3DXMATRIX				m_mtxWorld;												// ワールドマトリックス

	int m_nNumVertex;																// 総頂点数
	int m_nNumIndex;																// インデックス数
	int m_nNumPolygon;																// 総ポリゴン数

	RANDTYPE m_type;																// 床のタイプ
};
#endif