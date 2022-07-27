//-----------------------------------------------------------------------------------------------
//
// 弾の処理[bullet.cpp]
// Author : SHUGO kURODA
//
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------------------------
#include "enemy.h"
#include "manager.h"
#include "renderer.h"

#include "game.h"
#include "library.h"

#include "player.h"
#include "explosion.h"

//-----------------------------------------------------------------------------------------------
// 定数変数
//-----------------------------------------------------------------------------------------------
// 体力
const int CEnemy::LIFE = 50;
// 幅
const float CEnemy::SIZE_WIDTH = 50.0f;
// 高さ
const float CEnemy::SIZE_HEIGHT = 50.0f;

//-----------------------------------------------------------------------------------------------
// 静的メンバ変数
//-----------------------------------------------------------------------------------------------
// テクスチャのポインタ
LPDIRECT3DTEXTURE9 CEnemy::m_apTexture[TYPE_MAX] = { nullptr };

//-----------------------------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------------------------
CEnemy::CEnemy() :
	m_move(0.0f, 0.0f, 0.0f), m_state(STATE_NORMAL), m_type(TYPE_NONE), m_nLife(0), m_nCntState(0), m_nCntAnim(0), m_nPatternAnim(0), m_nCounterAnim(0)
{
	SetObjectType(EObject::OBJ_ENEMY);
}

//-----------------------------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------------------------
CEnemy::~CEnemy()
{
}

//-----------------------------------------------------------------------------------------------
// 生成
//-----------------------------------------------------------------------------------------------
CEnemy *CEnemy::Create(const D3DXVECTOR3& pos, TYPE type, EnemyMove *pEnemyMove)
{
	// ポインタクラスを宣言
	CEnemy* pEnemy = new CEnemy;

	if (pEnemy != nullptr)
	{
		//位置設定
		pEnemy->SetPosition(pos);

		//移動情報の設定
		pEnemy->m_pMoveInfo = pEnemyMove;

		//種類の設定
		pEnemy->m_type = type;

		// 初期化
		pEnemy->Init();

		// テクスチャの設定
		pEnemy->BindTexture(m_apTexture[type]);
	}

	return pEnemy;
}

//-----------------------------------------------------------------------------------------------
// テクスチャの読み込み
//-----------------------------------------------------------------------------------------------
HRESULT CEnemy::Load()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Enemy000.png", &m_apTexture[TYPE_STARFISH]);			// ヒトデ型の敵
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Enemy001.png", &m_apTexture[TYPE_MOSQUITO]);			// 羽虫型の敵
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Enemy002.png", &m_apTexture[TYPE_SPHERE]);				// 球体型の敵
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Enemy003.png", &m_apTexture[TYPE_ASSAULT]);			// 突撃型の敵(バトミントンの玉みたいな敵)
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Enemy004.png", &m_apTexture[TYPE_SEAURCHIN]);			// ウニ型の敵
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Enemy005.png", &m_apTexture[TYPE_ROWLING]);			// 回転型の敵
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Enemy006.png", &m_apTexture[TYPE_FREEFALL]);			// 自由落下型の敵
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Enemy007.png", &m_apTexture[TYPE_SHOT]);				// 射撃型の敵
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Enemy008.png", &m_apTexture[TYPE_SENTRY_GUN]);			// 固定砲台の敵
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/EnemyBoss000.png", &m_apTexture[TYPE_RING_BOSS]);		// リング型の中ボス
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/EnemyBoss001.png", &m_apTexture[TYPE_DARK_BOSS]);		// 大ボス

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// テクスチャの削除
//-----------------------------------------------------------------------------------------------
void CEnemy::Unload()
{
	for (int nCntEnemy = 0; nCntEnemy < TYPE_MAX; nCntEnemy++)
	{
		// テクスチャの破棄
		if (m_apTexture[nCntEnemy] != nullptr)
		{
			m_apTexture[nCntEnemy]->Release();
			m_apTexture[nCntEnemy] = nullptr;
		}
	}
}

//-----------------------------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------------------------
HRESULT CEnemy::Init()
{
	//移動量の計算
	m_nRestTime = m_pMoveInfo->nFrameMove;
	// 寿命の設定
	m_nLife = LIFE;
	// サイズ設定
	CObject2D::SetSize(D3DXVECTOR2(SIZE_WIDTH, SIZE_HEIGHT));
	// オブジェクト情報の初期化
	CObject2D::Init();
	// 頂点カラーの設定
	CObject2D::SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------------------------
void CEnemy::Uninit()
{
	CObject2D::Uninit();
}

//-----------------------------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------------------------
void CEnemy::Update()
{
	// 位置の取得
	D3DXVECTOR3 pos = CObject2D::GetPosition();
	//移動量の設定
	m_move = (m_pMoveInfo->posEnd - pos) / (float)m_nRestTime;
	//移動までの時間を減少
	m_nRestTime--;

	// 移動量の更新
	pos += m_move;

	if (m_nLife <= 0)
	{// ライフが0
		// 爆発の生成
		CExplosion::Create(pos);
		// 破棄
		Uninit();
		return;
	}
	else if (LibrarySpace::OutScreen2D(&pos, CObject2D::GetSize()))
	{//画面外に出たら
		// 破棄
		Uninit();
		return;
	}

	//移動量の再設定
	if (m_nRestTime <= 0)
	{//目的の位置に到達したら
		SetMove();
	}

	//アニメーション処理
	SetAnim();
	//状態処理
	State();
	// 位置の更新
	CObject2D::SetPosition(pos);
	//頂点座標の設定
	CObject2D::SetVertex();
}

//-----------------------------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------------------------
void CEnemy::Draw()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャブレンディングを加算合成にする
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	//描画処理
	CObject2D::Draw();

	//テクスチャブレンディングの設定を元に戻す
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
}

//-----------------------------------------------------------------------------------------------
// 移動量の設定(計算)
//-----------------------------------------------------------------------------------------------
void CEnemy::SetMove()
{
	m_pMoveInfo++;
	m_nRestTime = m_pMoveInfo->nFrameMove;
}

//-----------------------------------------------------------------------------------------------
// 敵ごとにアニメーション(動き方)を設定
//-----------------------------------------------------------------------------------------------
void CEnemy::SetAnim()
{
	//向きの取得
	float fRot = CObject2D::GetRot();
	// 位置の取得
	D3DXVECTOR3 pos = CObject2D::GetPosition();

	switch (m_type)
	{
	case CEnemy::TYPE_STARFISH:
	case CEnemy::TYPE_ROWLING:

		fRot += 0.1f;
		//向きの更新
		CObject2D::SetRot(fRot);
		break;

	case CEnemy::TYPE_MOSQUITO:
		m_nCounterAnim++;
		if (m_nCounterAnim >= 2)
		{
			m_nPatternAnim++;
			m_nCounterAnim = 0;

			if (m_nPatternAnim >= 2)
			{
				m_nPatternAnim = 0;
			}
			CObject2D::SetAnimation(m_nPatternAnim, 0, 2, 1);
		}
		break;

	case CEnemy::TYPE_ASSAULT:
	{
		// プレイヤー情報の取得
		CPlayer *pPlayer = CGame::GetPlayer();
		//常にプレイヤーの方向を向く
		D3DXVECTOR3 posPlayer = pPlayer->GetPosition();
		D3DXVECTOR3 vec = posPlayer - pos;
		float fAngle = atan2f(vec.x, vec.y);
		CObject2D::SetRot(fAngle);
		CObject2D::SetVertex();
	}
	break;

	case CEnemy::TYPE_SPHERE:
		break;

	case CEnemy::TYPE_SEAURCHIN:
		break;

	case CEnemy::TYPE_FREEFALL:
		break;

	case CEnemy::TYPE_SHOT:
		break;

	case CEnemy::TYPE_RING_BOSS:
		break;

	case CEnemy::TYPE_DARK_BOSS:
		break;

	default:
		break;
	}
}

//-----------------------------------------------------------------------------------------------
// ダメージ処理
//-----------------------------------------------------------------------------------------------
void CEnemy::Damage(int nDamage)
{
	m_nLife -= nDamage;
	m_state = STATE_DAMAGE;
	m_nCntState = 5;
}

//-----------------------------------------------------------------------------------------------
// 状態
//-----------------------------------------------------------------------------------------------
void CEnemy::State()
{
	switch (m_state)
	{
	case STATE_NORMAL:			//エネミーの状態が通常の場合
		break;
	case STATE_DAMAGE:			//エネミーがダメージ状態の場合
		//状態カウンターの減算
		m_nCntState--;
		//点滅させる
		if (m_nCntState % 2 == 0)
		{//黄色
			CObject2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
		}
		else
		{//ピンク
			CObject2D::SetColor(D3DXCOLOR(1.0f, 0.0f, 0.5f, 1.0f));
		}

		//状態を通常に戻す
		if (m_nCntState <= 0)
		{
			m_state = STATE_NORMAL;

			CObject2D::SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		}
		break;
	}
}
