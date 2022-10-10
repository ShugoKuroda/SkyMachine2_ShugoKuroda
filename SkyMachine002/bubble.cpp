//=============================================================================
//
// 泡エフェクト処理[bubble.cpp]
// Author : SHUGO KURODA
//
//=============================================================================
#include "bubble.h"
#include "manager.h"
#include "library.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
// テクスチャのポインタ
LPDIRECT3DTEXTURE9 CBubble::m_pTexture = nullptr;
int CBubble::m_nReverseCount = 0;

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CBubble::CBubble() :m_move(0.0f, 0.0f, 0.0f),m_nReverse(0)
{
	// オブジェクトの種類設定
	SetObjType(EObject::OBJ_BUBBLE);
	// 移動量反転カウントの設定
	m_nReverseCount++;
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CBubble::~CBubble()
{
}

//-----------------------------------------------------------------------------
// 生成処理
//-----------------------------------------------------------------------------
CBubble *CBubble::Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& size)
{
	//インスタンス生成
	CBubble *pBubble = new CBubble;

	if (pBubble != nullptr)
	{
		//位置設定
		pBubble->SetPosition(pos);

		//サイズ設定
		pBubble->SetSize(size);

		//生成処理
		pBubble->Init();

		// テクスチャの設定
		pBubble->BindTexture(m_pTexture);
	}

	return pBubble;
}

//-----------------------------------------------------------------------------
// テクスチャの読み込み
//-----------------------------------------------------------------------------
HRESULT CBubble::Load()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/effect000.jpg",
		&m_pTexture);

	return S_OK;
}

//-----------------------------------------------------------------------------
// テクスチャの破棄
//-----------------------------------------------------------------------------
void CBubble::Unload()
{
	// テクスチャの破棄
	if (m_pTexture != nullptr)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}

	// 反転カウンターの初期化
	m_nReverseCount = 0;
}

//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
HRESULT CBubble::Init()
{
	//移動量の設定
	m_move.y = -1.0f;

	//揺れを反転させる
	if (m_nReverseCount % 2 == 0)
	{// 反転係数の設定
		m_nReverse = -1;
	}
	else
	{// 反転係数の設定
		m_nReverse = 1;
	}

	// 初期化
	CObject2D::Init();

	return S_OK;
}

//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void CBubble::Uninit()
{
	CObject2D::Uninit();
}

//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void CBubble::Update()
{
	// 位置の取得
	D3DXVECTOR3 pos = GetPosition();
	// 色の取得
	D3DXCOLOR col = GetColor();

	// α値の減算
	col.a -= 0.01f;
	// 移動量の加算
	m_move.x += 1.0f;

	//移動量を加算
	pos.x += sinf(m_move.x / 5.0f) * m_nReverse;
	//常に左上に流す
	pos.x -= 1.0f;
	pos.y += m_move.y;

	// α値が0以下または画面外で破棄
	if (col.a <= 0 || LibrarySpace::OutScreen2D(&pos, CObject2D::GetSize()))
	{// 終了
		Uninit();
		return;
	}

	//位置の設定
	SetPosition(pos);

	//色(α値)の設定
	SetColor(col);

	//頂点座標の設定
	SetVertex();
}

//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void CBubble::Draw()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// レンダーステート(加算合成にする)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 描画
	CObject2D::Draw();

	// レンダーステート(加算合成を戻す)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
