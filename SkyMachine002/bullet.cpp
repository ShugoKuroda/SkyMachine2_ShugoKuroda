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

#include "player.h"
#include "enemy.h"
#include "explosion.h"

//-----------------------------------------------------------------------------------------------
// 変数変数
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
LPDIRECT3DTEXTURE9 CBullet::m_pTexture = nullptr;

//-----------------------------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------------------------
CBullet::CBullet() :
	m_move(0.0f, 0.0f, 0.0f), m_nLife(0), m_nCntAnim(0), m_nPatternAnim(0)
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
CBullet* CBullet::Create(const D3DXVECTOR3& pos)
{
	// ポインタクラスを宣言
	CBullet* pBullet = new CBullet;

	if (pBullet != nullptr)
	{// もしnullptrではなかったら
		// 位置設定
		pBullet->SetPosition(pos);

		// 初期化
		pBullet->Init();

		// テクスチャの設定
		pBullet->BindTexture(m_pTexture);
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
		&m_pTexture);

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// テクスチャの削除
//-----------------------------------------------------------------------------------------------
void CBullet::Unload()
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
HRESULT CBullet::Init()
{
	// 移動量
	m_move.x = MOVE_DEFAULT;
	// 寿命
	m_nLife = LIFE;
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

	// ライフを減らしていく
	m_nLife--;

	if (m_nLife == 0)
	{// ライフが0
		// 爆発の生成
		CExplosion::Create(pos);
		// 弾の破棄
		Uninit();
	}
	else
	{
		//当たり判定(球体)
		bool bHit = CollisionSphere(pos);

		if (!bHit)
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
}

//-----------------------------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------------------------
void CBullet::Draw()
{
	CObject2D::Draw();
}

//-----------------------------------------------------------------------------------------------
// 当たり判定(球体)
//-----------------------------------------------------------------------------------------------
bool CBullet::CollisionSphere(D3DXVECTOR3 posStart)
{
	//弾のサイズ取得
	float fStartLength = GetLength();
	int nMaxObj = CObject::GetObjectNumAll();

	for (int nCntObject = 0; nCntObject < nMaxObj; nCntObject++)
	{
		CObject *pObject = CObject::GetObject(nCntObject);
		if (pObject != nullptr)
		{
			CObject::EObject objType = pObject->GetObjType();
			if (objType == OBJ_ENEMY && m_nType == TYPE_PLAYER)
			{
				CEnemy *pEnemy = (CEnemy*)pObject;

				//判定先の位置,サイズの取得
				D3DXVECTOR3 posEnd = pEnemy->GetPosition();
				float fEndLength = pEnemy->GetLength();

				//2つの座標差分を求める
				D3DXVECTOR3 Length = posStart - posEnd;

				//座標差分から距離を求める
				float fLength = D3DXVec3Length(&Length);

				if (fStartLength + fEndLength > fLength)
				{//お互いの球体サイズの合計より距離が小さかったら

					//ダメージ処理
					pEnemy->Damage();
					// 弾の破棄
					Uninit();
					return true;	//当たった
				}
			}
		}
	}

	return false;		//当たってない
}
