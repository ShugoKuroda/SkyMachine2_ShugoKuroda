//-----------------------------------------------------------------------------------------------
//
// 弾の処理[bullet.cpp]
// Author : SHUGO kURODA
//
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------------------------
#include "bullet.h"
#include "manager.h"
#include "renderer.h"

#include "library.h"

#include "player.h"
#include "enemy.h"
#include "explosion.h"

//-----------------------------------------------------------------------------------------------
// 定数定義
//-----------------------------------------------------------------------------------------------
// 幅
const float CBullet::SIZE_WIDTH = 30.0f;
// 高さ
const float CBullet::SIZE_HEIGHT = 30.0f;
// 基本移動量
const float CBullet::MOVE_DEFAULT = 15.0f;
// アニメーション間隔
const int CBullet::ANIM_INTERVAL = 5;
// アニメーション最大数
const int CBullet::MAX_ANIM = 4;
// U座標の最大分割数
const int CBullet::DIVISION_U = 4;
// V座標の最大分割数
const int CBullet::DIVISION_V = 1;

//-----------------------------------------------------------------------------------------------
// 静的メンバ変数
//-----------------------------------------------------------------------------------------------
// テクスチャのポインタ
LPDIRECT3DTEXTURE9 CBullet::m_apTexture = nullptr;

//-----------------------------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------------------------
CBullet::CBullet() :
	m_move(0.0f, 0.0f, 0.0f), m_nDamage(0), m_nCntAnim(0), m_nPatternAnim(0)
{
	SetObjectType(EObject::OBJ_BULLET);
}

//-----------------------------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------------------------
CBullet::~CBullet()
{

}

//-----------------------------------------------------------------------------------------------
// 生成
//-----------------------------------------------------------------------------------------------
CBullet* CBullet::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const int& nDamage)
{
	// ポインタクラスを宣言
	CBullet* pBullet = new CBullet;

	if (pBullet != nullptr)
	{// もしnullptrではなかったら
		// 位置設定
		pBullet->SetPosition(pos);

		//移動量の設定
		pBullet->m_move = move;

		//弾のダメージ量の設定
		pBullet->m_nDamage = nDamage;

		// 初期化
		pBullet->Init();

		// テクスチャの設定
		pBullet->BindTexture(m_apTexture);
	}

	return pBullet;
}

//-----------------------------------------------------------------------------------------------
// テクスチャの読み込み
//-----------------------------------------------------------------------------------------------
HRESULT CBullet::Load()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bullet000.png",
		&m_apTexture);

	if (m_apTexture != nullptr)
	{
		return S_OK;
	}

	return S_FALSE;
}

//-----------------------------------------------------------------------------------------------
// テクスチャの削除
//-----------------------------------------------------------------------------------------------
void CBullet::Unload()
{
	// テクスチャの破棄
	if (m_apTexture != nullptr)
	{
		m_apTexture->Release();
		m_apTexture = nullptr;
	}
}

//-----------------------------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------------------------
HRESULT CBullet::Init()
{
	// サイズ
	CObject2D::SetSize(D3DXVECTOR2(SIZE_WIDTH, SIZE_HEIGHT));

	CObject2D::Init();

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------------------------
void CBullet::Uninit()
{
	CObject2D::Uninit();
}

//-----------------------------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------------------------
void CBullet::Update()
{
	// 位置の取得
	D3DXVECTOR3 pos = CObject2D::GetPosition();

	// 移動量の更新
	pos += m_move;

	if (LibrarySpace::OutScreen2D(&pos, CObject2D::GetSize()))
	{//画面外
		// 弾の破棄
		Uninit();
		return;
	}

	//当たり判定(球体)
	if (!Collision(pos))
	{
		// 位置の更新
		CObject2D::SetPosition(pos);

		// カウントを増やす
		m_nCntAnim++;
		if (m_nCntAnim % ANIM_INTERVAL == 0)
		{
			// 今のアニメーションを1つ進める
			m_nPatternAnim++;
		}

		if (m_nPatternAnim == MAX_ANIM)
		{// アニメーションが終わったら
			// 終了する
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
}

//-----------------------------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------------------------
void CBullet::Draw()
{
	CObject2D::Draw();
}

//-----------------------------------------------------------------------------------------------
// 当たり判定
//-----------------------------------------------------------------------------------------------
bool CBullet::Collision(D3DXVECTOR3 posStart)
{
	//弾のサイズ取得
	float fStartLength = GetLength();

	for (int nCntObject = 0; nCntObject < CObject::MAX_OBJECT; nCntObject++)
	{
		CObject *pObject = CObject::GetObject(nCntObject);
		if (pObject != nullptr)
		{
			CObject::EObject objType = pObject->GetObjType();
			if (objType == OBJ_ENEMY && m_nType == TYPE_PLAYER)
			{
				CEnemy *pEnemy = (CEnemy*)pObject;

				if (LibrarySpace::SphereCollision2D(posStart, pEnemy->GetPosition(), fStartLength, pEnemy->GetLength()))
				{//弾と当たったら(球体の当たり判定)

					//ダメージ処理
					pEnemy->Damage(m_nDamage);
					// 弾の破棄
					Uninit();
					return true;	//当たった
				}
			}
		}
	}

	return false;		//当たってない
}
