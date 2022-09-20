//=============================================================================
//
// ポーズ画面情報 [pause.cpp]
// Author:SHUGO KURODA
//
//=============================================================================
#include "pause.h"
#include "game.h"
#include "fade.h"
#include "input_keyboard.h"
#include "sound.h"
#include "manager.h"
#include "renderer.h"
#include "object2D.h"
#include "input_joypad.h"

//-----------------------------------------------------------------------------
// 静的メンバ変数
//-----------------------------------------------------------------------------
// テクスチャのポインタ
LPDIRECT3DTEXTURE9 CPause::m_apTexture[TYPE_MAX] = {};
CObject2D *CPause::m_apObject2D[TYPE_MAX] = {};

//=============================================================================
// CBulletのコンストラクタ
//=============================================================================
CPause::CPause() :m_bPause(false), m_nPauseSelect(0)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_apObject2D[nCnt] = nullptr;
	}

	// オブジェクトタイプの設定
	SetObjType(OBJ_PAUSE);
}

//=============================================================================
// CPlayerのデストラクタ
//=============================================================================
CPause::~CPause()
{

}

//-----------------------------------------------------------------------------------------------
//	テクスチャ読み込み
//-----------------------------------------------------------------------------------------------
HRESULT CPause::Load()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause000.jpg", &m_apTexture[TYPE_FRAME]);		// ポーズ画面枠
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause001.png", &m_apTexture[TYPE_RESUME]);		// 再開
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause002.png", &m_apTexture[TYPE_RESTART]);	// やり直し
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause003.png", &m_apTexture[TYPE_EXIT]);		// タイトルに戻る
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause003.png", &m_apTexture[TYPE_SELECTOR]);	// 選択カーソル

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
//	テクスチャ破棄
//-----------------------------------------------------------------------------------------------
void CPause::Unload()
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
// 生成処理
//=============================================================================
CPause *CPause::Create()
{
	// ポインタ変数の生成
	CPause *pPause = new CPause;

	// NULLチェック
	if (pPause != nullptr)
	{
		// 初期化処理
		pPause->Init();
	}

	return pPause;
}

//=============================================================================
// ポーズの初期化処理
//=============================================================================
HRESULT CPause::Init()
{
	SetPause();

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{// 初期化とテクスチャの設定
		m_apObject2D[nCnt]->Init();
		m_apObject2D[nCnt]->BindTexture(m_apTexture[nCnt]);
	}

	return S_OK;
}

//=============================================================================
// ポーズの終了処理
//=============================================================================
void CPause::Uninit()
{
	// ポーズ状態をfalseにする
	CManager::SetPause(false);

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_apObject2D[nCnt] != nullptr)
		{
			m_apObject2D[nCnt]->Uninit();
			m_apObject2D[nCnt] = nullptr;
		}
	}

	// オブジェクト破棄
	Release();
}

//=============================================================================
// ポーズの更新処理
//=============================================================================
void CPause::Update()
{
	if (m_bPause == true)
	{
		// キーボード情報の取得
		CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
		// ゲームパッド情報の取得
		CInputJoypad *pJoypad = CManager::GetInputJoypad();

		if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_UP) || pJoypad->GetTrigger(CInputJoypad::JOYKEY_UP, 0))
		{
			// 現在選択されている項目の色を元に戻す
			m_apObject2D[m_nPauseSelect]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			// 上にいくとき番号を減らす
			m_nPauseSelect--;

			// ポーズ項目の最上部を超えたとき
			if (m_nPauseSelect < TYPE_RESUME)
			{
				m_nPauseSelect = TYPE_EXIT;
			}

			// 効果音
			CSound::Play(CSound::SOUND_LABEL_SE_MENU_SELECT);
		}
		else if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_DOWN) || pJoypad->GetTrigger(CInputJoypad::JOYKEY_DOWN, 0))
		{
			// 下にいくとき番号を増やす
			m_nPauseSelect++;

			// ポーズ項目の最下部を超えたとき
			if (m_nPauseSelect > TYPE_EXIT)
			{
				m_nPauseSelect = TYPE_RESUME;
			}

			// 効果音
			CSound::Play(CSound::SOUND_LABEL_SE_MENU_SELECT);
		}

		// 現在選択されている項目は色を黄色に設定
		m_apObject2D[m_nPauseSelect]->SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

		if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_OK) || pJoypad->GetTrigger(CInputJoypad::JOYKEY_A, 0))
		{
			// 選択されているUIを参照し、どの処理をするか決定
			switch (m_nPauseSelect)
			{
				// 再開ボタン
			case TYPE_RESUME:
				// ポーズを閉じる
				CManager::SetPause(false);
				Uninit();
				return;
				// リトライボタン
			case TYPE_RESTART:
				CFade::SetFade(CFade::FADE_OUT, CManager::MODE_GAME);
				Uninit();
				return;
				// 終了ボタン
			case TYPE_EXIT:
				CFade::SetFade(CFade::FADE_OUT, CManager::MODE_TITLE);
				Uninit();
				return;
			default:
				break;
			}
		}

		// ポーズ画面を終える
		if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_PAUSE) || pJoypad->GetTrigger(CInputJoypad::JOYKEY_START, 0))
		{
			SetPause();
			Uninit();
		}
	}
}

//=============================================================================
// ポーズの描画処理
//=============================================================================
void CPause::Draw(void)
{

}

//=============================================================================
// ポーズの処理
//=============================================================================
void CPause::SetPause()
{
	m_bPause = m_bPause ? false : true;

	if (m_bPause == true)
	{
		CSound::Play(CSound::SOUND_LABEL_SE_MENU_IN);
	}
	else if (m_bPause == false)
	{
		CSound::Play(CSound::SOUND_LABEL_SE_MENU_OUT);

		// 選択しているロゴを再開にする
		m_nPauseSelect = 0;
	}

	CManager::SetPause(m_bPause);
}
