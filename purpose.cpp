//=============================================================================
//
// 目的表示処理 [purpose.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "purpose.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CPurpose::CPurpose(CScene::PRIORITY obj = CScene::PRIORITY_UI) : CScene2D(obj)
{
	SetObjType(PRIORITY_UI);				//オブジェクトタイプ

	//値の初期化
	for (int nCount = 0; nCount < DRAWINGTYPE_MAX; nCount++)
	{
		m_apScene2D[nCount] = NULL;
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CPurpose::~CPurpose()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPurpose::Init(void)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(1000.0f, 50.0f, 0.0f);
	m_size = D3DXVECTOR3(500.0f, 150.0f, 0.0f);				// サイズの定義
	SetSize(m_size);			// サイズの設定

	// 看板の生成
	m_apScene2D[DRAWINGTYPE_SIGNBORAD] = CScene2D::Create(PRIORITY_UI);

	if (m_apScene2D[DRAWINGTYPE_SIGNBORAD] != NULL)
	{// ポインタがNULLではないとき
		m_apScene2D[DRAWINGTYPE_SIGNBORAD]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));		// 色の設定
		m_apScene2D[DRAWINGTYPE_SIGNBORAD]->SetPosition(pos);			// 位置の設定
		m_apScene2D[DRAWINGTYPE_SIGNBORAD]->SetSize(m_size);			// 大きさの設定
		m_apScene2D[DRAWINGTYPE_SIGNBORAD]->BindTexture(CManager::GetResource("data/tex/signeBoard.png"));		//テクスチャの設定
		m_apScene2D[DRAWINGTYPE_SIGNBORAD]->SetTransform();				// 頂点情報の設定
	}

	// メッセージの作成
	m_apScene2D[DRAWINGTYPE_MESSAGE] = CScene2D::Create(PRIORITY_UI);

	if (m_apScene2D[DRAWINGTYPE_MESSAGE] != NULL)
	{// ポインタがNULLではないとき
		m_apScene2D[DRAWINGTYPE_MESSAGE]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));			// 色の設定
		m_apScene2D[DRAWINGTYPE_MESSAGE]->SpriteAnimation(D3DXVECTOR2(2.0f, 2.0f), 0, 0);			// 位置の設定
		m_apScene2D[DRAWINGTYPE_MESSAGE]->SetPosition(D3DXVECTOR3(1000.0f, 70.0f, 0.0f));			// 位置の設定
		m_apScene2D[DRAWINGTYPE_MESSAGE]->SetSize(D3DXVECTOR3(350.0f, 150.0f, 0.0f));				// 大きさの設定
		m_apScene2D[DRAWINGTYPE_MESSAGE]->BindTexture(CManager::GetResource("data/tex/message.png"));		//テクスチャの設定
		m_apScene2D[DRAWINGTYPE_MESSAGE]->SetTransform();				// 頂点情報の設定
	}

	return S_OK;
}

//=============================================================================
// 更新処理
//=============================================================================
void CPurpose::Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void CPurpose::Draw(void)
{

}

//=============================================================================
// 開放処理
//=============================================================================
void CPurpose::Uninit(void)
{
	//終了処理
	for (int nCntLife = 0; nCntLife < DRAWINGTYPE_MAX; nCntLife++)
	{
		if (m_apScene2D[nCntLife] != NULL)
		{// シーンが存在していたとき
			m_apScene2D[nCntLife]->Release();			// 終了処理
			m_apScene2D[nCntLife] = NULL;				// NULLを代入する
		}
	}
}

//=============================================================================
// クリエイト処理
//=============================================================================
CPurpose *CPurpose::Create(void)
{
	CPurpose *pPurpose;
	pPurpose = new CPurpose(PRIORITY_UI);
	pPurpose->Init();
	return pPurpose;
}

//=============================================================================
// ロード処理
//=============================================================================
HRESULT CPurpose::Load(void)
{
	CManager::Load("data/tex/signeBoard.png");
	CManager::Load("data/tex/message.png");
	return S_OK;
}

//=============================================================================
// 目的変更処理
//=============================================================================
void CPurpose::ChangePurpose(MESSAGETYPE type)
{
	// 目的を設定
	m_type = type;

	// 目的に合わせる
	m_apScene2D[DRAWINGTYPE_MESSAGE]->SpriteAnimation(D3DXVECTOR2(2.0f, 2.0f), type, 0.0f);
}