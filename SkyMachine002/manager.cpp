//=============================================================================
//
// マネージャー処理 [manager.cpp]
// Author : 
//
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "base.h"
#include "fade.h"
#include "sound.h"
#include "load.h"

#include "input_keyboard.h"
#include "input_joypad.h"
#include "input_mouse.h"

#include "object.h"
#include "title.h"
#include "game.h"
#include "result.h"

#include <time.h>

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CBase *CManager::m_pBase = nullptr;
CRenderer *CManager::m_pRenderer = nullptr;
CInputKeyboard *CManager::m_pInputKeyboard = nullptr;
CInputJoypad *CManager::m_pInputJoypad = nullptr;
CInputMouse *CManager::m_pInputMouse = nullptr;
CSound *CManager::m_pSound = nullptr;	
// フェードクラス
CFade* CManager::m_pFade = nullptr;// サウンド情報のポインタ
bool CManager::m_bPause = false;
bool CManager::m_bEntry[CPlayer::PLAYER_MAX] = { false };

CManager::MODE CManager::m_mode = MODE_TITLE;

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CManager::CManager()
{
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CManager::~CManager()
{
}

//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	// 現在時刻によってランダムに設定する
	srand((unsigned)time(NULL));

	// レンダラーの初期化処理
	m_pRenderer = new CRenderer;

	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Init(hWnd, bWindow);
	}

	// キーボードの初期化処理
	m_pInputKeyboard = new CInputKeyboard;

	if (m_pInputKeyboard != nullptr)
	{
		m_pInputKeyboard->Init(hInstance, hWnd);
	}

	// ジョイパッド情報の初期化処理
	m_pInputJoypad = new CInputJoypad;

	if (m_pInputJoypad != nullptr)
	{
		m_pInputJoypad->Init();
	}

	// マウス情報の初期化処理
	m_pInputMouse = new CInputMouse;

	if (m_pInputMouse != nullptr)
	{
		m_pInputMouse->Init(hInstance, hWnd);
	}

	// サウンドの初期化処理
	m_pSound = new CSound;

	if (m_pSound != nullptr)
	{
		m_pSound->Init(hWnd);
	}

	// フェードの生成
	if (m_pFade == nullptr)
	{
		m_pFade = CFade::Create(m_mode);
	}

	//敵配置情報のロード
	LoadSpace::LoadEnemy(hWnd);

	// モードの設定
	SetMode(MODE_TITLE);

	return S_OK;
}

//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void CManager::Uninit()
{
	// サウンドの終了処理
	if (m_pSound != nullptr)
	{
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = nullptr;
	}

	// レンダラの終了処理
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	// フェードの破棄
	if (m_pFade != nullptr)
	{
		m_pFade->Uninit();
		m_pFade = nullptr;
	}

	// オブジェクトの終了処理
	CObject::ReleaseAll();

	// ベースの破棄
	if (m_pBase != nullptr)
	{
		m_pBase->Uninit();
		delete m_pBase;
		m_pBase = nullptr;
	}

	// キーボードの終了処理
	if (m_pInputKeyboard != nullptr)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = nullptr;
	}

	// ジョイパッドの終了処理
	if (m_pInputJoypad != nullptr)
	{
		m_pInputJoypad->Uninit();
		delete m_pInputJoypad;
		m_pInputJoypad = nullptr;
	}

	// マウスの終了処理
	if (m_pInputMouse != nullptr)
	{
		m_pInputMouse->Uninit();
		delete m_pInputMouse;
		m_pInputMouse = nullptr;
	}
}

//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void CManager::Update()
{
	// キーボードの更新処理(※最初に行う)
	if (m_pInputKeyboard != nullptr)
	{
		m_pInputKeyboard->Update();
	}

	// ジョイパッドの更新処理
	if (m_pInputJoypad != nullptr)
	{
		m_pInputJoypad->Update();
	}

	// マウスの更新処理
	if (m_pInputMouse != nullptr)
	{
		m_pInputMouse->Update();
	}

	// フェードの更新
	if (m_pFade != nullptr)
	{
		m_pFade->Update();
	}

	//ポーズ中でないなら
	if (m_bPause == false)
	{
		// ベースの更新処理
		if (m_pBase != nullptr)
		{
			m_pBase->Update();
		}
	}

	// レンダラの更新処理
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Update();
	}

	// ジョイパッド情報の取得
	CInputJoypad *pJoypad = CManager::GetInputJoypad();

	// プレイヤーのエントリー処理
	for (int nCntController = 0; nCntController < CPlayer::PLAYER_MAX; nCntController++)
	{
		for (int nCnt = CInputJoypad::JOYKEY_UP; nCnt < CInputJoypad::JOYKEY_MAX; nCnt++)
		{
			if (pJoypad->GetTrigger((CInputJoypad::JOYKEY)nCnt, nCntController) && m_bEntry[nCntController] == false)
			{
				// ボタンを押したコントローラーをENTRY状態にする
				SetEntry(nCntController, true);
				// エントリー音
				CSound::Play(CSound::SOUND_LABEL_SE_MENU_IN);
				break;
			}
		}
	}
}

//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void CManager::Draw()
{
	// フェードの更新
	if (m_pFade != nullptr)
	{
		m_pFade->Draw();
	}

	// レンダラの描画処理
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Draw();
	}
}

//-----------------------------------------------------------------------------
// モードの設定
//-----------------------------------------------------------------------------
void CManager::SetMode(MODE mode)
{
	if (m_pBase != nullptr)
	{// 現在の画面を破棄
		m_pBase->Uninit();
		delete m_pBase;
		m_pBase = nullptr;
	}

	// オブジェクトの削除
	CObject::ReleaseAll();

	// モードの設定
	m_mode = mode;

	// モードの確定
	switch (m_mode)
	{
	case MODE_TITLE:
		m_pBase = new CTitle;
		break;

	case MODE_GAME:
		m_pBase = new CGame;
		break;

	case MODE_RESULT:
		m_pBase = new CResult;
		break;
	}

	if (m_pBase != nullptr)
	{//初期化処理
		m_pBase->Init();
	}
}
