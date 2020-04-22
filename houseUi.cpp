//=============================================================================
//
// おばあちゃん家のUI管理処理 [houseUi.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "houseUi.h"
#include "manager.h"
#include "renderer.h"
#include "inputKeyboard.h"
#include "gauge2D.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CHouseUi::CHouseUi(CScene::PRIORITY obj = CScene::PRIORITY_UI) : CScene2D(obj)
{
	(CScene::PRIORITY_UI);				//オブジェクトタイプ

	//値の初期化
	m_move = D3DXVECTOR3(0, 0, 0.0f);		// 移動量の初期化
	m_nPage = 0;							// ページ数の初期化

	if (m_pGauge2D != NULL)
	{// 耐久値ゲージのポインタが存在したとき
		m_pGauge2D = NULL;					// NULLを代入
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CHouseUi::~CHouseUi()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CHouseUi::Init(void)
{
	// 初期化処理
	CScene2D::Init();

	// 背面テクスチャの設定
	BindTexture(CManager::GetResource("data/tex/SignBoard2.png"));

	SetPosition(D3DXVECTOR3(230.0f, 100.0f, 0.0f));		// 背面の位置設定
	SetSize(D3DXVECTOR3(420.0f, 150.0f, 0.0f));			// 背面の大きさ設定
	SetTransform();										// 頂点情報の更新

	m_pGauge2D = CGauge2D::Create();					// HPゲージの作成

	if (m_pGauge2D != NULL)
	{// HPゲージが存在していたとき
		m_pGauge2D->SetPosition(D3DXVECTOR3(490.0f, 90.0f, 0.0f));			// HPゲージの位置設定
		m_pGauge2D->SetGaugeSize(D3DXVECTOR3(520.0f, 15.0f, 0.0f), D3DXVECTOR3(600.0f, 50.0f, 0.0f));		// HPゲージの大きさ設定
	}

	m_apScene2D[DRAWINGTYPE_HP] = CScene2D::Create(PRIORITY_UI);		// HPタグの作成

	if (m_apScene2D[DRAWINGTYPE_HP] != NULL)
	{// HPタグが存在していたとき
		m_apScene2D[DRAWINGTYPE_HP]->BindTexture(CManager::GetResource("data/tex/Grandma's house_Tag.png"));			// テクスチャの取得
		m_apScene2D[DRAWINGTYPE_HP]->SetPosition(D3DXVECTOR3(SCREEN_WIDTH / 2, 35.0f, 0.0f));		// 位置設定
		m_apScene2D[DRAWINGTYPE_HP]->SetSize(D3DXVECTOR3(300.0f, 90.0f, 0.0f));					// 大きさ設定
		m_apScene2D[DRAWINGTYPE_HP]->SetTransform();				// 頂点情報の更新
	}
	return S_OK;
}

//=============================================================================
// 開放処理
//=============================================================================
void CHouseUi::Uninit(void)
{
	//終了処理
	for (int nCntLife = 0; nCntLife < DRAWINGTYPE_MAX; nCntLife++)
	{
		if (m_apScene2D[nCntLife] != NULL)
		{// シーンが存在していたとき
			m_apScene2D[nCntLife]->Release();		// 削除予約
			m_apScene2D[nCntLife] = NULL;			// NULLを代入
		}
	}

	// 終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CHouseUi::Update(void)
{
	if (m_nDamage * m_size.x < m_fHP * m_size.x)
	{// 現在の大きさよりダメージ用のゲージが大きい場合
		m_fHP -= 0.005f;			// 引く
	}

	// 更新処理
	CScene2D::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CHouseUi::Draw(void)
{
	// 描画処理
	CScene2D::Draw();
}

//=============================================================================
// クリエイト処理
//=============================================================================
CHouseUi *CHouseUi::Create(void)
{
	CHouseUi *pHouseUi;
	pHouseUi = new CHouseUi(PRIORITY_UI);

	if (pHouseUi != NULL)
	{// 家のUIが存在していたとき
		pHouseUi->Init();			// 初期化処理
	}
	return pHouseUi;
}

//=============================================================================
// ロード関数
//=============================================================================
HRESULT CHouseUi::Load(void)
{
	//テクスチャの読み込み
	CManager::Load("data/tex/SignBoard2.png");
	CManager::Load("data/tex/rose.png");
	return S_OK;
}