//===============================================================================================
//
// ゲーム画面処理 [game.cpp]
// Author : SHUGO KURODA
//
//===============================================================================================
#include "game.h"
#include "object.h"

#include "library.h"
#include "load.h"

#include "bg.h"
#include "cloud.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "explosion.h"
#include "spray.h"
#include "bubble.h"

//-----------------------------------------------------------------------------------------------
// 静的メンバ変数
//-----------------------------------------------------------------------------------------------
bool CGame::m_bCreateCloud = true; 
bool CGame::m_bCreateBubble = false;
CPlayer *CGame::m_pPlayer = nullptr;

//-----------------------------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------------------------
CGame::CGame() :m_nCntBubble(0), m_nRandBubble(0)
{
	//敵の生成情報を初期化
	ZeroMemory(&m_EnemySetInfo, sizeof(m_EnemySetInfo));
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
	m_EnemySetInfo.pCreateInfo = LoadSpace::GetEnemy();
	//ウェーブ数の読み込み
	m_EnemySetInfo.nWave = LoadSpace::GetWave();
	// テクスチャ読み込み
	LoadAll();
	// プレイヤー生成
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(300.0f, 300.0f, 0.0f));
	// 背景の生成
	CBg::Create(CBg::SET_A);

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
		m_CloudInfo.nRandTime = LibrarySpace::GetRandNum(250, 180);
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
		D3DXVECTOR3 posBubble = D3DXVECTOR3((float)LibrarySpace::GetRandNum(CRenderer::SCREEN_WIDTH, 0), (float)LibrarySpace::GetRandNum(CRenderer::SCREEN_HEIGHT, CRenderer::SCREEN_HEIGHT - 250), 0.0f);

		for (int nCntBubble = 0; nCntBubble < 3; nCntBubble++)
		{
			CBubble::Create(D3DXVECTOR3(posBubble.x, posBubble.y - (nCntBubble * 20), posBubble.z), D3DXVECTOR2((float)CBubble::MIN_SIZE * (nCntBubble + 1), (float)CBubble::MIN_SIZE * (nCntBubble + 1)));
		}

		//カウンターリセット
		m_nCntBubble = 0;

		//泡エフェクトの再出現時間を乱数で設定
		m_nRandBubble = LibrarySpace::GetRandNum(120, 30);
	}
}

//-----------------------------------------------------------------------------------------------
// 敵の生成(外部スクリプト使用)
//-----------------------------------------------------------------------------------------------
void CGame::CreateEnemy()
{
	// 敵生成カウンターの加算
	m_EnemySetInfo.nCreatenCount++;

	//敵のウェーブ数だけ生成する
	for (int nCntEnemy = 0; nCntEnemy < m_EnemySetInfo.nWave; nCntEnemy++)
	{
		// 敵が出現するフレーム数に達したら && 敵の出現が終わっていないなら
		if (m_EnemySetInfo.pCreateInfo[nCntEnemy].nFrame <= m_EnemySetInfo.nCreatenCount && m_EnemySetInfo.CountInfo[nCntEnemy].bCreate == false)
		{
			//敵が連続で出現するまでのカウンターを加算
			m_EnemySetInfo.CountInfo[nCntEnemy].nCreateTime++;
			//敵の連続出現間隔カウンターが最大に達したら
			if (m_EnemySetInfo.CountInfo[nCntEnemy].nCreateTime >= m_EnemySetInfo.pCreateInfo[nCntEnemy].nInterval)
			{
				//敵の生成(位置、種類、移動情報を引数に設定)
				CEnemy::Create(m_EnemySetInfo.pCreateInfo[nCntEnemy].pos, (CEnemy::TYPE)m_EnemySetInfo.pCreateInfo[nCntEnemy].nType, m_EnemySetInfo.pCreateInfo[nCntEnemy].nLife, &m_EnemySetInfo.pCreateInfo[nCntEnemy].MoveInfo[0]);

				//敵の生成数カウンターを加算
				m_EnemySetInfo.CountInfo[nCntEnemy].nCreateNum++;
				//敵の連続出現間隔カウンターのリセット
				m_EnemySetInfo.CountInfo[nCntEnemy].nCreateTime = 0;

				//敵が最大数まで生成したら
				if (m_EnemySetInfo.pCreateInfo[nCntEnemy].nNum <= m_EnemySetInfo.CountInfo[nCntEnemy].nCreateNum)
				{//このウェーブの生成をやめる
					m_EnemySetInfo.CountInfo[nCntEnemy].bCreate = true;
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------------------------
// テクスチャ読み込み
//-----------------------------------------------------------------------------------------------
void CGame::LoadAll()
{
	//背景
	CBg::Load();
	//雲
	CCloud::Load();
	// プレイヤー
	CPlayer::Load();
	//敵
	CEnemy::Load();
	// 弾
	CBullet::Load();
	// 爆発
	CExplosion::Load();
	//水しぶき
	CSpray::Load();
	// 泡エフェクト
	CBubble::Load();
}

//-----------------------------------------------------------------------------------------------
// テクスチャ破棄　
//-----------------------------------------------------------------------------------------------
void CGame::UnloadAll()
{
	//雲
	CCloud::Unload();
	// プレイヤー
	CPlayer::Unload();
	//敵
	CEnemy::Unload();
	// 弾
	CBullet::Unload();
	// 爆発
	CExplosion::Unload();
	//水しぶき
	CSpray::Unload();
	// 泡エフェクト
	CBubble::Unload();
	//背景
	CBg::Unload();
}
