//=============================================================================
//
// ゲーム処理 [game.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "game.h"
#include "fade.h"
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "inputKeyboard.h"
#include "meshField.h"
#include "player.h"
#include "scene3D.h"
#include "meshSphere.h"
#include "meshCube.h"
#include "meshCapsule.h"
#include "enemy.h"
#include "sky.h"
#include "tree.h"
#include "object.h"
#include "circle.h"
#include "stage.h"
#include "playerUi.h"
#include "meshOrbit.h"
#include "gauge2D.h"
#include "effect.h"
#include "tree.h"
#include "purpose.h"
#include "house.h"
#include "houseUi.h"
#include "phase.h"
#include "number.h"
#include "enemyUi.h"
#include "messageWindow.h"
#include "time.h"
#include "result.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
CMeshField	*CGame::m_pMeshField = NULL;		// メッシュフィールドのポインタ
CPlayer		*CGame::m_pPlayer = NULL;			// プレイヤーのポインタ
CMeshSphere *CGame::m_pMeshSphere = NULL;		// メッシュスフィアのポインタ
CEnemy		*CGame::m_pEnemy = NULL;			// 敵のポインタ
CSky		*CGame::m_pSky = NULL;				// 空のポインタ
CPurpose	*CGame::m_pPurpose = NULL;			// 目的表示のポインタ
CHouse		*CGame::m_pHouse = NULL;			// 家のポインタ

//=============================================================================
// コンストラクタ
//=============================================================================
CGame::CGame()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
//Init処理
//=============================================================================
HRESULT CGame::Init(void)
{
	// エフェクトの生成
	CEffect::Create();

	// プレイヤーの生成
	m_pPlayer = CPlayer::Create();

	// 空の作成
	m_pSky = CSky::Create();

	// 現在の目的表示UIの生成
	m_pPurpose = CPurpose::Create();

	// おばあちゃん家の生成
	m_pHouse = CHouse::Create();

	if (m_pHouse != NULL)
	{// おばあちゃんの家が存在していたとき
		m_pHouse->SetPosition(D3DXVECTOR3(8001.98f, 85.0f, 8194.22f));			// 位置情報の設定
	}

	// 各種アセットの生成＆設置
	CMeshField::LoadRand("data/stage/rand.csv", false);				// 床情報の読込
	CObject::LoadModel("data/stage/object.csv");					// モデル情報の読込
	CEnemy::LoadEnemy("data/stage/enemy.csv");						// 敵情報の読込
	CStage::LoadPhase("data/stage/phase_1.csv");					// [phase1]の敵読込

	// メッセージウィンドウの作成
	CMessageWindow::Create(CMessageWindow::MESSAGETYPE_START);

	// 時間のクリエイト処理
	CTime::Create();
	return S_OK;
}

//=============================================================================
// Update関数
//=============================================================================
void CGame::Update(void)
{
#ifdef _DEBUG
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	//ゲームの遷移
	if (pKeyboard->GetTriggerKeyboard(DIK_RETURN))
	{//エンターキーが押されたとき
		if (CFade::GetFade() == CFade::FADE_NONE)
		{//フェードが処理をしていないとき
			CResult::SetIdxKill(15);			// Kill数をリザルトに渡す
			//フェードを入れる
			CFade::SetFade(CManager::MODE_RESULT);
		}
	}

	// ボスへのショートカット
	if (pKeyboard->GetTriggerKeyboard(DIK_B))
	{
		if (m_pPlayer != NULL)
		{// プレイヤーが存在していたとき
			m_pPlayer->SetPosition(D3DXVECTOR3(5972.14f, 100.0f, 8000.62f));		// 位置の設定
		}
	}
#endif
}

//=============================================================================
// Draw関数
//=============================================================================
void CGame::Draw(void)
{

}

//=============================================================================
// Uninit関数
//=============================================================================
void CGame::Uninit(void)
{
	CObject::Unload();

	// ポリゴンの開放
	CScene::ReleaseAll();
}

//=============================================================================
// アセットの読み込み
//=============================================================================
void CGame::LoadAsset(void)
{
	// =============== アセットの読み込み ===============//
	CPlayer::Load();
	CSky::Load();
	CMeshField::Load();
	CMeshOrbit::Load();
	CEnemy::Load();
	CObject::Load();
	CMeshSphere::Load();
	CPlayerUi::Load();
	CGauge2D::Load();
	CEffect::Load();
	CPurpose::Load();
	CHouse::Load();
	CNumber::Load();
	CPhase::Load();
	CEnemyUi::Load();
	CMessageWindow::Load();
}