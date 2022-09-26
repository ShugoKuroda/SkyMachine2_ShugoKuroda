//===============================================================================================
//
// ゲーム画面処理 [title.cpp]
// Author : SHUGO KURODA
//
//===============================================================================================
#include "title.h"

#include "manager.h"
#include "input_keyboard.h"
#include "input_joypad.h"
#include "object.h"
#include "object2D.h"
#include "library.h"
#include "fade.h"

#include "cloud.h"
#include "fade_scene.h"
#include "library.h"
#include "ui.h"

//-----------------------------------------------------------------------------------------------
// using宣言
//-----------------------------------------------------------------------------------------------
using namespace LibrarySpace;

//-----------------------------------------------------------------------------------------------
// 静的メンバ変数
//-----------------------------------------------------------------------------------------------
// テクスチャのポインタ
LPDIRECT3DTEXTURE9 CTitle::m_apTexture[OBJ_MAX] = { nullptr };

//-----------------------------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------------------------
CTitle::CTitle() :m_nCountMoveBg(0), m_bTitleDraw(false), m_bPressFade(false), m_nCntLoop(0), m_bEntry{ false }, m_bTutorial(false)
{
	for (int nCnt = 0; nCnt < OBJ_MAX - 1; nCnt++)
	{
		m_apObject2D[nCnt] = nullptr;
	}

	// エントリー情報を初期化
	CManager::SetEntry(0, false);
	CManager::SetEntry(1, false);

	//雲の生成情報を初期化
	ZeroMemory(&m_CloudInfo, sizeof(m_CloudInfo));
}

//-----------------------------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------------------------
CTitle::~CTitle()
{
}

//-----------------------------------------------------------------------------------------------
// テクスチャの読み込み処理
//-----------------------------------------------------------------------------------------------
HRESULT CTitle::Load()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title000.jpg", &m_apTexture[BG_SKY]);			// 背景の空
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title001.png", &m_apTexture[LOGO_TITLE]);		// タイトルロゴ
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title002.png", &m_apTexture[LOGO_PLAYER]);		// プレイヤーロゴ
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title004.png", &m_apTexture[LOGO_PRESS]);		// PRESSロゴ
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/tutorial000.png", &m_apTexture[LOGO_TUTORIAL]);// チュートリアル
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title003.png", &m_apTexture[LOGO_SHADOW]);		// プレイヤーロゴ(黒)

	// UIテクスチャのロード
	CUi::Load();

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// テクスチャの破棄
//-----------------------------------------------------------------------------------------------
void CTitle::Unload()
{
	for (int nCnt = 0; nCnt < OBJ_MAX; nCnt++)
	{
		// テクスチャの破棄
		if (m_apTexture[nCnt] != nullptr)
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = nullptr;
		}
	}

	// UIテクスチャの破棄
	CUi::Unload();
}

//-----------------------------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------------------------
HRESULT CTitle::Init()
{
	// テクスチャのロード
	CTitle::Load();
	// 雲
	CCloud::Load();

	// スクリーンサイズの保存
	D3DXVECTOR2 ScreenSize = D3DXVECTOR2((float)CRenderer::SCREEN_WIDTH, (float)CRenderer::SCREEN_HEIGHT);

	for (int nCnt = 0; nCnt < OBJ_MAX - 1; nCnt++)
	{// 生成
		m_apObject2D[nCnt] = new CObject2D;
	}

	// 空
	m_apObject2D[BG_SKY]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, -ScreenSize.y / 2, 0.0f));
	m_apObject2D[BG_SKY]->SetSize(D3DXVECTOR2(ScreenSize.x, ScreenSize.y * 3));
	// タイトルロゴ
	m_apObject2D[LOGO_TITLE]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, ScreenSize.y - 450.0f, 0.0f));
	m_apObject2D[LOGO_TITLE]->SetSize(D3DXVECTOR2(ScreenSize.x - 550.0f, ScreenSize.y / 2.5f));
	// プレイヤーロゴ
	m_apObject2D[LOGO_PLAYER]->SetPosition(D3DXVECTOR3(-ScreenSize.x, 160.0f, 0.0f));
	m_apObject2D[LOGO_PLAYER]->SetSize(D3DXVECTOR2(300.0f, 150.0f));
	// Pressロゴ
	m_apObject2D[LOGO_PRESS]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, ScreenSize.y - 150.0f, 0.0f));
	m_apObject2D[LOGO_PRESS]->SetSize(D3DXVECTOR2(ScreenSize.x - 550.0f, 90.0f));
	// フェード
	m_apObject2D[BG_FADE]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, ScreenSize.y / 2, 0.0f));
	m_apObject2D[BG_FADE]->SetSize(D3DXVECTOR2(ScreenSize.x, ScreenSize.y));
	// チュートリアル
	m_apObject2D[LOGO_TUTORIAL]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, ScreenSize.y / 2, 0.0f));
	m_apObject2D[LOGO_TUTORIAL]->SetSize(D3DXVECTOR2(ScreenSize.x, ScreenSize.y));

	for (int nCnt = 0; nCnt < OBJ_MAX - 1; nCnt++)
	{// 初期化とテクスチャの設定
		m_apObject2D[nCnt]->Init();
		m_apObject2D[nCnt]->BindTexture(m_apTexture[nCnt]);
	}

	//背景とプレイヤーロゴ以外を前に描画する(タイプを設定する)
	for (int nCnt = LOGO_TITLE; nCnt < OBJ_MAX - 1; nCnt++)
	{
		m_apObject2D[nCnt]->SetObjType(CObject::OBJ_TITLE);
	}
	//プレイヤーロゴを背景の次に描画する
	m_apObject2D[LOGO_PLAYER]->SetObjType(CObject::OBJ_TITLE_LOGO);
	m_apObject2D[BG_FADE]->SetObjType(CObject::OBJ_BLACK);
	m_apObject2D[LOGO_TUTORIAL]->SetObjType(CObject::OBJ_LOGO);

	//タイトルとPressロゴを透明にする
	m_apObject2D[LOGO_TITLE]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	m_apObject2D[LOGO_PRESS]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	m_apObject2D[BG_FADE]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
	m_apObject2D[LOGO_TUTORIAL]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	// 雲を複数、初期配置する
	for (int nCntCloud = 0; nCntCloud < 25; nCntCloud++)
	{
		// 位置を設定
		D3DXVECTOR3 pos;
		pos.x = (float)GetRandNum(CRenderer::SCREEN_WIDTH, 0);
		pos.y = (float)GetRandNum(CRenderer::SCREEN_HEIGHT - 300, CRenderer::SCREEN_HEIGHT * -4);
		pos.z = 0.0f;

		// 雲の生成
		CCloud::Create(pos);
	}

	// プレイヤー生成
	for (int nCntPlayer = 0; nCntPlayer < CPlayer::PLAYER_MAX; nCntPlayer++)
	{
		// プレイヤーENTRY情報の取得
		bool bEntry = CManager::GetEntry(nCntPlayer);

		// エントリーしていなければ
		if (bEntry == false)
		{
			switch (nCntPlayer)
			{
			case CPlayer::PLAYER_1:
				// エントリー待ちのUI1を生成
				CUi::Create(D3DXVECTOR3((CRenderer::SCREEN_WIDTH / 4) - 50.0f, 40.0f, 0.0f), D3DXVECTOR2(400.0f, 50.0f),
					CUi::TYPE_PRESS_ANY_BUTTON, CUi::ANIM_FLASHING, CUi::PLAYER_1_NOT_ENTRY);
				// エントリー待ちのUI2を生成
				CUi::Create(D3DXVECTOR3((CRenderer::SCREEN_WIDTH / 4) - 50.0f, 80.0f, 0.0f), D3DXVECTOR2(400.0f, 50.0f),
					CUi::TYPE_TO_START, CUi::ANIM_NONE, CUi::PLAYER_1_NOT_ENTRY);
				break;

			case CPlayer::PLAYER_2:
				// エントリー待ちのUI1を生成
				CUi::Create(D3DXVECTOR3(CRenderer::SCREEN_WIDTH - 300.0f, 40.0f, 0.0f), D3DXVECTOR2(400.0f, 50.0f),
					CUi::TYPE_PRESS_ANY_BUTTON, CUi::ANIM_FLASHING, CUi::PLAYER_2_NOT_ENTRY);
				// エントリー待ちのUI2を生成
				CUi::Create(D3DXVECTOR3(CRenderer::SCREEN_WIDTH - 300.0f, 80.0f, 0.0f), D3DXVECTOR2(400.0f, 50.0f),
					CUi::TYPE_TO_START, CUi::ANIM_NONE, CUi::PLAYER_2_NOT_ENTRY);
				break;
			default:
				break;
			}
		}
	}

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------------------------
void CTitle::Uninit()
{
	// ポインタの破棄
	for (int nCnt = 0; nCnt < OBJ_MAX; nCnt++)
	{
		if (m_apObject2D[nCnt] != nullptr)
		{
			m_apObject2D[nCnt] = nullptr;
		}
	}

	//タイトルテクスチャの破棄
	CTitle::Unload();
	//雲テクスチャの破棄
	CCloud::Unload();
}

//-----------------------------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------------------------
void CTitle::Update()
{
	// 背景アニメーション用カウンターの加算
	m_nCountMoveBg++;

	//背景位置の取得
	D3DXVECTOR3 aPosBg[OBJ_MAX - 1];
	for (int nCnt = 0; nCnt < OBJ_MAX - 1; nCnt++)
	{
		aPosBg[nCnt] = m_apObject2D[nCnt]->GetPosition();
	}

	{
		// プレイヤーのENTRY情報取得
		bool bEntry = CManager::GetEntry(CPlayer::PLAYER_1);

		// エントリーしていれば
		if (bEntry == true)
		{
			// キーボード情報の取得
			CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

			for (int nCnt = CInputKeyboard::KEYINFO_OK; nCnt < CInputKeyboard::KEYINFO_MAX; nCnt++)
			{
				//キーを押されたら
				if (pKeyboard->GetTrigger(nCnt) == true)
				{
					if (m_bTitleDraw == false)
					{
						aPosBg[BG_SKY].y = CRenderer::SCREEN_HEIGHT;
						aPosBg[LOGO_PLAYER].x = 960.0f;
					}
					else
					{
						if (m_bTutorial == true)
						{
							// モードの設定
							CManager::GetFade()->SetFade(CFade::FADE_OUT, CManager::MODE::MODE_GAME);
							return;
						}

						m_bTutorial = true;
						m_apObject2D[BG_FADE]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.7f));

						// 色の取得
						D3DXCOLOR col = m_apObject2D[LOGO_TUTORIAL]->GetColor();

						if (col.a <= 1.0f)
						{
							col.a += 0.05f;
							m_apObject2D[LOGO_TUTORIAL]->SetColor(col);
						}
					}
				}
			}
		}
	}

	// ジョイパッド情報の取得
	CInputJoypad *pJoypad = CManager::GetInputJoypad();

	for (int nCntPlayer = 0; nCntPlayer < CPlayer::PLAYER_MAX; nCntPlayer++)
	{
		// プレイヤーのENTRY情報取得
		bool bEntry = CManager::GetEntry(nCntPlayer);

		// エントリーしていれば
		if (bEntry == true)
		{
			for (int nCnt = CInputJoypad::JOYKEY_UP; nCnt < CInputJoypad::JOYKEY_MAX; nCnt++)
			{
				// キーを押されたら
				if (pJoypad->GetTrigger((CInputJoypad::JOYKEY)nCnt, nCntPlayer) == true)
				{
					if (m_bTitleDraw == false)
					{
						aPosBg[BG_SKY].y = CRenderer::SCREEN_HEIGHT;
						aPosBg[LOGO_PLAYER].x = 960.0f;
					}
					else
					{
						if (m_bTutorial == true)
						{
							// モードの設定
							CManager::GetFade()->SetFade(CFade::FADE_OUT, CManager::MODE::MODE_GAME);
							return;
						}

						m_bTutorial = true;
						m_apObject2D[BG_FADE]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.7f));
					}
				}
			}
		}
	}

	if (m_bTutorial == true)
	{
		// 色の取得
		D3DXCOLOR col = m_apObject2D[LOGO_TUTORIAL]->GetColor();

		if (col.a <= 1.0f)
		{
			col.a += 0.05f;
			m_apObject2D[LOGO_TUTORIAL]->SetColor(col);
		}
	}

	// ゲームスタートが可能になったら
	if (m_bTitleDraw == true)
	{
		// PRESSロゴの色を取得
		D3DXCOLOR col = m_apObject2D[LOGO_PRESS]->GetColor();

		// PRESSロゴを点滅させる
		if (m_bPressFade == false)
		{
			// a値を加算
			col.a += 0.02f;
			// a値の加算が終わったら
			if (col.a >= 1.0f)
			{// a値の減算を始める
				m_bPressFade = true;
			}
		}
		else if (m_bPressFade == true)
		{
			// a値を減算
			col.a -= 0.02f;
			// a値の減算が終わったら
			if (col.a <= 0.0f)
			{// a値の加算を始める
				m_bPressFade = false;
			}
		}

		// PRESSロゴの色を設定
		m_apObject2D[LOGO_PRESS]->SetColor(col);
	}
	// 背景の移動を終えたら
	else if (aPosBg[BG_SKY].y >= CRenderer::SCREEN_HEIGHT)
	{
		// プレイヤーのロゴを左から登場(移動)させる
		aPosBg[LOGO_PLAYER].x += 25.0f;

		// プレイヤーロゴの移動が終わったら
		if (aPosBg[LOGO_PLAYER].x >= 960.0f)
		{
			// タイトルロゴを出現させる
			m_apObject2D[LOGO_TITLE]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			//プレイヤーロゴを所定の位置に設定
			aPosBg[LOGO_PLAYER].x = 960.0f;
			//プレイヤーロゴのテクスチャを変更
			m_apObject2D[LOGO_PLAYER]->BindTexture(m_apTexture[LOGO_SHADOW]);

			//白フェードの設定
			CFadeScene::Create(CFadeScene::TYPE_WHITE);

			// ゲームスタート可能にする
			m_bTitleDraw = true;

			//カウンターのリセット
			m_nCountMoveBg = 0;
		}
	}
	// カウンターが一定数以上
	else if (m_nCountMoveBg >= 60)
	{// 背景の移動を開始する

		//雲の移動処理
		CCloud::Move(3.0f);

		//空を下に移動させる
		aPosBg[BG_SKY].y += 3.0f;
	}

	//背景情報の設定
	for (int nCnt = 0; nCnt < OBJ_MAX - 1; nCnt++)
	{
		//位置設定
		m_apObject2D[nCnt]->SetPosition(aPosBg[nCnt]);
		//頂点座標の設定
		m_apObject2D[nCnt]->SetVertex();
	}

	//雲の生成
	CreateCloud();

	// タイトル画面をループさせるまでの時間
	m_nCntLoop++;

	// 50秒経過でタイトル画面リセット
	if (m_nCntLoop >= 3000)
	{
		//カウンターリセット
		m_nCntLoop = 0;

		// モードの設定
		CManager::GetFade()->SetFade(CFade::FADE_OUT, CManager::MODE::MODE_TITLE);
	}

	// プレイヤー生成
	for (int nCntPlayer = 0; nCntPlayer < CPlayer::PLAYER_MAX; nCntPlayer++)
	{
		// プレイヤーENTRY情報の取得
		bool bEntry = CManager::GetEntry(nCntPlayer);

		// エントリーしていれば
		if (bEntry == true && m_bEntry[nCntPlayer] == false)
		{
			// エントリー状態にする
			m_bEntry[nCntPlayer] = true;

			switch (nCntPlayer)
			{
			case CPlayer::PLAYER_1:
				// PLAYER1のUI
				CUi::Create(D3DXVECTOR3(90.0f, 30.0f, 0.0f), D3DXVECTOR2(120.0f, 50.0f),
					CUi::TYPE_PLAYER1, CUi::ANIM_NONE, CUi::PLAYER_NONE);
				// OKのUI
				CUi::Create(D3DXVECTOR3(190.0f, 30.0f, 0.0f), D3DXVECTOR2(60.0f, 50.0f),
					CUi::TYPE_OK, CUi::ANIM_NONE, CUi::PLAYER_NONE);
				break;

			case CPlayer::PLAYER_2:
				// PLAYER2のUI
				CUi::Create(D3DXVECTOR3(CRenderer::SCREEN_WIDTH - 170.0f, 30.0f, 0.0f), D3DXVECTOR2(120.0f, 50.0f),
					CUi::TYPE_PLAYER2, CUi::ANIM_NONE, CUi::PLAYER_NONE);
				// OKのUI
				CUi::Create(D3DXVECTOR3(CRenderer::SCREEN_WIDTH - 60.0f, 30.0f, 0.0f), D3DXVECTOR2(60.0f, 50.0f),
					CUi::TYPE_OK, CUi::ANIM_NONE, CUi::PLAYER_NONE);
				break;
			default:
				break;
			}
		}
	}
}

//-----------------------------------------------------------------------------------------------
// 雲の生成
//-----------------------------------------------------------------------------------------------
void CTitle::CreateCloud()
{
	m_CloudInfo.nCount++;
	if (m_CloudInfo.nCount >= m_CloudInfo.nRandTime)
	{
		//雲の生成
		CCloud::Create();
		//カウンターリセット
		m_CloudInfo.nCount = 0;

		//雲の再出現時間を乱数で設定
		m_CloudInfo.nRandTime = LibrarySpace::GetRandNum(300, 60);
	}
}
