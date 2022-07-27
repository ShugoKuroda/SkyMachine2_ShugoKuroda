//===============================================================================================
//
// ゲーム画面処理 [title.cpp]
// Author : SHUGO KURODA
//
//===============================================================================================
#include "title.h"

#include "manager.h"
#include "input_keyboard.h"
#include "object.h"
#include "object2D.h"
#include "library.h"

#include "cloud.h"
#include "fade_white.h"
#include "library.h"

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
CTitle::CTitle() :m_nCountMoveBg(0), m_bTitleDraw(false), m_bPressFade(false)
{
	for (int nCnt = 0; nCnt < OBJ_MAX - 1; nCnt++)
	{
		m_apObject2D[nCnt] = nullptr;
	}

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
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title003.png", &m_apTexture[LOGO_SHADOW]);		// プレイヤーロゴ(黒)

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

	for (int nCnt = 0; nCnt < OBJ_MAX - 1; nCnt++)
	{// 初期化とテクスチャの設定
		m_apObject2D[nCnt]->Init();
		m_apObject2D[nCnt]->BindTexture(m_apTexture[nCnt]);
	}

	//背景以外を前に描画する(タイプを設定する)
	for (int nCnt = LOGO_PLAYER; nCnt < OBJ_MAX - 1; nCnt++)
	{
		m_apObject2D[nCnt]->SetObjectType(CObject::OBJ_TITLE);
	}

	//タイトルとPressロゴを透明にする
	m_apObject2D[LOGO_TITLE]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	m_apObject2D[LOGO_PRESS]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------------------------
void CTitle::Uninit()
{
	//テクスチャの破棄
	CTitle::Unload();
	//雲
	CCloud::Unload();
}

//-----------------------------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------------------------
void CTitle::Update()
{
	m_nCountMoveBg++;

	//背景位置の取得
	D3DXVECTOR3 aPosBg[OBJ_MAX - 1];
	for (int nCnt = 0; nCnt < OBJ_MAX - 1; nCnt++)
	{
		aPosBg[nCnt] = m_apObject2D[nCnt]->GetPosition();
	}

	// 背景が移動しきったら
	if (m_bTitleDraw == true && m_nCountMoveBg >= 40)
	{
		D3DXCOLOR col = m_apObject2D[LOGO_PRESS]->GetColor();
		if (m_bPressFade == false)
		{
			col.a += 0.02f;
			if (col.a >= 1.0f)
			{
				m_bPressFade = true;
			}
		}
		if (m_bPressFade == true)
		{
			col.a -= 0.02f;
			if (col.a <= 0.0f)
			{
				m_bPressFade = false;
			}
		}
		m_apObject2D[LOGO_PRESS]->SetColor(col);
	}
	else if (m_nCountMoveBg >= 540)
	{
		aPosBg[LOGO_PLAYER].x += 25.0f;
		if (aPosBg[LOGO_PLAYER].x >= 960.0f)
		{
			m_apObject2D[LOGO_TITLE]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			aPosBg[LOGO_PLAYER].x = 960.0f;
			m_apObject2D[LOGO_PLAYER]->BindTexture(m_apTexture[LOGO_SHADOW]);

			//タイトルを描画する
			m_bTitleDraw = true;

			//白フェードの設定
			CFadeWhite::Create();

			//カウンターのリセット
			m_nCountMoveBg = 0;
		}
	}
	// 背景の移動を開始する
	else if (m_nCountMoveBg >= 120)
	{
		//雲の移動処理
		CCloud::MoveCloud(3.0f);

		aPosBg[BG_SKY].y += 3.0f;
	}

	//背景位置の設定
	for (int nCnt = 0; nCnt < OBJ_MAX - 1; nCnt++)
	{
		m_apObject2D[nCnt]->SetPosition(aPosBg[nCnt]);
		m_apObject2D[nCnt]->SetVertex();
	}

	// キーボード情報の取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_ATTACK) == true)
	{//SPACEキーを押された
		// モードの設定
		CManager::SetMode(CManager::MODE_GAME);
	}

	//雲の生成
	CreateCloud();
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
