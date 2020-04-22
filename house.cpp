//=============================================================================
//
// 家処理 [house.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "house.h"
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "colliderBox.h"
#include "colliderSphere.h"
#include "houseUi.h"
#include "gauge2D.h"
#include "phase.h"
#include "enemyUi.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define HOUSE_LIFE 100								// 家の耐久値

//=============================================================================
// 静的メンバ変数
//=============================================================================
LPD3DXMESH		CHouse::m_pMesh = NULL;				// メッシュ情報のポインタ
DWORD			CHouse::m_nNumMat = 0;				// マテリアル情報の数
LPD3DXBUFFER	CHouse::m_pBuffMat = NULL;			// マテリアル情報のポインタ

//=============================================================================
// コンストラクタ
//=============================================================================
CHouse::CHouse(CScene::PRIORITY obj = CScene::PRIORITY_MODEL) : CObject(obj)
{
	SetObjType(CScene::PRIORITY_MODEL);				// オブジェクトタイプの設定

	m_nLife = HOUSE_LIFE;							// 耐久値の設定

	m_ColliderBox = NULL;							// ボックスコライダーの初期化
	m_ColliderSphere = NULL;						// スフィアコライダーの初期化
	m_pHouseUi = NULL;								// 家Uiの初期化
	m_pPhase = NULL;								// フェーズ数の初期化
	m_pEnemyUi = NULL;								// 敵の残り数の初期化
}

//=============================================================================
// デストラクタ
//=============================================================================
CHouse::~CHouse()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CHouse::Init(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	CSceneX::Init();			// 初期化処理

	// モデルの情報をセット
	BindModel(m_pVtxBuff, m_pMesh, m_nNumMat, m_pBuffMat, NULL);

	D3DXVECTOR3 pos = D3DXVECTOR3(500.0f, 0.0f, 0.0f);

	m_ColliderBox = CColliderBox::Create(false, D3DXVECTOR3(450.0f, 500.0f, 500.0f));		// 生成

	if (m_ColliderBox != NULL)
	{// 立方体の当たり判定が存在していたとき
		m_ColliderBox->SetScene(this);									// 持ち主設定
		m_ColliderBox->SetTag("house");									// タグをhouseに設定
		m_ColliderBox->SetPosition(pos);								// 位置の設定
		m_ColliderBox->SetOffset(D3DXVECTOR3(0.0f, 200.0f, 0.0f));		// オフセットの設定
	}

	m_ColliderSphere = CColliderSphere::Create(true, 1000.0f);								// 生成

	if (m_ColliderSphere != NULL)
	{// 球の当たり判定が存在していたとき
		m_ColliderSphere->SetScene(this);								// 持ち主設定
		m_ColliderSphere->SetTag("house");								// タグをhouseに設定
		m_ColliderSphere->SetPosition(pos);								// 位置の設定
		m_ColliderSphere->SetOffset(D3DXVECTOR3(0.0f, 85.0f, -10.0f));	// オフセットの設定
	}

	SetPosition(pos);			// 位置の設定
	return S_OK;
}

//=============================================================================
// 開放処理
//=============================================================================
void CHouse::Uninit(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void CHouse::Update(void)
{
	D3DXVECTOR3 pos = GetPosition();				// 位置の取得

	if (m_ColliderBox != NULL)
	{// ボックスコライダーが存在していたとき
		m_ColliderBox->SetPosition(pos);			// 位置の設定
	}
	if (m_ColliderSphere != NULL)
	{// スフィアコライダーが存在していたとき
		m_ColliderSphere->SetPosition(pos);			// 位置の設定
	}

	SetPosition(pos);			// 位置の設定
#ifdef _DEBUG
	Debug();					// デバッグ処理
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void CHouse::Draw(void)
{
	// 描画
	CSceneX::Draw();
}

//=============================================================================
// クリエイト処理
//=============================================================================
CHouse *CHouse::Create()
{
	CHouse *pHouse;
	pHouse = new CHouse(CScene::PRIORITY_MODEL);

	if (pHouse != NULL)
	{// 家が存在していたとき
		pHouse->Init();			// 初期化処理
	}
	return pHouse;
}

//=============================================================================
// ロード処理
//=============================================================================
HRESULT CHouse::Load(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスを取得する
	pDevice = pRenderer->GetDevice();

	// Xファイルの読み込み
	D3DXLoadMeshFromX("data/model/house/Grandma'sHouse.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	D3DXMATERIAL	*pMat;							//現在のマテリアル保存用

	// マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// マテリアルのテクスチャ読み込み
	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{// テクスチャがあったとき
			char *pFileName = NULL;
			pFileName = pMat[nCntMat].pTextureFilename;			// テクスチャのアドレスを取得
			CManager::Load(pFileName);		// テクスチャの読み込み
		}
	}
	return S_OK;
}

//=============================================================================
// 当たり判定(trigger)
//=============================================================================
void CHouse::OnTriggerEnter(CCollider *col)
{
	std::string sTag = col->GetTag();		// タグの取得

	if (col->GetScene()->GetObjType() == PRIORITY_PLAYER)
	{// 当たったオブジェクトがプレイヤーだったとき
		if (sTag == "player")
		{
			if (m_pHouseUi == NULL)
			{// おばあちゃん家の耐久値UIの生成されていなかったとき
				m_pHouseUi = CHouseUi::Create();			// おばあちゃん家の耐久値UIの生成

				if (m_pHouseUi != NULL)
				{// おばあちゃん家の耐久値UIの生成されていたとき
					m_pHouseUi->SetPosition(D3DXVECTOR3(SCREEN_WIDTH / 2, 50.0f, 0.0f));	// 位置の設定
					m_pHouseUi->SetSize(D3DXVECTOR3(450.0f, 80.0f, 0.0f));					// サイズの設定
					m_pHouseUi->SetTransform();				// 頂点情報の更新
				}

				m_ColliderSphere->SetRadius(350.0f);		// 当たり判定の半径の大きさ変更
				m_ColliderSphere->SetTrigger(false);		// 当たり判定の種類を変更
				m_ColliderSphere->SetMoving(false);			// 対象の移動を禁止する
			}

			if (m_pPhase == NULL)
			{// 現在のphase数のUIが生成されていなかったとき
				m_pPhase = CPhase::Create();				// phase数のUIを生成

				if (m_pPhase != NULL)
				{
					m_pPhase->SetPosition(D3DXVECTOR3(120, 50.0f, 0.0f));			// 位置の設定
					m_pPhase->SetSize(D3DXVECTOR3(100.0f, 50.0f, 0.0f));			// サイズの設定
					m_pPhase->SetTransform();				// 頂点情報の更新
				}
			}

			if (m_pEnemyUi == NULL)
			{// 現在のphase数のUIが生成されていなかったとき
				m_pEnemyUi = CEnemyUi::Create();				// 敵の残り数のUIを生成

				if (m_pEnemyUi != NULL)
				{
					m_pEnemyUi->SetPosition(D3DXVECTOR3(120, 150.0f, 0.0f));		// 位置の設定
					m_pEnemyUi->SetSize(D3DXVECTOR3(100.0f, 50.0f, 0.0f));			// サイズの設定
					m_pEnemyUi->SetTransform();				// 頂点情報の更新
				}
			}
		}
	}

	else if (col->GetScene()->GetObjType() == PRIORITY_ENEMY)
	{// 当たったオブジェクトが敵だったとき
		if (sTag == "weapon")
		{// タグがweaponだったとき
			m_nLife--;			// 耐久値を-1する

			if (m_pHouseUi != NULL)
			{// 家の耐久値ゲージが存在していたとき
				CGauge2D *pHitPoint = m_pHouseUi->GetGauge();				// ゲージの取得
				pHitPoint->SetLifeBar(false, (float)m_nLife / 100.0f);		// ゲージの更新
			}
		}
	}
}

//=============================================================================
// 当たり判定(collision)
//=============================================================================
void CHouse::OnCollisionEnter(CCollider *col)
{
	std::string sTag = col->GetTag();		// タグを取得

	if (col->GetScene()->GetObjType() == PRIORITY_ENEMY)
	{// 当たったオブジェクトが敵だったとき
		if (sTag == "weapon")
		{// タグがweaponだったとき
			m_nLife--;			// 耐久値を-1する

			if (m_pHouseUi != NULL)
			{// 家の耐久値ゲージが存在していたとき
				CGauge2D *pHitPoint = m_pHouseUi->GetGauge();				// ゲージの取得
				pHitPoint->SetLifeBar(false, (float)m_nLife / 100.0f);		// ゲージの更新
			}
		}
	}
}

#ifdef _DEBUG
//=============================================================================
// デバッグ処理
//=============================================================================
void CHouse::Debug(void)
{

}
#endif