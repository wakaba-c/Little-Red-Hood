//=============================================================================
//
// scene処理 [scene.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "scene.h"
#include "game.h"
#include "renderer.h"
#include "manager.h"
#include "light.h"
#include "meshField.h"
#include "meshCube.h"
#include "meshSphere.h"
#include "enemy.h"
#include "player.h"
#include "camera.h"
#include "debug.h"
#include "object.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
CScene *CScene::m_apTop[CScene::PRIORITY_MAX] = {};
CScene *CScene::m_apCur[CScene::PRIORITY_MAX] = {};
int CScene::m_nNumAll = 0;

//=============================================================================
// コンストラクタ
//=============================================================================
CScene::CScene(CScene::PRIORITY Type)
{
	if (m_apTop[Type] == NULL)
	{
		m_apTop[Type] = this;																	//自分をトップに設定
	}

	if (m_apCur[Type] != NULL)
	{
		m_apCur[Type]->m_pNext[Type] = this;													//最後尾の次を自分に
	}

	m_pPrev[Type] = m_apCur[Type];																//自分の前に最後尾を入れる
	m_apCur[Type] = this;																		//最後尾を自分にする
	m_pNext[Type] = NULL;																		//自分の次をNULL
	m_Obj = Type;																				//オブジェクトタイプを定義
	m_nNumAll++;																				//シーン数をプラスする
	m_bActive = true;
	m_bDie = false;

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// デストラクタ
//=============================================================================
CScene::~CScene()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CScene::Init(void)
{
	return S_OK;
}

//=============================================================================
// 開放処理
//=============================================================================
void CScene::Uninit(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void CScene::Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void CScene::Draw(void)
{

}

//====================================================================
// 全てのオブジェクトを破開放
//====================================================================
void CScene::ReleaseAll(void)
{
	CScene *pSceneNext = NULL;																	//次回削除対象
	CScene *pSceneNow = NULL;

	CCollider::ReleaseAll();

	for (int nCount = 0; nCount < PRIORITY_MAX; nCount++)
	{
		pSceneNow = m_apTop[nCount];

		//死亡フラグを確認
		while (pSceneNow != NULL)
		{
			pSceneNext = pSceneNow->m_pNext[nCount];											//次回フラグ確認対象を控える

			pSceneNow->Delete();																//削除
			pSceneNow = NULL;

			pSceneNow = pSceneNext;																//次回フラグ確認対象を格納
		}
	}
}

//====================================================================
// 全てのオブジェクトを更新
//====================================================================
void CScene::UpdateAll(void)
{
	CScene *pSceneNext = NULL;																	//次回アップデート対象
	CScene *pSceneNow = NULL;

	for (int nCount = 0; nCount < PRIORITY_MAX; nCount++)
	{
		pSceneNow = m_apTop[nCount];

		//次がなくなるまで繰り返す
		while (pSceneNow != NULL)
		{
			pSceneNext = pSceneNow->m_pNext[nCount];											// 次回アップデート対象を控える

			if (pSceneNow->GetActive())
			{
				pSceneNow->Update();																// アップデート
			}
			pSceneNow = pSceneNext;																// 次回アップデート対象を格納
		}
	}

	// 当たり判定
	CCollider::UpdateAll();

	for (int nCount = 0; nCount < PRIORITY_MAX; nCount++)
	{
		pSceneNow = m_apTop[nCount];

		//死亡フラグを確認
		while (pSceneNow != NULL)
		{
			pSceneNext = pSceneNow->m_pNext[nCount];											// 次回フラグ確認対象を控える

			if (pSceneNow->m_bDie)
			{
				pSceneNow->Delete();															// 削除
				pSceneNow = NULL;																// NULLを代入
			}

			pSceneNow = pSceneNext;																// 次回フラグ確認対象を格納
		}
	}
}

//====================================================================
// 全てのオブジェクトを描画
//====================================================================
void CScene::DrawAll(void)
{
	CScene *pSceneNext = NULL;																	// 次回描画対象
	CScene *pSceneNow = NULL;
	for (int nCount = 0; nCount < PRIORITY_MAX; nCount++)
	{
		pSceneNow = m_apTop[nCount];

		//次がなくなるまで繰り返す
		while (pSceneNow != NULL)
		{
			CCamera *pCamera = CManager::GetCamera();
			pSceneNext = pSceneNow->m_pNext[nCount];											//次回描画対象を控える

			if (pSceneNow->GetActive())
			{
				if (pSceneNow->GetObjType() == PRIORITY_BG || pSceneNow->GetObjType() == PRIORITY_SKY || pSceneNow->GetObjType() == PRIORITY_ORBIT ||
					pSceneNow->GetObjType() == PRIORITY_UI || pSceneNow->GetObjType() == PRIORITY_EFFECT)
				{
					pSceneNow->Draw();																	//描画
				}
#ifdef _DEBUG
				else if (CDebugProc::GetDebugState())
				{
					pSceneNow->Draw();																	//描画
				}
#endif
				else if (pCamera->VFCulling(pSceneNow->GetPosition(), nCount, D3DXToRadian(45.0f), 1.0f, 1000.0f))
				{
					pSceneNow->Draw();																	//描画
				}
				else if (pSceneNow->GetObjType() == PRIORITY_ENEMY)
				{
					CEnemy *pEnemy = (CEnemy*)pSceneNow;
					if (pEnemy->GetTarget() == TARGETTYPE_HOUSE)
					{
						pSceneNow->Draw();
					}
				}
			}
			pSceneNow = pSceneNext;																//次回描画対象を格納
		}
	}
}

//====================================================================
// 自分の削除予約
//====================================================================
void CScene::Release(void)
{
	if (!m_bDie)
	{
		m_bDie = true;
	}
}

//====================================================================
// オブジェクトタイプの設定
//====================================================================
void CScene::SetObjType(PRIORITY obj)
{
	m_Obj = obj;
}

//====================================================================
// 更新描画対象設定
//====================================================================
void CScene::SetActive(bool bValue)
{
	m_bActive = bValue;
}

//=============================================================================
// 影の設定
//=============================================================================
void CScene::SetShadow(LPDIRECT3DDEVICE9 pDevice, D3DXMATRIX mtxWorld, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPD3DXMESH pMesh, D3DXVECTOR3 pos)
{
	D3DXMATRIX mtxShadow;
	D3DMATERIAL9 matShadow;
	D3DXPLANE planeField;
	D3DXVECTOR4 vecLight;
	D3DXVECTOR3 workPos, normal;

	D3DXMATERIAL	*pMat;							//現在のマテリアル保存用
	D3DMATERIAL9	matDef;							//マテリアルデータへのポインタ

	CLight *pLight = CManager::GetLight();
	D3DLIGHT9 light = pLight->GetLight(0);
	D3DXVECTOR3 lightDef = light.Direction;

	lightDef *= -1;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxShadow);

	vecLight = D3DXVECTOR4(lightDef.x, lightDef.y, lightDef.z, 0.0f);

	workPos = D3DXVECTOR3(pos.x, 85.0f, pos.z);
	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	D3DXPlaneFromPointNormal(&planeField, &workPos, &normal);
	D3DXMatrixShadow(&mtxShadow, &vecLight, &planeField);

	// ポリゴンにあった高さ分影を計算する
	D3DXMatrixMultiply(&mtxShadow, &mtxWorld, &mtxShadow);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);					//すべてのデータ収録

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)nNumMat; nCntMat++)
	{
		matShadow = pMat[nCntMat].MatD3D;

		matShadow.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		// マテリアルの設定
		pDevice->SetMaterial(&matShadow);

		// 描画
		pMesh->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

//====================================================================
// 現在立っている床の面
//====================================================================
CScene *CScene::NowFloor(D3DXVECTOR3 pos)
{
	CScene *pSceneNext = NULL;														//次回アップデート対象
	CScene *pSceneNow = NULL;

	pSceneNow = m_apTop[PRIORITY_FLOOR];

	//次がなくなるまで繰り返す
	while (pSceneNow != NULL)
	{
		pSceneNext = pSceneNow->m_pNext[PRIORITY_FLOOR];							//次回アップデート対象を控える

		CMeshField *pField = (CMeshField*)pSceneNow;								// クラスチェンジ(床)

		if (pField->CollisionRange(pos))
		{// プレイヤー が床に乗っていたとき
			return pField;
		}

		pSceneNow = pSceneNext;														//次回アップデート対象を格納
	}

	return NULL;
}

//====================================================================
// 床の高さをリセット
//====================================================================
void CScene::ResetFloor(void)
{
	CScene *pSceneNext = NULL;														//次回アップデート対象
	CScene *pSceneNow = NULL;

	pSceneNow = m_apTop[PRIORITY_FLOOR];

	//次がなくなるまで繰り返す
	while (pSceneNow != NULL)
	{
		pSceneNext = pSceneNow->m_pNext[PRIORITY_FLOOR];							//次回アップデート対象を控える

		CMeshField *pField = (CMeshField*)pSceneNow;								// クラスチェンジ(床)

		if (pField != NULL)
		{// プレイヤー が床に乗っていたとき
			pField->Reset();
		}

		pSceneNow = pSceneNext;														//次回アップデート対象を格納
	}
}

//====================================================================
// 位置の取得
//====================================================================
void CScene::SetPosition(D3DXVECTOR3 pos)
{
	SetPosOld(m_pos);
	m_pos = pos;
}

//====================================================================
// 前回位置の取得
//====================================================================
void CScene::SetPosOld(D3DXVECTOR3 pos)
{
	m_posOld = pos;
}

//====================================================================
// シーンを取得
//====================================================================
CScene *CScene::GetScene(CScene::PRIORITY obj)
{
	return m_apTop[obj];
}

//====================================================================
// 次のシーンを取得
//====================================================================
CScene *CScene::GetSceneNext(CScene *pScene, PRIORITY type)
{
	return pScene->m_pNext[type];
}

//=============================================================================
// 当たり判定 トリガー
//=============================================================================
void CScene::OnTriggerEnter(CCollider *col)
{

}

//=============================================================================
// 当たり判定 コリジョン
//=============================================================================
void CScene::OnCollisionEnter(CCollider *col)
{

}

//====================================================================
// 自分の削除
//====================================================================
void CScene::Delete(void)
{
	if (this != NULL)
	{
		//終了処理及びメモリ解放
		this->Uninit();

		if (this->m_pPrev[m_Obj] == NULL)
		{//自分が先頭だったとき

			if (this->m_pNext[m_Obj] != NULL)
			{//自分の次のオブジェクトがあるとき
				m_apTop[m_Obj] = this->m_pNext[m_Obj];											//先頭を次のオブジェクトに譲渡する

				if (m_apTop[m_Obj]->m_pPrev[m_Obj] != NULL)
				{//前の人の情報がある場合
					m_apTop[m_Obj]->m_pPrev[m_Obj] = NULL;										//前にあるオブジェクト情報を NULL にする
				}
			}
			else
			{//自分の次のオブジェクトがなかったとき
				m_apTop[m_Obj] = NULL;
				m_apCur[m_Obj] = NULL;
			}
		}
		else if (this->m_pNext[m_Obj] == NULL)
		{//自分が最後尾だったとき
			m_apCur[m_Obj] = m_pPrev[m_Obj];													//最後尾を前のオブジェクトに譲渡する

			if (m_apCur[m_Obj]->m_pNext[m_Obj] != NULL)
			{
				m_apCur[m_Obj]->m_pNext[m_Obj] = NULL;											//後ろにあるオブジェクト情報を NULL にする
			}
		}
		else
		{//どちらでもない場合
			m_pNext[m_Obj]->m_pPrev[m_Obj] = m_pPrev[m_Obj];									//次のオブジェクトの前のオブジェクト情報に自分の前のオブジェクト情報を格納する
			m_pPrev[m_Obj]->m_pNext[m_Obj] = m_pNext[m_Obj];									//前のオブジェクトの次のオブジェクト情報に自分の次のオブジェクト情報を格納する
		}

		delete this;
		m_nNumAll--;																			//生成数を一つ減らす
	}
}

#ifdef _DEBUG
//=============================================================================
// オブジェクト配置保存関数
//=============================================================================
void CScene::SaveModel(void)
{
	FILE *pFile;																	// ファイル
	char cWriteText[128];															// 文字として書き込み用
	CScene *pSceneNext = NULL;														//次回アップデート対象
	CScene *pSceneNow = NULL;
	D3DXVECTOR3 pos, rot;

	pFile = fopen("data/stage/object.csv", "w");									// ファイルを開くまたは作る

	if (pFile != NULL)																// ファイルが読み込めた場合
	{
		sprintf(cWriteText, "object\n");
		fputs(cWriteText, pFile);													// 書き込み
		sprintf(cWriteText, "位置 X軸, Y軸, Z軸, 回転 X軸, Y軸, Z軸, タイプ\n");
		fputs(cWriteText, pFile);													// 書き込み

		// treeのオブジェクトのポジションを参照
		pSceneNow = m_apTop[PRIORITY_MODEL];

		//次がなくなるまで繰り返す
		while (pSceneNow != NULL)
		{
			pSceneNext = pSceneNow->m_pNext[PRIORITY_MODEL];							//次回アップデート対象を控える
			CObject *pObject = (CObject*)pSceneNow;
			pos = pObject->GetPosition();
			rot = pObject->GetRotation();
			sprintf(cWriteText, "%.2f, %.2f, %.2f,%.2f, %.2f, %.2f, %d\n", pos.x, pos.y, pos.z, rot.x, rot.y, rot.z, pObject->GetType());
			fputs(cWriteText, pFile);												// 書き込み
			pSceneNow = pSceneNext;													//次回アップデート対象を格納
		}

		sprintf(cWriteText, "End");
		fputs(cWriteText, pFile);												// 書き込み

		//ファイル閉
		fclose(pFile);

		MessageBox(NULL, "object情報の書込に成功！", "SUCCESS", MB_ICONASTERISK);
	}
	else
	{
		MessageBox(NULL, "objectファイルのアクセス失敗！", "WARNING", MB_ICONWARNING);
	}
}

//=============================================================================
// 床情報保存関数
//=============================================================================
void CScene::SaveRand(void)
{
	FILE *pFile;																	// ファイル
	char cWriteText[128];															// 文字として書き込み用
	CScene *pSceneNext = NULL;														//次回アップデート対象
	CScene *pSceneNow = NULL;
	D3DXVECTOR3 pos;

	pFile = fopen("data/stage/rand.csv", "w");									// ファイルを開くまたは作る

	if (pFile != NULL)																// ファイルが読み込めた場合
	{
		// treeのオブジェクトのポジションを参照
		pSceneNow = m_apTop[PRIORITY_FLOOR];

		//次がなくなるまで繰り返す
		while (pSceneNow != NULL)
		{
			pSceneNext = pSceneNow->m_pNext[PRIORITY_FLOOR];						//次回アップデート対象を控える
			CMeshField *pMeshField = (CMeshField*)pSceneNow;
			pos = pMeshField->GetPosition();
			sprintf(cWriteText, "pos\n");
			fputs(cWriteText, pFile);												// 書き込み
			sprintf(cWriteText, "%.2f, %.2f, %.2f\n", pos.x, pos.y, pos.z);
			fputs(cWriteText, pFile);												// 書き込み

			pMeshField->SaveRand(pFile);											// 頂点情報の書き込み

			fputs("\n", pFile);														// 書き込み

			pSceneNow = pSceneNext;													// 次回アップデート対象を格納
		}

		sprintf(cWriteText, "End\n");
		fputs(cWriteText, pFile);												// 書き込み

		//ファイル閉
		fclose(pFile);

		MessageBox(NULL, "床情報の書込に成功！", "SUCCESS", MB_ICONASTERISK);
	}
	else
	{
		MessageBox(NULL, "地面情報ファイルのアクセス失敗！", "WARNING", MB_ICONWARNING);
	}
}

//=============================================================================
// 敵配置保存関数
//=============================================================================
void CScene::SaveEnemy(void)
{
	FILE *pFile;																	// ファイル
	char cWriteText[128];															// 文字として書き込み用
	CScene *pSceneNext = NULL;														//次回アップデート対象
	CScene *pSceneNow = NULL;
	D3DXVECTOR3 pos;

	pFile = fopen("data/stage/enemy.csv", "w");									// ファイルを開くまたは作る

	if (pFile != NULL)																// ファイルが読み込めた場合
	{
		sprintf(cWriteText, "enemy\n");
		fputs(cWriteText, pFile);													// 書き込み
		sprintf(cWriteText, "位置 X軸, Y軸, Z軸\n");
		fputs(cWriteText, pFile);													// 書き込み

		// treeのオブジェクトのポジションを参照
		pSceneNow = m_apTop[PRIORITY_ENEMY];

		//次がなくなるまで繰り返す
		while (pSceneNow != NULL)
		{
			pSceneNext = pSceneNow->m_pNext[PRIORITY_ENEMY];							//次回アップデート対象を控える
			CEnemy *pEnemy = (CEnemy*)pSceneNow;
			pos = pEnemy->GetPosition();
			sprintf(cWriteText, "%.2f, %.2f, %.2f\n", pos.x, pos.y, pos.z);
			fputs(cWriteText, pFile);												// 書き込み
			pSceneNow = pSceneNext;													//次回アップデート対象を格納
		}

		sprintf(cWriteText, "End");
		fputs(cWriteText, pFile);												// 書き込み

		//ファイル閉
		fclose(pFile);

		MessageBox(NULL, "敵情報の書込に成功！", "SUCCESS", MB_ICONASTERISK);
	}
	else
	{
		MessageBox(NULL, "敵ファイルのアクセス失敗！", "WARNING", MB_ICONWARNING);
	}
}
#endif