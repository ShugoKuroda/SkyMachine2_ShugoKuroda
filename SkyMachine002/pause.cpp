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
CPause::CPause() :m_bPause(false), m_nPauseSelect(0), m_bWait(false), m_nNumPlayer(0)
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
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/option000.png", &m_apTexture[TYPE_SELECTOR]);	// 選択カーソル

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
CPause *CPause::Create(int nNumPlayer)
{
	// ポインタ変数の生成
	CPause *pPause = new CPause;

	// NULLチェック
	if (pPause != nullptr)
	{
		// ポーズメニューを開いたプレイヤー
		pPause->m_nNumPlayer = nNumPlayer;

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
	{// 生成
		m_apObject2D[nCnt] = new CObject2D;
		// オブジェクトタイプの設定
		m_apObject2D[nCnt]->SetObjType(OBJ_PAUSE_MENU);
	}
	m_apObject2D[TYPE_FRAME]->SetObjType(OBJ_PAUSE);

	// スクリーンサイズの保存
	D3DXVECTOR2 ScreenSize = D3DXVECTOR2((float)CRenderer::SCREEN_WIDTH, (float)CRenderer::SCREEN_HEIGHT);

	// 
	m_nPauseSelect = TYPE_RESUME;

	// ポーズ画面枠
	m_apObject2D[TYPE_FRAME]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, ScreenSize.y / 2, 0.0f));
	m_apObject2D[TYPE_FRAME]->SetSize(D3DXVECTOR2(ScreenSize.x - 350.0f, ScreenSize.y - 200.0f));
	// 再開
	m_apObject2D[TYPE_RESUME]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, 200.0f, 0.0f));
	m_apObject2D[TYPE_RESUME]->SetSize(D3DXVECTOR2(ScreenSize.x / 2, 80.0f));
	// やり直し
	m_apObject2D[TYPE_RESTART]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, 350.0f, 0.0f));
	m_apObject2D[TYPE_RESTART]->SetSize(D3DXVECTOR2(ScreenSize.x / 2, 80.0f));
	// タイトルに戻る
	m_apObject2D[TYPE_EXIT]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, 500.0f, 0.0f));
	m_apObject2D[TYPE_EXIT]->SetSize(D3DXVECTOR2(ScreenSize.x / 2, 80.0f));
	// 選択カーソル
	m_apObject2D[TYPE_SELECTOR]->SetPosition(D3DXVECTOR3((ScreenSize.x / 2)- 380.0f, 200.0f, 0.0f));
	m_apObject2D[TYPE_SELECTOR]->SetSize(D3DXVECTOR2(80.0f, 80.0f));

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
	// 選択カーソルの角度取得
	float fRot = m_apObject2D[TYPE_SELECTOR]->GetRot();
	// 角度加算
	fRot += 0.01f;
	// 角度の設定
	m_apObject2D[TYPE_SELECTOR]->SetRot(fRot);

	if (m_bPause == true)
	{
		// キーボード情報の取得
		CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
		// ゲームパッド情報の取得
		CInputJoypad *pJoypad = CManager::GetInputJoypad();

		if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_UP) == true || pJoypad->GetTrigger(CInputJoypad::JOYKEY_UP, m_nNumPlayer) == true)
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

			// 選択されているポーズ項目の位置を取得
			D3DXVECTOR3 pos = m_apObject2D[m_nPauseSelect]->GetPosition();
			// 選択カーソルの位置設定
			m_apObject2D[TYPE_SELECTOR]->SetPosition(D3DXVECTOR3(pos.x - 380.0f, pos.y, pos.z));

			// 効果音
			CSound::Play(CSound::SOUND_LABEL_SE_MENU_SELECT);
		}
		else if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_DOWN) || pJoypad->GetTrigger(CInputJoypad::JOYKEY_DOWN, m_nNumPlayer))
		{
			// 現在選択されている項目の色を元に戻す
			m_apObject2D[m_nPauseSelect]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			// 下にいくとき番号を増やす
			m_nPauseSelect++;

			// ポーズ項目の最下部を超えたとき
			if (m_nPauseSelect > TYPE_EXIT)
			{
				m_nPauseSelect = TYPE_RESUME;
			}

			// 選択されているポーズ項目の位置を取得
			D3DXVECTOR3 pos = m_apObject2D[m_nPauseSelect]->GetPosition();
			// 選択カーソルの位置設定
			m_apObject2D[TYPE_SELECTOR]->SetPosition(D3DXVECTOR3(pos.x - 380.0f, pos.y, pos.z));

			// 効果音
			CSound::Play(CSound::SOUND_LABEL_SE_MENU_SELECT);
		}

		// 現在選択されている項目は色を黄色に設定
		m_apObject2D[m_nPauseSelect]->SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

		if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_OK) || pJoypad->GetTrigger(CInputJoypad::JOYKEY_A, m_nNumPlayer))
		{
			// 選択されているUIを参照し、どの処理をするか決定
			switch (m_nPauseSelect)
			{
				// 再開ボタン
			case TYPE_RESUME:
				// ポーズを閉じる
				SetPause();
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
		if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_PAUSE) ||
			pJoypad->GetTrigger(CInputJoypad::JOYKEY_START, m_nNumPlayer))
		{
			if (m_bWait == true)
			{
				SetPause();
				Uninit();
				return;
			}
		}

		// 頂点座標の設定
		m_apObject2D[TYPE_SELECTOR]->SetVertex();

		m_bWait = true;
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
	}

	CManager::SetPause(m_bPause);
}
