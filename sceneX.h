//=============================================================================
//
// sceneX処理 [sceneX.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _SCENEX_H_
#define _SCENEX_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
// クラス定義
//=============================================================================
class CSceneX : public CScene
{
public:
	CSceneX(PRIORITY obj);				// コンストラクタ
	~CSceneX();							// デストラクタ
	HRESULT Init(void);					// 初期化処理
	void Uninit(void);					// 開放処理
	void Update(void);					// 更新処理
	void Draw(void);					// 描画処理

	static CSceneX *Create(void);		// クリエイト処理
	void BindModel(LPDIRECT3DVERTEXBUFFER9 pVtxBuff, LPD3DXMESH pMesh, DWORD nNumMat, LPD3DXBUFFER pBuffMat, LPDIRECT3DTEXTURE9 *tex);	// モデルの設定

	void SetColor(D3DXCOLOR col);													// 色を設定する
	void SetSize(D3DXVECTOR3 size);													// 大きさを設定する
	void SetRotation(D3DXVECTOR3 rot);												// 回転値を設定

	D3DXCOLOR GetColor(void) { return m_col; }										// 色を返す
	D3DXVECTOR3 GetSize(void) { return m_size; }									// 大きさを返す
	D3DXVECTOR3 GetRotation(void) { return m_rot; }									// 回転値を取得

	void OnTriggerEnter(CCollider *col) {};
	void OnCollisionEnter(CCollider *col) {};

private:
#ifdef _DEBUG
	void Debug(void);
#endif
	LPDIRECT3DTEXTURE9	*m_pTexture;											//テクスチャへのポインタ
	D3DXVECTOR3 m_size;																//大きさ
	D3DXVECTOR3 m_rot;																//回転量
	D3DXCOLOR	m_col;																//色
	D3DXMATRIX	m_mtxWorld;															//ワールドマトリックス

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;												//頂点バッファへのポインタ
	LPD3DXMESH		m_pMesh;														//メッシュ情報へのポインタ
	DWORD			m_nNumMat;														//マテリアル情報の数
	LPD3DXBUFFER	m_pBuffMat;														//マテリアル情報へのポインタ

	int m_nLife;																	//ライフ
	int m_nLifeMax;
	float m_fAngle;																	//角度
	float m_fLength;																//長さ
};
#endif