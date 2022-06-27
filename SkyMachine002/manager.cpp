//=============================================================================
//
// マネージャー処理 [manager.cpp]
// Author : 
//
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "sound.h"

#include "input_keyboard.h"
#include "input_joypad.h"
#include "input_mouse.h"

#include "object.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "explosion.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
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

	// オブジェクトの初期化
	CObject::ReleaseAll();

	// テクスチャの読み込み
	CManager::LoadAll();

	// プレイヤー生成
	CPlayer::Create(D3DXVECTOR3(300.0f, 300.0f, 300.0f));
	//敵生成
	CEnemy::Create(D3DXVECTOR3(0.0f, 400.0f, 0.0f));

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CManager::Uninit(void)
{
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

	// オブジェクトの終了処理
	CObject::ReleaseAll();

	//テクスチャの破棄
	CManager::UnloadAll();

	// レンダラの終了処理
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
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
//void CManager::SetMode(CBase *pBase)
//{
//	// オブジェクトの削除
//	CObject::ReleaseAll();
//
//	// ベースの破棄
//	if (m_pBase != NULL)
//	{
//		m_pBase->Uninit();
//		delete m_pBase;
//		m_pBase = NULL;
//	}
//
//	// モードの移行
//	m_pBase = pBase;
//
//	//シーンの初期化
//	if (m_pBase != NULL)
//	{
//		m_pBase->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));
//	}
//}

//=============================================================================
// モードの確定
//=============================================================================
//CBase *CManager::SetMode(void)
//{
//	CBase *pBase = NULL;
//
//	// モードの確定
//	switch (m_mode)
//	{
//	case MODE_TITLE:
//		pBase = new CTitle;
//		break;
//
//	case MODE_GAME:
//		pBase = new CGame;
//		break;
//
//	case MODE_RESULT:
//		pBase = new CResult;
//		break;
//	}
//
//	return pBase;
//}
