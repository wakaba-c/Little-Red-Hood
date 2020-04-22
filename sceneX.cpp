//=============================================================================
//
// sceneX処理 [sceneX.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "sceneX.h"
#include "manager.h"
#include "renderer.h"
#include "light.h"
#include "object.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CSceneX::CSceneX(CScene::PRIORITY obj = CScene::PRIORITY_MODEL) : CScene(obj)
{
	SetObjType(CScene::PRIORITY_MODEL);

	m_pBuffMat = NULL;
	m_pMesh = NULL;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_size = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}

//=============================================================================
// デストラクタ
//=============================================================================
CSceneX::~CSceneX()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSceneX::Init(void)
{
	return S_OK;
}

//=============================================================================
// 開放処理
//=============================================================================
void CSceneX::Uninit(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void CSceneX::Update(void)
{
#ifdef _DEBUG
	Debug();
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void CSceneX::Draw(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	D3DXVECTOR3 pos = GetPosition();

	D3DXMATRIX		mtxRot, mtxTrans;				//計算用マトリックス
	D3DXMATERIAL	*pMat;							//現在のマテリアル保存用
	D3DMATERIAL9	matDef;							//マテリアルデータへのポインタ

	//デバイスを取得する
	pDevice = pRenderer->GetDevice();

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);					//すべてのデータ収録

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, CManager::GetResource(pMat[nCntMat].pTextureFilename));
		}

		// 描画
		m_pMesh->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// クリエイト関数
//=============================================================================
CSceneX *CSceneX::Create(void)
{
	CSceneX *pSceneX;
	pSceneX = new CSceneX(CScene::PRIORITY_MODEL);

	if (pSceneX != NULL)
	{// シーンが存在していたとき
		pSceneX->Init();				// 初期化処理
	}
	return pSceneX;
}

//=============================================================================
// モデル情報の設定
//=============================================================================
void CSceneX::BindModel(LPDIRECT3DVERTEXBUFFER9 pVtxBuff, LPD3DXMESH pMesh, DWORD nNumMat, LPD3DXBUFFER pBuffMat, LPDIRECT3DTEXTURE9 *tex)
{
	m_pVtxBuff = pVtxBuff;
	m_pMesh = pMesh;
	m_nNumMat = nNumMat;
	m_pBuffMat = pBuffMat;
	m_pTexture = tex;
}

//=============================================================================
// カラーの設定
//=============================================================================
void CSceneX::SetColor(D3DXCOLOR col)
{
	m_col = col;
}

//=============================================================================
// サイズの設定
//=============================================================================
void CSceneX::SetSize(D3DXVECTOR3 size)
{
	m_size = size;
}

//=============================================================================
// 回転値の設定
//=============================================================================
void CSceneX::SetRotation(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

#ifdef _DEBUG
//=============================================================================
// デバッグ処理
//=============================================================================
void CSceneX::Debug(void)
{

}
#endif