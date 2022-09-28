//-----------------------------------------------------------------------------------------------
//
// 弾の処理[bullet.cpp]
// Author : SHUGO kURODA
//
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------------------------
#include "item.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"

#include "sound.h"
#include "library.h"

#include "player.h"
#include "score.h"

//-----------------------------------------------------------------------------------------------
// 定数定義
//-----------------------------------------------------------------------------------------------
// 幅
const float CItem::SIZE_WIDTH = 40.0f;
// 高さ
const float CItem::SIZE_HEIGHT = 40.0f;
// 基本移動量
const float CItem::MOVE_DEFAULT = 6.0f;
// アニメーション間隔
const int CItem::ANIM_INTERVAL = 5;
// アニメーション最大数
const int CItem::MAX_ANIM = 4;
// U座標の最大分割数
const int CItem::DIVISION_U = 4;
// V座標の最大分割数
const int CItem::DIVISION_V = 1;

//-----------------------------------------------------------------------------------------------
// 静的メンバ変数
//-----------------------------------------------------------------------------------------------
// テクスチャのポインタ
LPDIRECT3DTEXTURE9 CItem::m_apTexture[TYPE_MAX] = { nullptr };

//-----------------------------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------------------------
CItem::CItem() :
	m_fRot(0.0f), m_nCntAnim(0), m_nPatternAnim(0), m_type(TYPE_NONE)
{
	SetObjType(EObject::OBJ_ITEM);
}

//-----------------------------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------------------------
CItem::~CItem()
{

}

//-----------------------------------------------------------------------------------------------
// 生成
//-----------------------------------------------------------------------------------------------
CItem* CItem::Create(const D3DXVECTOR3& pos, const EType type)
{
	// ポインタクラスを宣言
	CItem* pItem = new CItem;

	if (pItem != nullptr)
	{// もしnullptrではなかったら

		// 位置の設定
		pItem->SetPosition(pos);

		//テクスチャ種類の設定
		pItem->m_type = type;

		// 初期化
		pItem->Init();

		// テクスチャの設定
		pItem->BindTexture(m_apTexture[pItem->m_type]);
	}

	return pItem;
}

//-----------------------------------------------------------------------------------------------
// テクスチャの読み込み
//-----------------------------------------------------------------------------------------------
HRESULT CItem::Load()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/item000.png",
		&m_apTexture[TYPE_RED]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/item001.png",
		&m_apTexture[TYPE_BLUE]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/item002.png",
		&m_apTexture[TYPE_GREEN]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/item003.png",
		&m_apTexture[TYPE_ORANGE]);

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// テクスチャの削除
//-----------------------------------------------------------------------------------------------
void CItem::Unload()
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
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
HRESULT CItem::Init()
{
	// サイズ
	CObject2D::SetSize(D3DXVECTOR2(SIZE_WIDTH, SIZE_HEIGHT));
	// 初期化
	CObject2D::Init();

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------------------------
void CItem::Uninit()
{
	CObject2D::Uninit();
}

//-----------------------------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------------------------
void CItem::Update()
{
	// 位置の取得
	D3DXVECTOR3 pos = GetPosition();

	// 回転量の加算
	m_fRot += 0.05f;
	if (m_fRot >= D3DX_PI * 2)
	{
		m_fRot = 0.0f;
	}

	// 移動点を中心に回転させる
	pos += D3DXVECTOR3(sinf(m_fRot) * MOVE_DEFAULT - 2.5f,
		-cosf(m_fRot) * MOVE_DEFAULT,
		0.0f);

	// サイズの取得
	D3DXVECTOR2 size = CObject2D::GetSize();

	if (pos.x + (size.x / 2) <= 0.0f)
	{//左画面端に出たら終了
		Uninit();
		return;
	}

	//当たり判定
	if (Collision(pos))
	{// 当たったら終了	
		Uninit();
		return;
	}

	// 位置の更新
	CObject2D::SetPosition(pos);

	// カウントを増やす
	m_nCntAnim++;

	// 一定フレーム数経ったら
	if (m_nCntAnim % ANIM_INTERVAL == 0)
	{
		// アニメーション枚数を1つ進める
		m_nPatternAnim++;
	}

	// アニメーションが終わったら
	if (m_nPatternAnim == MAX_ANIM)
	{
		// アニメーション枚数を初期化
		m_nPatternAnim = 0;
	}
	else
	{
		//頂点座標の設定
		CObject2D::SetVertex();

		//テクスチャアニメーション
		CObject2D::SetAnimation(m_nPatternAnim, 1, DIVISION_U, DIVISION_V);
	}
}

//-----------------------------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------------------------
void CItem::Draw()
{
	CObject2D::Draw();
}

//-----------------------------------------------------------------------------------------------
// 当たり判定
//-----------------------------------------------------------------------------------------------
bool CItem::Collision(D3DXVECTOR3 posStart)
{
	//アイテムのサイズ取得
	float fStartLength = GetLength();

	for (int nCntPlayer = 0; nCntPlayer < CPlayer::PLAYER_MAX; nCntPlayer++)
	{
		// プレイヤー情報の取得
		CPlayer *pPlayer = CGame::GetPlayer(nCntPlayer);

		if (pPlayer != nullptr)
		{
			if (LibrarySpace::SphereCollision2D(posStart, pPlayer->GetPosition(), fStartLength, pPlayer->GetLength()))
			{//アイテムと当たったら(球体の当たり判定)

				//パワーアップ処理
				pPlayer->SetLevel(m_type);

				// プレイヤー死亡音
				CSound::Play(CSound::SOUND_LABEL_SE_ITEM);

				//スコア加算
				CScore* pScore = pPlayer->GetScore();
				if (pScore != nullptr)
				{
					pScore->Add(200);
				}

				return true;	//当たった
			}
		}
	}

	return false;		//当たってない
}
