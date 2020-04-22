//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "player.h"
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "inputKeyboard.h"
#include "inputController.h"
#include "scene3D.h"
#include "meshField.h"
#include "model.h"
#include "meshCapsule.h"
#include "colliderSphere.h"
#include "meshCube.h"
#include "enemy.h"
#include "meshOrbit.h"
#include "camera.h"
#include "colliderBox.h"
#include "stage.h"
#include "sound.h"
#include "scene2D.h"
#include "effect.h"
#include "playerUi.h"
#include "gauge2D.h"
#include "purpose.h"
#include "messageWindow.h"
#include "result.h"
#include "fade.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define	SCRIPT_PLAYER "data/animation/girl.txt"		// 赤ずきんのモデル情報アドレス
#define COUNTER_COMBO 30							// 派生攻撃受付カウンタ
#define JUMP_MAX 10									// ジャンプの加速度

//=============================================================================
// コンストラクタ
//=============================================================================
CPlayer::CPlayer(CScene::PRIORITY obj = CScene::PRIORITY_PLAYER) : CScene(obj)
{
	// 優先度の設定
	SetObjType(CScene::PRIORITY_PLAYER);				// オブジェクトタイプ

	m_nLife = 100;										// 体力の初期化
	m_fSpeed = NORMAL_SPEED;							// スピードの初期化
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 回転の初期化
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 移動量の初期化
	m_dest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 移動先の初期化
	m_difference = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 差の初期化
	m_nNumParts = 0;									// パーツ総数の初期化
	m_nCurrentFrame = 0;								// フレーム数の初期化
	m_nCurrentKey = 0;									// キー数の初期化
	m_pColPlayerSphere = NULL;							// プレイヤー当たり判定ポインタの初期化
	m_pColWeaponSphere = NULL;							// 刃当たり判定ポインタの初期化
	m_pColHandSphere = NULL;							// 手の当たり判定の初期化
	m_pBox = NULL;
	m_bAnimSwitch = false;								// アニメーションスイッチの初期化
	m_bJump = false;									// ジャンプフラグの初期化
	m_nCntAttacCombo = COUNTER_COMBO;					// 攻撃派生カウンタの初期化
	m_AttackTypeOld = ANIMATIONTYPE_ATTACK_1;			// 前の攻撃タイプを初期化
	m_AnimationTypeOld = ANIMATIONTYPE_NONE;			// 前のアニメーションタイプを初期化
	m_bAnimation = true;								// アニメーションフラグの初期化
	m_state = PLAYERSTATE_NORMAL;						// プレイヤーステータスの初期化
	m_nActionCount = 0;									// アクションカウンタの初期化
	m_nParticleCount = 0;								// パーティクルカウンタの初期化
	m_fDeathblow = 0.0f;								// 必殺技ポイントの初期化
	m_bEvent = false;									// イベント発生フラグの初期化

	// アニメーション情報の初期化
	for (int nCount = 0; nCount < ANIMATIONTYPE_MAX; nCount++)
	{
		m_pAnimation[nCount].apKeyInfo = NULL;			// アニメーション情報の初期化
	}

	m_apCurrentPos = NULL;								// 現在位置 の初期化
	m_apCurrentRot = NULL;								// 現在回転 の初期化
	m_pModel = NULL;									// モデルポインタ の初期化
	m_pPlayerUi = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPlayer::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	CRenderer *pRenderer = CManager::GetRenderer();

	//デバイスを取得する
	pDevice = pRenderer->GetDevice();

	CCamera *pCamera = CManager::GetCamera();
	D3DXVECTOR3 pos = GetPosition();						//プレイヤーの位置取得

	pos = D3DXVECTOR3(9276.96f, 10.0f, -9218.10f);			// プレイヤーの位置設定

	if (pCamera != NULL)
	{
		pCamera->SetPosCamera(pos, D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	}

	m_AnimationType = ANIMATIONTYPE_NEUTRAL;				// アニメーションタイプ の初期化

	// プレイヤーモデル情報の読み込み
	LoadScript();

	// 武器の当たり判定を生成
	m_pColWeaponSphere = CColliderSphere::Create(true, 30.0f);

	if (m_pColWeaponSphere != NULL)
	{ // 球体のポインタがNULLではないとき
		m_pColWeaponSphere->SetScene(this);											// 呼び出し主 の設定
		m_pColWeaponSphere->SetTag("weapon");										// タグ の設定
	}

	// 武器の当たり判定を生成
	m_pColHandSphere = CColliderSphere::Create(true, 50.0f);

	if (m_pColHandSphere != NULL)
	{ // 球体のポインタがNULLではないとき
		m_pColHandSphere->SetScene(this);											// 呼び出し主 の設定
		m_pColHandSphere->SetTag("weapon");											// タグ の設定
	}

	// プレイヤーの当たり判定を生成
	m_pColPlayerSphere = CColliderSphere::Create(false, 20.0f);

	if (m_pColPlayerSphere != NULL)
	{ //球体のポインタがNULLではないとき
		m_pColPlayerSphere->SetScene(this);
		m_pColPlayerSphere->SetTag("player");										// タグ の設定
		m_pColPlayerSphere->SetPosition(pos);										// 位置 の設定
		m_pColPlayerSphere->SetOffset(D3DXVECTOR3(0.0f, 100.0f, 0.0f));
	}

	SetPosition(pos);

	// 軌跡の生成
	m_pMeshOrbit = CMeshOrbit::Create();

	m_pPlayerUi = CPlayerUi::Create();

	return S_OK;
}

//=============================================================================
// 開放処理
//=============================================================================
void CPlayer::Uninit(void)
{
	if (m_pColWeaponSphere != NULL)
	{// 武器の当たり判定が存在していたとき
		if (m_pColWeaponSphere != NULL)
		{
			m_pColWeaponSphere->Release();
		}
	}

	if (m_pColPlayerSphere != NULL)
	{// 武器の当たり判定が存在していたとき
		m_pColPlayerSphere->Release();
	}

	if (m_apCurrentPos != NULL)
	{// パーツの1フレームごとの移動量を格納する変数がNULLじゃなかったとき
		delete[] m_apCurrentPos;
		m_apCurrentPos = NULL;
	}

	if (m_apCurrentRot != NULL)
	{// パーツの1フレームごとの回転量を格納する変数がNULLじゃなかったとき
		delete[] m_apCurrentRot;
		m_apCurrentRot = NULL;
	}

	if (m_pModel != NULL)
	{//パーツが存在する場合
		for (int nCount = 0; nCount < m_nNumParts; nCount++)
		{
			m_pModel[nCount].Uninit();								// 開放処理
		}

		delete[] m_pModel;											// パーツの総数分のメモリを開放する
		m_pModel = NULL;											// パーツのアドレスにNULLを入れる
	}

	//アニメーション分回す
	for (int nCount = 0; nCount < ANIMATIONTYPE_MAX; nCount++)
	{
		if (m_pAnimation[nCount].apKeyInfo != NULL)
		{//NULLではないとき
			delete[] m_pAnimation[nCount].apKeyInfo;										//パーツの総数分メモリを取得する
			m_pAnimation[nCount].apKeyInfo = NULL;											//パーツ分のアドレスにNULLを入れる
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CPlayer::Update(void)
{
	D3DXVECTOR3 pos;
	CSound *pSound = CManager::GetSound();
	float fHeight = 0.0f;

	if (!m_bEvent)
	{// イベントが発生していなかったとき
		// 入力処理
		Input();
	}

	pos = GetPosition();				// 位置の取得

	//床の高さを取得する
	CScene *pSceneNext = NULL;														// 初期化
	CScene *pSceneNow = GetScene(PRIORITY_FLOOR);									// シーンの先頭アドレスを取得

	CScene *pScene = NowFloor(pos);													// 現在いるフィールドを取得

	if (pScene != NULL)
	{// 今立っている床が存在したとき
		CMeshField *pFloor = (CMeshField*)pScene;									// キャスト
		fHeight = pFloor->GetHeight(pos);

		RANDTYPE Type = pFloor->GetRandType();

		if (m_AnimationType == ANIMATIONTYPE_WALK || m_AnimationType == ANIMATIONTYPE_RUN)
		{
			if (m_nCurrentKey == 5 || m_nCurrentKey == 0)
			{
				if (m_nCurrentFrame == 0)
				{
					if (Type == RANDTYPE_GRASS)
					{
						pSound->PlaySoundA((SOUND_LABEL)(CManager::GetRand(3) + (int)SOUND_LABEL_SE_GRASS_1));
					}
					else if (Type == RANDTYPE_SAND)
					{
						pSound->PlaySoundA((SOUND_LABEL)(CManager::GetRand(3) + (int)SOUND_LABEL_SE_SAND_1));
					}
				}
			}
		}
	}

	if (m_bEvent)
	{
		if (m_AnimationTypeOld == ANIMATIONTYPE_SWITCHWEAPON)
		{
			if (m_state == PLAYERSTATE_FLOWER)
			{
				if (m_nActionCount == 30)
				{
					CRenderer *pRenderer = CManager::GetRenderer();
					CCamera *pCamera = CManager::GetCamera();
					pRenderer->SetUpdate(false);
					pCamera->SetRotMotion(D3DXVECTOR3(0.0f, D3DX_PI * 2, 0.0f), D3DXVECTOR3(0.0f, 0.1f, 0.0f));
					m_nActionCount++;
				}
				else if (m_nActionCount == 31)
				{
					m_bEvent = false;
				}
				else
				{
					m_nActionCount++;
				}
			}
		}
	}
	else
	{
		// 今の状態が暴走状態だったとき
		if (m_state == PLAYERSTATE_FLOWER)
		{// 暴走状態だったとき
			if (m_nParticleCount > 15)
			{// カウンタが15以上だったとき
				for (int nCount = 0; nCount < 3; nCount++)
				{
					// エフェクトの生成処理
					CEffect::WingStay(D3DXVECTOR3(pos.x, pos.y + 100.0f, pos.z));
					CEffect::PetalsSoaring(pos);
				}

				m_nParticleCount = 0;			// カウンタのリセット
				m_nActionCount = 0;				// カウンタのリセット
			}
			else
			{
				m_nParticleCount++;				// カウンタに1プラスする
			}

			CEffect::Aura(m_nActionCount, m_bJump, pos);		// オーラエフェクトの生成
			m_nActionCount++;					// カウンタに1プラスする

			m_fDeathblow -= 0.1f;
			CGauge2D *pGauge2D = m_pPlayerUi->GetGauge(GAUGETYPE_DEATHBLOW);
			pGauge2D->SetLifeBar(false, (float)m_fDeathblow / 50.0f);				// ゲージの更新

			if (m_fDeathblow <= 0.0f)
			{// 必殺技ポイントが0以下だったとき
				if (m_state == PLAYERSTATE_FLOWER)
				{// イベント発生フラグが立っていなかったとき
					if (!m_bEvent)
					{
						AnimationSwitch(ANIMATIONTYPE_SWITCHWEAPON);			// アニメーションの変更
						m_nActionCount = 0;			// カウンタの初期化
						m_bEvent = true;
					}
				}
			}
		}
	}

	if (m_bAnimation)
	{// アニメーション再生
		// アニメーション更新処理
		Animation();
	}

	//角度
	if (m_AnimationType != ANIMATIONTYPE_ATTACK_4)
	{
		if (!m_bJump)
		{
			m_dest.y = atan2f(-m_move.x, -m_move.z);
		}
	}

	// 当たり判定管理処理
	Collision();

	// 位置更新
	pos += m_move;

	if (!m_bJump)
	{
		// 減速
		m_move.x += (0 - m_move.x) * 0.12f;
		m_move.z += (0 - m_move.z) * 0.12f;
	}

	//重力処理
	if (m_bJump)
	{// ジャンプしていたとき
		if (m_AnimationType != ANIMATIONTYPE_JUMP_1)
		{
			if (pos.y > fHeight)
			{// 現在の高さが床より高かったとき
				m_move.y += -0.7f;
			}
			else
			{// 現在の高さが床より低かった時
				pos.y = fHeight;											// 床の高さを求める
				m_move.y = 0.0f;
				if (m_bJump)
				{
					// 砂煙のエフェクト表現
					CEffect::SandSmokeEffect(pos);

					m_bJump = false;										// ジャンプ判定を変える
					m_bAnimation = true;									// アニメーションの再開
					AnimationSwitch(ANIMATIONTYPE_JUMP_5);					// アニメーションの変更
					pSound->PlaySoundA(SOUND_LABEL_SE_LANDING);				// 着地音の再生
				}
			}
		}
	}
	else
	{
		pos.y = fHeight;											// 床の高さを求める
	}

	// エフェクトの生成
	if (m_AnimationType == ANIMATIONTYPE_ATTACK_5)
	{
		if (m_nCurrentKey == 1)
		{
			if (m_nCurrentFrame > 0)
			{
				CEffect::PetalCluster(D3DXVECTOR3(m_pModel[4].GetMtxWorld()._41, m_pModel[4].GetMtxWorld()._42, m_pModel[4].GetMtxWorld()._43), m_rot);
			}
		}
	}

	//球体のポインタがNULLではないとき
	if (m_pColWeaponSphere != NULL)
	{
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 40.0f, -80.0f);
		D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXMATRIX	mtxRot, mtxTrans, mtxView, mtxMeshRot, mtxMeshTrans;				//計算用マトリックス
		D3DXMATRIX mtx;			// 武器のマトリックス

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&mtx);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, rot.y, rot.x, rot.z);
		D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxMeshTrans, pos.x, pos.y, pos.z);
		D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshTrans);

		D3DXMatrixMultiply(&mtx, &mtx, &m_pModel[14].GetMtxWorld());

		m_pColWeaponSphere->SetPosition(D3DXVECTOR3(mtx._41, mtx._42, mtx._43));
	}

	if (m_pColHandSphere != NULL)
	{// 手の当たり判定が存在していたとき
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, -50.0f, 0.0f);
		D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXMATRIX	mtxRot, mtxTrans, mtxView, mtxMeshRot, mtxMeshTrans;				//計算用マトリックス
		D3DXMATRIX mtx;			// 武器のマトリックス

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&mtx);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, rot.y, rot.x, rot.z);
		D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxMeshTrans, pos.x, pos.y, pos.z);
		D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshTrans);

		D3DXMatrixMultiply(&mtx, &mtx, &m_pModel[4].GetMtxWorld());

		m_pColHandSphere->SetPosition(D3DXVECTOR3(mtx._41, mtx._42, mtx._43));
	}

	//球体のポインタがNULLではないとき
	if (m_pColPlayerSphere != NULL)
	{
		m_pColPlayerSphere->SetPosition(pos);		// 現在位置 の更新
	}

	SetPosition(pos);

	if (m_pBox != NULL)
	{
		m_pBox->SetPosition(pos);
	}

	//キャラクターの振り向き
	m_difference.y = m_rot.y - m_dest.y;

	//ディファレンスの値が指定した値
	if (m_difference.y > D3DX_PI)
	{
		m_difference.y -= D3DX_PI * 2;
	}
	else if (m_difference.y < -D3DX_PI)
	{
		m_difference.y += D3DX_PI * 2;
	}

	// 1フレームあたりの回転量を代入
	m_rot.y -= m_difference.y * 0.1f;

	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2;
	}
	else if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2;
	}

	// 武器出現エフェクト処理
	if (m_AnimationType == ANIMATIONTYPE_SWITCHWEAPON)
	{// 前のアニメーションが武器の切り替えだったとき
		if (!m_bAnimation)
		{// 現在アニメーションをしていなかったとき
			m_nActionCount++;				// カウンタに+1する

			if (m_nActionCount > 50)
			{// カウンタが50以上だったとき
				m_pModel[14].SetActive(true);		// 武器の更新を許可する
				m_bAnimation = true;				// アニメーションの再開
				AnimationSwitch(ANIMATIONTYPE_NEUTRAL);	// 待機モーションに設定
				m_nActionCount = 0;					// カウンタをリセット

				int nNumVertices;					//頂点数
				DWORD sizeFVF;						//頂点フォーマット
				BYTE	*pVertexBuffer;				//頂点バッファへのポインタ
				LPD3DXMESH pMesh = m_pModel[14].GetMesh();		// メッシュ情報の取得

				//頂点数を取得
				nNumVertices = pMesh->GetNumVertices();

				//頂点フォーマットのサイズを取得
				sizeFVF = D3DXGetFVFVertexSize(pMesh->GetFVF());

				//頂点バッファをロック
				pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertexBuffer);

				// パーティクル生成
				for (int nCount = 0; nCount < nNumVertices; nCount++)
				{
					D3DXVECTOR3	vtx = *(D3DXVECTOR3*)pVertexBuffer;

					if (nCount % 50 == 0)
					{// 50回に1回生成する
						D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						D3DXMATRIX	mtxRot, mtxTrans, mtxView, mtxMeshRot, mtxMeshTrans;				//計算用マトリックス
						D3DXMATRIX mtx;			// 武器のマトリックス
						D3DXVECTOR3 pos = GetPosition();

						// ワールドマトリックスの初期化
						D3DXMatrixIdentity(&mtx);

						// 回転を反映
						D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, rot.y, rot.x, rot.z);
						D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshRot);

						// 位置を反映
						D3DXMatrixTranslation(&mtxMeshTrans, vtx.x, vtx.y, vtx.z);
						D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshTrans);

						// マトリックス情報の合成
						D3DXMatrixMultiply(&mtx, &mtx, &m_pModel[14].GetMtxWorld());

						// エフェクトの生成
						CEffect::FallingPetals(false, D3DXVECTOR3(mtx._41, mtx._42, mtx._43), 50);
					}

					pVertexBuffer += sizeFVF;			//サイズ分のポインタを進める
				}

				//頂点バッファをアンロック
				pMesh->UnlockVertexBuffer();
			}
		}
	}

#ifdef _DEBUG
	Debug();
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void CPlayer::Draw(void)
{
	D3DXVECTOR3 pos = GetPosition();
	D3DXMATRIX		mtxRot, mtxTrans;				//計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	for (int nCount = 0; nCount < m_nNumParts; nCount++)
	{
		if (m_pModel != NULL)
		{//パーツが存在する場合
			m_pModel[nCount].Draw(&m_mtxWorld);							//描画
		}
	}
}

//=============================================================================
// クリエイト処理
//=============================================================================
CPlayer *CPlayer::Create(void)
{
	CPlayer *pPlayer;
	pPlayer = new CPlayer(CScene::PRIORITY_PLAYER);
	pPlayer->Init();
	return pPlayer;
}

//=============================================================================
// ロード処理
//=============================================================================
HRESULT CPlayer::Load(void)
{
	return S_OK;
}

//=============================================================================
// 必殺技ポイント数の設定
//=============================================================================
void CPlayer::SetDeathblow(float nValue)
{
	m_fDeathblow = nValue;
}

//=============================================================================
// 当たり判定(trigger)
//=============================================================================
void CPlayer::OnTriggerEnter(CCollider *col)
{
	std::string sTag = col->GetTag();

	if (col->GetScene()->GetObjType() == PRIORITY_ENEMY)
	{
		if (sTag == "weapon")
		{
			CSound *pSound = CManager::GetSound();				// サウンドの取得
			pSound->PlaySoundA(SOUND_LABEL_SE_PUNCH);			// ダメージ音の再生
			m_nLife -= 5;										// 体力を削る
			AnimationSwitch(ANIMATIONTYPE_DAMAGE);				// アニメーションを変更

			D3DXVECTOR3 vec;

			vec = GetPosition() - col->GetPosition();			//差分を求める(方向を求めるため)
			D3DXVec3Normalize(&vec, &vec);						//正規化する

			m_move.x = vec.x * 25;
			m_move.z = vec.z * 25;

			if (m_pPlayerUi != NULL)
			{
				CGauge2D *pHitPoint = m_pPlayerUi->GetGauge(GAUGETYPE_HP);
				pHitPoint->SetLifeBar(false, (float)m_nLife / 100.0f);
			}

			if (m_nLife < 0)
			{
				CResult::SetIdxKill(CEnemy::GetEnemyKill());			// Kill数をリザルトに渡す
				CFade::SetFade(CManager::MODE_RESULT);					// リザルトに遷移
			}
		}

		if (sTag == "enemy")
		{
			if (m_AnimationType != ANIMATIONTYPE_ATTACK_5)
			{
				D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 40.0f, -80.0f);
				D3DXMATRIX	mtxRot, mtxTrans, mtxView, mtxMeshRot, mtxMeshTrans;				//計算用マトリックス
				D3DXMATRIX mtx;			// 武器のマトリックス

										// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&mtx);

				// 位置を反映
				D3DXMatrixTranslation(&mtxMeshTrans, pos.x, pos.y, pos.z);
				D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshTrans);

				D3DXMatrixMultiply(&mtx, &mtx, &m_pModel[14].GetMtxWorld());

				for (int nCount = 0; nCount < 20; nCount++)
				{
					CEffect::FallingPetals(true, D3DXVECTOR3(mtx._41, mtx._42, mtx._43), 150);
				}
			}
		}
	}
	if (sTag == "house")
	{
		CPurpose *pPurpose = CGame::GetPurpose();				// 目的用看板の取得

		if (pPurpose->GetPurpose() != MESSAGETYPE_HOUSEGUARD)
		{// 現在の目的が[防衛]ではないとき
			pPurpose->ChangePurpose(MESSAGETYPE_HOUSEGUARD);	// メッセージの設定
			CStage::SetPhase();									// メッセージの変更
		}
	}
}

//=============================================================================
// 当たり判定(collider)
//=============================================================================
void CPlayer::OnCollisionEnter(CCollider *col)
{
	std::string sTag = col->GetTag();
}

//========================================================================================
// パーツのロード
//========================================================================================
void CPlayer::LoadScript(void)
{
	FILE *pFile;
	char cReadText[128];		//文字
	char cHeadText[128];		//比較
	char cDie[128];
	int nCntPointer = 0;			//ポインターの数値

	char sAdd[64];											//モデルのアドレス
	int nIndex;
	int nParent;

	int nCntMotion = 0;												//参照するポインタの値を初期化
	int nCntKey = 0;

	int nMaxModel = 0;

	int nCntMaya = 0;				//mayaのデータ参照

	//テキストデータロード
	pFile = fopen(SCRIPT_PLAYER, "r");

	if (pFile != NULL)
	{
		//ポインターのリセット
		nCntPointer = 0;

		//スクリプトが来るまでループ
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			fgets(cReadText, sizeof(cReadText), pFile);
			sscanf(cReadText, "%s", &cHeadText);
		}

		//スクリプトだったら
		if (strcmp(cHeadText, "SCRIPT") == 0)
		{
			//エンドスクリプトが来るまで
			while (strcmp(cHeadText, "END_SCRIPT") != 0)
			{
				fgets(cReadText, sizeof(cReadText), pFile);
				sscanf(cReadText, "%s", &cHeadText);

				//改行
				if (strcmp(cReadText, "\n") != 0)
				{
					if (strcmp(cHeadText, "NUM_MODEL") == 0)
					{//パーツ総数のとき
						if (m_pModel == NULL)
						{
							sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nMaxModel);					//パーツの数を取得

							const int nNumParts = nMaxModel;											//パーツの数を定数として定義

							if (m_pModel == NULL)
							{//モデルを格納する変数がNULLだったとき
								m_pModel = new CModel[nNumParts];											//パーツの総数分メモリを取得する
							}

							if (m_apCurrentPos == NULL)
							{// パーツの1フレームごとの移動量を格納する変数がNULLだったとき
								m_apCurrentPos = new D3DXVECTOR3[nNumParts];
							}

							if (m_apCurrentRot == NULL)
							{// パーツの1フレームごとの回転量を格納する変数がNULLだったとき
								m_apCurrentRot = new D3DXVECTOR3[nNumParts];
							}

							for (int nCount = 0; nCount < ANIMATIONTYPE_MAX; nCount++)
							{
								if (m_pAnimation[nCount].apKeyInfo == NULL)
								{
									m_pAnimation[nCount].apKeyInfo = new KEY_INFO[nNumParts];										//パーツの総数分メモリを取得する
								}
							}

							m_nNumParts = nNumParts;													//パーツの総数を格納する
						}
					}
					else if (strcmp(cHeadText, "MODEL_FILENAME") == 0)
					{//パーツモデルのアドレス情報のとき
						sscanf(cReadText, "%s %s %s", &cDie, &cDie, &sAdd[0]);						//アドレスの取得

						if (m_pModel != NULL)
						{
							m_pModel[nCntPointer].Load(sAdd);										//パーツモデルのロード
							m_pModel[nCntPointer].Init();											//パーツモデルの初期化
							nCntPointer++;																//ポインターを4Bプラス
						}
					}

					else if (strcmp(cHeadText, "CHARACTERSET") == 0)
					{//キャラクターの初期情報のとき
						nCntPointer = 0;															//参照するポインタの値を初期化

						//エンドキャラクターセットが来るまでループ
						while (strcmp(cHeadText, "END_CHARACTERSET") != 0)
						{
							fgets(cReadText, sizeof(cReadText), pFile);
							sscanf(cReadText, "%s", &cHeadText);

							//パーツセットだったら
							if (strcmp(cHeadText, "PARTSSET") == 0)
							{
								//エンドパーツセットが来るまでループ
								while (strcmp(cHeadText, "END_PARTSSET") != 0)
								{
									fgets(cReadText, sizeof(cReadText), pFile);
									sscanf(cReadText, "%s", &cHeadText);

									//インデックス
									if (strcmp(cHeadText, "INDEX") == 0)
									{
										sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nIndex);			//インデックスの値を取得
										m_pModel[nCntPointer].SetIndex(nIndex);							//インデックスの値を現在参照中のパーツモデルに格納
									}

									//親
									if (strcmp(cHeadText, "PARENT") == 0)
									{
										sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nParent);			//親の値を取得

										if (nParent == -1)
										{//親の値が-1だったとき
											m_pModel[nCntPointer].SetParent(NULL);				//NULLを格納する
										}
										else
										{//親の値が-1ではないとき
											if (m_pModel != NULL)
											{//モデルの中身がNULLではないとき
												//現在ロードされているモデル分の中から対象が見つかるまで回す
												for (int nCount = 0; nCount < nMaxModel; nCount++)
												{
													if (nParent == m_pModel[nCount].GetIndex())
													{//パーツモデルのインデック値と自分の親の値が一致したとき
														m_pModel[nCntPointer].SetParent(&m_pModel[nCount]);				//親のポインタを格納する
														break;
													}
												}
											}
										}
									}

									if (strcmp(cHeadText, "POS") == 0)
									{//位置
										D3DXVECTOR3 originPos;
										sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
											&originPos.x,
											&originPos.y,
											&originPos.z);

										m_pModel[nCntPointer].SetPosPreset(originPos);
										m_pModel[nCntPointer].SetPosition(originPos);
									}

									if (strcmp(cHeadText, "ROT") == 0)
									{//回転
										D3DXVECTOR3 originRot;
										sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
											&originRot.x,
											&originRot.y,
											&originRot.z);

										m_pModel[nCntPointer].SetRotPreset(originRot);
										m_pModel[nCntPointer].SetRotation(originRot);
									}
								}

								nCntPointer++;											//参照するポインタの値を進める
							}
						}
					}
					else if (strcmp(cHeadText, "MOTIONSET") == 0)
					{//キャラクターセットだったら

						//エンドキャラクターセットが来るまでループ
						while (strcmp(cHeadText, "END_MOTIONSET") != 0)
						{
							fgets(cReadText, sizeof(cReadText), pFile);
							sscanf(cReadText, "%s", &cHeadText);

							if (strcmp(cHeadText, "LOOP") == 0)
							{//ループするかどうか
								sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_pAnimation[nCntMotion].nLoop);
							}
							else if (strcmp(cHeadText, "NUM_KEY") == 0)
							{//キー数
								sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_pAnimation[nCntMotion].nMaxKey);
							}
							else if (strcmp(cHeadText, "SWITCHFRAME") == 0)
							{// フレーム間の移動フレーム数
								sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_pAnimation[nCntMotion].nSwitchFrame);
							}

							if (strcmp(cHeadText, "KEYSET") == 0)
							{//キーセットだったら

								//参照するポインタの値を初期化
								nCntPointer = 0;
								//エンドキーセットが来るまでループ
								while (strcmp(cHeadText, "END_KEYSET") != 0)
								{
									fgets(cReadText, sizeof(cReadText), pFile);
									sscanf(cReadText, "%s", &cHeadText);

									//フレーム数
									if (strcmp(cHeadText, "FRAME") == 0)
									{
										sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_pAnimation[nCntMotion].apKeyInfo[nCntKey].nFrame);
									}

									//キーだったら
									if (strcmp(cHeadText, "KEY") == 0)
									{
										//エンドキーが来るまでループ
										while (strcmp(cHeadText, "END_KEY") != 0)
										{
											fgets(cReadText, sizeof(cReadText), pFile);
											sscanf(cReadText, "%s", &cHeadText);

											//位置
											if (strcmp(cHeadText, "POS") == 0)
											{
												sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
													&m_pAnimation[nCntMotion].apKeyInfo[nCntPointer].aKey[nCntKey].pos.x,
													&m_pAnimation[nCntMotion].apKeyInfo[nCntPointer].aKey[nCntKey].pos.y,
													&m_pAnimation[nCntMotion].apKeyInfo[nCntPointer].aKey[nCntKey].pos.z);
											}

											//回転
											if (strcmp(cHeadText, "ROT") == 0)
											{
												sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
													&m_pAnimation[nCntMotion].apKeyInfo[nCntPointer].aKey[nCntKey].rot.x,
													&m_pAnimation[nCntMotion].apKeyInfo[nCntPointer].aKey[nCntKey].rot.y,
													&m_pAnimation[nCntMotion].apKeyInfo[nCntPointer].aKey[nCntKey].rot.z);
											}
										}

										//パーツのキー情報を格納する場所を進める
										nCntPointer++;
									}
								}

								//keyのカウンターに1プラス
								nCntKey++;
								nCntPointer = 0;
							}
						}

						//アニメーションの種類を1プラスする
						nCntMotion++;
						nCntKey = 0;
					}
				}
			}
		}

		//ファイル閉
		fclose(pFile);
	}
	else
	{

	}
}

//=============================================================================
// 近くにいる敵の近くまで移動する処理
//=============================================================================
void CPlayer::MoveNearEnemy(void)
{
	bool bTarget = false;				// 範囲内に敵が存在していたとき
	float fMinDistance = 750000.0f;		// 現在の最短距離
	float fDistance = 0.0f;				// 現在の距離
	D3DXVECTOR3 enemyPos;				// 最短距離にいる敵の位置
	CScene *pSceneNext = NULL;														// 初期化
	CScene *pSceneNow = CScene::GetScene(CScene::PRIORITY_ENEMY);					// 先頭アドレスの取得

																					// 次がなくなるまで繰り返す
	while (pSceneNow != NULL)
	{
		pSceneNext = CScene::GetSceneNext(pSceneNow, CScene::PRIORITY_ENEMY);		//次回アップデート対象を控える
		fDistance = CManager::GetDistance(GetPosition(), pSceneNow->GetPosition());		// 距離を求める

		if (fDistance < fMinDistance)
		{
			if (!bTarget)
			{// ターゲットの存在が確認されていなかったとき
				bTarget = true;				// 位置の移動を許可する
			}

			enemyPos = pSceneNow->GetPosition();			// 敵の位置を記録する
		}

		pSceneNow = pSceneNext;								//次回アップデート対象を格納
	}

	if (bTarget)
	{// 移動の許可が出ていたとき
		SetPosition(enemyPos);			// 位置の移動
	}
}

//=============================================================================
// 当たり判定処理
//=============================================================================
void CPlayer::Collision(void)
{
	switch (m_AnimationType)
	{
	case ANIMATIONTYPE_NEUTRAL:					// 待機モーションのとき
		if (m_pColWeaponSphere != NULL)
		{// 武器の当たり判定が存在していたとき
			if (m_pColWeaponSphere->GetUse())
			{// 当たり判定の対象だったとき
				m_pColWeaponSphere->SetUse(false);		// 対象から外す
			}
			if (m_pColHandSphere->GetUse())
			{
				m_pColHandSphere->SetUse(false);
			}
		}
		if (m_pMeshOrbit != NULL)
		{// 軌跡が存在していたとき
			if (m_pMeshOrbit->GetOrbit())
			{
				m_pMeshOrbit->SetOrbit(false);
			}
		}
		break;
	case ANIMATIONTYPE_WALK:						// 歩くモーションのとき
		if (m_pColWeaponSphere != NULL)
		{// 武器の当たり判定が存在していたとき
			if (m_pColWeaponSphere->GetUse())
			{// 当たり判定の対象だったとき
				m_pColWeaponSphere->SetUse(false);		// 対象から外す
			}
			if (m_pColHandSphere->GetUse())
			{
				m_pColHandSphere->SetUse(false);
			}
		}
		if (m_pMeshOrbit != NULL)
		{// 軌跡が存在していたとき
			if (m_pMeshOrbit->GetOrbit())
			{
				m_pMeshOrbit->SetOrbit(false);
			}
		}
		break;
	case ANIMATIONTYPE_ATTACK_1:				// 攻撃モーションのとき
		if (m_pColWeaponSphere != NULL)
		{// 武器の当たり判定が存在していたとき
			if (!m_pColWeaponSphere->GetUse())
			{// 当たり判定の対象外だったとき
				m_pColWeaponSphere->SetUse(true);		// 対象にする
			}
		}
		if (m_pMeshOrbit != NULL)
		{// 軌跡が存在していたとき
			if (!m_pMeshOrbit->GetOrbit())
			{
				m_pMeshOrbit->SetOrbit(true);
			}
		}
		break;
	case ANIMATIONTYPE_ATTACK_2:				// 攻撃モーションのとき
		if (m_pColWeaponSphere != NULL)
		{// 武器の当たり判定が存在していたとき
			if (!m_pColWeaponSphere->GetUse())
			{// 当たり判定の対象外だったとき
				m_pColWeaponSphere->SetUse(true);		// 対象にする
			}
		}
		if (m_pMeshOrbit != NULL)
		{// 軌跡が存在していたとき
			if (!m_pMeshOrbit->GetOrbit())
			{// 軌跡の更新が停止していたとき
				m_pMeshOrbit->SetOrbit(true);
			}
		}
		break;
	case ANIMATIONTYPE_ATTACK_3:				// 攻撃モーションのとき
		if (m_pColWeaponSphere != NULL)
		{// 武器の当たり判定が存在していたとき
			if (!m_pColWeaponSphere->GetUse())
			{// 当たり判定の対象外だったとき
				m_pColWeaponSphere->SetUse(true);		// 対象にする
			}
		}
		if (m_pMeshOrbit != NULL)
		{// 軌跡が存在していたとき
			if (!m_pMeshOrbit->GetOrbit())
			{// 軌跡の更新が停止していたとき
				m_pMeshOrbit->SetOrbit(true);
			}
		}
	case ANIMATIONTYPE_ATTACK_4:				// 攻撃モーションのとき
		if (m_pColWeaponSphere != NULL)
		{// 武器の当たり判定が存在していたとき
			if (!m_pColWeaponSphere->GetUse())
			{// 当たり判定の対象外だったとき
				m_pColWeaponSphere->SetUse(true);		// 対象にする
			}
		}
		if (m_pMeshOrbit != NULL)
		{// 軌跡が存在していたとき
			if (!m_pMeshOrbit->GetOrbit())
			{// 軌跡の更新が停止していたとき
				m_pMeshOrbit->SetOrbit(true);
			}
		}
		break;
	case ANIMATIONTYPE_ATTACK_6:				// 攻撃モーションのとき
		if (m_pColWeaponSphere != NULL)
		{// 武器の当たり判定が存在していたとき
			if (!m_pColWeaponSphere->GetUse())
			{// 当たり判定の対象外だったとき
				m_pColWeaponSphere->SetUse(true);		// 対象にする
			}
		}
		if (m_pMeshOrbit != NULL)
		{// 軌跡が存在していたとき
			if (!m_pMeshOrbit->GetOrbit())
			{// 軌跡の更新が停止していたとき
				m_pMeshOrbit->SetOrbit(true);
			}
		}
		break;
	}
}

//=============================================================================
// キー入力情報処理
//=============================================================================
void CPlayer::Input(void)
{
	// キーボードの取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	// ゲームパッドの取得
	CInputController *pGamepad = CManager::GetInputController();

	// カメラの取得
	CCamera *pCamera = CManager::GetCamera();

	// サウンドの取得
	CSound *pSound = CManager::GetSound();

	D3DXVECTOR3 rot = pCamera->GetRotation();
	D3DXVECTOR3 nor;
	float nValueH = 0;									//コントローラー
	float nValueV = 0;									//コントローラー

	if (m_nCntAttacCombo <= COUNTER_COMBO)
	{// 攻撃派生受付カウンタが15カウント未満だったとき
		m_nCntAttacCombo++;				// カウンタに1プラス
	}

	// ====================== コントローラー ====================== //

	// 攻撃
	if (m_AnimationType != ANIMATIONTYPE_ATTACK_1 && m_AnimationType != ANIMATIONTYPE_ATTACK_2 &&
		m_AnimationType != ANIMATIONTYPE_ATTACK_3 && m_AnimationType != ANIMATIONTYPE_ATTACK_4 &&
		m_AnimationType != ANIMATIONTYPE_ATTACK_5 && m_AnimationType != ANIMATIONTYPE_ATTACK_6 &&
		m_AnimationType != ANIMATIONTYPE_RUNATTACK && m_AnimationType != ANIMATIONTYPE_DAMAGE &&
		m_AnimationType != ANIMATIONTYPE_SWITCHWEAPON && m_AnimationType != ANIMATIONTYPE_JUMP_1 &&
		m_AnimationType != ANIMATIONTYPE_JUMP_2 && m_AnimationType != ANIMATIONTYPE_JUMP_3 &&
		m_AnimationType != ANIMATIONTYPE_JUMP_4 && m_AnimationType != ANIMATIONTYPE_JUMP_5)
	{
		if (pGamepad != NULL)
		{// ゲームパッドが存在したとき
			if (pGamepad->GetJoypadUse(0))
			{// 使用可能だったとき
				if (pGamepad->GetControllerTrigger(0, JOYPADKEY_X))
				{// Xボタンが押されたとき
					pSound->PlaySoundA(SOUND_LABEL_SE_SLASHING);

					if (m_nCntAttacCombo < COUNTER_COMBO)
					{
						if (m_AttackTypeOld == ANIMATIONTYPE_ATTACK_1)
						{
							AnimationSwitch(ANIMATIONTYPE_ATTACK_2);								// アニメーションの切り替え
							m_AttackTypeOld = ANIMATIONTYPE_ATTACK_2;
						}
						else if (m_AttackTypeOld == ANIMATIONTYPE_ATTACK_2)
						{
							AnimationSwitch(ANIMATIONTYPE_ATTACK_3);
							m_AttackTypeOld = ANIMATIONTYPE_ATTACK_3;
						}
						else if (m_AttackTypeOld == ANIMATIONTYPE_ATTACK_3)
						{
							AnimationSwitch(ANIMATIONTYPE_ATTACK_4);
							m_AttackTypeOld = ANIMATIONTYPE_ATTACK_4;
						}

						m_nCntAttacCombo = COUNTER_COMBO;										// カウンタ受付終了
					}
					else
					{
						AnimationSwitch(ANIMATIONTYPE_ATTACK_1);								// アニメーションの切り替え
						m_AttackTypeOld = ANIMATIONTYPE_ATTACK_1;							// 前の攻撃タイプを設定
					}
				}
				if (pGamepad->GetControllerTrigger(0, JOYPADKEY_Y))
				{// Yボタンが押されたとき
					if (m_nCntAttacCombo < COUNTER_COMBO)
					{
						if (m_AttackTypeOld == ANIMATIONTYPE_ATTACK_1)
						{
							AnimationSwitch(ANIMATIONTYPE_ATTACK_6);							// アニメーションの切り替え
							m_AttackTypeOld = ANIMATIONTYPE_ATTACK_6;
						}

						m_nCntAttacCombo = COUNTER_COMBO;										// カウンタ受付終了
					}
					else
					{
						AnimationSwitch(ANIMATIONTYPE_ATTACK_5);								// アニメーションの切り替え
						m_AttackTypeOld = ANIMATIONTYPE_ATTACK_5;							// 前の攻撃タイプを設定

						if (pSound != NULL)
						{// サウンドが存在していたとき
							pSound->PlaySoundA(SOUND_LABEL_SE_CHARGE);							// チャージ音の再生
						}
					}
				}
				if (pGamepad->GetControllerTrigger(0, JOYPADKEY_A))
				{
					if (!m_bJump)
					{// ジャンプしていないとき
						AnimationSwitch(ANIMATIONTYPE_JUMP_1);		// ジャンプモーションに切り替え
						m_bJump = true;					// ジャンプ判定
						pSound->PlaySoundA(SOUND_LABEL_SE_JUMP);		// ジャンプ音の再生
					}
				}
				if (pGamepad->GetControllerTrigger(0, JOYPADKEY_RIGHT_TRIGGER))
				{
					D3DXVECTOR3 pos = GetPosition();
					pos.y += 100.0f;
					AnimationSwitch(ANIMATIONTYPE_RUNATTACK);
					CEffect::Star(pos);
					CEffect::Halo(pos, D3DXVECTOR3(30.0f, 30.0f, 0.0f));
				}
				if (pGamepad->GetControllerTrigger(0, JOYPADKEY_LEFT_THUMB))
				{// 本気の力開放
					if (m_state != PLAYERSTATE_FLOWER)
					{// 現在のステータスが暴走状態ではないとき
						if (m_fDeathblow > 25.0f)
						{
							AnimationSwitch(ANIMATIONTYPE_SWITCHWEAPON);			// アニメーションの変更
							m_nActionCount = 0;			// カウンタの初期化

							D3DXVECTOR3 cameraRot = m_rot;				// 回転量を格納
							cameraRot.y += D3DX_PI;						// 回転量に3.14を加算
							pCamera->SetDistance(300.0f);				// 視点注視点の距離を設定
							pCamera->SetPosCamera(GetPosition(), cameraRot);		// カメラの場所を設定
							m_bEvent = true;
						}
					}
				}
			}
		}

		if (pGamepad != NULL)
		{// ゲームパッドが存在したとき
			if (pGamepad->GetJoypadUse(0))
			{// 使用可能だったとき
				pGamepad->GetJoypadStickLeft(0, &nValueH, &nValueV);

				//float fRot = fabs(nValueH) + (nValueV);

				//float fAngle = atan2f(nValueH, nValueV);
				//float fDistance = D3DXVec2Length(&D3DXVECTOR2(nValueH, nValueV));

				//移動
				m_move += D3DXVECTOR3(sinf(D3DX_PI * 1.0f + rot.y) * (m_fSpeed * nValueV), 0, cosf(D3DX_PI * 1.0f + rot.y) * (m_fSpeed * nValueV));
				m_move += D3DXVECTOR3(sinf(D3DX_PI * 0.5f + rot.y) * (m_fSpeed * nValueH), 0, cosf(D3DX_PI * 0.5f + rot.y) * (m_fSpeed * nValueH));

				//fDistance /= 1;

				//m_move.x += sinf(fAngle - (rot.y + D3DX_PI)) * -fDistance;
				//m_move.z += cosf(fAngle + (rot.y - D3DX_PI)) * fDistance;


#ifdef _DEBUG
				//CDebugProc::Log("方向 : %.2f\n", fAngle);
				//CDebugProc::Log("方向経産の答え : %.2f\n",(rot.y - D3DX_PI));

				//CDebugProc::Log("計算 : %f\n", fDistance);

				CDebugProc::Log("移動量 : %.2f %.2f %.2f", m_move.x, m_move.y, m_move.z);

				if (pGamepad->GetControllerPress(0, JOYPADKEY_A))
				{
					CDebugProc::Log("コントローラー : 0番\n");
				}
				if (pGamepad->GetControllerPress(1, JOYPADKEY_A))
				{
					CDebugProc::Log("コントローラー : 1番\n");
				}

				CDebugProc::Log("コントローラーH : %f\n", nValueH);
				CDebugProc::Log("コントローラーV : %f\n", nValueV);
#endif
			}
		}

		// ====================== キーボード ====================== //

		if (pKeyboard->GetTriggerKeyboard(DIK_1))
		{
			for (int nCount = 0; nCount < 20; nCount++)
			{
				float fAngle = float(CManager::GetRand(314)) - float(CManager::GetRand(314));
				float fAngle_x = float(CManager::GetRand(314)) - float(CManager::GetRand(314));

				D3DXVECTOR3 particlePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				particlePos.x = cosf(D3DX_PI + fAngle) * cosf(D3DX_PI + fAngle_x);
				particlePos.y = sinf(D3DX_PI + fAngle_x);
				particlePos.z = sinf(D3DX_PI + fAngle) * cosf(D3DX_PI + fAngle_x);

				fAngle = float(CManager::GetRand(314)) / 100.0f - float(CManager::GetRand(314)) / 100.0f;
				D3DXVECTOR3 rot;
				rot = D3DXVECTOR3(sinf(fAngle) * 10, cosf(fAngle) * 10, 0.0f);

				CEffect::SetEffect(EFFECTTYPE_ROSE,										// パーティクルのタイプ
					GetPosition(),											// 発生位置
					D3DXVECTOR3(8.0f, 8.0f, 0.0f),							// サイズ
					particlePos * 5.0f,										// 方向ベクトル
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					EASINGTYPE_NONE,
					rot,													// テクスチャの回転
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),						// カラー
					200,													// パーティクルの生存カウント数
					true,													// 重力
					0,														// 抵抗
					true,													// ビルボード
					0,														// 表示する箇所(横)
					0,														// 表示する箇所(縦)
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					0,
					0,
					0);
			}
		}

		if (pKeyboard->GetTriggerKeyboard(DIK_LSHIFT))
		{
			D3DXVECTOR3 pos = GetPosition();
			pos.y += 100.0f;
			AnimationSwitch(ANIMATIONTYPE_RUNATTACK);
			CEffect::Star(pos);
			CEffect::Halo(pos, D3DXVECTOR3(30.0f, 30.0f, 0.0f));
		}

		if (pKeyboard->GetTriggerKeyboard(DIK_X))
		{// Xボタンが押されたとき
			switch (m_state)
			{
			case PLAYERSTATE_NORMAL:
				if (m_nCntAttacCombo < COUNTER_COMBO)
				{
					if (m_AttackTypeOld == ANIMATIONTYPE_ATTACK_1)
					{
						AnimationSwitch(ANIMATIONTYPE_ATTACK_2);								// アニメーションの切り替え
						m_AttackTypeOld = ANIMATIONTYPE_ATTACK_2;
					}
					else if (m_AttackTypeOld == ANIMATIONTYPE_ATTACK_2)
					{
						AnimationSwitch(ANIMATIONTYPE_ATTACK_3);
						m_AttackTypeOld = ANIMATIONTYPE_ATTACK_3;
					}
					else if (m_AttackTypeOld == ANIMATIONTYPE_ATTACK_3)
					{
						AnimationSwitch(ANIMATIONTYPE_ATTACK_4);
						m_AttackTypeOld = ANIMATIONTYPE_ATTACK_4;
					}

					m_nCntAttacCombo = COUNTER_COMBO;										// カウンタ受付終了
				}
				else
				{
					AnimationSwitch(ANIMATIONTYPE_ATTACK_1);								// アニメーションの切り替え
					m_AttackTypeOld = ANIMATIONTYPE_ATTACK_1;							// 前の攻撃タイプを設定
				}
				break;
			case PLAYERSTATE_FLOWER:
				MoveNearEnemy();					// 敵にワープする
				break;
			}
		}
		if (pKeyboard->GetTriggerKeyboard(DIK_C))
		{// Cボタンが押されたとき
			if (m_nCntAttacCombo < COUNTER_COMBO)
			{
				if (m_AttackTypeOld == ANIMATIONTYPE_ATTACK_1)
				{
					AnimationSwitch(ANIMATIONTYPE_ATTACK_6);							// アニメーションの切り替え
					m_AttackTypeOld = ANIMATIONTYPE_ATTACK_6;
				}

				m_nCntAttacCombo = COUNTER_COMBO;										// カウンタ受付終了
			}
			else
			{
				AnimationSwitch(ANIMATIONTYPE_ATTACK_5);								// 攻撃モーション5に切り替え
				m_AttackTypeOld = ANIMATIONTYPE_ATTACK_5;							// 前の攻撃タイプを設定
			}
		}

		if (pKeyboard->GetTriggerKeyboard(DIK_SPACE))
		{// スペースキーが押されたとき
			if (!m_bJump)
			{
				AnimationSwitch(ANIMATIONTYPE_JUMP_1);		// ジャンプモーションに切り替え
				m_bJump = true;			// ジャンプしている
			}
		}

		//左右操作
		if (pKeyboard->GetPressKeyboard(DIK_LEFTARROW))
		{
			if (pKeyboard->GetPressKeyboard(DIK_UPARROW))
			{
				m_move.x += sinf(D3DX_PI * 0.75f + rot.y) * m_fSpeed;
				m_move.z += cosf(D3DX_PI * 0.75f + rot.y) * m_fSpeed;

				D3DXVec3Normalize(&nor, &D3DXVECTOR3(m_move.z, m_move.y, -m_move.x));
				m_dest.y = -D3DX_PI * 0.25f + rot.y;
			}
			else if (pKeyboard->GetPressKeyboard(DIK_DOWNARROW))
			{
				m_move.x += sinf(D3DX_PI * 0.25f + rot.y) * m_fSpeed;
				m_move.z += cosf(D3DX_PI * 0.25f + rot.y) * m_fSpeed;

				D3DXVec3Normalize(&nor, &D3DXVECTOR3(m_move.z, m_move.y, -m_move.x));
				m_dest.y = -D3DX_PI * 0.75f + rot.y;
			}
			else
			{
				m_move.x += sinf(D3DX_PI * 0.5f + rot.y) * m_fSpeed;
				m_move.z += cosf(D3DX_PI * 0.5f + rot.y) * m_fSpeed;

				D3DXVec3Normalize(&nor, &D3DXVECTOR3(m_move.z, m_move.y, -m_move.x));
				m_dest.y = -D3DX_PI * 0.5f + rot.y;
			}
		}

		else if (pKeyboard->GetPressKeyboard(DIK_RIGHTARROW))
		{
			//上下操作
			if (pKeyboard->GetPressKeyboard(DIK_UPARROW))
			{
				m_move.x += sinf(-D3DX_PI * 0.75f + rot.y) * m_fSpeed;
				m_move.z += cosf(-D3DX_PI * 0.75f + rot.y) * m_fSpeed;

				D3DXVec3Normalize(&nor, &D3DXVECTOR3(m_move.z, m_move.y, -m_move.x));
				m_dest.y = D3DX_PI * 0.25f + rot.y;
			}
			else if (pKeyboard->GetPressKeyboard(DIK_DOWNARROW))
			{
				m_move.x += sinf(-D3DX_PI * 0.25f + rot.y) * m_fSpeed;
				m_move.z += cosf(-D3DX_PI * 0.25f + rot.y) * m_fSpeed;

				D3DXVec3Normalize(&nor, &D3DXVECTOR3(m_move.z, m_move.y, -m_move.x));
				m_dest.y = D3DX_PI * 0.75f + rot.y;
			}
			else
			{
				m_move.x += sinf(-D3DX_PI * 0.5f + rot.y) * m_fSpeed;
				m_move.z += cosf(-D3DX_PI * 0.5f + rot.y) * m_fSpeed;

				D3DXVec3Normalize(&nor, &D3DXVECTOR3(m_move.z, m_move.y, -m_move.x));
				m_dest.y = D3DX_PI * 0.5f + rot.y;
			}
		}

		//上下操作
		else if (pKeyboard->GetPressKeyboard(DIK_UPARROW))
		{
			m_move.x += sinf(D3DX_PI * 1.0f + rot.y) * m_fSpeed;
			m_move.z += cosf(D3DX_PI * 1.0f + rot.y) * m_fSpeed;

			D3DXVec3Normalize(&nor, &D3DXVECTOR3(m_move.z, m_move.y, -m_move.x));
			m_dest.y = D3DX_PI * 0.0f + rot.y;
		}
		else if (pKeyboard->GetPressKeyboard(DIK_DOWNARROW))
		{
			D3DXVec3Normalize(&nor, &D3DXVECTOR3(m_move.z, m_move.y, -m_move.x));

			m_move.x += sinf(D3DX_PI * 0.0f + rot.y) * m_fSpeed;
			m_move.z += cosf(D3DX_PI * 0.0f + rot.y) * m_fSpeed;
			m_dest.y = D3DX_PI * 1.0f + rot.y;
		}
	}

	if (pGamepad != NULL)
	{// ゲームパッドが存在したとき
		if (pGamepad->GetJoypadUse(0))
		{// 使用可能だったとき
			// 溜め攻撃
			if (pGamepad->GetControllerRelease(0, JOYPADKEY_Y))
			{
				if (m_AnimationType == ANIMATIONTYPE_ATTACK_5)
				{
					if (!m_bAnimation)
					{// アニメーションが止まっているとき
						if (m_pColHandSphere != NULL)
						{// 武器の当たり判定が存在していたとき
							if (!m_pColHandSphere->GetUse())
							{// 当たり判定の対象外だったとき
								m_pColHandSphere->SetUse(true);		// 対象にする
							}
						}

						m_nCurrentKey = 1;				// 現在のキー数を[1]に設定
						m_nCurrentFrame = 0;			// 現在のフレーム数を[0]に設定
						m_bAnimation = true;			// アニメーションを再開
						pSound->PlaySoundA(SOUND_LABEL_SE_BOMB);	// 爆発音の再生
					}
					else
					{
						AnimationSwitch(ANIMATIONTYPE_NEUTRAL);
					}
				}
			}
			else if (pGamepad->GetControllerPress(0, JOYPADKEY_Y))
			{// Yボタンが押されているとき
				if (m_AnimationType == ANIMATIONTYPE_ATTACK_5)
				{// 現在攻撃5の最中だったとき
					if (m_bAnimation)
					{// アニメーションが更新中だったとき
						D3DXMATRIX mtx = m_pModel[4].GetMtxWorld();				// モデルのマトリックス取得
						CEffect::Halo(D3DXVECTOR3(mtx._41, mtx._42, mtx._43), D3DXVECTOR3(15.0f, 15.0f, 0.0f));	// モデルのマトリックス情報から位置情報を渡す
					}
				}
			}
		}
	}

	if (pKeyboard->GetReleaseKeyboard(DIK_C))
	{
		if (m_AnimationType == ANIMATIONTYPE_ATTACK_5)
		{
			if (!m_bAnimation)
			{// アニメーションが止まっているとき
				if (m_pColHandSphere != NULL)
				{// 武器の当たり判定が存在していたとき
					if (!m_pColHandSphere->GetUse())
					{// 当たり判定の対象外だったとき
						m_pColHandSphere->SetUse(true);		// 対象にする
					}
				}

				m_nCurrentKey = 1;				// 現在のキー数を[1]に設定
				m_nCurrentFrame = 0;			// 現在のフレーム数を[0]に設定
				m_bAnimation = true;			// アニメーションを再開
			}
			else
			{
				AnimationSwitch(ANIMATIONTYPE_NEUTRAL);
			}
		}
	}
	if (pKeyboard->GetPressKeyboard(DIK_C))
	{
		if (m_AnimationType == ANIMATIONTYPE_ATTACK_5)
		{
			if (m_pColHandSphere != NULL)
			{// 武器の当たり判定が存在していたとき
				if (m_bAnimation)
				{
					D3DXMATRIX mtx = m_pModel[4].GetMtxWorld();
					CEffect::Halo(D3DXVECTOR3(mtx._41, mtx._42, mtx._43), D3DXVECTOR3(15.0f, 15.0f, 0.0f));
				}
			}
		}
	}

#ifdef _DEBUG
	if (pKeyboard->GetTriggerKeyboard(DIK_K))
	{// Kが押されたとき
		m_fDeathblow = 50.0f;				// 必殺技ポイントを最大値まで上げる
	}
	if (pKeyboard->GetTriggerKeyboard(DIK_F8))
	{// F8が押されたとき
		switch (m_state)
		{
		case PLAYERSTATE_NORMAL:
			m_state = PLAYERSTATE_FLOWER;
			m_fSpeed = SP_SPEED;
			break;
		case PLAYERSTATE_FLOWER:
			m_state = PLAYERSTATE_NORMAL;
			m_fSpeed = NORMAL_SPEED;
			break;
		}
	}
	if (pKeyboard->GetTriggerKeyboard(DIK_L))
	{
		AnimationSwitch(ANIMATIONTYPE_SWITCHWEAPON);
	}
#endif
}

//=============================================================================
// アニメーション処理
//=============================================================================
void CPlayer::Animation(void)
{
	//最初のフレームの時
	if (m_nCurrentFrame == 0)
	{
		for (int nCount = 0; nCount < m_nNumParts; nCount++)
		{
			D3DXVECTOR3 pos = m_pModel[nCount].GetPosition();
			D3DXVECTOR3 rot = m_pModel[nCount].GetRotation();

			if (m_bAnimSwitch)
			{
				//posの計算((目標のkey + プリセットの配置) - 現在のキー)
				m_apCurrentPos[nCount] = ((m_pAnimation[m_AnimationType].apKeyInfo[nCount].aKey[m_nCurrentKey].pos + m_pModel[nCount].GetPosPreset()) - pos) / 0.1f * (float)m_pAnimation[m_AnimationType].nSwitchFrame;

				//rotの計算((目標のkey + プリセットの配置) - 現在のキー)
				m_apCurrentRot[nCount] = ((m_pAnimation[m_AnimationType].apKeyInfo[nCount].aKey[m_nCurrentKey].rot + m_pModel[nCount].GetRotPreset()) - rot) / 0.1f * (float)m_pAnimation[m_AnimationType].nSwitchFrame;
			}
			else
			{
				//posの計算((目標のkey + プリセットの配置) - 現在のキー)
				m_apCurrentPos[nCount] = ((m_pAnimation[m_AnimationType].apKeyInfo[nCount].aKey[m_nCurrentKey].pos + m_pModel[nCount].GetPosPreset()) - pos) / (float)m_pAnimation[m_AnimationType].apKeyInfo[m_nCurrentKey].nFrame;

				//rotの計算((目標のkey + プリセットの配置) - 現在のキー)
				m_apCurrentRot[nCount] = ((m_pAnimation[m_AnimationType].apKeyInfo[nCount].aKey[m_nCurrentKey].rot + m_pModel[nCount].GetRotPreset()) - rot) / (float)m_pAnimation[m_AnimationType].apKeyInfo[m_nCurrentKey].nFrame;
			}
		}
	}
	else
	{//それ以外のフレーム
		for (int nCount = 0; nCount < m_nNumParts; nCount++)
		{
			D3DXVECTOR3 pos = m_pModel[nCount].GetPosition();
			D3DXVECTOR3 rot = m_pModel[nCount].GetRotation();

			//rotの移動
			m_pModel[nCount].SetRotation(rot + m_apCurrentRot[nCount]);

			//posの移動
			m_pModel[nCount].SetPosition(pos + m_apCurrentPos[nCount]);
		}
	}

	if (m_state == PLAYERSTATE_NORMAL)
	{
		if (m_AnimationType == ANIMATIONTYPE_SWITCHWEAPON)
		{
			if (m_state == PLAYERSTATE_NORMAL)
				if (m_nCurrentKey == 0)
				{
					CCamera *pCamera = CManager::GetCamera();
					float fDistance = pCamera->GetDistance();
					fDistance -= 10;
					pCamera->SetDistance(fDistance);
				}
		}
	}

	if (m_pAnimation[m_AnimationType].apKeyInfo != NULL)
	{// キー情報がNULLではないとき
		//フレームの最大数に満たない場合
		if (m_nCurrentFrame < m_pAnimation[m_AnimationType].apKeyInfo[m_nCurrentKey].nFrame)
		{
			m_nCurrentFrame++;
		}
		//フレーム数の最大値に達した場合
		else if (m_nCurrentFrame >= m_pAnimation[m_AnimationType].apKeyInfo[m_nCurrentKey].nFrame)
		{
			if (m_AnimationType == ANIMATIONTYPE_ATTACK_4)
			{// モーションが攻撃4だったとき
				if (m_nCurrentKey == 4)
				{// 現在のキー数が4だったとき
					D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 40.0f, -80.0f);
					D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					D3DXMATRIX	mtxRot, mtxTrans, mtxView, mtxMeshRot, mtxMeshTrans;				//計算用マトリックス
					D3DXMATRIX mtx;			// 武器のマトリックス

					// ワールドマトリックスの初期化
					D3DXMatrixIdentity(&mtx);

					// 回転を反映
					D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, rot.y, rot.x, rot.z);
					D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshRot);

					// 位置を反映
					D3DXMatrixTranslation(&mtxMeshTrans, pos.x, pos.y, pos.z);
					D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshTrans);

					D3DXMatrixMultiply(&mtx, &mtx, &m_pModel[14].GetMtxWorld());

					CEffect::SandSmokeExplosion(D3DXVECTOR3(mtx._41, mtx._42, mtx._43));
					CSound *pSound = CManager::GetSound();			// サウンドの取得
					pSound->PlaySoundA(SOUND_LABEL_SE_BOMB);		// 爆発音の再生

					m_move.y += 20;			// 上方向に力を加える
					m_bJump = true;			// ジャンプ中に設定
				}
			}
			else if (m_AnimationType == ANIMATIONTYPE_ATTACK_5)
			{// モーションが攻撃5だったとき
				if (m_nCurrentKey == 0)
				{// 現在のキー数が0だったとき
					m_bAnimation = false;
				}
				else if (m_nCurrentKey == 1)
				{
					D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					D3DXMATRIX	mtxRot, mtxTrans, mtxView, mtxMeshRot, mtxMeshTrans;				//計算用マトリックス
					D3DXMATRIX mtx;			// 武器のマトリックス

					// ワールドマトリックスの初期化
					D3DXMatrixIdentity(&mtx);

					// 回転を反映
					D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, rot.y, rot.x, rot.z);
					D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshRot);

					// 位置を反映
					D3DXMatrixTranslation(&mtxMeshTrans, 0.0f, 0.0f, 0.0f);
					D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshTrans);

					D3DXMatrixMultiply(&mtx, &mtx, &m_pModel[4].GetMtxWorld());

					CEffect::Shockwave(D3DXVECTOR3(mtx._41, mtx._42, mtx._43),
						m_rot,
						15);
				}
			}
			else if (m_AnimationType == ANIMATIONTYPE_JUMP_1)
			{// ジャンプ_1だったとき
				if (m_nCurrentKey == 1)
				{
					m_move.y += JUMP_MAX;				// 上方向に力を加える
				}
			}
			m_nCurrentKey++;			// キーの値に1プラス
			m_nCurrentFrame = 0;		// フレームのリセット

			if (m_AnimationType == ANIMATIONTYPE_RUNATTACK)
			{
				if (m_nCurrentKey == 1)
				{
					if (m_nCurrentFrame == 0)
					{

						// カメラ の取得
						CCamera *pCamera = CManager::GetCamera();
						D3DXVECTOR3 rot = pCamera->GetRotation();

						if (m_bJump)
						{
							m_move.x += sinf(D3DX_PI * 1.0f + m_rot.y) * 50.0f;
							m_move.z += cosf(D3DX_PI * 1.0f + m_rot.y) * 50.0f;
						}
						else
						{
							m_move.x += sinf(D3DX_PI * 1.0f + m_rot.y) * 25.0f;
							m_move.z += cosf(D3DX_PI * 1.0f + m_rot.y) * 25.0f;
						}

						m_dest.y = D3DX_PI * 0.0f + rot.y;
					}
				}
			}
		}
	}

	//キー数の最大値に達した場合
	if (m_nCurrentKey >= m_pAnimation[m_AnimationType].nMaxKey)
	{
		D3DXVECTOR3 rot = m_pModel[0].GetRotation();		// 回転量取得

		switch (m_AnimationType)
		{
		case ANIMATIONTYPE_ATTACK_3:
			rot.y = 1.5f;										// Y軸の回転量変更
			m_pModel[0].SetRotation(rot);						// 回転量の設定
			break;
		case ANIMATIONTYPE_ATTACK_4:
			rot.x = 0.0f;										// X軸の回転量変更
			m_pModel[0].SetRotation(rot);						// 回転量の設定
			break;
		case ANIMATIONTYPE_RUNATTACK:
			rot.x = 0.72f;										// X軸の回転量変更
			m_pModel[0].SetRotation(rot);						// 回転量の設定
			break;
		case ANIMATIONTYPE_JUMP_1:
			AnimationSwitch(ANIMATIONTYPE_JUMP_2);
			break;
		case ANIMATIONTYPE_JUMP_2:
			AnimationSwitch(ANIMATIONTYPE_JUMP_3);
			break;
		case ANIMATIONTYPE_JUMP_3:
			AnimationSwitch(ANIMATIONTYPE_JUMP_4);
			break;
		case ANIMATIONTYPE_JUMP_4:
			m_bAnimation = false;
			break;
		case ANIMATIONTYPE_SWITCHWEAPON:
			int nNumVertices;			//頂点数
			DWORD sizeFVF;				//頂点フォーマット
			BYTE	*pVertexBuffer;		//頂点バッファへのポインタ
			LPD3DXMESH pMesh = m_pModel[14].GetMesh();

			//頂点数を取得
			nNumVertices = pMesh->GetNumVertices();

			//頂点フォーマットのサイズを取得
			sizeFVF = D3DXGetFVFVertexSize(pMesh->GetFVF());

			//頂点バッファをロック
			pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertexBuffer);

			// パーティクル生成
			for (int nCount = 0; nCount < nNumVertices; nCount++)
			{
				D3DXVECTOR3	vtx = *(D3DXVECTOR3*)pVertexBuffer;

				if (nCount % 50 == 0)
				{// 50回に1回生成する
					D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					D3DXMATRIX	mtxRot, mtxTrans, mtxView, mtxMeshRot, mtxMeshTrans;				//計算用マトリックス
					D3DXMATRIX mtx;			// 武器のマトリックス
					D3DXVECTOR3 pos = GetPosition();

					// ワールドマトリックスの初期化
					D3DXMatrixIdentity(&mtx);

					// 回転を反映
					D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, rot.y, rot.x, rot.z);
					D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshRot);

					// 位置を反映
					D3DXMatrixTranslation(&mtxMeshTrans, vtx.x, vtx.y, vtx.z);
					D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshTrans);

					D3DXMatrixMultiply(&mtx, &mtx, &m_pModel[14].GetMtxWorld());

					switch (m_state)
					{
					case PLAYERSTATE_NORMAL:
						CEffect::FallingPetals(false, D3DXVECTOR3(mtx._41, mtx._42, mtx._43), 50);
						break;
					case PLAYERSTATE_FLOWER:
						CEffect::PetalsGather(D3DXVECTOR3(mtx._41, mtx._42, mtx._43));
						break;
					}
				}

				pVertexBuffer += sizeFVF;			//サイズ分のポインタを進める
			}

			//頂点バッファをアンロック
			pMesh->UnlockVertexBuffer();

			CCamera *pCamera = CManager::GetCamera();

			switch (m_state)
			{
			case PLAYERSTATE_NORMAL:
				pCamera->SetDistance(350.0f);			// 視点と注視点の距離を変える
				m_pModel[14].SetActive(false);			// 武器の描画を停止
				m_state = PLAYERSTATE_FLOWER;			// ステータスを暴走に変更
				m_fSpeed = SP_SPEED;					// スピードを変える
				m_nActionCount = 0;						// カウンタの初期化
				break;
			case PLAYERSTATE_FLOWER:
				m_state = PLAYERSTATE_NORMAL;			// ステータスを通常に戻す
				m_fSpeed = NORMAL_SPEED;				// スピードを通常に戻す
				m_bAnimation = false;					// アニメーションの停止
				m_bEvent = false;						// イベントフラグを下す
				return;
				break;
			}
			break;
		}

		switch (m_state)
		{
		case PLAYERSTATE_NORMAL:
			//攻撃の派生カウンタのリセット
			if (m_AnimationType == ANIMATIONTYPE_ATTACK_1 || m_AnimationType == ANIMATIONTYPE_ATTACK_2 ||
				m_AnimationType == ANIMATIONTYPE_ATTACK_3)
			{// モーションが攻撃モーションだったとき
				m_nCntAttacCombo = 0;
			}
			break;
		case PLAYERSTATE_FLOWER:
			//攻撃の派生カウンタのリセット
			if (m_AnimationType == ANIMATIONTYPE_ATTACK_1)
			{// モーションが攻撃モーションだったとき
				m_nCntAttacCombo = 0;
			}
			break;
		}

		if (m_AnimationType != ANIMATIONTYPE_JUMP_2 && m_AnimationType != ANIMATIONTYPE_JUMP_3 && m_AnimationType != ANIMATIONTYPE_JUMP_4)
		{
			//ループするかどうか
			if (m_pAnimation[m_AnimationType].nLoop)
			{
				//キーのリセット
				m_nCurrentKey = 0;
				m_nCurrentFrame = 0;
			}
			else
			{
				//ニュートラルモーション
				AnimationSwitch(ANIMATIONTYPE_NEUTRAL);
			}
		}
	}

	if (m_AnimationType != ANIMATIONTYPE_ATTACK_1 && m_AnimationType != ANIMATIONTYPE_ATTACK_2 &&
		m_AnimationType != ANIMATIONTYPE_ATTACK_3 && m_AnimationType != ANIMATIONTYPE_ATTACK_4 &&
		m_AnimationType != ANIMATIONTYPE_ATTACK_5 && m_AnimationType != ANIMATIONTYPE_ATTACK_6 &&
		m_AnimationType != ANIMATIONTYPE_RUNATTACK && m_AnimationType != ANIMATIONTYPE_DAMAGE &&
		m_AnimationType != ANIMATIONTYPE_SWITCHWEAPON && m_AnimationType != ANIMATIONTYPE_JUMP_1 &&
		m_AnimationType != ANIMATIONTYPE_JUMP_2 && m_AnimationType != ANIMATIONTYPE_JUMP_3 &&
		m_AnimationType != ANIMATIONTYPE_JUMP_4 && m_AnimationType != ANIMATIONTYPE_JUMP_5)
	{
		if (fabs(m_move.x) > 2 || fabs(m_move.z) > 2)
		{
			switch (m_state)
			{
			case PLAYERSTATE_NORMAL:
				if (m_AnimationType != ANIMATIONTYPE_WALK)
				{
					AnimationSwitch(ANIMATIONTYPE_WALK);
				}
				break;
			case PLAYERSTATE_FLOWER:
				if (m_AnimationType != ANIMATIONTYPE_RUN)
				{
					AnimationSwitch(ANIMATIONTYPE_RUN);
				}
			}
		}
		else
		{
			if (m_AnimationType != ANIMATIONTYPE_NEUTRAL)
			{
				AnimationSwitch(ANIMATIONTYPE_NEUTRAL);
			}
		}
	}

	if (m_AnimationType == ANIMATIONTYPE_ATTACK_1 || m_AnimationType == ANIMATIONTYPE_ATTACK_2)
	{
		if (m_nCurrentKey == 1)
		{
			if (m_nCurrentFrame == 0)
			{// 鎌による攻撃時の遠心力による移動処理
				m_move.x += sinf(D3DX_PI * 1.0f + m_rot.y) * 10.0f;
				m_move.z += cosf(D3DX_PI * 1.0f + m_rot.y) * 10.0f;
			}
		}
	}
	else if (m_AnimationType == ANIMATIONTYPE_ATTACK_4)
	{
		if (m_nCurrentKey == 5)
		{
			if (m_nCurrentFrame == 0)
			{// 鎌の攻撃による衝撃波で後ろに飛ばす処理
				m_move.x += sinf(D3DX_PI * 0.0f + m_rot.y) * 15.0f;
				m_move.z += cosf(D3DX_PI * 0.0f + m_rot.y) * 15.0f;
			}
		}
	}
}

//=============================================================================
// アニメーションの切り替え
//=============================================================================
void CPlayer::AnimationSwitch(ANIMATIONTYPE Type)
{
	m_AnimationTypeOld = m_AnimationType;								// 前のアニメーションに今のアニメーションを入れる
	m_AnimationType = Type;												// アニメーションの切り替え
	m_nCurrentFrame = 0;												// 現在のフレーム数をリセット
	m_nCurrentKey = 0;													// 現在のキー数をリセット

	if (m_pMeshOrbit != NULL)
	{// 中身があるとき
		if (m_AnimationType == ANIMATIONTYPE_ATTACK_1 || m_AnimationType == ANIMATIONTYPE_ATTACK_2)
		{// 攻撃モーションだったとき
			if (!m_pMeshOrbit->GetOrbit())
			{// 軌跡が描画されていないとき
				m_pMeshOrbit->SetOrbit(true);
			}
		}
		else
		{// 指定したモーション以外だったとき
			if (m_pMeshOrbit->GetOrbit())
			{// 軌跡が描画されていたら
				m_pMeshOrbit->SetOrbit(false);
			}
		}
	}

	if (m_AnimationType == ANIMATIONTYPE_ATTACK_1 || m_AnimationType == ANIMATIONTYPE_ATTACK_2 ||
		m_AnimationType == ANIMATIONTYPE_ATTACK_3 || m_AnimationType == ANIMATIONTYPE_ATTACK_4)
	{
		CEffect::SandSmokeEffect(GetPosition());
	}
}

#ifdef _DEBUG
//=============================================================================
// デバッグ処理
//=============================================================================
void CPlayer::Debug(void)
{
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 posOld = GetPosOld();
	ImGui::Begin("System");													// 「System」ウィンドウに追加します。なければ作成します。

	if (ImGui::CollapsingHeader("player"))
	{
		ImGui::Text("pos = %.2f, %.2f, %.2f", pos.x, pos.y, pos.z);								// プレイヤーの現在位置を表示
		ImGui::Text("posOld = %.2f, %.2f, %.2f", posOld.x, posOld.y, posOld.z);								// プレイヤーの現在位置を表示
		ImGui::Text("move = %.2f, %.2f, %.2f", m_move.x, m_move.y, m_move.z);								// プレイヤーの現在位置を表示
		ImGui::Text("HP = %d", m_nLife);				// プレイヤーの体力を表示

		switch (m_state)
		{
		case PLAYERSTATE_NORMAL:
			ImGui::Text("normal");
			break;
		case PLAYERSTATE_FLOWER:
			ImGui::Text("flower");
			break;
		}

		if (ImGui::Button("BOSS"))
		{
			pos = D3DXVECTOR3(5972.14f, 100.0f, 8000.62f);
			SetPosition(pos);
		}

		if (ImGui::TreeNode("model"))
		{//プレイヤーのツリーノードを探します。なければ作成します。
			for (int nCount = 0; nCount < m_nNumParts; nCount++)
			{
				if (m_pModel != NULL)
				{
					D3DXVECTOR3 pos = m_pModel[nCount].GetPosition();													// 位置を取得
					ImGui::Text("pos%d = %.2f, %.2f, %.2f", nCount, pos.x, pos.y, pos.z);								// プレイヤーの現在位置を表示
				}
			}

			ImGui::TreePop();																		// 終了
		}
		if (ImGui::TreeNode("animation"))
		{//アニメーションのツリーノードを探します。なければ作成します。
			ImGui::Text("frame = %d", m_nCurrentFrame);
			ImGui::Text("Key = %d", m_nCurrentKey);
			ImGui::Text("animationType = %d", m_AnimationType);

			ImGui::TreePop();																		// 終了
}
	}

	//デバッグ処理を終了
	ImGui::End();
}
#endif