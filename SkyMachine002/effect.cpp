//===============================================
//
// エフェクト処理[effect.cpp]
// Author : SHUGO KURODA
//
//===============================================

//========================
// インクルードファイル
//========================
#include "effect.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "library.h"
#include "player.h"
#include "bullet.h"
#include "game.h"
#include "enemy.h"
#include "bg.h"
#include "enemy_boss.h"

//========================
// マクロ定義
//========================
#define DEFAULT_COL		(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f))		// 初期色

//=======================================
// 静的メンバ変数宣言
//=======================================
LPDIRECT3DTEXTURE9 CEffect::m_apTexture[TEX_MAX] = {};

//========================================
// コンストラクタ
//========================================
CEffect::CEffect() :m_move(0.0f, 0.0f, 0.0f), m_nLife(0), m_type(TYPE_SPHERE)
{
	// オブジェクトタイプの設定
	CObject2D::SetObjType(EObject::OBJ_EFFECT);
}

//========================================
// デストラクタ
//========================================
CEffect::~CEffect()
{

}

//================================
// 生成
//================================
CEffect *CEffect::Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& size, const TYPE& type, const TEX& tex)
{
	// 変数宣言
	CEffect *pEffect = new CEffect;

	if (pEffect != nullptr)
	{
		// 位置設定
		pEffect->SetPosition(pos);

		// サイズ設定
		pEffect->SetSize(size);

		// タイプの設定
		pEffect->m_type = type;

		// 初期化
		pEffect->Init();

		// テクスチャを割り当て
		pEffect->BindTexture(m_apTexture[tex]);
	}

	return pEffect;
}


//========================================
// エフェクトの画像ロード処理
//========================================
HRESULT CEffect::Load()
{
	// レンダラーからデバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/particle000.jpg", &m_apTexture[TEX_SPHERE]);		// 球体パーティクル
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/EnemyBoss001.png", &m_apTexture[TEX_BOSS]);		// 敵ボス

	return S_OK;
}

//========================================
// エフェクトの画像破棄処理
//========================================
void CEffect::Unload()
{
	// テクスチャの破棄
	for (int nCntTexture = 0; nCntTexture < TYPE_MAX; nCntTexture++)
	{
		if (m_apTexture[nCntTexture] != nullptr)
		{
			m_apTexture[nCntTexture]->Release();
			m_apTexture[nCntTexture] = nullptr;
		}
	}
}

//========================================
// 初期化
//========================================
HRESULT CEffect::Init()
{
	//寿命の設定
	m_nLife = MAX_LIFE;

    // 初期化
	CObject2D::Init();

	// 色の設定
	CObject2D::SetColor(DEFAULT_COL);

	// テクスチャ座標の設定
	CObject2D::SetAnimation(0, 0, CEnemyBoss::DIVISION_U, CEnemyBoss::DIVISION_V);

    return S_OK;
}

//========================================
// 終了
//========================================
void CEffect::Uninit()
{
    // 終了処理
	CObject2D::Uninit();
}

//========================================
// 更新
//========================================
void CEffect::Update()
{
    // 色の取得
    D3DXCOLOR col = GetColor();

	// 透明にする
	col.a -= 0.1f;

	// 完全に透明になったら
	if (col.a <= 0.0f)
	{//破棄
		Uninit();
		return;
	}

	// 色の設定
	SetColor(col);
}

//========================================
// エフェクトの描画処理
//========================================
void CEffect::Draw()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャブレンディングを加算合成にする
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

    // 描画処理
    CObject2D::Draw();

	//テクスチャブレンディングの設定を元に戻す
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
}
