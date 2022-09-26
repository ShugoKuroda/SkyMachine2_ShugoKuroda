//=============================================================================
//
// コンティニュー処理[continue.cpp]
// Author : SHUGO KURODA
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object2D.h"
#include "manager.h"
#include "continue.h"
#include "number.h"
#include "ui.h"
#include "logo.h"
#include "player.h"

#include "input_keyboard.h"
#include "input_joypad.h"

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CContinue::m_pTexture = nullptr;
bool CContinue::m_bContinue = false;

//=============================================================================
// コンストラクタ
//=============================================================================
CContinue::CContinue() :m_nContinue(0), m_pVtxBuff(nullptr), m_apNumber{ nullptr }, m_pUi{ nullptr }, m_nCountDown(0)
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CContinue::~CContinue()
{
}

//=============================================================================
// 生成
//=============================================================================
CContinue *CContinue::Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& size)
{
	CContinue *pContinue = new CContinue;

	for (int nCntScore = 0; nCntScore < MAX_CONTINUE; nCntScore++)
	{
		// 数字ポリゴン生成
		pContinue->m_apNumber[nCntScore] = new CNumber;

		if (pContinue->m_apNumber[nCntScore] != nullptr)
		{
			// 位置設定
			pContinue->m_apNumber[nCntScore]->SetPosition(D3DXVECTOR3(pos.x + (nCntScore * 30.0f), pos.y, pos.z));
			// サイズ設定
			pContinue->m_apNumber[nCntScore]->SetSize(size);
			// 初期化
			pContinue->m_apNumber[nCntScore]->Init();
		}
	}

	// 初期化
	pContinue->Init();

	return pContinue;
}

//=============================================================================
// ポリゴンの初期化
//=============================================================================
HRESULT CContinue::Init()
{
	// コンティニュー演出状態にする
	m_bContinue = true;

	// ポーズ状態にする
	CManager::SetPause(true);

	//オブジェクトの種類設定
	SetObjType(EObject::OBJ_PAUSE);

	// コンティニュー？のUI
	m_pUi[0] = CUi::Create(D3DXVECTOR3(500.0f, 300.0f, 0.0f), D3DXVECTOR2(300.0f, 50.0f),
		CUi::TYPE_CONTINUE, CUi::ANIM_NONE, CUi::PLAYER_NONE);
	// エントリー待ちUIを生成
	m_pUi[1] = CUi::Create(D3DXVECTOR3(CRenderer::SCREEN_WIDTH / 2, 400.0f, 0.0f), D3DXVECTOR2(400.0f, 50.0f),
		CUi::TYPE_PRESS_ANY_BUTTON, CUi::ANIM_FLASHING, CUi::PLAYER_NONE);
	m_pUi[1]->SetObjType(OBJ_PAUSE);

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CContinue::Uninit()
{
	for (int nCntNumber = 0; nCntNumber < MAX_CONTINUE; nCntNumber++)
	{
		// 中身があるなら
		if (m_apNumber[nCntNumber] != nullptr)
		{
			// 数字情報の終了処理
			m_apNumber[nCntNumber]->Uninit();
			m_apNumber[nCntNumber] = nullptr;
		}
	}

	for (int nCntUi = 0; nCntUi < MAX_UI; nCntUi++)
	{
		// 中身があるなら
		if (m_pUi[nCntUi] != nullptr)
		{
			// UI情報の終了処理
			m_pUi[nCntUi]->Uninit();
			m_pUi[nCntUi] = nullptr;
		}
	}

	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新
//=============================================================================
void CContinue::Update()
{
	m_nCountDown++;

	if (m_nCountDown >= 60)
	{// 一秒置きにカウントダウンを進める
		m_nCountDown = 0;
		Add(-1);
	}

	// キーボード情報の取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	// ジョイパッド情報の取得
	CInputJoypad *pJoypad = CManager::GetInputJoypad();

	// プレイヤーのエントリー処理
	for (int nCntPlayer = 0; nCntPlayer < CPlayer::PLAYER_MAX; nCntPlayer++)
	{
		// プレイヤーENTRY情報の取得
		bool bEntry = CManager::GetEntry(nCntPlayer);

		if (pJoypad->GetTrigger(CInputJoypad::JOYKEY_A, nCntPlayer) == true && bEntry == false)
		{// カウントダウンを早める
			m_nCountDown = 0;
			Add(-1);
			break;
		}
		else if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_ATTACK) == true && bEntry == false)
		{// カウントダウンを早める
			m_nCountDown = 0;
			Add(-1);
			break;
		}
		// プレイヤーのどちらかが参加したら
		else if (bEntry == true)
		{// 破棄
			Uninit();
			SetContinue(false);
			// ポーズ状態を解除
			CManager::SetPause(false);
			return;
		}
	}

	// カウントダウン終了
	if (m_nContinue < 0)
	{// 破棄
		Uninit();
		
		// ゲームオーバーロゴの生成
		CLogo::Create(D3DXVECTOR3(CRenderer::SCREEN_WIDTH / 2, CRenderer::SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR2(300.0f, 50.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, CLogo::TYPE_GAMEOVER, CLogo::ANIM_LENGTHWISE, 300)->SetObjType(OBJ_PAUSE);
	}
}

//=============================================================================
// 描画
//=============================================================================
void CContinue::Draw()
{
}

//=============================================================================
// スコアの加算
//=============================================================================
void CContinue::Add(const int& nScore)
{
	m_nContinue += nScore;
	Set();
}

//=============================================================================
// スコアの設定
//=============================================================================
void CContinue::Set()
{
	//各桁の数値を格納(pPostexU[桁ごとの数値])
	int aPosTexU[MAX_CONTINUE];

	aPosTexU[0] = (m_nContinue % 100) / 10;
	aPosTexU[1] = m_nContinue % 10;

	//テクスチャを更新する
	for (int nCntScore = 0; nCntScore < MAX_CONTINUE; nCntScore++)
	{
		m_apNumber[nCntScore]->SetAnimation(aPosTexU[nCntScore], 0, CNumber::DIVISION_U, CNumber::DIVISION_V);
	}
}