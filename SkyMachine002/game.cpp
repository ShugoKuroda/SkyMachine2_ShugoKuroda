//===============================================================================================
//
// ゲーム画面処理 [game.cpp]
// Author : SHUGO KURODA
//
//===============================================================================================
#include "manager.h"
#include "game.h"
#include "object.h"

#include "library.h"
#include "load.h"
#include "fade_scene.h"
#include "input_keyboard.h"
#include "input_joypad.h"

#include "score.h"
#include "number.h"
#include "logo.h"
#include "pause.h"

#include "bg.h"
#include "cloud.h"
#include "enemy.h"
#include "enemy_boss.h"
#include "bullet.h"
#include "bullet_option.h"
#include "explosion.h"
#include "spray.h"
#include "bubble.h"
#include "effect.h"
#include "bg_move.h"
#include "meshfield.h"

//-----------------------------------------------------------------------------------------------
// using宣言
//-----------------------------------------------------------------------------------------------
using namespace LibrarySpace;

//-----------------------------------------------------------------------------------------------
// 静的メンバ変数
//-----------------------------------------------------------------------------------------------
bool CGame::m_bCreateCloud = true; 
bool CGame::m_bCreateBubble = false;
bool CGame::m_bDieBoss = false;
CPlayer *CGame::m_pPlayer[CPlayer::PLAYER_MAX] = {};
CScore *CGame::m_pScore = nullptr;
CMeshField *CGame::m_pMeshField = nullptr;

//-----------------------------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------------------------
CGame::CGame() :m_nCntBubble(0), m_nRandBubble(0)
{
	//敵の生成情報を初期化
	ZeroMemory(&m_EnemyInfo, sizeof(m_EnemyInfo));
	//雲の生成情報を初期化
	ZeroMemory(&m_CloudInfo, sizeof(m_CloudInfo));
}

//-----------------------------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------------------------
CGame::~CGame()
{
}

//-----------------------------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------------------------
HRESULT CGame::Init()
{
	//敵情報読み込み
	m_EnemyInfo.pCreate = LoadSpace::GetEnemy();
	//ウェーブ数の読み込み
	m_EnemyInfo.nWave = LoadSpace::GetWave();
	// テクスチャ読み込み
	LoadAll();

	// プレイヤー生成
	for (int nCntPlayer = 0; nCntPlayer < CPlayer::PLAYER_MAX; nCntPlayer++)
	{
		// プレイヤーENTRY情報の取得
		bool bEntry = CManager::GetEntry(nCntPlayer);

		// エントリーしていれば
		if (bEntry == true)
		{// プレイヤー生成
			m_pPlayer[nCntPlayer] = CPlayer::Create(D3DXVECTOR3(-630.0f, CRenderer::SCREEN_HEIGHT, 0.0f), nCntPlayer);
		}
	}

	// 背景の生成
	CBg::Create(CBg::SET_A);
	// スコアの生成
	m_pScore = CScore::Create();

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------------------------
void CGame::Uninit()
{
	//テクスチャの破棄
	UnloadAll();
}

//-----------------------------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------------------------
void CGame::Update()
{
	// プレイヤー生成
	for (int nCntPlayer = 0; nCntPlayer < CPlayer::PLAYER_MAX; nCntPlayer++)
	{
		if (m_pPlayer[nCntPlayer] == nullptr)
		{
			// プレイヤーENTRY情報の取得
			bool bEntry = CManager::GetEntry(nCntPlayer);

			// エントリーしていれば
			if (bEntry == true)
			{// プレイヤー生成
				m_pPlayer[nCntPlayer] = CPlayer::Create(D3DXVECTOR3(-630.0f, CRenderer::SCREEN_HEIGHT, 0.0f), nCntPlayer);
			}
		}
	}

	//雲を生成するかどうか
	if (m_bCreateCloud == true)
	{
		//雲の生成処理
		CreateCloud();
	}
	//泡エフェクトを生成するかどうか
	if (m_bCreateBubble == true)
	{
		//泡エフェクトの生成処理
		CreateBubble();
	}

	// キーボード情報の取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	// ゲームパッド情報の取得
	CInputJoypad *pJoypad = CManager::GetInputJoypad();

	// ボス死亡フラグが立っていなければ
	if (m_bDieBoss == false)
	{
		// ポーズ生成
		if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_PAUSE) == true || pJoypad->GetTrigger(CInputJoypad::JOYKEY_START, 0) == true)
		{
			// ポーズ状態の取得
			bool bPause = CManager::GetPause();
			// ポーズ中でなければ
			if (bPause == false)
			{//ポーズ画面を生成
				CPause::Create();
			}
		}
	}

	//敵の生成処理	
	CreateEnemy();
}

//-----------------------------------------------------------------------------------------------
// 雲のランダム生成
//-----------------------------------------------------------------------------------------------
void CGame::CreateCloud()
{
	m_CloudInfo.nCount++;
	if (m_CloudInfo.nCount >= m_CloudInfo.nRandTime)
	{
		//雲の生成
		CCloud::Create();
		//カウンターリセット
		m_CloudInfo.nCount = 0;

		//雲の再出現時間を乱数で設定
		m_CloudInfo.nRandTime = GetRandNum(250, 180);
	}
}

//-----------------------------------------------------------------------------------------------
// 泡エフェクトの生成
//-----------------------------------------------------------------------------------------------
void CGame::CreateBubble()
{
	m_nCntBubble++;

	if (m_nCntBubble >= m_nRandBubble)
	{
		D3DXVECTOR3 posBubble = D3DXVECTOR3((float)GetRandNum(CRenderer::SCREEN_WIDTH, 0), (float)GetRandNum(CRenderer::SCREEN_HEIGHT, CRenderer::SCREEN_HEIGHT - 250), 0.0f);

		for (int nCntBubble = 0; nCntBubble < 3; nCntBubble++)
		{
			CBubble::Create(D3DXVECTOR3(posBubble.x, posBubble.y - (nCntBubble * 20), posBubble.z), D3DXVECTOR2((float)CBubble::MIN_SIZE * (nCntBubble + 1), (float)CBubble::MIN_SIZE * (nCntBubble + 1)));
		}

		//カウンターリセット
		m_nCntBubble = 0;

		//泡エフェクトの再出現時間を乱数で設定
		m_nRandBubble = GetRandNum(120, 30);
	}
}

//-----------------------------------------------------------------------------------------------
// 敵の生成(外部スクリプト使用)
//-----------------------------------------------------------------------------------------------
void CGame::CreateEnemy()
{
	// 敵生成カウンターの加算
	m_EnemyInfo.nCreatenCount++;

	//敵のウェーブ数だけ生成する
	for (int nCntEnemy = 0; nCntEnemy < m_EnemyInfo.nWave; nCntEnemy++)
	{
		// 敵が出現するフレーム数に達したら && 敵の出現が終わっていないなら
		if (m_EnemyInfo.pCreate[nCntEnemy].nFrame <= m_EnemyInfo.nCreatenCount && m_EnemyInfo.count[nCntEnemy].bCreate == false)
		{
			//敵が連続で出現するまでのカウンターを加算
			m_EnemyInfo.count[nCntEnemy].nCreate++;
			//敵の連続出現間隔カウンターが最大に達したら
			if (m_EnemyInfo.count[nCntEnemy].nCreate >= m_EnemyInfo.pCreate[nCntEnemy].nInterval)
			{
				//敵の生成(位置、種類、体力、移動情報を引数に設定)
				CEnemy *pEnemy = CEnemy::Create(m_EnemyInfo.pCreate[nCntEnemy].pos, (CEnemy::TYPE)m_EnemyInfo.pCreate[nCntEnemy].nType, m_EnemyInfo.pCreate[nCntEnemy].nLife, &m_EnemyInfo.pCreate[nCntEnemy].move[0]);

				//敵の生成数カウンターを加算
				m_EnemyInfo.count[nCntEnemy].nNum++;
				//敵の連続出現間隔カウンターのリセット
				m_EnemyInfo.count[nCntEnemy].nCreate = 0;

				//敵が最大数まで生成したら
				if (m_EnemyInfo.pCreate[nCntEnemy].nNum <= m_EnemyInfo.count[nCntEnemy].nNum)
				{
					// 色の設定がされていたら
					if (m_EnemyInfo.pCreate[nCntEnemy].nColor > 0)
					{// 最後尾の敵の色を設定
						pEnemy->SetItemColor((CEnemy::COLORITEM)m_EnemyInfo.pCreate[nCntEnemy].nColor);
					}
					// このウェーブの生成をやめる
					m_EnemyInfo.count[nCntEnemy].bCreate = true;
				}
			}
		}
	}

	// ボスを出現させる
	if (m_EnemyInfo.nCreatenCount == 60)//420
	{
		CEnemyBoss::Create(D3DXVECTOR3((float)CRenderer::SCREEN_WIDTH, (float)CRenderer::SCREEN_HEIGHT + CEnemyBoss::SIZE_HEIGHT, 0.0f), CEnemy::TYPE_DARK_BOSS);
	}

	if (m_EnemyInfo.nCreatenCount == 100)
	{
		CFadeScene::Create(CFadeScene::TYPE_BLACK);
	}

	// ボス戦用背景の生成
	if (m_EnemyInfo.nCreatenCount == 520)
	{
		//CBgMove::Create();
		m_pMeshField = CMeshField::Create();
	}

	//ロゴの生成
	CreateLogo(m_EnemyInfo.nCreatenCount);
}

//-----------------------------------------------------------------------------------------------
// ロゴの生成
//-----------------------------------------------------------------------------------------------
void CGame::CreateLogo(int nCounter)
{
	if (nCounter == 100)
	{
		// ボス接近中のロゴ
		CLogo::Create(D3DXVECTOR3(CRenderer::SCREEN_WIDTH / 2, 300.0f, 0.0f), D3DXVECTOR2(CRenderer::SCREEN_WIDTH - 400.0f, 90.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, CLogo::TYPE_WARNING, CLogo::ANIM_LENGTHWISE, 420);
	}

	if (nCounter == 220)
	{
		// ボス接近中の説明ロゴ
		CLogo::Create(D3DXVECTOR3(CRenderer::SCREEN_WIDTH / 2, 500.0f, 0.0f), D3DXVECTOR2(CRenderer::SCREEN_WIDTH - 760.0f, 270.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, CLogo::TYPE_WARNING_SUB, CLogo::ANIM_HORIZONTALLY, 300);
	}
}

//-----------------------------------------------------------------------------------------------
// ボスの死亡フラグ設定
//-----------------------------------------------------------------------------------------------
void CGame::SetDieBoss(bool bDie)
{
	// ボスの死亡状態を設定
	m_bDieBoss = bDie;
	// 画面を止める
	CManager::SetPause(true);
}

//-----------------------------------------------------------------------------------------------
// テクスチャ読み込み
//-----------------------------------------------------------------------------------------------
void CGame::LoadAll()
{
	// エフェクト
	CEffect::Load();
	// 雲
	CCloud::Load();
	// プレイヤー
	CPlayer::Load();
	// 敵
	CEnemy::Load();
	// 弾
	CBullet::Load();
	// 爆発
	CExplosion::Load();
	// 水しぶき
	CSpray::Load();
	// 泡エフェクト
	CBubble::Load();
	// 背景
	CBg::Load();
	// スコア
	CNumber::Load();
	// ロゴ
	CLogo::Load();
	// ポーズ画面
	CPause::Load();
	// 高速移動中の背景
	CBgMove::Load();
	// ボス背景
	CMeshField::Load();
	// 弾オプション
	CBulletOption::Load();
}

//-----------------------------------------------------------------------------------------------
// テクスチャ破棄　
//-----------------------------------------------------------------------------------------------
void CGame::UnloadAll()
{
	// エフェクト
	CEffect::Unload();
	// 雲
	CCloud::Unload();
	// プレイヤー
	CPlayer::Unload();
	// 敵
	CEnemy::Unload();
	// 弾
	CBullet::Unload();
	// 爆発
	CExplosion::Unload();
	// 水しぶき
	CSpray::Unload();
	// 泡エフェクト
	CBubble::Unload();
	// 背景
	CBg::Unload();
	// スコア
	CNumber::Unload();
	// ロゴ
	CLogo::Unload();
	// ポーズ画面
	CPause::Unload();
	// 高速移動中の背景
	CBgMove::Unload();
	// ボス背景
	CMeshField::Unload();
	// 弾オプション
	CBulletOption::Unload();
}
