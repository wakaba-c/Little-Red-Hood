//=============================================================================
//
// オブジェクト処理 [object.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "meshField.h"
#include "colliderBox.h"
#include "colliderSphere.h"
#include "camera.h"
#include "house.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
PDIRECT3DTEXTURE9 *CObject::m_pTexture[OBJTYPE_MAX] = {};
LPD3DXMESH		CObject::m_pMesh[OBJTYPE_MAX] = {};
DWORD			CObject::m_nNumMat[OBJTYPE_MAX] = {};
LPD3DXBUFFER		CObject::m_pBuffMat[OBJTYPE_MAX] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CObject::CObject(CScene::PRIORITY obj = CScene::PRIORITY_MODEL) : CSceneX(obj)
{
	SetObjType(CScene::PRIORITY_MODEL);

	m_ColliderBox = NULL;
	m_ColliderSphere = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CObject::~CObject()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CObject::Init(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();	// デバイスの取得

	// 初期化処理
	CSceneX::Init();

	// モデルの情報をセット
	BindModel(m_pVtxBuff, m_pMesh[m_Type], m_nNumMat[m_Type], m_pBuffMat[m_Type], m_pTexture[m_Type]);

	D3DXVECTOR3 pos = D3DXVECTOR3(500.0f, 0.0f, 0.0f);

	switch (m_Type)
	{
	case OBJTYPE_FENCE:
		m_ColliderBox = CColliderBox::Create(false, D3DXVECTOR3(550.0f, 120.0f, 50.0f));		// 生成
		break;
	}

	if (m_ColliderBox != NULL)
	{// ボックスコライダーが存在していたとき
		switch (m_Type)
		{
		case OBJTYPE_FENCE:
			m_ColliderBox->SetScene(this);									// 持ち主を設定
			m_ColliderBox->SetTag("fence");									// fenceを設定
			m_ColliderBox->SetPosition(pos);								// 位置の設定
			m_ColliderBox->SetOffset(D3DXVECTOR3(0.0f, 60.0f, 0.0f));		// オフセット値の設定
			break;
		}
	}

	if (m_ColliderSphere != NULL)
	{// 当たり判定が存在していたとき
		m_ColliderSphere->SetScene(this);									// 持ち主を設定
		m_ColliderSphere->SetTag("house");									// houseを設定
		m_ColliderSphere->SetPosition(pos);									// 位置の設定
		m_ColliderSphere->SetOffset(D3DXVECTOR3(0.0f, 85.0f, -10.0f));		// オフセット値の設定
	}

	SetPosition(pos);			// 位置の設定
	return S_OK;
}

//=============================================================================
// 開放処理
//=============================================================================
void CObject::Uninit(void)
{
	if (m_ColliderBox != NULL)
	{// ボックスコライダーが存在していたとき
		m_ColliderBox->Release();			// 削除予約
		m_ColliderBox = NULL;				// NULLを代入
	}

	// 開放処理
	CSceneX::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CObject::Update(void)
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

	SetPosition(pos);		// 位置の設定
#ifdef _DEBUG
	Debug();				// デバッグ処理
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void CObject::Draw(void)
{
	if (m_Type != OBJTYPE_HOUSE_1 && m_Type != OBJTYPE_HOUSE_2 && m_Type != OBJTYPE_HOUSE_3 && m_Type != OBJTYPE_HOUSE_4)
	{
		CSceneX::Draw();
	}
}

//=============================================================================
// クリエイト関数
//=============================================================================
CObject *CObject::Create(OBJTYPE type)
{
	CObject *pObject;
	pObject = new CObject(CScene::PRIORITY_MODEL);
	if (pObject != NULL)
	{
		pObject->SetType(type);
		pObject->Init();
	}
	return pObject;
}

//=============================================================================
// ロード処理
//=============================================================================
HRESULT CObject::Load(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスを取得する
	pDevice = pRenderer->GetDevice();

	// Xファイルの読み込み
	D3DXLoadMeshFromX("data/model/house/house.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_pBuffMat[OBJTYPE_HOUSE_1],
		NULL,
		&m_nNumMat[OBJTYPE_HOUSE_1],
		&m_pMesh[OBJTYPE_HOUSE_1]);

	// Xファイルの読み込み
	D3DXLoadMeshFromX("data/model/house/MedievalHouse_1.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_pBuffMat[OBJTYPE_HOUSE_2],
		NULL,
		&m_nNumMat[OBJTYPE_HOUSE_2],
		&m_pMesh[OBJTYPE_HOUSE_2]);

	// Xファイルの読み込み
	D3DXLoadMeshFromX("data/model/house/MedievalHouse_2.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_pBuffMat[OBJTYPE_HOUSE_3],
		NULL,
		&m_nNumMat[OBJTYPE_HOUSE_3],
		&m_pMesh[OBJTYPE_HOUSE_3]);

	// Xファイルの読み込み
	D3DXLoadMeshFromX("data/model/house/MedievalHouse_3.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_pBuffMat[OBJTYPE_HOUSE_4],
		NULL,
		&m_nNumMat[OBJTYPE_HOUSE_4],
		&m_pMesh[OBJTYPE_HOUSE_4]);

	// Xファイルの読み込み
	D3DXLoadMeshFromX("data/model/wood1.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_pBuffMat[OBJTYPE_TREE_1],
		NULL,
		&m_nNumMat[OBJTYPE_TREE_1],
		&m_pMesh[OBJTYPE_TREE_1]);

	// Xファイルの読み込み
	D3DXLoadMeshFromX("data/model/wood2.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_pBuffMat[OBJTYPE_TREE_2],
		NULL,
		&m_nNumMat[OBJTYPE_TREE_2],
		&m_pMesh[OBJTYPE_TREE_2]);

	// Xファイルの読み込み
	D3DXLoadMeshFromX("data/model/wood3.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_pBuffMat[OBJTYPE_TREE_3],
		NULL,
		&m_nNumMat[OBJTYPE_TREE_3],
		&m_pMesh[OBJTYPE_TREE_3]);

	// Xファイルの読み込み
	D3DXLoadMeshFromX("data/model/fence/fence.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_pBuffMat[OBJTYPE_FENCE],
		NULL,
		&m_nNumMat[OBJTYPE_FENCE],
		&m_pMesh[OBJTYPE_FENCE]);

	// Xファイルの読み込み
	D3DXLoadMeshFromX("data/model/Arrow.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&m_pBuffMat[OBJTYPE_ARROW],
		NULL,
		&m_nNumMat[OBJTYPE_ARROW],
		&m_pMesh[OBJTYPE_ARROW]);

	D3DXMATERIAL	*pMat;							//現在のマテリアル保存用

	for (int nCount = 0; nCount < OBJTYPE_MAX; nCount++)
	{
		if (nCount != OBJTYPE_HOUSE_1 && nCount != OBJTYPE_HOUSE_2 && nCount != OBJTYPE_HOUSE_3 && nCount != OBJTYPE_HOUSE_4)
		{
			// マテリアル情報に対するポインタを取得
			pMat = (D3DXMATERIAL*)m_pBuffMat[nCount]->GetBufferPointer();

			// マテリアルのテクスチャ読み込み
			for (int nCntMat = 0; nCntMat < (int)m_nNumMat[nCount]; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{// テクスチャがあったとき
					char *pFileName = NULL;
					pFileName = pMat[nCntMat].pTextureFilename;			// テクスチャのアドレスを取得
					CManager::Load(pFileName);		// テクスチャの読み込み
				}
			}
		}
	}
	return S_OK;
}

//=============================================================================
// テクスチャの解放関数
//=============================================================================
void CObject::Unload(void)
{
	// テクスチャの開放
	for (int nCntTex = 0; nCntTex < OBJTYPE_MAX; nCntTex++)
	{
		if (m_pTexture[nCntTex] != NULL)
		{
			for (int nCount = 0; nCount < (int)m_nNumMat[nCntTex]; nCount++)
			{
				if (m_pTexture[nCntTex][nCount] != NULL)
				{
					m_pTexture[nCntTex][nCount]->Release();
				}
			}
			delete[] m_pTexture[nCntTex];
			m_pTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
// モデルタイプの設定
//=============================================================================
void CObject::SetType(OBJTYPE type)
{
	m_Type = type;
}

//=============================================================================
// モデルの読込
//=============================================================================
void CObject::LoadModel(char *add)
{
	FILE *pFile = NULL;											// ファイル
	char cReadText[128] = {};									// 文字
	char cHeadText[128] = {};									// 比較
	float fData[7];												// 答え
	CObject *pObject = NULL;

	pFile = fopen(add, "r");									// ファイルを開くまたは作る

	if (pFile != NULL)											// ファイルが読み込めた場合
	{
		fgets(cReadText, sizeof(cReadText), pFile);				// 行を飛ばす
		fgets(cReadText, sizeof(cReadText), pFile);				// 行を飛ばす
		fgets(cReadText, sizeof(cReadText), pFile);				// 行を飛ばす

		while (strcmp(cHeadText, "End") != 0)
		{
			CManager::ConvertStringToFloat(cReadText, ",", fData);

			pObject = CObject::Create((OBJTYPE)(int)fData[6]);
			pObject->SetPosition(D3DXVECTOR3(fData[0], fData[1], fData[2]));			// 位置の設定
			pObject->SetRotation(D3DXVECTOR3(fData[3], fData[4], fData[5]));			// 回転値を設定

			fgets(cReadText, sizeof(cReadText), pFile);									// 行を飛ばす
			sscanf(cReadText, "%s", &cHeadText);
		}

		fclose(pFile);					// ファイルを閉じる
	}
	else
	{
		MessageBox(NULL, "モデル情報のアクセス失敗！", "WARNING", MB_ICONWARNING);	// メッセージボックスの生成
	}
}

#ifdef _DEBUG
//=============================================================================
// デバッグ処理
//=============================================================================
void CObject::Debug(void)
{

}
#endif