//-----------------------------------------------------------------------------------------------
//
// 弾の処理[bullet.cpp]
// Author : SHUGO kURODA
//
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------------------------
#include "bullet_option.h"
#include "bullet.h"
#include "manager.h"
#include "renderer.h"

#include "library.h"
#include "game.h"

#include "enemy.h"
#include "explosion.h"

//-----------------------------------------------------------------------------------------------
// 静的メンバ変数
//-----------------------------------------------------------------------------------------------
// テクスチャのポインタ
LPDIRECT3DTEXTURE9 CBulletOption::m_apTexture = { nullptr };

//-----------------------------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------------------------
CBulletOption::CBulletOption() :m_fRad(0.0f), m_nDamage(0), m_col(FADE_NONE)
{
	SetObjType(EObject::OBJ_BULLET);
}

//-----------------------------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------------------------
CBulletOption::~CBulletOption()
{

}

//-----------------------------------------------------------------------------------------------
// 生成
//-----------------------------------------------------------------------------------------------
CBulletOption* CBulletOption::Create(const D3DXVECTOR3& pos, const CPlayer::PLAYER& parent)
{
	// ポインタクラスを宣言
	CBulletOption* pOption = new CBulletOption;

	if (pOption != nullptr)
	{// もしnullptrではなかったら

		// 位置設定
		pOption->SetPosition(pos);

		//親の設定(所有するプレイヤー)
		pOption->m_parent = parent;

		// 初期化
		pOption->Init();

		// テクスチャの設定
		pOption->BindTexture(m_apTexture);
	}

	return pOption;
}

//-----------------------------------------------------------------------------------------------
// テクスチャの読み込み
//-----------------------------------------------------------------------------------------------
HRESULT CBulletOption::Load()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/option000.png",
		&m_apTexture);

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// テクスチャの削除
//-----------------------------------------------------------------------------------------------
void CBulletOption::Unload()
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
HRESULT CBulletOption::Init()
{
	//ダメージ量の設定
	m_nDamage = DAMAGE;

	// 色状態の初期化
	m_col = FADE_GREEN;

	//サイズの設定
	CObject2D::SetSize(D3DXVECTOR2((float)SIZE, (float)SIZE));

	// 初期化
	CObject2D::Init();

	// 色設定
	CObject2D::SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------------------------
void CBulletOption::Uninit()
{
	CObject2D::Uninit();
}

//-----------------------------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------------------------
void CBulletOption::Update()
{
	// 色の取得
	D3DXCOLOR col = GetColor();

	//色を常に変化させる
	if (m_col == FADE_RED)
	{
		col.r += 0.01f;
		col.b -= 0.01f;
		if (col.r >= 1.0f)
		{
			m_col = FADE_GREEN;
		}
	}
	else if (m_col == FADE_GREEN)
	{
		col.g += 0.01f;
		col.r -= 0.01f;
		if (col.g >= 1.0f)
		{
			m_col = FADE_BLUE;
		}
	}
	else if (m_col == FADE_BLUE)
	{
		col.b += 0.01f;
		col.g -= 0.01f;
		if (col.b >= 1.0f)
		{
			m_col = FADE_RED;
		}
	}


	// 位置の取得
	D3DXVECTOR3 pos = CObject2D::GetPosition();
	// プレイヤー位置の取得
	D3DXVECTOR3 posPlayer = CGame::GetPlayer(m_parent)->GetPosition();

	// 回転量の加算
	m_fRad += 0.1f;
	if (m_fRad >= D3DX_PI * 2)
	{
		m_fRad = 0.0f;
	}

	// 位置の更新(プレイヤーを中心に回転させる)
	pos = D3DXVECTOR3(posPlayer.x - sinf(m_fRad) * 100,
		posPlayer.y - cosf(m_fRad) * 100,
		0.0f);

	//当たり判定(球体)
	Collision(pos);

	// 位置の更新
	CObject2D::SetPosition(pos);

	// 色の設定
	SetColor(col);

	//頂点座標の設定
	CObject2D::SetVertex();
}

//-----------------------------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------------------------
void CBulletOption::Draw()
{
	CObject2D::Draw();
}

//-----------------------------------------------------------------------------------------------
// 当たり判定
//-----------------------------------------------------------------------------------------------
bool CBulletOption::Collision(D3DXVECTOR3 posStart)
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
			if (objType == OBJ_ENEMY)
			{
				//オブジェクトポインタを敵にキャスト
				CEnemy *pEnemy = (CEnemy*)pObject;

				if (LibrarySpace::SphereCollision2D(posStart, pEnemy->GetPosition(), fStartLength, pEnemy->GetLength()))
				{//弾と当たったら(球体の当たり判定)
				 //ダメージ処理
					pEnemy->Damage(m_nDamage);
				}
			}

			//プレイヤーの弾と敵ボスの判定
			else if (objType == OBJ_ENEMYBOSS)
			{
				//オブジェクトポインタを敵にキャスト
				CEnemy *pEnemy = (CEnemy*)pObject;

				if (LibrarySpace::SphereCollision2D(posStart, pEnemy->GetPosition(), fStartLength - 60.0f, pEnemy->GetLength()))
				{//弾と当たったら(球体の当たり判定)
				 //ダメージ処理
					pEnemy->Damage(m_nDamage);
				}
			}
		}
	}

	return false;		//当たってない
}
