//-----------------------------------------------------------------------------------------------
//
// メッシュフィールド処理[meshfield.cpp]
// Author : SHUGO KURODA
//
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------------------------
#include "manager.h"
#include "renderer.h"
#include "object2D.h"
#include "meshfield.h"
#include "input_keyboard.h"

//-----------------------------------------------------------------------------------------------
//	マクロ定義
//-----------------------------------------------------------------------------------------------
#define MESHFIELD_VERTEX_NUM		((MESHFIELD_X_BLOCK + 1) * (MESHFIELD_Y_BLOCK + 1))		//頂点数
#define MESHFIELD_INDEX_NUM			((MESHFIELD_X_BLOCK + 1) * 2 * MESHFIELD_Y_BLOCK + (MESHFIELD_Y_BLOCK - 1) * 2)		//インデックス数
#define MESHFIELD_PRIMITIVE_NUM		(MESHFIELD_X_BLOCK * MESHFIELD_Y_BLOCK * 2 + (MESHFIELD_Y_BLOCK - 1) * 4)			//ポリゴン数

//-----------------------------------------------------------------------------------------------
//	静的メンバ変数宣言
//-----------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CMeshField::m_pTexture = {};

//-----------------------------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------------------------
CMeshField::CMeshField() :m_pos(0.0f, 0.0f, 0.0f), m_pVtxBuff(nullptr), m_pIdxBuff(nullptr),
m_nCounterAnim(0), m_nPatternAnim(0), m_col(0.0f, 0.0f, 0.0f, 0.0f), m_bCol(false)
{
	SetObjType(EObject::OBJ_BG_MOVE);
}

//-----------------------------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------------------------
CMeshField::~CMeshField()
{
}

//-----------------------------------------------------------------------------------------------
// 生成
//-----------------------------------------------------------------------------------------------
CMeshField *CMeshField::Create()
{
	// ポインタクラスを宣言
	CMeshField* pMeshField = new CMeshField;

	// 初期化
	pMeshField->Init();

	return pMeshField;
}

//-----------------------------------------------------------------------------------------------
// テクスチャの読み込み
//-----------------------------------------------------------------------------------------------
HRESULT CMeshField::Load()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	HRESULT hr = D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bg000_11.jpg",
		&m_pTexture);

	return hr;
}

//-----------------------------------------------------------------------------------------------
// テクスチャの破棄
//-----------------------------------------------------------------------------------------------
void CMeshField::Unload()
{
	//テクスチャの破棄
	if (m_pTexture != nullptr)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}
}

//-----------------------------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------------------------
HRESULT CMeshField::Init()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(CObject2D::VERTEX_2D) * MESHFIELD_VERTEX_NUM,
		D3DUSAGE_WRITEONLY,
		CObject2D::FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * MESHFIELD_INDEX_NUM,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	CObject2D::VERTEX_2D *pVtx;		//頂点情報へのポインタ
	WORD *pIdx;				//インデックス情報へのポインタ

	m_pos = D3DXVECTOR3(-CRenderer::SCREEN_WIDTH / 2, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//各頂点座標の設定
	for (int nCntBlockY = 0; nCntBlockY < MESHFIELD_Y_BLOCK + 1; nCntBlockY++)
	{//Y方向の座標設定
		for (int nCntBlockX = 0; nCntBlockX < MESHFIELD_X_BLOCK + 1; nCntBlockX++)
		{//X方向の座標設定
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(m_pos.x + ((CRenderer::SCREEN_WIDTH * 1.6f) / MESHFIELD_X_BLOCK) * nCntBlockX, m_pos.y + (CRenderer::SCREEN_HEIGHT / MESHFIELD_Y_BLOCK) * nCntBlockY, m_pos.z);
			//各頂点の法線の設定(ベクトルの大きさは１にする必要がある)
			pVtx[0].rhw = 1.0f;
			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(m_col);
			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2((1.0f / MESHFIELD_X_BLOCK) * nCntBlockX, (1.0f / MESHFIELD_Y_BLOCK) * nCntBlockY);

			pVtx++;
		}
	}

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	//インデックスバッファをロック
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	{
		int nCntIdxX, nCntIdxY;

		//インデックスの設定
		for (nCntIdxY = 0; nCntIdxY < MESHFIELD_Y_BLOCK; nCntIdxY++)
		{//Z方向のインデックス設定
			for (nCntIdxX = 0; nCntIdxX < MESHFIELD_X_BLOCK + 1; nCntIdxX++)
			{//X方向のインデックス設定
				pIdx[0] = (WORD)(nCntIdxX + ((MESHFIELD_X_BLOCK + 1) * nCntIdxY) + MESHFIELD_X_BLOCK + 1);
				pIdx[1] = (WORD)(nCntIdxX + ((MESHFIELD_X_BLOCK + 1) * nCntIdxY));

				pIdx += 2;
			}

			//縮退ポリゴン用のインデックス設定
			if (nCntIdxY <= MESHFIELD_Y_BLOCK - 2)
			{//最後のインデックス番号ではない時
				pIdx[0] = (WORD)((nCntIdxX - 1) + ((MESHFIELD_X_BLOCK + 1) * nCntIdxY));
				pIdx[1] = (WORD)((nCntIdxX + (MESHFIELD_X_BLOCK + 1)) + ((MESHFIELD_X_BLOCK + 1) * nCntIdxY));
			}
			pIdx += 2;
		}
	}

	//インデックスバッファのアンロック
	m_pIdxBuff->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// 破棄
//-----------------------------------------------------------------------------------------------
void CMeshField::Uninit()
{
	//頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
	//インデックスバッファの解放
	if (m_pIdxBuff != nullptr)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;
	}
}

//-----------------------------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------------------------
void CMeshField::Update()
{
	// アニメーション
	m_nCounterAnim++;	//カウンタ加算

	if (m_nCounterAnim == 2)//速さ
	{
		// オーバーフロー防止
		m_nCounterAnim = 0;  // カウンタを0に戻す

		// アニメーションを切り替える
		m_nPatternAnim = (m_nPatternAnim + 1) % 1000;  // 枚数
	}

	// 何等分するか計算
	float fEqualDivision = 1.0f / (float)1000;

	// 頂点情報へのポインタ
	CObject2D::VERTEX_2D *pVtx;

	if (m_bCol == true)
	{
		m_col.r += 0.01f;
		m_col.b -= 0.01f;
		if (m_col.r >= 1.0f)
		{
			m_bCol = false;
		}
	}
	else if (m_bCol == false)
	{
		m_col.r -= 0.01f;
		m_col.b += 0.01f;
		if (m_col.r <= 0.0f)
		{
			m_bCol = true;
		}
	}

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//各頂点座標の設定
	for (int nCntBlockY = 0; nCntBlockY < MESHFIELD_Y_BLOCK + 1; nCntBlockY++)
	{//Y方向の座標設定

		for (int nCntBlockX = 0; nCntBlockX < MESHFIELD_X_BLOCK + 1; nCntBlockX++)
		{//X方向の座標設定
			float offsetX = sinf(D3DX_PI * 2.0f * (nCntBlockY + m_nPatternAnim) / MESHFIELD_Y_BLOCK * 0.2f) * 20.0f;

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(m_pos.x + ((CRenderer::SCREEN_WIDTH * 1.6f) / MESHFIELD_X_BLOCK) * nCntBlockX + offsetX,
				m_pos.y + (CRenderer::SCREEN_HEIGHT / MESHFIELD_Y_BLOCK) * nCntBlockY,
				m_pos.z);

			// 色の設定
			pVtx[0].col = D3DXCOLOR(m_col);

			// テクスチャの座標を反映
			pVtx[0].tex = D3DXVECTOR2(((1.0f / MESHFIELD_X_BLOCK) * nCntBlockX) + m_nPatternAnim * fEqualDivision, (1.0f / MESHFIELD_Y_BLOCK) * nCntBlockY);

			pVtx++;
		}
	}

	//頂点バッファの解放
	m_pVtxBuff->Unlock();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

#ifdef _DEBUG
	// キーボード情報の取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_1) == true)
	{//3キーが押された
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
	if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_2) == true)
	{//4キーが押された
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
#endif // _DEBUG
}

//-----------------------------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------------------------
void CMeshField::Draw()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(CObject2D::VERTEX_2D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	//頂点フォーマットの設定
	pDevice->SetFVF(CObject2D::FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
		0,			//
		0,			//
		MESHFIELD_VERTEX_NUM,		//頂点数
		0,							//描画する最初の頂点インデックス
		MESHFIELD_PRIMITIVE_NUM);	//プリミティブ（ポリゴン）数
}
