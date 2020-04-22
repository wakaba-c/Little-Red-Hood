//=============================================================================
//
// プレイヤーのUI管理処理 [playerUi.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "playerUi.h"
#include "manager.h"
#include "renderer.h"
#include "inputKeyboard.h"
#include "gauge2D.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CPlayerUi::CPlayerUi(CScene::PRIORITY obj = CScene::PRIORITY_UI) : CScene2D(obj)
{
	(CScene::PRIORITY_UI);				//オブジェクトタイプ

	//値の初期化
	m_move = D3DXVECTOR3(0, 0, 0.0f);	// 移動量の初期化
	m_nPage = 0;						// ページ数の初期化

	for (int nCount = 0; nCount < GAUGETYPE_MAX; nCount++)
	{
		if (m_apGauge2D[nCount] != NULL)
		{// HPと必殺ワザゲージのポインタが存在したとき
			m_apGauge2D[nCount] = NULL;		// NULLにする
		}
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CPlayerUi::~CPlayerUi()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPlayerUi::Init(void)
{
	// 初期化処理
	CScene2D::Init();

	// 背面テクスチャの設定
	BindTexture(CManager::GetResource("data/tex/status_back.png"));

	SetPosition(D3DXVECTOR3(230.0f, 600.0f, 0.0f));		// 背面の位置設定
	SetSize(D3DXVECTOR3(420.0f, 150.0f, 0.0f));			// 背面の大きさ設定
	SetTransform();										// 頂点情報の更新

	m_apGauge2D[GAUGETYPE_HP] = CGauge2D::Create();		// HPゲージの作成

	if (m_apGauge2D[GAUGETYPE_HP] != NULL)
	{// HPゲージが存在していたとき
		m_apGauge2D[GAUGETYPE_HP]->SetPosition(D3DXVECTOR3(130.0f, 650.0f, 0.0f));											// HPゲージの位置設定
		m_apGauge2D[GAUGETYPE_HP]->SetGaugeSize(D3DXVECTOR3(520.0f, 15.0f, 0.0f), D3DXVECTOR3(600.0f, 50.0f, 0.0f));		// HPゲージの大きさ設定
	}

	m_apGauge2D[GAUGETYPE_DEATHBLOW] = CGauge2D::Create();				// 必殺技ゲージの作成

	if (m_apGauge2D[GAUGETYPE_DEATHBLOW] != NULL)
	{// 必殺技ゲージが存在していたとき
		m_apGauge2D[GAUGETYPE_DEATHBLOW]->SetPosition(D3DXVECTOR3(130.0f, 600.0f, 0.0f));										// 必殺技ゲージの位置設定
		m_apGauge2D[GAUGETYPE_DEATHBLOW]->SetGaugeSize(D3DXVECTOR3(520.0f, 15.0f, 0.0f), D3DXVECTOR3(600.0f, 50.0f, 0.0f));		// 必殺技ゲージの大きさ設定
		m_apGauge2D[GAUGETYPE_DEATHBLOW]->SetLifeBar(false, 0.0f);
	}

	m_apScene2D[DRAWINGTYPE_HP] = CScene2D::Create(PRIORITY_UI);		// HPタグの作成

	if (m_apScene2D[DRAWINGTYPE_HP] != NULL)
	{// HPタグが存在していたとき
		m_apScene2D[DRAWINGTYPE_HP]->BindTexture(CManager::GetResource("data/tex/HP.png"));			// テクスチャの設定
		m_apScene2D[DRAWINGTYPE_HP]->SetPosition(D3DXVECTOR3(190.0f, 600.0f, 0.0f));				// 位置設定
		m_apScene2D[DRAWINGTYPE_HP]->SetSize(D3DXVECTOR3(100.0f, 30.0f, 0.0f));						// 大きさ設定
		m_apScene2D[DRAWINGTYPE_HP]->SetTransform();												// 頂点情報の更新
	}

	m_apScene2D[DRAWINGTYPE_DEATHBLOW] = CScene2D::Create(PRIORITY_UI);	// 必殺技タグの作成

	if (m_apScene2D[DRAWINGTYPE_DEATHBLOW] != NULL)
	{// 必殺技タグが存在していたとき
		m_apScene2D[DRAWINGTYPE_DEATHBLOW]->BindTexture(CManager::GetResource("data/tex/end.png"));	// テクスチャの設定
		m_apScene2D[DRAWINGTYPE_DEATHBLOW]->SetPosition(D3DXVECTOR3(190.0f, 550.0f, 0.0f));			// 位置設定
		m_apScene2D[DRAWINGTYPE_DEATHBLOW]->SetSize(D3DXVECTOR3(70.0f, 30.0f, 0.0f));				// 大きさ設定
		m_apScene2D[DRAWINGTYPE_DEATHBLOW]->SetTransform();											// 頂点情報の更新
	}

	m_apScene2D[DRAWINGTYPE_BACK] = CScene2D::Create(PRIORITY_UI);		// 背景の作成

	if (m_apScene2D[DRAWINGTYPE_BACK] != NULL)
	{// 背景が存在していたとき
		m_apScene2D[DRAWINGTYPE_BACK]->BindTexture(CManager::GetResource("data/tex/circle.png"));	// テクスチャの設定
		m_apScene2D[DRAWINGTYPE_BACK]->SetPosition(D3DXVECTOR3(100.0f, 600.0f, 0.0f));				// 位置設定
		m_apScene2D[DRAWINGTYPE_BACK]->SetSize(D3DXVECTOR3(120.0f, 120.0f, 0.0f));					// 大きさ設定
		m_apScene2D[DRAWINGTYPE_BACK]->SetTransform();												// 頂点情報の更新
	}

	m_apScene2D[DRAWINGTYPE_CHARACTER] = CScene2D::Create(PRIORITY_UI);	// 赤ずきんの作成

	if (m_apScene2D[DRAWINGTYPE_CHARACTER] != NULL)
	{// 赤ずきんが存在していたとき
		m_apScene2D[DRAWINGTYPE_CHARACTER]->BindTexture(CManager::GetResource("data/tex/silhouette.png"));	// テクスチャの設定
		m_apScene2D[DRAWINGTYPE_CHARACTER]->SetPosition(D3DXVECTOR3(100.0f, 600.0f, 0.0f));			// 位置の設定
		m_apScene2D[DRAWINGTYPE_CHARACTER]->SetSize(D3DXVECTOR3(100.0f, 100.0f, 0.0f));				// 大きさ設定
		m_apScene2D[DRAWINGTYPE_CHARACTER]->SetTransform();											// 頂点情報の更新
	}
	return S_OK;
}

//=============================================================================
// 開放処理
//=============================================================================
void CPlayerUi::Uninit(void)
{
	//終了処理
	for (int nCntLife = 0; nCntLife < DRAWINGTYPE_MAX; nCntLife++)
	{
		if (m_apScene2D[nCntLife] != NULL)
		{
			m_apScene2D[nCntLife]->Uninit();
			m_apScene2D[nCntLife]->Release();
			m_apScene2D[nCntLife] = NULL;
		}
	}

	// 開放処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPlayerUi::Update(void)
{
	if (m_nDamage * m_size.x < m_fHP * m_size.x)
	{// 現在の大きさよりダメージ用のゲージが大きい場合
		m_fHP -= 0.005f;
	}

	// 更新処理
	CScene2D::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CPlayerUi::Draw(void)
{
	// 描画処理
	CScene2D::Draw();
}

//=============================================================================
// クリエイト処理
//=============================================================================
CPlayerUi *CPlayerUi::Create(void)
{
	CPlayerUi *pPlayerUi;
	pPlayerUi = new CPlayerUi(PRIORITY_UI);

	if (pPlayerUi != NULL)
	{// プレイヤーUiが存在していたとき
		pPlayerUi->Init();
	}
	return pPlayerUi;
}

//=============================================================================
// ロード処理
//=============================================================================
HRESULT CPlayerUi::Load(void)
{
	//テクスチャの読み込み
	CManager::Load("data/tex/status_back.png");
	CManager::Load("data/tex/rose.png");
	CManager::Load("data/tex/HP.png");
	CManager::Load("data/tex/end.png");
	CManager::Load("data/tex/circle.png");
	CManager::Load("data/tex/silhouette.png");

	return S_OK;
}