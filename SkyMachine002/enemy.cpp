//-----------------------------------------------------------------------------------------------
//
// 敵の処理[enemy.cpp]
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

#include "score.h"

#include "player.h"
#include "bullet.h"
#include "explosion.h"
#include "spray.h"

//-----------------------------------------------------------------------------------------------
// 定数変数
//-----------------------------------------------------------------------------------------------
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
	m_move(0.0f, 0.0f, 0.0f), m_state(STATE_NORMAL), m_type(TYPE_NONE), m_nLife(0), m_nCntState(0), m_nPattern(0), m_nCounter(0), m_nCountAttack(0), m_nNumPatten(0)
{
	SetObjType(EObject::OBJ_ENEMY);
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
CEnemy *CEnemy::Create(const D3DXVECTOR3& pos, TYPE type, int nLife, EnemyMove *pEnemyMove)
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
		pEnemy->SetType(type);

		//ライフの設定
		pEnemy->SetLife(nLife);

		// 初期化
		pEnemy->Init();

		// テクスチャの設定
		pEnemy->BindTexture(m_apTexture[pEnemy->m_type]);
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
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Enemy008.png", &m_apTexture[TYPE_SENTRYGUN]);			// 固定砲台の敵
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Enemy009.png", &m_apTexture[TYPE_FLYINGFISH]);			// 水面を飛ぶ敵
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
	// 移動量の計算
	m_nRestTime = m_pMoveInfo->nFrameMove;

	// ウニ型の敵だけサイズを2倍にする
	if (m_type == CEnemy::TYPE_SEAURCHIN)
	{
		// サイズ設定
		CObject2D::SetSize(D3DXVECTOR2(SIZE_WIDTH * 2, SIZE_HEIGHT * 2));
	}
	// 固定砲台、敵生み出し型の敵だけサイズを1.5倍にする
	else if (m_type == CEnemy::TYPE_SENTRYGUN || m_type == CEnemy::TYPE_FREEFALL)
	{
		// サイズ設定
		CObject2D::SetSize(D3DXVECTOR2(SIZE_WIDTH * 1.5f, SIZE_HEIGHT * 1.5f));
	}
	else
	{
		// サイズ設定
		CObject2D::SetSize(D3DXVECTOR2(SIZE_WIDTH, SIZE_HEIGHT));
	}
	
	// オブジェクト情報の初期化
	CObject2D::Init();

	//羽虫型と射撃型の敵だけテクスチャ座標を設定する
	if (m_type == CEnemy::TYPE_MOSQUITO || m_type == CEnemy::TYPE_SHOT)
	{
		CObject2D::SetAnimation(m_nPattern, 0, 2, 1);
	}
	// 水面を飛ぶ敵のみ、水しぶきを生成
	else if (m_type == CEnemy::TYPE_FLYINGFISH)
	{
		D3DXVECTOR3 pos = GetPosition();
		CSpray::Create(D3DXVECTOR3(pos.x, pos.y + 20.0f, pos.z));
	}
	
	if (CGame::GetBubble() == false)
	{
		// 頂点カラーの設定
		CObject2D::SetColor(D3DXCOLOR(0.0f, 0.0f, 0.3f, 1.0f));
	}
	else
	{
		// 頂点カラーの設定
		CObject2D::SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	}

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

		// スコア加算
		CScore *pScore = CGame::GetScore();
		pScore->Add(400);
		// 爆発の生成
		CExplosion::Create(pos,GetSize());
		// 破棄
		Uninit();
		return;
	}
	else if (LibrarySpace::OutScreen2D(&pos, CObject2D::GetSize()))
	{//画面外に出たら
		// 水面を飛ぶ敵のみ、水しぶきを生成
		if (m_type == CEnemy::TYPE_FLYINGFISH)
		{
			pos = GetPosition();
			CSpray::Create(D3DXVECTOR3(pos.x, pos.y + 20.0f, pos.z));
		}
		// 破棄
		Uninit();
		return;
	}

	//移動量の再設定
	if (m_nRestTime <= 0)
	{//目的の位置に到達したら
		SetMove(); 
	}

	//当たり判定
	Collision(pos);
	//アニメーション処理
	SetAnim();
	//状態管理
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

			if (CGame::GetBubble() == false)
			{
				CObject2D::SetColor(D3DXCOLOR(0.0f, 0.0f, 0.3f, 1.0f));
			}
			else
			{
				CObject2D::SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
			}
		}
		break;
	}
}

//-----------------------------------------------------------------------------------------------
// プレイヤーとの当たり判定
//-----------------------------------------------------------------------------------------------
bool CEnemy::Collision(D3DXVECTOR3 posStart)
{
	//敵のサイズ取得
	float fStartLength = GetLength();
	//プレイヤー情報の取得
	CPlayer *pPlayer = CGame::GetPlayer();

	if (pPlayer->GetState() == CPlayer::STATE_NORMAL)
	{
		if (LibrarySpace::SphereCollision2D(posStart, pPlayer->GetPosition(), fStartLength, pPlayer->GetLength() - 30.0f))
		{//敵と当たったら(球体の当たり判定)
			//ダメージ処理
			pPlayer->Damage(1);
			return true;	//当たった
		}
	}

	return false;		//当たってない
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
// 敵ごとにアニメーション,挙動を設定
//-----------------------------------------------------------------------------------------------
void CEnemy::SetAnim()
{
	// 位置の取得
	D3DXVECTOR3 pos = CObject2D::GetPosition();
	//向きの取得
	float fRot = CObject2D::GetRot();

	switch (m_type)
	{
		//--------------------------------------
		// 星型、回転型の敵
		//--------------------------------------
	case CEnemy::TYPE_STARFISH:
	case CEnemy::TYPE_ROWLING:

		fRot -= 0.1f;

		//向きの更新
		CObject2D::SetRot(fRot);

		break;

		//--------------------------------------
		// 羽虫型の敵
		//--------------------------------------
	case CEnemy::TYPE_MOSQUITO:

		// 弾の配置
		SetVector();

		// 常にアニメーションする
		m_nCounter++;
		if (m_nCounter >= 2)
		{
			m_nPattern++;
			m_nCounter = 0;

			if (m_nPattern >= 2)
			{
				m_nPattern = 0;
			}
			CObject2D::SetAnimation(m_nPattern, 0, 2, 1);
		}

		break;

		//--------------------------------------
		// 突撃型の敵
		//--------------------------------------
	case CEnemy::TYPE_ASSAULT:
	{
		// 弾の配置
		D3DXVECTOR3 vec = SetVector();

		float fAngle = atan2f(vec.x, vec.y);
		CObject2D::SetRot(fAngle);
	}
	break;

	//--------------------------------------
	// 丸型の敵
	//--------------------------------------
	case CEnemy::TYPE_SPHERE:
		// 弾の配置
		SetVector();
		break;

		//--------------------------------------
		// ウニ型の敵
		//--------------------------------------
	case CEnemy::TYPE_SEAURCHIN:
	{
		// サイズの取得
		D3DXVECTOR2 size = CObject2D::GetSize();
		m_nCounter++;

		if (m_nCounter >= 30)
		{
			//カウンターのリセット
			m_nCounter = 0;
			//パターン番号を切り替える
			m_nPattern = ~m_nPattern;

			if (m_nPattern)
			{
				CObject2D::SetSize(D3DXVECTOR2(size.x + 20.0f, size.y + 20.0f));
			}
			else
			{
				CObject2D::SetSize(D3DXVECTOR2(size.x - 20.0f, size.y - 20.0f));
			}

			//攻撃カウンターを加算
			m_nCountAttack++;
			if (m_nCountAttack >= 3)
			{
				//カウンターリセット
				m_nCountAttack = 0;

				float fDeg = 0.0f;

				while (fDeg <= 340.0f)
				{
					// 弾を円状に配置
					float fRad = fDeg * (D3DX_PI / 180);
					D3DXVECTOR3 vec = D3DXVECTOR3(sinf(fRad) * 7.0f, cosf(fRad) * 7.0f, 0);
					CBullet* pBullet = CBullet::Create(pos, vec, 1, CBullet::TYPE_ENEMY_LASER);
					pBullet->SetParent(CBullet::PARENT_ENEMY);
					pBullet->SetRot(fRad);
					fDeg += 20.0f;
				}
			}
		}
	}
	break;

	//--------------------------------------
	// 自由落下型の敵
	//--------------------------------------
	case CEnemy::TYPE_FREEFALL:
		break;

		//--------------------------------------
		// 射撃型の敵
		//--------------------------------------
	case CEnemy::TYPE_SHOT:
		//攻撃カウンターを加算
		m_nCounter++;

		if (m_nCounter >= 60)
		{
			//連続攻撃カウンターを加算
			m_nCountAttack++;

			if (m_nCountAttack >= 5)
			{
				CBullet::Create(pos, D3DXVECTOR3(-7.0f, 0.0f, 0.0f), 1, CBullet::TYPE_ENEMY_RED)->SetParent(CBullet::PARENT_ENEMY);

				m_nCountAttack = 0;
				m_nPattern = ~m_nPattern;

				m_nNumPatten++;
				if (m_nNumPatten >= 3)
				{
					m_nNumPatten = 0;
					m_nCounter = 0;
					m_nPattern = 0;
				}

				CObject2D::SetAnimation(m_nPattern, 0, 2, 1);
			}
		}
		break;

		//--------------------------------------
		// 固定砲台型の敵
		//--------------------------------------
	case CEnemy::TYPE_SENTRYGUN:
		//攻撃カウンターを加算
		m_nCounter++;
		//常に回転させる
		fRot += 0.01f;

		if (m_nCounter >= 180)
		{
			//連続攻撃カウンターを加算
			m_nCountAttack++;

			if (m_nCountAttack >= 5)
			{
				m_nCountAttack = 0;
				m_nPattern++;
				float fDeg = 0.0f;

				while (fDeg <= 330.0f)
				{
					// 弾を円状に配置
					float fRad = (fDeg * (D3DX_PI / 180)) + fRot;
					D3DXVECTOR3 vec = D3DXVECTOR3(sinf(fRad) * 4.0f, cosf(fRad) * 4.0f, 0);
					CBullet* pBullet = CBullet::Create(pos, vec, 1, CBullet::TYPE_ENEMY_RED);
					pBullet->SetParent(CBullet::PARENT_ENEMY);
					pBullet->SetRot(fRad);
					fDeg += 30.0f;
				}

				if (m_nPattern >= 3)
				{
					m_nCounter = 0;
					m_nPattern = 0;
				}
			}
		}
		CObject2D::SetRot(fRot);

		break;

		//--------------------------------------
		// 水面を飛ぶ敵
		//--------------------------------------
	case CEnemy::TYPE_FLYINGFISH:
		//常に回転させる
		fRot += 0.025f;

		CObject2D::SetRot(fRot);
		break;

	default:
		break;
	}
}

//-----------------------------------------------------------------------------------------------
// 弾ベクトル設定処理
//-----------------------------------------------------------------------------------------------
D3DXVECTOR3 CEnemy::SetVector()
{
	// プレイヤー情報の取得
	D3DXVECTOR3 posPlayer = CGame::GetPlayer()->GetPosition();
	// 位置の取得
	D3DXVECTOR3 pos = CObject2D::GetPosition();

	//常にプレイヤーの方向を向く
	D3DXVECTOR3 vec = posPlayer - pos;

	m_nCounter++;

	if (m_nCounter >= 120)
	{
		//敵からプレイヤーへのベクトル(移動量)に変換する
		D3DXVec3Normalize(&vec, &vec);
		//ベクトル量の調整
		vec.x *= 5.0f;
		vec.y *= 5.0f;
		//弾の生成
		CBullet::Create(pos, vec, 1, CBullet::TYPE_ENEMY_ORANGE)->SetParent(CBullet::PARENT_ENEMY);
		//攻撃カウンターのリセット
		m_nCounter = 0;
	}

	return vec;
}
