//-----------------------------------------------------------------------------------------------
//
// 弾の処理[bullet.cpp]
// Author : SHUGO kURODA
//
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------------------------
#include "barrier.h"
#include "bullet.h"
#include "manager.h"
#include "renderer.h"

#include "library.h"
#include "game.h"

#include "enemy.h"
#include "explosion.h"

//-----------------------------------------------------------------------------------------------
// 定数宣言
//-----------------------------------------------------------------------------------------------
// アニメーション間隔
const int CBarrier::ANIM_INTERVAL = 5;
// アニメーション最大数
const int CBarrier::MAX_ANIM = 6;
// U座標(X方向)の最大分割数
const int CBarrier::DIVISION_U = 4;
// V座標(Y方向)の最大分割数
const int CBarrier::DIVISION_V = 2;

//サイズ(X)
const float CBarrier::SIZE_X = 110.0f;
//サイズ(Y)
const float CBarrier::SIZE_Y = 60.0f;

//-----------------------------------------------------------------------------------------------
// 静的メンバ変数
//-----------------------------------------------------------------------------------------------
// テクスチャのポインタ
LPDIRECT3DTEXTURE9 CBarrier::m_apTexture[LEVEL_MAX] = { nullptr };

//-----------------------------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------------------------
CBarrier::CBarrier() :m_fRad(0.0f), m_nDamage(0), m_col(LEVEL_GREEN), m_nCntAnim(0), m_nPatternAnim(0), m_nPatterAnimV(0)
{
	SetObjType(EObject::OBJ_BARRIER);
}

//-----------------------------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------------------------
CBarrier::~CBarrier()
{

}

//-----------------------------------------------------------------------------------------------
// 生成
//-----------------------------------------------------------------------------------------------
CBarrier* CBarrier::Create(const D3DXVECTOR3& pos, const PARENT& parent)
{
	// ポインタクラスを宣言
	CBarrier* pBarrier = new CBarrier;

	if (pBarrier != nullptr)
	{// もしnullptrではなかったら

		// 位置設定
		pBarrier->SetPosition(D3DXVECTOR3(pos.x - 10.0f, pos.y, pos.z));

		//親の設定(所有するプレイヤー)
		pBarrier->m_parent = parent;

		// 初期化
		pBarrier->Init();

		// テクスチャの設定
		pBarrier->BindTexture(m_apTexture[LEVEL_GREEN]);
	}

	return pBarrier;
}

//-----------------------------------------------------------------------------------------------
// テクスチャの読み込み
//-----------------------------------------------------------------------------------------------
HRESULT CBarrier::Load()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/barrier000.png",
		&m_apTexture[LEVEL_GREEN]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/barrier001.png",
		&m_apTexture[LEVEL_SILVER]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/barrier002.png",
		&m_apTexture[LEVEL_GOLD]);

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// テクスチャの削除
//-----------------------------------------------------------------------------------------------
void CBarrier::Unload()
{
	for (int nCnt = 0; nCnt < LEVEL_MAX; nCnt++)
	{
		// テクスチャの破棄
		if (m_apTexture[nCnt] != nullptr)
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = nullptr;
		}
	}
}

//-----------------------------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------------------------
HRESULT CBarrier::Init()
{
	//サイズの設定
	CObject2D::SetSize(D3DXVECTOR2(SIZE_X, SIZE_Y));
	// 初期化
	CObject2D::Init();

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------------------------
void CBarrier::Uninit()
{
	CObject2D::Uninit();
}

//-----------------------------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------------------------
void CBarrier::Update()
{
	// カウントを増やす
	m_nCntAnim++;

	if (m_nCntAnim % ANIM_INTERVAL == 0)
	{
		// カウンターの初期化
		m_nCntAnim = 0;
		// 今のアニメーションを1つ進める
		m_nPatternAnim++;
	}

	// アニメーションが終わったら
	if (m_nPatternAnim == MAX_ANIM)
	{// アニメーションの初期化
		m_nPatternAnim = 0;
		m_nPatterAnimV = 0;
	}

	if (m_nPatternAnim >= DIVISION_U && m_nPatterAnimV <= 0)
	{
		m_nPatterAnimV++;
	}

	// 位置の取得
	D3DXVECTOR3 pos = CGame::GetPlayer(m_parent)->GetPosition();

	// 位置の更新(常に親プレイヤーの位置に設定)
	CObject2D::SetPosition(D3DXVECTOR3(pos.x - 10.0f, pos.y, pos.z));

	//頂点座標の設定
	CObject2D::SetVertex();

	// テクスチャ更新
	CObject2D::SetAnimation(m_nPatternAnim, m_nPatterAnimV, DIVISION_U, DIVISION_V);
}

//-----------------------------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------------------------
void CBarrier::Draw()
{
	CObject2D::Draw();
}

//-----------------------------------------------------------------------------------------------
// バリア色の設定
//-----------------------------------------------------------------------------------------------
void CBarrier::SetBarrier(LEVEL col)
{
	m_col = col;

	BindTexture(m_apTexture[m_col]);
}
