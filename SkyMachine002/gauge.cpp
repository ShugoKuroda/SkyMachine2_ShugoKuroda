//-----------------------------------------------------------------------------------------------
//
// ロゴ処理[logo.cpp]
// Author : SHUGO KURODA
//
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------------------------
#include "gauge.h"
#include "manager.h"	// アプリケーション
#include "renderer.h"	// レンダリング

#include "game.h"
#include "player.h"

//-----------------------------------------------------------------------------------------------
// 定数宣言
//-----------------------------------------------------------------------------------------------
// サイズX
const float CGauge::SIZE_X = 75.0f;
// サイズY
const float CGauge::SIZE_Y = 30.0f;

//-----------------------------------------------------------------------------------------------
// 静的メンバ変数
//-----------------------------------------------------------------------------------------------
// テクスチャのポインタ
LPDIRECT3DTEXTURE9 CGauge::m_pTexture = {};

//-----------------------------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------------------------
CGauge::CGauge() :m_type(TYPE_NONE), m_parent(PLAYER_1)
{
	SetObjType(EObject::OBJ_UI);
}

//-----------------------------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------------------------
CGauge::~CGauge()
{
}

//-----------------------------------------------------------------------------------------------
// 生成
// const D3DXVECTOR3& pos → 生成する位置
// const TYPE& type → 生成する種類
// const PARENT& parent →	どのプレイヤーのゲージか
//-----------------------------------------------------------------------------------------------
CGauge* CGauge::Create(const D3DXVECTOR3& pos, const TYPE& type, const PARENT& parent)
{
	// ポインタクラスを宣言
	CGauge* pGauge = new CGauge;

	if (pGauge != nullptr)
	{// もしnullptrではなかったら

		 // 位置設定
		pGauge->SetPosition(pos);

		// 種類を設定
		pGauge->m_type = type;

		// 親の設定
		pGauge->m_parent = parent;

		// 初期化
		pGauge->Init();

		// テクスチャの設定
		pGauge->BindTexture(m_pTexture);
	}

	return pGauge;
}

//-----------------------------------------------------------------------------------------------
// テクスチャの読み込み
//-----------------------------------------------------------------------------------------------
HRESULT CGauge::Load()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ui010.png",
		&m_pTexture);

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// テクスチャの削除
//-----------------------------------------------------------------------------------------------
void CGauge::Unload()
{
	// テクスチャの破棄
	if (m_pTexture != nullptr)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}
}

//-----------------------------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------------------------
HRESULT CGauge::Init()
{
	//サイズの設定
	CObject2D::SetSize(D3DXVECTOR2(SIZE_X, SIZE_Y));

	//初期化
	CObject2D::Init();

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------------------------
void CGauge::Uninit()
{
	CObject2D::Uninit();
}

//-----------------------------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------------------------
void CGauge::Update()
{
	// プレイヤー情報の取得
	CPlayer *pPlayer = CGame::GetPlayer(m_parent);

	// 親プレイヤーが破棄されていたら
	if (pPlayer == nullptr)
	{// 破棄
		Uninit();
		return;
	}

	// 変数宣言
	D3DXVECTOR2 size = GetSize();
	int nPlayerLevel = 0;

	switch (m_type)
	{
		// メイン弾
	case CGauge::TYPE_BULLET:
		// メイン弾の強化段階の取得
		nPlayerLevel = pPlayer->GetBulletLevel();

		// 横サイズ調整
		size.x = SIZE_X / ((float)MAX_LEVEL / ((float)nPlayerLevel + 1.0f));

		// 画像の描画範囲を調整
		CObject2D::SetTextureRange(nPlayerLevel + 1, MAX_LEVEL);
		break;

		// オプション弾
	case CGauge::TYPE_BULLET_OPTION:

		// オプション弾の強化段階の取得
		nPlayerLevel = pPlayer->GetOptionLevel();

		// 横サイズ調整
		size.x = SIZE_X / ((float)MAX_LEVEL / ((float)nPlayerLevel));

		// 画像の描画範囲を調整
		CObject2D::SetTextureRange(nPlayerLevel, MAX_LEVEL);
		break;

		// バリア
	case CGauge::TYPE_BARRIER:
		// バリアの強化段階の取得
		nPlayerLevel = pPlayer->GetBarrierLevel();

		// 横サイズ調整
		size.x = SIZE_X / ((float)MAX_LEVEL / ((float)nPlayerLevel));

		// 画像の描画範囲を調整
		CObject2D::SetTextureRange(nPlayerLevel, MAX_LEVEL);
		break;
	default:
		break;
	}

	//サイズの設定
	CObject2D::SetSize(size);

	//頂点座標の設定
	CObject2D::SetVertex();
}

//-----------------------------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------------------------
void CGauge::Draw()
{
	CObject2D::Draw();
}