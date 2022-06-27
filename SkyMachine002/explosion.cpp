//-----------------------------------------------------------------------------------------------
//
// 爆発の処理[explosion.cpp]
// Author : SHUGO KURODA
//
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------------------------
#include "explosion.h"		// プレイヤー
#include "manager.h"		// アプリケーション
#include "renderer.h"		// レンダリング

//-----------------------------------------------------------------------------------------------
// 変数変数
//-----------------------------------------------------------------------------------------------
// 幅
const float CExplosion::SIZE_WIDTH = 40.0f;
// 高さ
const float CExplosion::SIZE_HEIGHT = 40.0f;
// アニメーション間隔
const int CExplosion::ANIM_INTERVAL = 4;
// アニメーション最大数
const int CExplosion::ANIM_MAXANIM = 7;
// U座標の最大分割数
const int CExplosion::DIVISION_U = 8;
// V座標の最大分割数
const int CExplosion::DIVISION_V = 1;

//-----------------------------------------------------------------------------------------------
// 静的メンバ変数
//-----------------------------------------------------------------------------------------------
// テクスチャのポインタ
LPDIRECT3DTEXTURE9 CExplosion::m_pTexture = nullptr;

//-----------------------------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------------------------
CExplosion::CExplosion()
	:m_nCntAnim(0), m_nPatterAnim(0)
{

}

//-----------------------------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------------------------
CExplosion::~CExplosion()
{

}

//-----------------------------------------------------------------------------------------------
// 生成
//
// const D3DXVECTOR3& pos → 生成する位置
//-----------------------------------------------------------------------------------------------
CExplosion* CExplosion::Create(const D3DXVECTOR3& pos)
{
	// ポインタクラスを宣言
	CExplosion* pExplosion = new CExplosion;

	if (pExplosion != nullptr)
	{// もしnullptrではなかったら
	 // 初期化
		pExplosion->Init(pos);

		// テクスチャの設定
		pExplosion->BindTexture(m_pTexture);
	}

	return pExplosion;
}

//-----------------------------------------------------------------------------------------------
// テクスチャの読み込み
//-----------------------------------------------------------------------------------------------
HRESULT CExplosion::Load()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/back001.png",
		&m_pTexture);

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// テクスチャの削除
//-----------------------------------------------------------------------------------------------
void CExplosion::Unload()
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
//
//const D3DXVECTOR3& pos → 最初に表示する位置
//-----------------------------------------------------------------------------------------------
HRESULT CExplosion::Init(const D3DXVECTOR3& pos)
{
	// サイズ
	CObject2D::SetSize(SIZE_WIDTH, SIZE_HEIGHT);

	CObject2D::Init(pos);

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------------------------
void CExplosion::Uninit()
{
	CObject2D::Uninit();
}

//-----------------------------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------------------------
void CExplosion::Update()
{
	// カウントを増やす
	m_nCntAnim++;

	if (m_nCntAnim % ANIM_INTERVAL == 0)
	{
		// 今のアニメーションを1つ進める
		m_nPatterAnim++;
	}

	if (m_nPatterAnim == ANIM_MAXANIM)
	{// アニメーションが終わったら
	 // 終了する
		Uninit();
	}
	else
	{
		// テクスチャ更新
		CObject2D::SetAnimation(DIVISION_U, DIVISION_V, m_nPatterAnim, 0);
	}
}

//-----------------------------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------------------------
void CExplosion::Draw()
{
	CObject2D::Draw();
}