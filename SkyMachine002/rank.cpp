//=============================================================================
//
// ランク処理 [rank.cpp]
// Author : SHUGO KURODA
//
//=============================================================================
#include "rank.h"

#include "manager.h"
#include "renderer.h"
#include "load.h"

#include "score.h"
#include "input_joypad.h"
#include "input_keyboard.h"
#include "sound.h"
#include "fade.h"

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define SCORE_POS				(D3DXVECTOR3(470.0f,150.0f,0.0f))
#define PLAYER_SCORE_POS		(D3DXVECTOR3(220.0f,CRenderer::SCREEN_HEIGHT - 50.0f,0.0f))
#define SCORE_SIZE				(D3DXVECTOR2(70.0f,90.0f))
#define PLAYER_SCORE_SIZE		(D3DXVECTOR2(50.0f,60.0f))
#define SCORE_SPACE				(50)
#define PLAYER_SCORE_SPACE		(40)

//-----------------------------------------------------------------------------
// 静的メンバ変数
//-----------------------------------------------------------------------------
// テクスチャのポインタ
LPDIRECT3DTEXTURE9 CRank::m_apTexture[TYPE_MAX] = {};
// プレイヤーのスコア
int CRank::m_nScorePlayer[MAX_PLAYER_SCORE] = { 0 };

//=============================================================================
// CRankのコンストラクタ
//=============================================================================
CRank::CRank() :m_pScore{ nullptr }, m_apObject2D{ nullptr }, m_aScore{ 0 }, m_bMyScoreFade{ 0 }, m_bNextMode(false)
{
	SetObjType(CObject::OBJ_UI);
}

//=============================================================================
// CRankのデストラクタ
//=============================================================================
CRank::~CRank()
{
}

//=============================================================================
// 生成処理
//=============================================================================
CRank *CRank::Create()
{
	// ポインタ変数の生成
	CRank *pRank = new CRank;

	// 初期化処理
	pRank->Init();

	return pRank;
}

//-----------------------------------------------------------------------------------------------
//	テクスチャ読み込み
//-----------------------------------------------------------------------------------------------
HRESULT CRank::Load()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CustomBg001.jpg", &m_apTexture[TYPE_BG]);	// 背景
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/rank000.png", &m_apTexture[TYPE_RANK]);	// ランキングUI
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/UI001.png", &m_apTexture[TYPE_PLAYER1]);	// プレイヤー1
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/UI002.png", &m_apTexture[TYPE_PLAYER2]);	// プレイヤー2

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
//	テクスチャ破棄
//-----------------------------------------------------------------------------------------------
void CRank::Unload()
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

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CRank::Init()
{
	for (int nCnt = TYPE_BG; nCnt < TYPE_MAX; nCnt++)
	{// 生成
		m_apObject2D[nCnt] = new CObject2D;
		m_apObject2D[nCnt]->SetPosition(D3DXVECTOR3(CRenderer::SCREEN_WIDTH / 2, CRenderer::SCREEN_HEIGHT / 2, 0.0f));
		m_apObject2D[nCnt]->SetSize(D3DXVECTOR2((float)CRenderer::SCREEN_WIDTH, (float)CRenderer::SCREEN_HEIGHT));
		m_apObject2D[nCnt]->Init();
		m_apObject2D[nCnt]->BindTexture(m_apTexture[nCnt]);
	}

	//オブジェクトの種類設定
	m_apObject2D[TYPE_BG]->SetObjType(EObject::OBJ_BG);
	m_apObject2D[TYPE_RANK]->SetObjType(EObject::OBJ_UI);

	// スコアの生成
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		m_pScore[nCnt] = CScore::Create(D3DXVECTOR3(SCORE_POS.x, SCORE_POS.y + (nCnt * 100.0f), SCORE_POS.z),
			SCORE_SIZE, SCORE_SPACE);
	}

	// プレイヤースコアの生成
	for (int nCnt = 0; nCnt < MAX_PLAYER_SCORE; nCnt++)
	{
		// プレイヤーENTRY情報の取得
		bool bEntry = CManager::GetEntry(nCnt);

		// エントリーしていれば
		if (bEntry == true)
		{// プレイヤースコア生成
			m_pScore[nCnt + MAX_RANKING] = CScore::Create(D3DXVECTOR3(PLAYER_SCORE_POS.x + (nCnt * 650.0f), PLAYER_SCORE_POS.y, SCORE_POS.z),
				PLAYER_SCORE_SIZE, PLAYER_SCORE_SPACE);

			// スコアの設定
			m_aScore[nCnt + MAX_RANKING] = m_nScorePlayer[nCnt];

			// UIの生成
			m_apObject2D[nCnt] = new CObject2D;
			m_apObject2D[nCnt]->SetObjType(EObject::OBJ_UI);
			m_apObject2D[nCnt]->SetPosition(D3DXVECTOR3(100.0f + (nCnt * 650.0f), CRenderer::SCREEN_HEIGHT - 50.0f, 0.0f));
			m_apObject2D[nCnt]->SetSize(D3DXVECTOR2(200.0f, 100.0f));
			m_apObject2D[nCnt]->Init();
			m_apObject2D[nCnt]->BindTexture(m_apTexture[nCnt]);
		}
	}

	// テキストからスコアをロード
	int *pScore = LoadSpace::LoadScore();

	// スコア情報を格納
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		m_aScore[nCnt] = pScore[nCnt];
	}

	//ランキングの更新
	int nData = 0;
	for (int nCnt1 = 0; nCnt1 < MAX_RANKING; nCnt1++)
	{
		for (int nCnt2 = (nCnt1 + 1); nCnt2 < MAX_RANKING + MAX_PLAYER_SCORE; nCnt2++)
		{
			if (m_aScore[nCnt1] <= m_aScore[nCnt2])
			{
				nData = m_aScore[nCnt2];
				m_aScore[nCnt2] = m_aScore[nCnt1];
				m_aScore[nCnt1] = nData;
			}
		}
	}

	m_aScore[5] = m_nScorePlayer[0];
	m_aScore[6] = m_nScorePlayer[1];

	// テキストにスコアをセーブ
	LoadSpace::SaveScore(&m_aScore[0]);

	// Top3のスコア色を設定
	m_pScore[0]->SetColor(D3DXCOLOR(0.8f, 0.8f, 0.1f, 1.0f));
	m_pScore[1]->SetColor(D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f));
	m_pScore[2]->SetColor(D3DXCOLOR(0.8f, 0.6f, 0.3f, 1.0f));

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CRank::Uninit()
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_apObject2D[nCnt] != nullptr)
		{
			m_apObject2D[nCnt]->Uninit();
			m_apObject2D[nCnt] = nullptr;
		}
	}

	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CRank::Update()
{
	// テクスチャの頂点座標を変更
	m_apObject2D[TYPE_BG]->SetAnimBgLeftUp(1, 400, true);

	// キーボード情報の取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	// ゲームパッド情報の取得
	CInputJoypad *pJoypad = CManager::GetInputJoypad();

	for (int nCnt = CInputKeyboard::KEYINFO_OK; nCnt < CInputKeyboard::KEYINFO_MAX; nCnt++)
	{
		// キーボードのENTRY処理
		if (pKeyboard->GetTrigger(nCnt))
		{
			// 決定音
			CSound::Play(CSound::SOUND_LABEL_SE_MENU_OK);

			for (int nCnt = 0; nCnt < MAX_RANKING + MAX_PLAYER_SCORE; nCnt++)
			{
				if (m_pScore[nCnt] != nullptr)
				{// スコア設定
					if (m_aScore[nCnt] > m_pScore[nCnt]->GetScore())
					{
						m_pScore[nCnt]->Set(m_aScore[nCnt]);
					}
				}
			}

			if (m_bNextMode == true)
			{
				// モードの設定
				CManager::GetFade()->SetFade(CFade::FADE_OUT, CManager::MODE::MODE_TITLE);
				return;
			}

			m_bNextMode = true;
			break;
		}
	}

	// プレイヤーのエントリー処理
	for (int nCntController = 0; nCntController < CPlayer::PLAYER_MAX; nCntController++)
	{
		for (int nCnt = CInputJoypad::JOYKEY_UP; nCnt < CInputJoypad::JOYKEY_MAX; nCnt++)
		{
			if (pJoypad->GetTrigger((CInputJoypad::JOYKEY)nCnt, nCntController))
			{
				// 決定音
				CSound::Play(CSound::SOUND_LABEL_SE_MENU_OK);

				for (int nCnt = 0; nCnt < MAX_RANKING + MAX_PLAYER_SCORE; nCnt++)
				{
					if (m_pScore[nCnt] != nullptr)
					{// スコア設定
						if (m_aScore[nCnt] > m_pScore[nCnt]->GetScore())
						{
							m_pScore[nCnt]->Set(m_aScore[nCnt]);
						}
					}
				}

				if (m_bNextMode == true)
				{
					// モードの設定
					CManager::GetFade()->SetFade(CFade::FADE_OUT, CManager::MODE::MODE_TITLE);
					return;
				}

				m_bNextMode = true;
				break;
			}
		}
	}

	// スコア情報を格納
	for (int nCnt = 0; nCnt < MAX_RANKING + MAX_PLAYER_SCORE; nCnt++)
	{
		if (m_pScore[nCnt] != nullptr)
		{// 毎秒スコア加算
			if (m_aScore[nCnt] > m_pScore[nCnt]->GetScore())
			{
				m_pScore[nCnt]->Add(200);
			}
			else if (m_nScorePlayer[0] == m_pScore[nCnt]->GetScore() ||
				m_nScorePlayer[1] == m_pScore[nCnt]->GetScore())
			{
				// PRESSロゴの色を取得
				D3DXCOLOR col = m_pScore[nCnt]->GetColor();

				// PRESSロゴを点滅させる
				if (m_bMyScoreFade[nCnt] == false)
				{
					// a値を加算
					col.a += 0.02f;
					// a値の加算が終わったら
					if (col.a >= 1.0f)
					{// a値の減算を始める
						m_bMyScoreFade[nCnt] = true;
					}
				}
				else if (m_bMyScoreFade[nCnt] == true)
				{
					// a値を減算
					col.a -= 0.02f;
					// a値の減算が終わったら
					if (col.a <= 0.0f)
					{// a値の加算を始める
						m_bMyScoreFade[nCnt] = false;
					}
				}

				// 色の設定
				m_pScore[nCnt]->SetColor(col);
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CRank::Draw()
{
}
