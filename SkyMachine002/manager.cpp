//=============================================================================
//
// マネージャー処理 [manager.cpp]
// Author : 
//
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "base.h"
#include "sound.h"

#include "input_keyboard.h"
#include "input_joypad.h"
#include "input_mouse.h"

#include "title.h"
#include "game.h"
#include "result.h"

#include "object.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "explosion.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CBase *CManager::m_pBase = nullptr;
CManager::MODE CManager::m_mode = MODE_TITLE;
CRenderer *CManager::m_pRenderer = nullptr;
CInputKeyboard *CManager::m_pInputKeyboard = nullptr;
CInputJoypad *CManager::m_pInputJoypad = nullptr;
CInputMouse *CManager::m_pInputMouse = nullptr;
CSound *CManager::m_pSound = nullptr;					// サウンド情報のポインタ

//=============================================================================
// CManagerのコンストラクタ
//=============================================================================
CManager::CManager()
{
}

//=============================================================================
// CManagerのデストラクタ
//=============================================================================
CManager::~CManager()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
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

	// モードの設定
	SetMode(MODE_TITLE);

	// テクスチャの読み込み
	CManager::LoadAll();

	// プレイヤー生成
	CPlayer::Create(D3DXVECTOR3(300.0f, 300.0f, 0.0f));
	//敵生成
	CEnemy::Create(D3DXVECTOR3(0.0f, 400.0f, 0.0f));

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CManager::Uninit(void)
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

	// オブジェクトの終了処理
	CObject::ReleaseAll();

	//テクスチャの破棄
	CManager::UnloadAll();

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

//=============================================================================
// 更新処理
//=============================================================================
void CManager::Update(void)
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

	// ベースの更新処理
	if (m_pBase != nullptr)
	{
		m_pBase->Update();
	}

	// レンダラの更新処理
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Update();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CManager::Draw(void)
{
	// ベースの描画処理
	if (m_pBase != nullptr)
	{
		m_pBase->Draw();
	}

	// レンダラの描画処理
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Draw();
	}
}

//-------------------------------------------
//	テクスチャ読み込み
//-------------------------------------------
void CManager::LoadAll()
{
	// プレイヤー
	CPlayer::Load();
	//敵
	CEnemy::Load();
	// 弾
	CBullet::Load();
	// 爆発
	CExplosion::Load();
}

//-------------------------------------------
//	テクスチャ破棄
//-------------------------------------------
void CManager::UnloadAll()
{
	// プレイヤー
	CPlayer::Unload();
	//敵
	CEnemy::Unload();
	// 弾
	CBullet::Unload();
	// 爆発
	CExplosion::Unload();
}

//=============================================================================
// モードの設定
//=============================================================================
void CManager::SetMode(MODE mode)
{
	// オブジェクトの削除
	CObject::ReleaseAll();

	if (m_pBase != nullptr)
	{// 現在の画面を破棄
		m_pBase->Uninit();
		delete m_pBase;
		m_pBase = nullptr;
	}

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
}
