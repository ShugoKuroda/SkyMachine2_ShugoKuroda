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
#include "game.h"
#include "sound.h"

#include "player.h"
#include "enemy.h"
#include "enemy_boss.h"
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
LPDIRECT3DTEXTURE9 CBullet::m_apTexture[TYPE_MAX] = { nullptr };

//-----------------------------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------------------------
CBullet::CBullet() :
	m_move(0.0f, 0.0f, 0.0f), m_nDamage(0), m_nCntAnim(0), m_nPatternAnim(0)
{
	SetObjType(EObject::OBJ_BULLET);
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
CBullet* CBullet::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const int& nDamage, const EType type)
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

		//テクスチャ種類の設定
		pBullet->m_type = type;

		// 初期化
		pBullet->Init();

		// テクスチャの設定
		pBullet->BindTexture(m_apTexture[pBullet->m_type]);
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
		&m_apTexture[TYPE_PLAYER_BLUE]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bullet001.png",
		&m_apTexture[TYPE_ENEMY_ORANGE]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bullet003.png",
		&m_apTexture[TYPE_ENEMY_LASER]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bullet002.png",
		&m_apTexture[TYPE_ENEMY_RED]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bullet004.png",
		&m_apTexture[TYPE_PLAYER_GREEN]);

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// テクスチャの削除
//-----------------------------------------------------------------------------------------------
void CBullet::Unload()
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
HRESULT CBullet::Init()
{
	if (m_type == TYPE_ENEMY_LASER)
	{
		// サイズ
		CObject2D::SetSize(D3DXVECTOR2(SIZE_WIDTH / 2, SIZE_HEIGHT * 3));
	}
	else
	{
		// サイズ
		CObject2D::SetSize(D3DXVECTOR2(SIZE_WIDTH, SIZE_HEIGHT));
	}

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

			//プレイヤーの弾と敵の判定
			if (objType == OBJ_ENEMY && m_parent == PARENT_PLAYER1||
				objType == OBJ_ENEMY && m_parent == PARENT_PLAYER2)
			{
				//オブジェクトポインタを敵にキャスト
				CEnemy *pEnemy = (CEnemy*)pObject;

				if (LibrarySpace::SphereCollision2D(posStart, pEnemy->GetPosition(), fStartLength, pEnemy->GetLength()))
				{//弾と当たったら(球体の当たり判定)

					// プレイヤー情報の取得
					CPlayer *pPlayer = CGame::GetPlayer(m_parent);

					// 被弾音
					CSound::Play(CSound::SOUND_LABEL_SE_HIT);

					//ダメージ処理
					pEnemy->Damage(m_nDamage, pPlayer);
					// 弾の破棄
					Uninit();
					return true;	//当たった
				}
			}

			//プレイヤーの弾と敵ボスの判定
			else if (objType == OBJ_ENEMYBOSS && m_parent == PARENT_PLAYER1 ||
					objType == OBJ_ENEMYBOSS && m_parent == PARENT_PLAYER2)
			{
				//オブジェクトポインタを敵にキャスト
				CEnemyBoss *pEnemyBoss = (CEnemyBoss*)pObject;
				CEnemyBoss::PATTERN pat = pEnemyBoss->GetPattern();

				// ボスが登場中または死亡中以外
				if (pat != CEnemyBoss::PATTERN_ENTRY && pat != CEnemyBoss::PATTERN_DIE)
				{
					if (LibrarySpace::SphereCollision2D(posStart, pEnemyBoss->GetPosition(), fStartLength - 60.0f, pEnemyBoss->GetLength()))
					{//弾と当たったら(球体の当たり判定)

						// プレイヤー情報の取得
						CPlayer *pPlayer = CGame::GetPlayer(m_parent);

						// 被弾音
						CSound::Play(CSound::SOUND_LABEL_SE_HIT);

						//ダメージ処理
						pEnemyBoss->Damage(m_nDamage, pPlayer);
						// 弾の破棄
						Uninit();
						return true;	//当たった
					}
				}
			}

			//敵の弾とプレイヤーの判定
			if (objType == OBJ_PLAYER && m_parent == PARENT_ENEMY)
			{
				//オブジェクトポインタをプレイヤーにキャスト
				CPlayer *pPlayer = (CPlayer*)pObject;

				//プレイヤーの状態が通常なら
				if (pPlayer->GetState() == CPlayer::STATE_NORMAL)
				{
					if (m_type == TYPE_ENEMY_LASER)
					{
						fStartLength -= 20.0f;
					}
					if (LibrarySpace::SphereCollision2D(posStart, pPlayer->GetPosition(), fStartLength - 10.0f, pPlayer->GetLength() - 30.0f))
					{//弾と当たったら(球体の当たり判定)

						//ダメージ処理
						pPlayer->Damage();
						// 弾の破棄
						Uninit();
						return true;	//当たった
					}
				}
			}
		}
	}

	return false;		//当たってない
}
