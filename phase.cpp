//=============================================================================
//
// フェーズ数表示処理 [phase.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "phase.h"
#include "manager.h"
#include "renderer.h"
#include "number.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CPhase::CPhase(CScene::PRIORITY obj = CScene::PRIORITY_UI) : CScene2D(obj)
{
	SetObjType(PRIORITY_UI);				//オブジェクトタイプ

	//値の初期化
	m_nNumber = 0;

	for (int nCount = 0; nCount < MAX_DAYNUMBER; nCount++)
	{
		m_apNumber[nCount] = NULL;
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CPhase::~CPhase()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPhase::Init(void)
{
	CScene2D::Init();

	//テクスチャの割当
	BindTexture(CManager::GetResource("data/tex/Phase.png"));

	//テクスチャの中心位置の設定
	SetCenter(TEXTUREVTX_CENTER);

	//番号表示
	for (int nCntNumber = 0; nCntNumber < MAX_DAYNUMBER; nCntNumber++)
	{
		m_apNumber[nCntNumber] = CNumber::Create();			// 生成

		if (m_apNumber[nCntNumber] != NULL)
		{
			m_apNumber[nCntNumber]->SetPosition(D3DXVECTOR3(180.0f + 30 * nCntNumber, 50.0f, 0.0f));		// 位置の設定
			m_apNumber[nCntNumber]->SetSize(D3DXVECTOR3(50.0f, 100.0f, 0.0f));								// サイズの設定
			m_apNumber[nCntNumber]->Init();																	// 初期化処理
		}
	}

	//スコアの設定
	SetNumber(1);									// 数字の設定
	SetSize(D3DXVECTOR3(50.0f, 50.0f, 0.0f));		// サイズの設定
	SetPosition(D3DXVECTOR3(300, 60.0f, 0.0f));		// 位置の設定
	SetTransform();									// 頂点情報の更新
	return S_OK;
}

//=============================================================================
// 開放処理
//=============================================================================
void CPhase::Uninit(void)
{
	//UIの開放
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPhase::Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void CPhase::Draw(void)
{
	// 描画処理
	CScene2D::Draw();

	for (int nCntNumber = 0; nCntNumber < MAX_DAYNUMBER; nCntNumber++)
	{
		if (m_apNumber[nCntNumber] != NULL)
		{// 数字が存在していたとき
			m_apNumber[nCntNumber]->Draw();		// 描画処理
		}
	}
}

//=============================================================================
// クリエイト処理
//=============================================================================
CPhase *CPhase::Create(void)
{
	CPhase *pPhase;
	pPhase = new CPhase(PRIORITY_UI);
	pPhase->Init();
	return pPhase;
}

//=============================================================================
// ロード処理
//=============================================================================
HRESULT CPhase::Load(void)
{
	CManager::Load("data/tex/SignBoard3.png");
	return S_OK;
}

//=============================================================================
// フェーズ数の設定
//=============================================================================
void CPhase::SetNumber(int nValue)
{
	int nNumber;
	m_nNumber = nValue;

	for (int nCntNumber = 0; nCntNumber < MAX_DAYNUMBER; nCntNumber++)
	{
		nNumber = (int)nValue % (int)pow(10, MAX_DAYNUMBER - nCntNumber) / (int)pow(10, MAX_DAYNUMBER - 1 - nCntNumber);

		if (m_apNumber[nCntNumber] != NULL)
		{
			m_apNumber[nCntNumber]->SetNumber(nNumber);
		}
	}
}

//=============================================================================
// フェーズ数の加算
//=============================================================================
void CPhase::AddNumber(int nValue)
{
	m_nNumber += nValue;

	SetNumber(m_nNumber);
}