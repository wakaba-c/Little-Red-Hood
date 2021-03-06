//=============================================================================
//
// タイトル処理 [title.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "fade.h"
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "inputKeyboard.h"
#include "title.h"
#include "bg.h"
#include "inputController.h"
#include "player.h"
#include "meshField.h"
#include "object.h"
#include "camera.h"
#include "sky.h"
#include "house.h"
#include "titleLogo.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
CTitlelogo	*CTitle::m_pTitleLogo = NULL;		// タイトルロゴのポインタ

//=============================================================================
// コンストラクタ
//=============================================================================
CTitle::CTitle()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTitle::Init(void)
{
	CCamera *pCamera = CManager::GetCamera();		// カメラの取得

	if (pCamera != NULL)
	{// カメラが存在していたとき
		pCamera->SetPosCamera(D3DXVECTOR3(7099.40f, 187.26f, 7523.8f), D3DXVECTOR3(0.1f, -2.33f, 0.0f));		// 位置の設定
	}

	m_pTitleLogo = CTitlelogo::Create();		// タイトルロゴの生成

	// 空の作成
	CSky::Create();

	// おばあちゃん家の生成
	CHouse *pHouse = CHouse::Create();

	if (pHouse != NULL)
	{// おばあちゃんの家が存在していたとき
		pHouse->SetPosition(D3DXVECTOR3(8001.98f, 85.0f, 8194.22f));			// 位置情報の設定
	}

	// 各種アセットの生成＆設置
	CMeshField::LoadRand("data/stage/rand.csv", false);				// 床情報の読込
	CObject::LoadModel("data/stage/object.csv");					// モデル情報の読込
	return S_OK;
}

//=============================================================================
// 開放処理
//=============================================================================
void CTitle::Uninit(void)
{
	// タイトルロゴの開放
	if (m_pTitleLogo != NULL)
	{// 存在していたとき
		m_pTitleLogo->Uninit();			// 開放処理
		delete m_pTitleLogo;			// 削除
		m_pTitleLogo = NULL;			// NULLの代入
	}

	//ポリゴンの開放
	CScene::ReleaseAll();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTitle::Update(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();
	CInputController *pInputController = CManager::GetInputController();

	if (CFade::GetFade() == CFade::FADE_NONE)
	{//フェードが処理をしていないとき
		if (pInputKeyboard != NULL)
		{// キーボードが存在していたとき
			if (pInputKeyboard->GetTriggerKeyboard(DIK_RETURN))
			{// 指定のキーが押されたとき
				CFade::SetFade(CManager::MODE_GAME);					//フェードを入れる
			}
		}
		if (pInputController->GetJoypadUse(0))
		{// コントローラーが生成されているとき
			//ゲームの遷移
			if (pInputController->GetControllerTrigger(0, JOYPADKEY_A) ||			// ゲームパッドのAボダンが押されたとき
				pInputController->GetControllerTrigger(0, JOYPADKEY_START))			// ゲームパッドのSTARTボタンが押されたとき
			{
				CFade::SetFade(CManager::MODE_GAME);					//フェードを入れる
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CTitle::Draw(void)
{

}

//=============================================================================
// アセットの読み込み
//=============================================================================
void CTitle::LoadAsset(void)
{
	CTitlelogo::Load();
}