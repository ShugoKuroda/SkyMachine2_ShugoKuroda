//===============================================================================================
//
// ゲーム画面処理 [title.cpp]
// Author : SHUGO KURODA
//
//===============================================================================================
#include "title.h"

#include "manager.h"
#include "input_keyboard.h"
#include "object2D.h"

#include "cloud.h"
#include "library.h"

//-----------------------------------------------------------------------------------------------
// 静的メンバ変数
//-----------------------------------------------------------------------------------------------
// テクスチャのポインタ
LPDIRECT3DTEXTURE9 CTitle::m_apTexture[OBJ_MAX] = { nullptr };

//-----------------------------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------------------------
CTitle::CTitle() :m_nCountMoveBg(0)
{
	for (int nCnt = 0; nCnt < OBJ_MAX; nCnt++)
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
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title003.png", &m_apTexture[LOGO_SHADOW]);		// プレイヤーロゴ(黒)
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title004.png", &m_apTexture[LOGO_PRESS]);		// PRESSロゴ

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
	//テクスチャのロード
	CTitle::Load();
	//雲
	CCloud::Load();

	//スクリーンサイズの保存
	D3DXVECTOR2 ScreenSize = D3DXVECTOR2((float)CRenderer::SCREEN_WIDTH, (float)CRenderer::SCREEN_HEIGHT);

	for (int nCnt = 0; nCnt < OBJ_MAX; nCnt++)
	{// 生成
		m_apObject2D[nCnt] = new CObject2D;
	}

	//空
	m_apObject2D[BG_SKY]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, -ScreenSize.y / 2, 0.0f));
	m_apObject2D[BG_SKY]->SetSize(D3DXVECTOR2(ScreenSize.x, ScreenSize.y * 3));

	for (int nCnt = 0; nCnt < OBJ_MAX; nCnt++)
	{// 初期化とテクスチャの設定
		m_apObject2D[nCnt]->Init();
		m_apObject2D[nCnt]->BindTexture(m_apTexture[nCnt]);
	}

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------------------------
void CTitle::Uninit()
{
	// 終了処理
	for (int nCntBg = 0; nCntBg < OBJ_MAX; nCntBg++)
	{
		if (m_apObject2D[nCntBg] != nullptr)
		{
			m_apObject2D[nCntBg]->Uninit();
			m_apObject2D[nCntBg] = nullptr;
		}
	}

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
		m_CloudInfo.nRandTime = LibrarySpace::GetRandNum(300, 240);
	}
}
