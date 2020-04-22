#ifdef _DEBUG
//=============================================================================
//
// デバッグ処理 [debug.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "inputMouse.h"
#include "camera.h"
#include "scene.h"
#include "circle.h"
#include "inputKeyboard.h"
#include "meshField.h"
#include "game.h"
#include "object.h"
#include "sceneX.h"
#include "collider.h"
#include "enemy.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define CONTINUE_MAX 10													// 再計算回数

//=============================================================================
// 静的メンバ変数
//=============================================================================
char CDebugProc::m_aStr[1024] = {};										// 文字列
D3DXVECTOR3 CDebugProc::m_mouseOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// マウスの前のワールド座標
CCircle	*CDebugProc::m_pCircle = NULL;									// 円のポインタ
CObject	*CDebugProc::m_pObject = NULL;									// 木のポインタ
CEnemy *CDebugProc::m_pEnemy = NULL;									// 敵のポインタ
float CDebugProc::m_fPaintSize = 1;										// 地形編集ブラシの大きさ
int CDebugProc::m_nCreateIndex = 0;										// 1フレームの生成数
bool CDebugProc::m_bDebug = false;										// デバッグモードの切り替え
int CDebugProc::m_nCntGeneration = 0;									// 生成数
int	CDebugProc::m_nType = 0;											// オブジェクトタイプ
int	CDebugProc::m_nMode = 0;											// モード選択
int	CDebugProc::m_nCntContinue = 0;										// 再確認回数

//=============================================================================
// コンストラクタ
//=============================================================================
CDebugProc::CDebugProc()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CDebugProc::~CDebugProc()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CDebugProc::Init(HWND hWnd)
{
	CRenderer *Renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = Renderer->GetDevice();

	// デバッグ表示用フォントを設定
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "ＭＳ ゴシック", &m_pFont);

	//IMGUIの初期化
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(pDevice);

	// 円の作成
	m_pCircle = CCircle::Create();
	return S_OK;
}

//=============================================================================
// 開放処理
//=============================================================================
void CDebugProc::Uninit(void)
{
	if (m_pCircle != NULL)
	{// 円のポインタが開放されていないとき
		m_pCircle->Uninit();														// 開放処理
		delete m_pCircle;															// 円の開放
		m_pCircle = NULL;															// 円のポインタをNULLに
	}

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	if (m_pFont != NULL)
	{// デバッグ表示用フォントの開放
		m_pFont->Release();
		m_pFont = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CDebugProc::Update(void)
{
	// フレーム開始
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// 自分で作成した簡単なウィンドウを表示します。 Begin / Endペアを使用して、名前付きウィンドウを作成します。
	ImGui::Begin("System");                          // 「System」というウィンドウを作成しますそれに追加します。

	ImGui::SameLine();
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::Text("rand = %d", CManager::GetRand(10));								// プレイヤーの現在位置を表示
	ImGui::End();

	Debug();
}

//=============================================================================
// 描画処理
//=============================================================================
void CDebugProc::Draw(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();	// デバイスの取得

	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	int nTime = 0;

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	// 円の描画
	if (m_bDebug)
	{
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// 裏面(左回り)をカリングする
		m_pCircle->Draw();
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面(左回り)をカリングする
	}

	//ImGuiの描画
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	// テキスト描画
	m_pFont->DrawText(NULL, &m_aStr[0], -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	*m_aStr = NULL;
}

//=============================================================================
// ログ表示処理
//=============================================================================
void CDebugProc::Log(char* frm, ...)
{
	va_list args;			// リストの取得
	char* c = "\n";			// 改行用

	va_start(args, frm);		// リストの先頭を取得

	vsprintf(&m_aStr[strlen(m_aStr)], frm, args);		// 文字に当てはめる

	va_end(args);						// リストを開放する
}

//=============================================================================
// デバッグ処理
//=============================================================================
void CDebugProc::Debug(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();		// キーボードの取得
	CCamera *pCamera = CManager::GetCamera();		// カメラ の取得

	ImGui::Begin("System");			// Systemウィンドウ の生成またはアクセス

	ImGui::Checkbox("DebugMode", &m_bDebug);		// 地形編集モード切り替え

	//デバッグ処理を終了
	ImGui::End();

	// デバッグウィンドウの生成
	if (m_bDebug)
	{
		ImGui::Begin("Debug", &m_bDebug, ImGuiWindowFlags_MenuBar);   // デバッグウィンドウ生成

		CRenderer *pRenderer = CManager::GetRenderer();
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();	// デバイスの取得

		pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);

		if (ImGui::BeginMenuBar())
		{// メニューバーの生成
			if (ImGui::BeginMenu("File"))
			{// ファイルタブの生成
				if (ImGui::MenuItem("Load"))
				{// ロード
					// 床情報の読み込み
					CMeshField::LoadRand("data/stage/center/rand.csv", false);

					// モデル情報の読み込み
					CObject::LoadModel("data/stage/center/object.csv");

					// 敵の配置情報
					CEnemy::LoadEnemy("data/stage/center/enemy.csv");
				}
				if (ImGui::MenuItem("Save"))
				{// セーブ
					// 床情報の書き込み
					CScene::SaveRand();

					// モデル情報の書き込み
					CScene::SaveModel();

					// 敵情報の書き込み
					CScene::SaveEnemy();
				}

				ImGui::EndMenu();			// メニューの更新終了
			}
			ImGui::EndMenuBar();		// メニューバーの更新終了
		}
		ImGui::Text("[R]Press is Create");			// デバッグモードテキスト表示

		if (pKeyboard->GetTriggerKeyboard(DIK_RCONTROL))
		{
			m_nMode = DEBUGMODE_NONE;
		}

		ImGui::Text("Rand DebugMode");			// デバッグモードテキスト表示

		D3DXVECTOR3 worldPos = pCamera->GetWorldPos();			// マウスのワールド座標を取得
		m_pCircle->SetPosition(worldPos);						// 生成範囲の位置を設定
		m_pCircle->SetRadius(m_fPaintSize);						// 生成範囲の大きさを設定
		m_pCircle->MakeVertex();								// 円の頂点生成

		CDebugProc::Log("マウスのワールド座標 : %f, %f, %f\n",			// マウスのワールド座標を出力
			worldPos.x,
			worldPos.y,
			worldPos.z);

		if (m_nMode != DEBUGMODE_INDIVIDUAL)
		{// 個々配置モードじゃないとき
			ImGui::SliderFloat("paintSize", &m_fPaintSize, 1.0f, 10000.0f);			// スライダーを使用して1つのフロートを編集します
		}

		if (m_nMode != DEBUGMODE_RAND)
		{// 地形編集モードじゃないとき
			ImGui::InputInt("Type", &m_nType);

			if (m_nType >= OBJTYPE_MAX)
			{// タイプが用意したモデルの個数を超えたとき
				m_nType = (int)OBJTYPE_TREE_1;										// モデルタイプをノーマルに設定
			}
			else if (m_nType < OBJTYPE_TREE_1)
			{// タイプが0以下に設定されているとき
				m_nType = (int)OBJTYPE_MAX - 1;										// タイプをモデルの最大値に設定
			}
		}

		ImGui::RadioButton("none", &m_nMode, DEBUGMODE_NONE);						// 選択肢 なし を追加
		ImGui::SameLine();															// 改行回避
		ImGui::RadioButton("rand", &m_nMode, DEBUGMODE_RAND);						// 選択肢 地面変形モード を追加
		ImGui::SameLine();															// 改行回避
		ImGui::RadioButton("many", &m_nMode, DEBUGMODE_MANY);						// 選択肢 範囲内多数生成モード を追加
		ImGui::SameLine();															// 改行回避
		if (ImGui::RadioButton("individual", &m_nMode, DEBUGMODE_INDIVIDUAL))		// 選択肢 個々配置モード を追加
		{
			if (m_pObject == NULL)
			{
				m_pObject = CObject::Create((OBJTYPE)m_nType);						// 見本用オブジェクトを作成

				if (m_pObject != NULL)
				{
					m_pObject->SetPosition(worldPos);								// 位置をマウスのワールド座標に設定
					m_pObject->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));			// 色の変更
				}
			}
		}
		ImGui::RadioButton("paint", &m_nMode, DEBUGMODE_PAINT);
		ImGui::SameLine();															// 改行回避
		if (ImGui::RadioButton("enemy", &m_nMode, DEBUGMODE_ENEMY))
		{
			if (m_pEnemy == NULL)
			{
				m_pEnemy = CEnemy::Create();			// 見本用オブジェクトを作成

				if (m_pEnemy != NULL)
				{
					m_pEnemy->SetPosition(worldPos);								// 位置をマウスのワールド座標に設定
				}
			}
		}
		ImGui::SameLine();															// 改行回避
		ImGui::RadioButton("delete", &m_nMode, DEBUGMODE_DELETE);					// 選択肢 範囲内多数生成モード を追加

		if (m_nMode != DEBUGMODE_INDIVIDUAL)
		{// 個々配置モードではなかったとき
			if (m_pObject != NULL)
			{// ポインタが存在していたとき
				m_pObject->Release();				// 開放予約
				m_pObject = NULL;
			}
		}

		if (m_nMode != DEBUGMODE_ENEMY)
		{// エネミー配置モードではなかったとき
			if (m_pEnemy != NULL)
			{// ポインタが存在していたとき
				m_pEnemy->Release();
				m_pEnemy = NULL;
			}
		}

		if (ImGui::Button("Reset"))
		{// Reset ボタンが押されたとき
			CScene::ResetFloor();					// 床の高低をリセット
		}

		if (m_nMode == DEBUGMODE_NONE)
		{// 何もしないモードだったとき
			ImGui::Text("None");

			// デバッグ終了ボタン
			if (ImGui::Button("Debug End"))
			{
				m_bDebug = false;
				m_nType = 0;
				for (int nCount = 0; nCount < OBJTYPE_MAX; nCount++)
				{
					if (m_pObject != NULL)
					{// 見本用のモデルがあったとき
						m_pObject->Uninit();		// 終了処理
						m_pObject->Release();		// メモリの開放
						m_pObject = NULL;			// NULLを代入
					}
				}
			}
		}
		else if (m_nMode == DEBUGMODE_RAND)
		{// 地形編集モードだったとき
			CreateRand(worldPos);

			ImGui::Text("Rand Debug");				// 現在のデバッグモードの表示
		}
		else if (m_nMode == DEBUGMODE_MANY)
		{// 範囲内多数生成モードだったとき
			ImGui::SliderInt("Generation", &m_nCntGeneration, 0, 50);
			CreateObject(worldPos);					// 多数配置モードの実行

			ImGui::Text("Many Debug");
		}
		else if (m_nMode == DEBUGMODE_INDIVIDUAL)
		{// 個々配置モードだったとき
			CreateIndividual(worldPos);				// 個々配置モードの実行

			// 現在のデバッグタイプを表示
			ImGui::Text("individual Debug");
		}
		else if (m_nMode == DEBUGMODE_DELETE)
		{// 個々配置モードだったとき
			DeleteObject(worldPos);					// 個々配置モードの実行

			// 現在のデバッグタイプを表示
			ImGui::Text("individual Debug");
		}
		else if (m_nMode == DEBUGMODE_PAINT)
		{// 地形編集モードだったとき
			Paint(worldPos);

			ImGui::Text("Paint Debug");				// 現在のデバッグモードの表示
		}
		else if (m_nMode == DEBUGMODE_ENEMY)
		{
			CreateEnemy(worldPos);
			ImGui::Text("Enemy Debug");				// 現在のデバッグモードの表示
		}
		// 更新終了
		ImGui::End();
	}
}

//=============================================================================
// 地形編集処理
//=============================================================================
void CDebugProc::CreateRand(D3DXVECTOR3 &worldPos)
{
	CInputMouse *pMouse = CManager::GetInputMouse();								// マウスの取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();						// キーボードの取得

	// 現在マウスの座標の範囲内にいる床を取得する
	CScene *pSceneNext = NULL;														// 初期化
	CScene *pSceneNow = CScene::GetScene(CScene::PRIORITY_FLOOR);					// 床の先頭アドレスの取得

	//次がなくなるまで繰り返す
	while (pSceneNow != NULL)
	{
		pSceneNext = CScene::GetSceneNext(pSceneNow, CScene::PRIORITY_FLOOR);		//次回アップデート対象を控える
		CMeshField *pField = (CMeshField*)pSceneNow;
		if (pField->CollisionRangeToCircle(worldPos, m_fPaintSize))					// 円が床の範囲内に入っているかどうか
		{
			if (m_mouseOld != worldPos)
			{// 前の座標と今回の座標が違うとき
				pField->CreateRand(worldPos, m_fPaintSize);
			}
		}
		pSceneNow = pSceneNext;														//次回アップデート対象を格納
	}

	if (!pKeyboard->GetPressKeyboard(DIK_LALT))
	{
		if (pKeyboard->GetPressKeyboard(DIK_R))
		{
			if (pMouse->GetReleaseMouse(MOUSE_RIGHT))
			{// マウスの右ボタンが離されたとき
				pSceneNow = CScene::GetScene(CScene::PRIORITY_FLOOR);					// 床の先頭アドレスの取得

				//次がなくなるまで繰り返す
				while (pSceneNow != NULL)
				{
					pSceneNext = CScene::GetSceneNext(pSceneNow, CScene::PRIORITY_FLOOR);		//次回アップデート対象を控える
					CMeshField *pField = (CMeshField*)pSceneNow;
					pField->CalculationNormalize();
					pSceneNow = pSceneNext;														//次回アップデート対象を格納
				}
			}
			else if (pMouse->GetReleaseMouse(MOUSE_LEFT))
			{// マウスの左ボタンが離されたとき
				pSceneNow = CScene::GetScene(CScene::PRIORITY_FLOOR);					// 床の先頭アドレスの取得

				//次がなくなるまで繰り返す
				while (pSceneNow != NULL)
				{
					pSceneNext = CScene::GetSceneNext(pSceneNow, CScene::PRIORITY_FLOOR);		//次回アップデート対象を控える
					CMeshField *pField = (CMeshField*)pSceneNow;
					pField->CalculationNormalize();
					pSceneNow = pSceneNext;														//次回アップデート対象を格納
				}
			}
		}
	}

	m_mouseOld = worldPos;
}

//=============================================================================
// 頂点色変更処理
//=============================================================================
void CDebugProc::Paint(D3DXVECTOR3 & worldPos)
{
	CInputMouse *pMouse = CManager::GetInputMouse();								// マウスの取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();						// キーボードの取得

	// 現在マウスの座標の範囲内にいる床を取得する
	CScene *pSceneNext = NULL;														// 初期化
	CScene *pSceneNow = CScene::GetScene(CScene::PRIORITY_FLOOR);					// 床の先頭アドレスの取得

	// 次がなくなるまで繰り返す
	while (pSceneNow != NULL)
	{
		pSceneNext = CScene::GetSceneNext(pSceneNow, CScene::PRIORITY_FLOOR);		// 次回アップデート対象を控える
		CMeshField *pField = (CMeshField*)pSceneNow;
		if (pField->CollisionRangeToCircle(worldPos, m_fPaintSize))					// 円が床の範囲内に入っているかどうか
		{
			if (m_mouseOld != worldPos)
			{// 前の座標と今回の座標が違うとき
				pField->Paint(worldPos, m_fPaintSize);
			}
		}
		pSceneNow = pSceneNext;														// 次回アップデート対象を格納
	}

	m_mouseOld = worldPos;
}

//=============================================================================
// 敵配置処理
//=============================================================================
void CDebugProc::CreateEnemy(D3DXVECTOR3 &worldPos)
{
	CInputMouse *pMouse = CManager::GetInputMouse();			// マウスの取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// キーボードの取得
	CScene *pScene = CScene::NowFloor(worldPos);				// 現在いる場所のフィールドを取得

	D3DXVECTOR3 pos = worldPos;

	if (pScene != NULL)
	{// 床が存在していたとき
		CMeshField *pMeshField = (CMeshField*)pScene;			// 床の取得
		pos.y = pMeshField->GetHeight(worldPos);				// 床の高さを取得
	}

	if (m_pEnemy != NULL)
	{
		m_pEnemy->SetPosition(pos);								// 見本用モデルの位置を現在のマウス座標に設定
	}

	if (!pKeyboard->GetPressKeyboard(DIK_LALT))
	{// Altキー が押されていないとき
		if (pKeyboard->GetPressKeyboard(DIK_R))
		{
			if (pMouse->GetTriggerMouse(MOUSE_LEFT))
			{// マウスの左ボタンが押されたとき
				CEnemy *pEnemy = CEnemy::Create();				// 現在の見本を作成
				pEnemy->SetPosition(worldPos);					// 見本の場所に設置
			}
		}
	}
}

//=============================================================================
// 複数配置処理
//=============================================================================
void CDebugProc::CreateObject(D3DXVECTOR3 &worldPos)
{
	CInputMouse *pMouse = CManager::GetInputMouse();				// マウスの取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();		// キーボードの取得
	CCamera *pCamera = CManager::GetCamera();						// カメラの取得
	CMeshField *pMeshField = CGame::GetMesh();
	CObject *pObject = NULL;

	if (!pKeyboard->GetPressKeyboard(DIK_LALT))
	{// 左Altキーが押されていないとき
		if (pKeyboard->GetPressKeyboard(DIK_R))
		{
			if (pMouse->GetPressMouse(MOUSE_LEFT))
			{// マウスの左ボタンが押されているとき
				D3DXVECTOR3 Difference;				// 差分
				float fDistance;					// 距離

				// 前の位置と現在の位置との距離を算出
				Difference.x = m_mouseOld.x - worldPos.x;
				Difference.z = m_mouseOld.z - worldPos.z;
				fDistance = (float)sqrt(Difference.x * Difference.x + Difference.z * Difference.z);

				if (fDistance > m_fPaintSize)
				{// 前の座標と今回の座標が違うとき
					// モデルの範囲配置
					for (int nCount = 0; nCount < m_nCntGeneration; nCount++)
					{
						float fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						D3DXVECTOR3 pos = D3DXVECTOR3(sinf(D3DX_PI + 6.28f * fAngle) * ((float)CManager::GetRand((int)m_fPaintSize * 10) * 0.1f) + worldPos.x,
							0.0f,
							cosf(D3DX_PI + 6.28f * fAngle) * ((float)CManager::GetRand((int)m_fPaintSize * 10) * 0.1f) + worldPos.z);

						//床の高さを取得する
						CScene *pSceneNext = NULL;			// 初期化
						CScene *pSceneNow = CScene::GetScene(CScene::PRIORITY_MODEL);			// シーンの先頭アドレスを取得

						//次がなくなるまで繰り返す
						while (pSceneNow != NULL)
						{
							pSceneNext = CScene::GetSceneNext(pSceneNow, CScene::PRIORITY_MODEL);		// 次回アップデート対象を控える
							CMeshField *pField = (CMeshField*)pSceneNow;								// クラスチェンジ(床)

							if (m_nCntContinue > CONTINUE_MAX)
							{// 再計算回数が10回を超えたとき
								break;
							}
							else if (50.0f > CManager::GetDistance(pSceneNow->GetPosition(), pos))
							{
								// 値の再取得
								float fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
								D3DXVECTOR3 pos = D3DXVECTOR3(sinf(D3DX_PI + 6.28f * fAngle) * ((float)CManager::GetRand((int)m_fPaintSize * 10) * 0.1f) + worldPos.x,
									0.0f,
									cosf(D3DX_PI + 6.28f * fAngle) * ((float)CManager::GetRand((int)m_fPaintSize * 10) * 0.1f) + worldPos.z);

								pSceneNext = CScene::GetScene(CScene::PRIORITY_MODEL);			// シーンの先頭アドレスを取得
								m_nCntContinue++;					// 再確認回数にプラスする
							}

							pSceneNow = pSceneNext;				//次回アップデート対象を格納
						}

						if (m_nCntContinue < CONTINUE_MAX)
						{
							// オブジェクトの作成
							pObject = CObject::Create((OBJTYPE)m_nType);
						}

						m_nCntContinue = 0;

						if (pObject != NULL)
						{
							//床の高さを取得する
							CScene *pSceneNext = NULL;			// 初期化
							CScene *pSceneNow = CScene::GetScene(CScene::PRIORITY_FLOOR);			// シーンの先頭アドレスを取得

							//次がなくなるまで繰り返す
							while (pSceneNow != NULL)
							{
								pSceneNext = CScene::GetSceneNext(pSceneNow, CScene::PRIORITY_FLOOR);		// 次回アップデート対象を控える
								CMeshField *pField = (CMeshField*)pSceneNow;								// クラスチェンジ(床)

								if (pField->CollisionRange(pos))
								{// オブジェクト が床に乗っていたとき
									pos.y = pField->GetHeight(pos);										// 床の高さを求める
									pObject->SetPosition(pos);
									break;
								}

								pSceneNow = pSceneNext;				//次回アップデート対象を格納
							}
						}
					}

					m_mouseOld = worldPos;					// 最後の加工位置を保存
				}
			}
		}
	}
}

//=============================================================================
// 単体配置処理
//=============================================================================
void CDebugProc::CreateIndividual(D3DXVECTOR3 &worldPos)
{
	CInputMouse *pMouse = CManager::GetInputMouse();								// マウスの取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();						// キーボードの取得
	CScene *pScene = CScene::NowFloor(worldPos);									// 現在いる場所のフィールドを取得

	D3DXVECTOR3 pos = worldPos;
	D3DXVECTOR3 rot;

	if (m_pObject != NULL)
	{// 見本用オブジェクトが存在していたとき
		rot = m_pObject->GetRotation();					// 回転値の取得
	}

	ImGui::DragFloat3("rot", (float*)&rot, 0.01f);

	if (pScene != NULL)
	{// 床が存在していたとき
		CMeshField *pMeshField = (CMeshField*)pScene;								// 床の取得
		pos.y = pMeshField->GetHeight(worldPos);									// 床の高さを取得
	}

	if (m_pObject != NULL)
	{
		if (m_pObject->GetType() != (OBJTYPE)m_nType)
		{// 現在のモデルタイプが前回のモデルと違うとき
			m_pObject->Uninit();															// 前のモデルを開放
			m_pObject->Release();															// 前のモデルの開放フラグを立てる
			m_pObject = NULL;																// 前のモデルのアドレスを捨てる

			if (m_pObject == NULL)
			{// モデルのアドレスが NULL だったとき
				m_pObject = CObject::Create((OBJTYPE)m_nType);								// 新しいモデルを生成
			}
		}
	}

	if (m_pObject != NULL)
	{// 見本用オブジェクトが存在していたとき
		m_pObject->SetPosition(pos);														// 見本用モデルの位置を現在のマウス座標に設定
		m_pObject->SetRotation(rot);					// 回転値の取得
	}

	if (!pKeyboard->GetPressKeyboard(DIK_LALT))
	{// Altキー が押されていないとき
		if (pKeyboard->GetPressKeyboard(DIK_R))
		{
			if (pMouse->GetTriggerMouse(MOUSE_LEFT))
			{// マウスの左ボタンが押されたとき
				CObject *pObject = CObject::Create((OBJTYPE)m_nType);							// 現在の見本を作成
				pObject->SetPosition(worldPos);												// 見本の場所に設置
				pObject->SetRotation(rot);
			}
		}
	}
}

//=============================================================================
// オブジェクト範囲内削除処理
//=============================================================================
void CDebugProc::DeleteObject(D3DXVECTOR3 &worldPos)
{
	CInputMouse *pMouse = CManager::GetInputMouse();								// マウスの取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();						// キーボードの取得

	// 現在マウスの座標の範囲内にいる床を取得する
	CScene *pSceneNext = NULL;														// 初期化
	CScene *pSceneNow = CScene::GetScene(CScene::PRIORITY_MODEL);					// 床の先頭アドレスの取得

	if (pKeyboard->GetPressKeyboard(DIK_R))
	{
		if (pMouse->GetPressMouse(MOUSE_LEFT))
		{// マウスの右ボタンが離されたとき
		// 次がなくなるまで繰り返す
			while (pSceneNow != NULL)
			{
				pSceneNext = CScene::GetSceneNext(pSceneNow, CScene::PRIORITY_MODEL);		//次回アップデート対象を控える
				CSceneX *pObject = (CSceneX*)pSceneNow;

				if (CMeshField::SphereModel(pObject->GetPosition(), worldPos, m_fPaintSize))					// 円が床の範囲内に入っているかどうか
				{
					pObject->Release();
				}
				pSceneNow = pSceneNext;														//次回アップデート対象を格納
			}
		}
	}
}
#endif