//-----------------------------------------------------------------------------------------------
//
// 敵ボスの処理[enemy_boss.cpp]
// Author : SHUGO kURODA
//
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------------------------
#include "enemy_boss.h"
#include "manager.h"
#include "renderer.h"

#include "load.h"

#include "game.h"
#include "library.h"
#include "effect.h"
#include "logo.h"
#include "score.h"

#include "player.h"
#include "bullet.h"
#include "explosion.h"

//-----------------------------------------------------------------------------------------------
// 定数宣言
//-----------------------------------------------------------------------------------------------
// 体力
const int CEnemyBoss::LIFE = 100;
// 幅
const float CEnemyBoss::SIZE_WIDTH = 300.0f;
// 高さ
const float CEnemyBoss::SIZE_HEIGHT = 270.0f;
// U座標の最大分割数
const int CEnemyBoss::DIVISION_U = 4;
// V座標の最大分割数
const int CEnemyBoss::DIVISION_V = 1;
// 攻撃前の予備動作間隔
const int CEnemyBoss::RUSH_OPERATION = 60;

//-----------------------------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------------------------
CEnemyBoss::CEnemyBoss() :m_pattern(PATTERN_ENTRY), m_nCounter(0), m_fAttackRot(0.0f), m_bSizeChange(false),
						m_nCountObject(0), m_PosOld(0.0f, 0.0f, 0.0f)
{
}

//-----------------------------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------------------------
CEnemyBoss::~CEnemyBoss()
{
}

//-----------------------------------------------------------------------------------------------
// 生成
//-----------------------------------------------------------------------------------------------
CEnemyBoss *CEnemyBoss::Create(const D3DXVECTOR3& pos, CEnemy::TYPE type)
{
	// ポインタクラスを宣言
	CEnemyBoss* pEnemy = new CEnemyBoss;

	if (pEnemy != nullptr)
	{
		//位置設定
		pEnemy->SetPosition(pos);

		//種類の設定
		pEnemy->SetType(type);

		// 初期化
		pEnemy->Init();

		// テクスチャの設定
		pEnemy->BindTexture(CEnemy::GetTexture(type));
	}

	return pEnemy;
}

//-----------------------------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------------------------
HRESULT CEnemyBoss::Init()
{
	// 種類を設定
	SetObjType(EObject::OBJ_ENEMYBOSS);
	// 寿命の設定
	CEnemy::SetLife(LIFE);
	// サイズ設定
	CObject2D::SetSize(D3DXVECTOR2(SIZE_WIDTH, SIZE_HEIGHT));
	// オブジェクト情報の初期化
	CObject2D::Init();
	// 頂点カラーの設定
	CObject2D::SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	//テクスチャ座標の設定
	CObject2D::SetAnimation(0, 0, DIVISION_U, DIVISION_V);

	// 攻撃前の予備動作間隔を設定
	m_nCountOperation = RUSH_OPERATION;

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------------------------
void CEnemyBoss::Uninit()
{
	CEnemy::Uninit();
}

//-----------------------------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------------------------
void CEnemyBoss::Update()
{
	// 位置の取得
	D3DXVECTOR3 pos = CObject2D::GetPosition();
	// サイズの取得
	D3DXVECTOR2 size = CEnemy::GetSize();
	// 移動量の取得
	D3DXVECTOR3 move = CEnemy::GetMove();
	//雑魚敵生成情報の取得
	ParentEnemyInfo *pBossInfo = LoadSpace::GetParentEnemy();

	//==================================================================
	// 行動パターン
	//==================================================================
	switch (m_pattern)
	{
		//=================================
		// 登場
		//=================================
	case CEnemyBoss::PATTERN_ENTRY:
		pos += D3DXVECTOR3(-0.5f, -2.0f, 0.0f);

		if (pos.y <= 250.0f)
		{
			m_pattern = PATTERN_NORMAL;
		}

		//拡縮させる
		ChangeSize(&size, 0.5f);
		break;
	
		//=================================
		// 通常
		//=================================
	case CEnemyBoss::PATTERN_NORMAL:

		//移動量の加算
		move += D3DXVECTOR3(0.1f, 0.05f, 0.0f);

		// 移動量の更新
		pos = D3DXVECTOR3((CRenderer::SCREEN_WIDTH - 200.0f) - sinf(move.x) * 100,
			(CRenderer::SCREEN_HEIGHT / 2) - cosf(move.y) * 100,
			0.0f);

		//次の行動に移すまでのカウンター加算
		m_nCounter++;

		if (m_nCounter >= 300)
		{
			//カウンターリセット
			m_nCounter = 0;
			//次の行動をランダムで決める
			m_pattern = (PATTERN)LibrarySpace::GetRandNum(PATTERN_BARRAGE, PATTERN_NORMAL);
		}

		//拡縮させる
		ChangeSize(&size, 0.5f);

		break;

		//=================================
		// 連続突進攻撃
		//=================================
	case CEnemyBoss::PATTERN_RUSH:

		if (m_nCountOperation <= 0)
		{
			m_nCountOperation--;

			// 画面の中心座標を保存
			D3DXVECTOR2 ScreenCenter = D3DXVECTOR2(CRenderer::SCREEN_WIDTH / 2, CRenderer::SCREEN_HEIGHT / 2);

			if (m_nCounter >= 5)
			{
				//位置の保存
				D3DXVECTOR3 vec = m_PosOld - pos;
				//敵からプレイヤーへのベクトル(移動量)に変換する
				D3DXVec3Normalize(&vec, &vec);
				//移動量に倍率を掛ける
				vec.x *= 4.0f;
				vec.y *= 4.0f;
				//移動量の加算
				pos += vec;

				//前回の位置に移動し終えたら
				if (pos.x <= m_PosOld.x)
				{
					// 通常状態に戻す(変数のリセット)
					StateReset();
				}
			}
			else if (m_nCountOperation == -120)
			{
				// 突進する角度を決める
				m_fAttackRot = LibrarySpace::GetRandFloat(3, 0, 100);

				// 敵の予測軌道範囲ロゴ
				CLogo::Create(D3DXVECTOR3(ScreenCenter.x, ScreenCenter.y, 0.0f), D3DXVECTOR2(ScreenCenter.x * 2.5f, SIZE_HEIGHT),
					D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.7f), m_fAttackRot, CLogo::TYPE_NONE, CLogo::ANIM_NONE, 60);

				// 注意喚起標識のロゴ
				CLogo::Create(D3DXVECTOR3(CRenderer::SCREEN_WIDTH / 2, CRenderer::SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR2(200.0f, 200.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, CLogo::TYPE_REMINDER, CLogo::ANIM_FLASHING, 60);

				pos = D3DXVECTOR3(ScreenCenter.x, ScreenCenter.y, 0.0f);
				float fLenght = sqrtf((ScreenCenter.x * ScreenCenter.x) + (ScreenCenter.y * ScreenCenter.y));
				pos += D3DXVECTOR3(cosf(m_fAttackRot) * (fLenght + SIZE_WIDTH), -sinf(m_fAttackRot) * (fLenght + SIZE_HEIGHT), 0);
			}
			else if (m_nCountOperation <= -180)
			{
				// エフェクト生成
				CEffect::Create(pos, D3DXVECTOR2(SIZE_WIDTH, SIZE_HEIGHT), CEffect::TYPE_AFTERIMAGE, CEffect::TEX_BOSS);
				D3DXVECTOR3 vec = D3DXVECTOR3((cosf(m_fAttackRot) * 30.0f), -(sinf(m_fAttackRot) * 30.0f), 0);
				pos -= vec;
				if (LibrarySpace::OutScreen2D(&pos, D3DXVECTOR2(SIZE_WIDTH, SIZE_HEIGHT)) && m_nCountOperation <= -200)
				{
					m_nCountOperation = -90;
					m_nCounter++;

					// 5回突進したら
					if (m_nCounter >= 5)
					{
						pos = D3DXVECTOR3((ScreenCenter.x * 2) + (SIZE_WIDTH / 2), ScreenCenter.y - 110.0f, 0.0f);
					}
				}
			}
			else if (!LibrarySpace::OutScreen2D(&pos, D3DXVECTOR2(SIZE_WIDTH, SIZE_HEIGHT)))
			{
				// エフェクト生成
				CEffect::Create(pos, D3DXVECTOR2(SIZE_WIDTH, SIZE_HEIGHT), CEffect::TYPE_AFTERIMAGE, CEffect::TEX_BOSS);
				// 左に突進する
				pos.x += -30.0f;
			}
		}
		else
		{
			//拡縮させる
			ChangeSize(&size, 2.0f);

			if (m_nCountOperation == RUSH_OPERATION)
			{// 最初のみ位置を保存
				m_PosOld = pos;
			}

			m_nCountOperation--;
			pos.x += m_nCountOperation * 0.1f;
		}

		break;
		//=================================
		// 雑魚敵の生成攻撃
		//=================================
	case CEnemyBoss::PATTERN_CREATE:

		//拡縮させる
		ChangeSize(&size, 3.0f);

		//攻撃カウンターを加算
		m_nCounter++;

		if (m_nCounter >= 60)
		{
			m_nCountOperation--;

			if (m_nCountOperation <= RUSH_OPERATION - pBossInfo->nInterval)
			{
				m_nCountOperation = RUSH_OPERATION;
				CEnemy::Create(pos, (CEnemy::TYPE)pBossInfo->nType, pBossInfo->nLife, pBossInfo->move);
				m_nCountObject++;
				if (m_nCountObject >= pBossInfo->nNum)
				{
					// 通常状態に戻す(変数のリセット)
					StateReset();
				}
			}
		}

		break;
	case CEnemyBoss::PATTERN_BARRAGE:

		//拡縮させる
		ChangeSize(&size, 3.0f);

		//攻撃カウンターを加算
		m_nCounter++;
		//常に回転させる
		m_fAttackRot += 0.05f;

		if (m_nCounter >= 60)
		{
			//連続攻撃カウンターを加算
			m_nCountOperation--;

			if (m_nCountOperation <= RUSH_OPERATION - 3)
			{
				m_nCountOperation = RUSH_OPERATION;
				m_nCountObject++;
				float fDeg = 0.0f;

				while (fDeg <= 330.0f)
				{
					// 弾を円状に配置
					float fRad = (fDeg * (D3DX_PI / 180)) + m_fAttackRot;
					D3DXVECTOR3 vec = D3DXVECTOR3(sinf(fRad) * 10.0f, cosf(fRad) * 10.0f, 0);
					CBullet* pBullet = CBullet::Create(pos, vec, 1, CBullet::TYPE_ENEMY_RED);
					pBullet->SetParent(CBullet::PARENT_ENEMY);
					pBullet->SetRot(fRad);
					fDeg += 60.0f;
				}

				if (m_nCountObject >= 40)
				{
					// 通常状態に戻す(変数のリセット)
					StateReset();
				}
			}
		}
		break;

		//=================================
		// 死亡
		//=================================
	case CEnemyBoss::PATTERN_DIE:

		break;
	default:
		break;
	}

	if (GetLife() <= 0)
	{// ライフが0

		//死亡アニメーションを再生する
		m_pattern = PATTERN_DIE;

		CObject::SetShake(30);

		// 爆発の生成
		CExplosion::Create(pos, GetSize());

		// スコア加算
		CScore *pScore = CGame::GetScore();
		pScore->Add(400);
		// 破棄
		Uninit();

		return;
	}

	//当たり判定
	Collision(pos);
	//移動量更新
	CEnemy::SetMove(move);
	//アニメーション処理
	SetAnim();
	//状態管理
	State();
	// 位置の更新
	CObject2D::SetPosition(pos);
	// サイズの更新
	CObject2D::SetSize(size);
	//頂点座標の設定
	CObject2D::SetVertex();
}

//-----------------------------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------------------------
void CEnemyBoss::Draw()
{
	//描画処理
	CEnemy::Draw();
}

//-----------------------------------------------------------------------------------------------
// プレイヤーとの当たり判定
//-----------------------------------------------------------------------------------------------
bool CEnemyBoss::Collision(D3DXVECTOR3 posStart)
{
	return CEnemy::Collision(posStart);
}

//-----------------------------------------------------------------------------------------------
// ダメージ処理
//-----------------------------------------------------------------------------------------------
void CEnemyBoss::Damage(int nDamage)
{
	CEnemy::Damage(nDamage);
}

//-----------------------------------------------------------------------------------------------
// 状態
//-----------------------------------------------------------------------------------------------
void CEnemyBoss::State()
{
	CEnemy::State();
}

//-----------------------------------------------------------------------------------------------
// 敵ごとにアニメーション,挙動を設定
//-----------------------------------------------------------------------------------------------
void CEnemyBoss::SetAnim()
{
	switch (GetType())
	{
	case CEnemy::TYPE_RING_BOSS:

		break;

	case CEnemy::TYPE_DARK_BOSS:

		break;

	default:
		break;
	}
}

//-----------------------------------------------------------------------------------------------
// 拡縮処理
//-----------------------------------------------------------------------------------------------
void CEnemyBoss::ChangeSize(D3DXVECTOR2 *pSize, const float& fSize)
{
	if (!m_bSizeChange)
	{
		*pSize -= D3DXVECTOR2(fSize, fSize);
		if (pSize->x <= SIZE_WIDTH - 20.0f)
		{
			m_bSizeChange = true;
		}
	}
	else if (m_bSizeChange)
	{
		*pSize += D3DXVECTOR2(fSize, fSize);
		if (pSize->x >= SIZE_WIDTH)
		{
			m_bSizeChange = false;
		}
	}
}

//-----------------------------------------------------------------------------------------------
// メンバ変数リセット
//-----------------------------------------------------------------------------------------------
void CEnemyBoss::StateReset()
{
	m_nCounter = 0;
	m_nCountObject = 0;
	m_fAttackRot = 0.0f;
	m_pattern = PATTERN_NORMAL;
	m_nCountOperation = RUSH_OPERATION;
}
