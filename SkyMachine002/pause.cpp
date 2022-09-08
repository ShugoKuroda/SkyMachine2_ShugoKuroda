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

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//=============================================================================
// CBulletのコンストラクタ
//=============================================================================
CPause::CPause():CObject()
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
HRESULT CPause::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause000.jpg", &m_apTexture[TYPE_FRAME]);		// ポーズ画面枠
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause001.png", &m_apTexture[TYPE_RESUME]);		// 再開
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause002.png", &m_apTexture[TYPE_RESTART]);	// やり直し
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause003.png", &m_apTexture[TYPE_EXIT]);		// タイトルに戻る

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
//	テクスチャ破棄
//-----------------------------------------------------------------------------------------------
void CPause::Unload(void)
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
CPause *CPause::Create(D3DXVECTOR3 pos,D3DXVECTOR2 size)
{
	// ポインタ変数の生成
	CPause *pPause = new CPause();

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
	// メンバ変数の初期化
	m_nPauseSelect = 0;
	m_nCntBg = 0;
	m_nCntMenuBg = 0;
	m_nCntMenuLogo = 0;
	m_nCntMenuLogoBg = 0;
	m_nCntLogo = 0;
	m_bPause = false;

	CManager::SetPause(m_bPause);

	return S_OK;
}

//=============================================================================
// ポーズの終了処理
//=============================================================================
void CPause::Uninit(void)
{
	// ポーズ状態をfalseにする
	CManager::SetPause(false);

	// オブジェクト破棄
	Release();
}

//=============================================================================
// ポーズの更新処理
//=============================================================================
void CPause::Update(void)
{
	if (m_bPause == true)
	{
		// キーボード情報の取得
		CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

		// ゲームパッド情報の取得
		CInputJoypad *pJoypad = CManager::GetInputJoypad();
		bool *bCrossTrigger = pJoypad->GetCrossKeyTrigger(m_nNumController);

		// フェード情報の取得
		CFade *pFade = CManager::GetFade();

		if (pFade->GetFade() == CFade::FADE_NONE)
		{
			if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_UP) || bCrossTrigger[CInputJoypad::STICK_AND_CROSSKEY_UP])
			{
				// 上にいくとき番号を減らす
				m_nPauseSelect--;

				if (m_nPauseSelect < 0)
				{
					// コンティニューより小さくなったとき
					m_nPauseSelect = m_nCntMenuLogoBg - 1;
				}

				// 効果音
				CSound::Play(CSound::SOUND_LABEL_SE_MOVE_CURSOR);
			}
			else if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_DOWN) || bCrossTrigger[CInputJoypad::STICK_AND_CROSSKEY_DOWN])
			{
				// 下にいくとき番号を増やす
				m_nPauseSelect++;

				if (m_nPauseSelect > m_nCntMenuLogoBg - 1)
				{
					// クイットより大きくなったとき
					m_nPauseSelect = 0;
				}

				// 効果音
				CSound::Play(CSound::SOUND_LABEL_SE_MOVE_CURSOR);
			}

			// 各UIの色を設定
			for (int nCntPauseUI = 0; nCntPauseUI < m_nCntMenuLogoBg; nCntPauseUI++)
			{
				if (m_nPauseSelect == nCntPauseUI)
				{
					// 現在選択されているものはα値1.0fに設定
					m_pPauseMenuLogoBg[nCntPauseUI]->SetTex(D3DXVECTOR2(1, 2));
				}
				else
				{
					// それ以外はα値0.3fに設定
					m_pPauseMenuLogoBg[nCntPauseUI]->SetTex(D3DXVECTOR2(1, 1));
				}
			}

			if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_OK) || pJoypad->GetButtonTrigger(m_nNumController,JOYPAD_DECISION))
			{
				// 選択されているUIを参照し、どの処理をするか決定
				switch (m_nPauseSelect)
				{
					// コンティニューボタン
				case 0:
					SetPause();										// ポーズを閉じる
					break;
					// リトライボタン
				case 1:
					CGame::SetGameMode(CGame::GAMEMODE_TRANSITION_GAME);
					break;
					// 終了ボタン
				case 2:
					CGame::SetGameMode(CGame::GAMEMODE_TRANSITION_TITLE);
					break;
				}
			}

			// ポーズ画面を終える
			if (pKeyboard->GetTrigger(KEY_PAUSE) || pJoypad->GetButtonTrigger(m_nNumController, JOYPAD_PAUSE))
			{
				SetPause();
			}
		}
	}
}

//=============================================================================
// ポーズの描画処理
//=============================================================================
void CPause::Draw(void)
{
	// 背景の描画
	for (int nCnt = 0; nCnt < m_nNumBg; nCnt++)
	{
		m_pBg[nCnt]->Draw();
	}

	// メニュー背景の描画
	for (int nCnt = 0; nCnt < m_nCntMenuBg; nCnt++)
	{
		m_pPauseMenuBg[nCnt]->Draw();
	}

	// その他ロゴの描画
	for (int nCnt = 0; nCnt < m_nLogo; nCnt++)
	{
		m_pLogo[nCnt]->Draw();
	}

	// メニューロゴの描画
	for (int nCnt = 0; nCnt < m_nCntMenuLogoBg; nCnt++)
	{
		m_pPauseMenuLogoBg[nCnt]->Draw();
	}

	// メニューロゴの描画
	for (int nCnt = 0; nCnt < m_nCntMenuLogo; nCnt++)
	{
		m_pPauseMenuLogo[nCnt]->Draw();
	}
}

//=============================================================================
// ポーズの処理
//=============================================================================
void CPause::SetPause()
{
	m_bPause = m_bPause ? false : true;

	if (m_bPause == true)
	{
		CSound::Play(CSound::SOUND_LABEL_SE_PAUSE_IN);
	}
	else if (m_bPause == false)
	{
		CSound::Play(CSound::SOUND_LABEL_SE_PAUSE_OUT);

		// 選択しているロゴを再開にする
		m_nPauseSelect = 0;
	}

	CManager::SetPause(m_bPause);
}

//=============================================================================
// ポーズの処理(プレイヤー番号保存用)
//=============================================================================
void CPause::SetPause(int nNumPlayer)
{
	// プレイヤー番号保存
	m_nNumController = nNumPlayer;

	// ポーズ処理
	SetPause();
}

//=============================================================================
// ポーズデータのロード
//=============================================================================
bool CPause::LoadData(void)
{
	// FILE型のポインタ関数
	FILE *pFile;

	// ファイルを開く
	pFile = fopen("data/TEXT/02_game/pause.txt", "r");

	// NULLチェック
	if (pFile != NULL)
	{
		// 変数宣言
		char aLine[128];
		char aText[128];
		bool bScript = false;
		int nCntTex = 0;
		int nNumTex = 0;
		char **pTextureFileName;

		// SCRIPTを探す
		while (fgets(aLine, 128, pFile) != NULL)
		{
			// 毎行空白を見つけるまで読み込む
			fscanf(pFile, "%s", &aText[0]);

			// SCRIPTを見つけたらそれ以降を読み込む許可を与える
			if (strcmp(&aText[0], "SCRIPT") == 0)
			{
				bScript = true;
			}
			if (bScript == true)
			{
				if (strcmp(&aText[0], "NUM_TEXTURE") == 0)
				{
					fscanf(pFile, "%s %d", &aText[0], &nNumTex);

					// モデルファイルネーム格納用変数の領域を動的に確保する
					pTextureFileName = new char*[nNumTex];

					for (int nCnt = 0; nCnt < nNumTex; nCnt++)
					{
						pTextureFileName[nCnt] = new char[128];
					}
				}
				// TEXTURE_FILENAMEを見つけたらテクスチャファイル名を格納
				if (strcmp(&aText[0], "TEXTURE_FILENAME") == 0 && nCntTex < nNumTex)
				{
					fscanf(pFile, "%s %s", &aText[0], &pTextureFileName[nCntTex][0]);

					nCntTex++;
				}
				// LOGOSETのときスキルゲージ情報を格納する許可を与える
				if (strcmp(&aText[0], "LOGOSET") == 0)
				{
					LoadLogo(pFile, nNumTex, pTextureFileName);
				}
				// BGSETのときスキルゲージ情報を格納する許可を与える
				if (strcmp(&aText[0], "BGSET") == 0)
				{
					LoadBg(pFile, nNumTex, pTextureFileName);
				}
				// PAUSEMENUSETのときスキルゲージ情報を格納する許可を与える
				if (strcmp(&aText[0], "PAUSEMENUSET") == 0)
				{
					LoadPauseMenu(pFile, nNumTex, pTextureFileName);
				}
				// END_SCRIPTを見つけたら読み込みを終える
				if (strcmp(&aText[0], "END_SCRIPT") == 0)
				{
					// メモリの開放
					delete[] pTextureFileName;

					break;
				}
			}
		}
		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{
		return false;
	}
	return true;
}

//=============================================================================
// 背景情報のロード
//=============================================================================
bool CPause::LoadBg(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// 変数宣言
	char aLine[128];
	char aText[128];
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };

	while (fgets(aLine, 128, pFile) != NULL)
	{
		// 毎行空白を見つけるまで読み込む
		fscanf(pFile, "%s", &aText[0]);

		// POSを見つけたら座標情報を格納
		if (strcmp(&aText[0], "NUM_BG") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &m_nNumBg);

			// メモリの確保
			m_pBg = new CScene2D*[m_nNumBg];
		}
		// SKILLGAUGESETのときスキルゲージ情報を格納する許可を与える
		if (strcmp(&aText[0], "BG") == 0)
		{
			Bg(pFile, nNumTex, pTextureFileName);
		}
		// END_PARTSSETを見つけたら各パーツの情報を格納を終える
		if (strcmp(&aText[0], "END_BGSET") == 0)
		{
			break;
		}
	}
	return false;
}

//=============================================================================
// ポーズメニュー情報のロード
//=============================================================================
bool CPause::LoadPauseMenu(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// 変数宣言
	char aLine[128];
	char aText[128];
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };

	while (fgets(aLine, 128, pFile) != NULL)
	{
		// 毎行空白を見つけるまで読み込む
		fscanf(pFile, "%s", &aText[0]);

		// POSを見つけたら座標情報を格納
		if (strcmp(&aText[0], "NUM_MENUBG") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &m_nNumMenuBg);

			// メモリの確保
			m_pPauseMenuBg = new CScene2D*[m_nNumMenuBg];
		}
		// POSを見つけたら座標情報を格納
		if (strcmp(&aText[0], "NUM_MENULOGO") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &m_nNumLogo);

			// メモリの確保
			m_pPauseMenuLogo = new CScene2D*[m_nNumLogo];

			// ロゴとロゴ背景の数を一致させる
			m_nNumLogoBg = m_nNumLogo;
			
			// メモリの確保
			m_pPauseMenuLogoBg = new CScene2D*[m_nNumLogoBg];
		}
		// SKILLGAUGESETのときスキルゲージ情報を格納する許可を与える
		if (strcmp(&aText[0], "MENUBG") == 0)
		{
			MenuBg(pFile, nNumTex, pTextureFileName);
		}
		// SKILLGAUGESETのときスキルゲージ情報を格納する許可を与える
		if (strcmp(&aText[0], "MENULOGO") == 0)
		{
			MenuLogo(pFile, nNumTex, pTextureFileName);
		}
		// SKILLGAUGESETのときスキルゲージ情報を格納する許可を与える
		if (strcmp(&aText[0], "MENULOGOBG") == 0)
		{
			MenuLogoBg(pFile, nNumTex, pTextureFileName);
		}
		// END_PARTSSETを見つけたら各パーツの情報を格納を終える
		if (strcmp(&aText[0], "END_BGSET") == 0)
		{
			break;
		}
	}
	return false;
}

//=============================================================================
// 背景情報のロード
//=============================================================================
bool CPause::Bg(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// 変数宣言
	char aLine[128];
	char aText[128];
	int nTex;
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };
	D3DXCOLOR col = { 0.0f,0.0f,0.0f,0.0f };

	while (fgets(aLine, 128, pFile) != NULL)
	{
		// 毎行空白を見つけるまで読み込む
		fscanf(pFile, "%s", &aText[0]);

		// POSを見つけたら座標情報を格納
		if (strcmp(&aText[0], "TYPE") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &nTex);
		}
		// POSを見つけたら座標情報を格納
		if (strcmp(&aText[0], "POS") == 0)
		{
			fscanf(pFile, "%s %f %f %f", &aText[0],
				&pos.x,
				&pos.y,
				&pos.z);
		}
		// SIZEを見つけたらサイズ情報を格納
		if (strcmp(&aText[0], "SIZE") == 0)
		{
			fscanf(pFile, "%s %f %f", &aText[0],
				&size.x,
				&size.y);
		}
		// POSを見つけたら座標情報を格納
		if (strcmp(&aText[0], "COLOR") == 0)
		{
			fscanf(pFile, "%s %f %f %f %f", &aText[0],
				&col.r,
				&col.g,
				&col.b,
				&col.a);
		}
		// END_PARTSSETを見つけたら各パーツの情報を格納を終える
		if (strcmp(&aText[0], "END_BG") == 0)
		{
			if (nTex < nNumTex)
			{
				// テクスチャ情報の取得
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL && m_nCntBg < m_nNumBg)
				{
					// スキルゲージの生成
					if (nTex != -1)
					{
						m_pBg[m_nCntBg] = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE, pTex->SetTexture(&pTextureFileName[nTex][0]));
						m_pBg[m_nCntBg]->SetCol(col);
						m_nCntBg++;
					}
					else
					{
						m_pBg[m_nCntBg] = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE, nTex);
						m_pBg[m_nCntBg]->SetCol(col);
						m_nCntBg++;
					}

					break;
				}
			}
		}
	}
	return false;
}

//=============================================================================
// メニュー背景情報のロード
//=============================================================================
bool CPause::MenuBg(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// 変数宣言
	char aLine[128];
	char aText[128];
	int nTex;
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };
	D3DXCOLOR col = { 0.0f,0.0f,0.0f,0.0f };

	while (fgets(aLine, 128, pFile) != NULL)
	{
		// 毎行空白を見つけるまで読み込む
		fscanf(pFile, "%s", &aText[0]);

		// POSを見つけたら座標情報を格納
		if (strcmp(&aText[0], "TYPE") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &nTex);
		}
		// POSを見つけたら座標情報を格納
		if (strcmp(&aText[0], "POS") == 0)
		{
			fscanf(pFile, "%s %f %f %f", &aText[0],
				&pos.x,
				&pos.y,
				&pos.z);
		}
		// SIZEを見つけたらサイズ情報を格納
		if (strcmp(&aText[0], "SIZE") == 0)
		{
			fscanf(pFile, "%s %f %f", &aText[0],
				&size.x,
				&size.y);
		}
		// POSを見つけたら座標情報を格納
		if (strcmp(&aText[0], "COLOR") == 0)
		{
			fscanf(pFile, "%s %f %f %f %f", &aText[0],
				&col.r,
				&col.g,
				&col.b,
				&col.a);
		}
		// END_PARTSSETを見つけたら各パーツの情報を格納を終える
		if (strcmp(&aText[0], "END_MENUBG") == 0)
		{
			if (nTex < nNumTex)
			{
				// テクスチャ情報の取得
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL && m_nCntMenuBg < m_nNumMenuBg)
				{
					// スキルゲージの生成
					if (nTex != -1)
					{
						m_pPauseMenuBg[m_nCntMenuBg] = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE, pTex->SetTexture(&pTextureFileName[nTex][0]));
						m_pPauseMenuBg[m_nCntMenuBg]->SetCol(col);
						m_nCntMenuBg++;
					}
					else
					{
						m_pPauseMenuBg[m_nCntMenuBg] = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE, nTex);
						m_pPauseMenuBg[m_nCntMenuBg]->SetCol(col);
						m_nCntMenuBg++;
					}

					break;
				}
			}
		}
	}
	return false;
}

//=============================================================================
// メニューロゴ情報のロード
//=============================================================================
bool CPause::MenuLogo(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// 変数宣言
	char aLine[128];
	char aText[128];
	int nTex;
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };
	D3DXCOLOR col = { 0.0f,0.0f,0.0f,0.0f };
	D3DXVECTOR2 tex = { 0.0f,0.0f };

	while (fgets(aLine, 128, pFile) != NULL)
	{
		// 毎行空白を見つけるまで読み込む
		fscanf(pFile, "%s", &aText[0]);

		// POSを見つけたら座標情報を格納
		if (strcmp(&aText[0], "TYPE") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &nTex);
		}
		// POSを見つけたら座標情報を格納
		if (strcmp(&aText[0], "POS") == 0)
		{
			fscanf(pFile, "%s %f %f %f", &aText[0],
				&pos.x,
				&pos.y,
				&pos.z);
		}
		// SIZEを見つけたらサイズ情報を格納
		if (strcmp(&aText[0], "SIZE") == 0)
		{
			fscanf(pFile, "%s %f %f", &aText[0],
				&size.x,
				&size.y);
		}
		// POSを見つけたら座標情報を格納
		if (strcmp(&aText[0], "COLOR") == 0)
		{
			fscanf(pFile, "%s %f %f %f %f", &aText[0],
				&col.r,
				&col.g,
				&col.b,
				&col.a);
		}
		// POSを見つけたら座標情報を格納
		if (strcmp(&aText[0], "TEX") == 0)
		{
			fscanf(pFile, "%s %f %f", &aText[0],
				&tex.x,
				&tex.y);
		}
		// END_PARTSSETを見つけたら各パーツの情報を格納を終える
		if (strcmp(&aText[0], "END_MENULOGO") == 0)
		{
			if (nTex < nNumTex)
			{
				// テクスチャ情報の取得
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL && m_nCntMenuLogo < m_nNumLogo)
				{
					// スキルゲージの生成
					if (nTex != -1)
					{
						m_pPauseMenuLogo[m_nCntMenuLogo] = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE, pTex->SetTexture(&pTextureFileName[nTex][0]));
						m_pPauseMenuLogo[m_nCntMenuLogo]->SetCol(col);
						m_nCntMenuLogo++;
					}
					else
					{
						m_pPauseMenuLogo[m_nCntMenuLogo] = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE, nTex);
						m_pPauseMenuLogo[m_nCntMenuLogo]->SetCol(col);
						m_nCntMenuLogo++;
					}

					break;
				}
			}
		}
	}
	return false;
}

//=============================================================================
// メニューロゴ背景情報のロード
//=============================================================================
bool CPause::MenuLogoBg(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// 変数宣言
	char aLine[128];
	char aText[128];
	int nTex;
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };
	D3DXCOLOR col = { 0.0f,0.0f,0.0f,0.0f };
	D3DXVECTOR2 tex = { 0.0f,0.0f };

	while (fgets(aLine, 128, pFile) != NULL)
	{
		// 毎行空白を見つけるまで読み込む
		fscanf(pFile, "%s", &aText[0]);

		// POSを見つけたら座標情報を格納
		if (strcmp(&aText[0], "TYPE") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &nTex);
		}
		// POSを見つけたら座標情報を格納
		if (strcmp(&aText[0], "POS") == 0)
		{
			fscanf(pFile, "%s %f %f %f", &aText[0],
				&pos.x,
				&pos.y,
				&pos.z);
		}
		// SIZEを見つけたらサイズ情報を格納
		if (strcmp(&aText[0], "SIZE") == 0)
		{
			fscanf(pFile, "%s %f %f", &aText[0],
				&size.x,
				&size.y);
		}
		// POSを見つけたら座標情報を格納
		if (strcmp(&aText[0], "COLOR") == 0)
		{
			fscanf(pFile, "%s %f %f %f %f", &aText[0],
				&col.r,
				&col.g,
				&col.b,
				&col.a);
		}
		// POSを見つけたら座標情報を格納
		if (strcmp(&aText[0], "TEX") == 0)
		{
			fscanf(pFile, "%s %f %f", &aText[0],
				&tex.x,
				&tex.y);
		}
		// END_PARTSSETを見つけたら各パーツの情報を格納を終える
		if (strcmp(&aText[0], "END_MENULOGOBG") == 0)
		{
			if (nTex < nNumTex)
			{
				// テクスチャ情報の取得
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL && m_nCntMenuLogoBg < m_nNumLogoBg)
				{
					// スキルゲージの生成
					if (nTex != -1)
					{
						m_pPauseMenuLogoBg[m_nCntMenuLogoBg] = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE, pTex->SetTexture(&pTextureFileName[nTex][0]));
						m_pPauseMenuLogoBg[m_nCntMenuLogoBg]->SetCol(col);
						m_pPauseMenuLogoBg[m_nCntMenuLogoBg]->SetTexSheet(tex);
						m_pPauseMenuLogoBg[m_nCntMenuLogoBg]->SetTex(D3DXVECTOR2(1,0.5));
						m_nCntMenuLogoBg++;
					}
					else
					{
						m_pPauseMenuLogoBg[m_nCntMenuLogoBg] = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE, nTex);
						m_pPauseMenuLogoBg[m_nCntMenuLogoBg]->SetCol(col);
						m_pPauseMenuLogoBg[m_nCntMenuLogoBg]->SetTexSheet(tex);
						m_pPauseMenuLogoBg[m_nCntMenuLogoBg]->SetTex(D3DXVECTOR2(1, 0.5));
						m_nCntMenuLogoBg++;
					}

					break;
				}
			}
		}
	}
	return false;
}

//=============================================================================
// ロゴ情報のロード
//=============================================================================
bool CPause::LoadLogo(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// 変数宣言
	char aLine[128];
	char aText[128];
	int nTex;
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };
	D3DXCOLOR col = { 0.0f,0.0f,0.0f,0.0f };

	while (fgets(aLine, 128, pFile) != NULL)
	{
		// 毎行空白を見つけるまで読み込む
		fscanf(pFile, "%s", &aText[0]);

		// POSを見つけたら座標情報を格納
		if (strcmp(&aText[0], "NUM_LOGO") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &m_nLogo);

			// メモリの確保
			m_pLogo = new CScene2D*[m_nLogo];
		}
		// END_PARTSSETを見つけたら各パーツの情報を格納を終える
		if (strcmp(&aText[0], "LOGO") == 0)
		{
			while (fgets(aLine, 128, pFile) != NULL)
			{
				// 毎行空白を見つけるまで読み込む
				fscanf(pFile, "%s", &aText[0]);

				// POSを見つけたら座標情報を格納
				if (strcmp(&aText[0], "TYPE") == 0)
				{
					fscanf(pFile, "%s %d", &aText[0], &nTex);
				}
				// POSを見つけたら座標情報を格納
				if (strcmp(&aText[0], "POS") == 0)
				{
					fscanf(pFile, "%s %f %f %f", &aText[0],
						&pos.x,
						&pos.y,
						&pos.z);
				}
				// SIZEを見つけたらサイズ情報を格納
				if (strcmp(&aText[0], "SIZE") == 0)
				{
					fscanf(pFile, "%s %f %f", &aText[0],
						&size.x,
						&size.y);
				}
				// POSを見つけたら座標情報を格納
				if (strcmp(&aText[0], "COLOR") == 0)
				{
					fscanf(pFile, "%s %f %f %f %f", &aText[0],
						&col.r,
						&col.g,
						&col.b,
						&col.a);
				}
				// END_PARTSSETを見つけたら各パーツの情報を格納を終える
				if (strcmp(&aText[0], "END_LOGO") == 0)
				{
					if (nTex < nNumTex)
					{
						// テクスチャ情報の取得
						CTexture *pTex = CManager::GetTexture();

						if (pTex != NULL && m_nCntLogo < m_nLogo)
						{
							// スキルゲージの生成
							m_pLogo[m_nCntLogo] = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE, pTex->SetTexture(&pTextureFileName[nTex][0]));
							m_pLogo[m_nCntLogo]->SetCol(col);
							m_nCntLogo++;

							break;
						}
						else if (pTex == NULL && m_nCntLogo < m_nLogo)
						{
							// スキルゲージの生成
							m_pLogo[m_nCntLogo] = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE,nTex);
							m_pLogo[m_nCntLogo]->SetCol(col);
							m_nCntLogo++;

							break;
						}
					}
				}
			}
		}
		if (strcmp(&aText[0], "END_LOGOSET") == 0)
		{
			break;
		}
	}
	return false;
}