//=============================================================================
//
// マネージャー処理 [manager.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "fade.h"
#include "game.h"
#include "title.h"
#include "result.h"
#include "ranking.h"
#include "inputKeyboard.h"
#include "inputMouse.h"
#include "inputController.h"
#include "scene2D.h"
#include "camera.h"
#include "collider.h"
#include "light.h"
#include "sound.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
CRenderer *CManager::m_pRenderer = NULL;											// レンダラー ポインタを初期化
CInputKeyboard *CManager::m_pInputKeyboard = NULL;									// キーボード ポインタを初期化
CInputMouse *CManager::m_pInputMouse = NULL;										// マウス ポインタを初期化
CInputController *CManager::m_pInputController = NULL;								// コントローラー ポインタを初期化
CManager::MODE CManager::m_mode = CManager::MODE_NONE;								// モード 変数の初期化

CCamera *CManager::m_pCamera = NULL;												// カメラ ポインタを初期化
CLight *CManager::m_pLight = NULL;													// ライト ポインタを初期化

CGame *CManager::m_pGame = NULL;													// ゲーム ポインタを初期化
CTitle *CManager::m_pTitle = NULL;													// タイトル ポインタを初期化
CResult *CManager::m_pResult = NULL;												// リザルト ポインタを初期化
CRanking *CManager::m_pRanking = NULL;												// ランキング ポインタを初期化

CSound *CManager::m_pSound = NULL;													// サウンド ポインタを初期化

std::map<std::string, LPDIRECT3DTEXTURE9> CManager::m_TexMap = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CManager::CManager()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CManager::~CManager()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	// 値リセット
	srand((unsigned int)time(NULL));

	m_pRenderer = new CRenderer;																//レンダラーのクリエイト
	if (m_pRenderer != NULL)
	{// レンダラーが存在していたとき
	 // レンダラーの初期化
		if (FAILED(m_pRenderer->Init(hInstance, hWnd, bWindow)))
		{
			MessageBox(hWnd, "レンダラーの初期化に失敗", "警告", MB_ICONWARNING);
			return E_FAIL;
		}
	}

	m_pInputKeyboard = new CInputKeyboard;														//キーボードのクリエイト

	if (m_pInputKeyboard != NULL)
	{// キーボードが存在していたとき
	 // キーボードの初期化
		if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
		{
			MessageBox(hWnd, "キーボードの初期化に失敗", "警告", MB_ICONWARNING);
			return E_FAIL;
		}
	}

	m_pInputMouse = new CInputMouse;															//マウスのクリエイト

	if (m_pInputMouse != NULL)
	{// マウスが存在していたとき
		// マウスの初期化
		if (FAILED(m_pInputMouse->Init(hInstance, hWnd)))
		{
			MessageBox(hWnd, "マウスの初期化に失敗", "警告", MB_ICONWARNING);
			return E_FAIL;
		}
	}

	m_pInputController = new CInputController;													// コントローラーの生成

	if (m_pInputController != NULL)
	{// コントローラーが存在していたとき
		// コントローラーの初期化
		if (FAILED(m_pInputController->Init(hInstance, hWnd)))
		{
			MessageBox(hWnd, "コントローラーの初期化に失敗", "警告", MB_ICONWARNING);
			return E_FAIL;
		}
	}

	LPDIRECT3DDEVICE9 pDevice;
	pDevice = m_pRenderer->GetDevice();

	m_pCamera = new CCamera;
	m_pCamera->Init();

	m_pLight = new CLight;																		//ライトのクリエイト
	m_pLight->Init();

	m_pSound = new CSound;
	m_pSound->Init(hWnd);

	CTitle::LoadAsset();
	CGame::LoadAsset();
	CResult::LoadAsset();

	Load("data/model/akazukin/clothes.jpg");
	Load("data/model/akazukin/hair.jpg");
	Load("data/model/akazukin/skin.jpg");
	Load("data/model/akazukin/skirt.png");

	CGame::LoadAsset();

	SetMode(MODE_TITLE);																		//モードセレクト

	return S_OK;
}

//=============================================================================
// 開放処理
//=============================================================================
void CManager::Uninit(void)
{
	// テクスチャの開放
	TexRelease();

	// キーボードの開放処理
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Uninit();																	// キーボードの終了処理
		delete m_pInputKeyboard;																	// キーボードのメモリ解放
		m_pInputKeyboard = NULL;																	// ポインタをNULLにする
	}

	// マウスの開放処理
	if (m_pInputMouse != NULL)
	{
		m_pInputMouse->Uninit();																	// マウスの終了処理
		delete m_pInputMouse;																		// マウスのメモリ解放
		m_pInputMouse = NULL;																		// ポインタをNULLにする
	}

	// コントローラーの開放処理
	if (m_pInputController != NULL)
	{
		m_pInputController->Uninit();																// コントローラーの終了処理
		delete m_pInputController;																	// コントローラーのメモリ解放
		m_pInputController = NULL;																	// ポインタをNULLにする
	}

	// サウンドの開放処理
	if (m_pSound != NULL)
	{
		m_pSound->Uninit();																			// Soundの終了処理
		delete m_pSound;																			// Soundのメモリ解放
		m_pSound = NULL;																			// ポインタをNULLにする
	}

	// カメラの開放処理
	if (m_pCamera != NULL)
	{
		m_pCamera->Uninit();																		// カメラの終了処理
		delete m_pCamera;																			// カメラのメモリ解放
		m_pCamera = NULL;																			// ポインタをNULLにする
	}

	// ライトの開放処理
	if (m_pLight != NULL)
	{
		m_pLight->Uninit();																			// ライトの終了処理
		delete m_pLight;																			// ライトのメモリ解放
		m_pLight = NULL;																			// ポインタをNULLにする
	}

	// Sceneの解放処理
	CScene::ReleaseAll();

	// タイトルの開放処理
	if (m_pTitle != NULL)
	{
		m_pTitle->Uninit();																			// タイトルの終了処理
		delete m_pTitle;																			// タイトルのメモリ解放
		m_pTitle = NULL;																			// ポインタをNULLにする
	}

	// ゲームの開放処理
	if (m_pGame != NULL)
	{
		m_pGame->Uninit();																			// ゲームの終了処理
		delete m_pGame;																				// ゲームのメモリ解放
		m_pGame = NULL;																				// ポインタをNULLにする
	}

	// リザルトの開放処理
	if (m_pResult != NULL)
	{
		m_pResult->Uninit();																		// リザルトの終了処理
		delete m_pResult;																			// リザルトのメモリ解放
		m_pResult = NULL;																			// ポインタをNULLにする
	}

	// ランキングの開放処理
	if (m_pRanking != NULL)
	{
		m_pRanking->Uninit();																		// ランキングの終了処理
		delete m_pRanking;																			// ランキングのメモリ解放
		m_pRanking = NULL;																			// ポインタをNULLにする
	}

	// レンダラーの開放処理
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();																		// Rendererの終了処理
		delete m_pRenderer;																			// Rendererのメモリ解放
		m_pRenderer = NULL;																			// ポインタをNULLにする
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CManager::Update(void)
{
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Update();
	}

	if (m_pInputMouse != NULL)
	{
		m_pInputMouse->Update();
	}

	if (m_pInputController != NULL)
	{
		m_pInputController->Update();
	}

	if (m_pInputKeyboard->GetTriggerKeyboard(DIK_G))
	{
		m_pSound->PlaySoundA(SOUND_LABEL_BGM000);
	}

	switch (m_mode)
	{
	case CManager::MODE_TITLE:
		if (m_pTitle != NULL)
		{
			m_pTitle->Update();
		}
		break;
	case CManager::MODE_GAME:
		if (m_pGame != NULL)
		{
			m_pGame->Update();
		}
		break;
	case CManager::MODE_RESULT:
		if (m_pResult != NULL)
		{
			m_pResult->Update();
		}
		break;
	case CManager::MODE_RANKING:
		if (m_pRanking != NULL)
		{
			m_pRanking->Update();
		}
		break;
	}

	if (m_pRenderer != NULL)
	{
		m_pRenderer->Update();
	}

	if (m_pSound != NULL)
	{
		m_pSound->Update();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CManager::Draw(void)
{
#ifdef _DEBUG
	ImGui::EndFrame();
#endif

	// 描画
	m_pRenderer->Draw();
}

//========================================================================================
// モードの設定
//========================================================================================
void CManager::SetMode(MODE mode)
{
	m_pSound->StopSound();
	switch (m_mode)
	{
	case MODE_TITLE:
		m_pTitle->Uninit();
		delete m_pTitle;
		m_pTitle = NULL;
		break;

	case MODE_GAME:
		m_pGame->Uninit();
		delete m_pGame;
		m_pGame = NULL;
		break;

	case MODE_RESULT:
		m_pResult->Uninit();
		delete m_pResult;
		m_pResult = NULL;
		break;

	case MODE_RANKING:
		m_pRanking->Uninit();
		delete m_pRanking;
		m_pRanking = NULL;
		break;
	}

	//現在のモードを代入
	m_mode = mode;

	switch (mode)
	{
	case MODE_TITLE:
		m_pTitle = new CTitle;
		m_pTitle->Init();
		break;

	case MODE_GAME:
		m_pGame = new CGame;
		m_pGame->Init();
		m_pSound->PlaySoundA(SOUND_LABEL_BGM000);
		break;
	case MODE_RESULT:
		m_pResult = new CResult;
		m_pResult->Init();
		break;

	case MODE_RANKING:
		m_pRanking = new CRanking;
		m_pRanking->Init();
		break;
	}
}

//=============================================================================
// 形式変換(float)関数
//=============================================================================
void CManager::ConvertStringToFloat(char* text, char* delimiter, float* pResult)
{
	char* tp;
	char* ctx;
	int nTemp = 1;

	tp = strtok_s(text, delimiter, &ctx);
	pResult[0] = (float)atof(tp);

	while (tp = strtok_s(NULL, ",", &ctx))
	{
		pResult[nTemp++] = (float)atof(tp);
	}
}

//=============================================================================
// 2点間の距離算出処理
//=============================================================================
float CManager::GetDistance(D3DXVECTOR3 FirstTarget, D3DXVECTOR3 SecondTarget)
{
	return ((FirstTarget.x - SecondTarget.x) * (FirstTarget.x - SecondTarget.x) +
		(FirstTarget.y - SecondTarget.y) * (FirstTarget.y - SecondTarget.y) +
		(FirstTarget.z - SecondTarget.z) * (FirstTarget.z - SecondTarget.z));
}

//=============================================================================
// Rand関数
//=============================================================================
int CManager::GetRand(int nValue)
{
	for (int nCount = 0; nCount < 5; nCount++)
	{
		rand();						//randの空実行
	}

	//ランダムな値を返す
	int nAnswer = rand() % (nValue);

	int Wrok = ((int)rand() + 1) / ((int)RAND_MAX + 2);

	//float frand(unsigned v) {
	unsigned res = (1 >> 9) | 0x3f800000;
	float work = (*(float*)&res) - 1.0f;
	//}
	return nAnswer;
}

//=============================================================================
// イージング(Out)
//=============================================================================
float CManager::easeOut(float fValue)
{
	return fValue * (2 - fValue);
}

//=============================================================================
// イージング(In)
//=============================================================================
float CManager::easeIn(float fTime, float fStart, float fDifference, float fTotal)
{
	fTime /= fTotal;
	return fDifference * fTime * fTime * fTime + fStart;
}

//=============================================================================
// イージング(InAndOut)
//=============================================================================
float CManager::easeInAndOut(float fTime, float fStart, float fDifference, float fTotal)
{
	fTime /= fTotal / 2.0f;

	if (fTime < 1)
	{
		return fDifference / 2.0f * fTime * fTime * fTime + fStart;
	}

	fTime = fTime - 2;
	return fDifference / 2.0f * (fTime * fTime * fTime + 2) + fDifference;
}

//=============================================================================
// 指定したテクスチャを使える状態にする処理
//=============================================================================
HRESULT CManager::Load(std::string Add)
{
	LPDIRECT3DTEXTURE9 tex = NULL;
	std::map<std::string, LPDIRECT3DTEXTURE9>::const_iterator it = m_TexMap.find(Add);

	if (it == m_TexMap.end())
	{// 見つからなかったとき
	 // 検索した文字列をキーとして
	 // 新しくマップにテクスチャを追加する
		CRenderer *pRenderer = m_pRenderer;
		LPDIRECT3DDEVICE9 pDevice;
		HRESULT hr;
		LPDIRECT3DTEXTURE9 tex;

		//デバイスを取得する
		pDevice = pRenderer->GetDevice();

		hr = D3DXCreateTextureFromFile(pDevice, Add.c_str(), &tex);


		if (FAILED(hr))
		{// 生成できなかったときはNULLを返す
			MessageBox(NULL, Add.c_str(), "LOAD ERROR", MB_OK);
			return hr;			// 結果を返す
		}

		// マップへ挿入する
		m_TexMap.insert(std::map<std::string, LPDIRECT3DTEXTURE9>::value_type(Add, tex));
		return hr;				// 結果を返す
	}

	return E_FAIL;
}

//=============================================================================
// 指定したテクスチャをもらう処理
//=============================================================================
LPDIRECT3DTEXTURE9 CManager::GetResource(std::string Add)
{
	LPDIRECT3DTEXTURE9 tex = NULL;
	std::map<std::string, LPDIRECT3DTEXTURE9>::const_iterator it = m_TexMap.find(Add);

	if (it == m_TexMap.end())
	{// 見つからなかったとき
	 // 検索した文字列をキーとして
	 // 新しくマップにテクスチャを追加する
		CRenderer *pRenderer = m_pRenderer;
		LPDIRECT3DDEVICE9 pDevice;
		HRESULT hr;
		LPDIRECT3DTEXTURE9 tex;

		//デバイスを取得する
		pDevice = pRenderer->GetDevice();

		hr = D3DXCreateTextureFromFile(pDevice, Add.c_str(), &tex);


		if (FAILED(hr))
		{// 生成できなかったときはNULLを返す
			//MessageBox(NULL, Add.c_str(), "LOAD ERROR", MB_OK);
			return NULL;
		}

		// マップへ挿入する
		m_TexMap.insert(std::map<std::string, LPDIRECT3DTEXTURE9>::value_type(Add, tex));
		return tex;
	}
	else
	{
		return it->second;
	}
}

//=============================================================================
// システム設定ファイル読込
//=============================================================================
void CManager::LoadSystemFile(void)
{
	FILE *pFile;																	// ファイル
	char cReadText[128];															// 文字
	char cHeadText[128];															// 比較

	pFile = fopen("data/system.ini", "r");											// ファイルを開くまたは作る

	if (pFile != NULL)																// ファイルが読み込めた場合
	{
		if (pFile != NULL)
		{
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

					}
					else if (strcmp(cHeadText, "CameraOffset") == 0)
					{// 追跡するカメラのオフセット

					}
				}
			}
			fclose(pFile);																// ファイルを閉じる

			MessageBox(NULL, "モデル情報の読込に成功！", "SUCCESS", MB_ICONASTERISK);		// メッセージボックスの生成
		}
		else
		{
			MessageBox(NULL, "モデル情報のアクセス失敗！", "WARNING", MB_ICONWARNING);	// メッセージボックスの生成
		}
	}
}

//=============================================================================
// テクスチャの開放
//=============================================================================
void CManager::TexRelease(void)
{
	for (auto itr = m_TexMap.begin(); itr != m_TexMap.end(); itr++)
	{
		// テクスチャの開放
		if (itr->second != NULL)
		{
			itr->second->Release();
			itr->second = NULL;
		}
	}
}