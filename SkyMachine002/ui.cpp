//-----------------------------------------------------------------------------------------------
//
// UI処理[ui.cpp]
// Author : SHUGO KURODA
//
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------------------------
#include "ui.h"
#include "manager.h"	// アプリケーション
#include "renderer.h"	// レンダリング

#include "game.h"
#include "player.h"

//-----------------------------------------------------------------------------------------------
// 定数宣言
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// 静的メンバ変数
//-----------------------------------------------------------------------------------------------
// テクスチャのポインタ
LPDIRECT3DTEXTURE9 CUi::m_pTexture[TYPE_MAX] = {};

//-----------------------------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------------------------
CUi::CUi() :m_nCountUninit(0), m_AnimType(ANIM_NONE)
{
	SetObjType(EObject::OBJ_UI);
}

//-----------------------------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------------------------
CUi::~CUi()
{
}

//-----------------------------------------------------------------------------------------------
// 生成
// const D3DXVECTOR3& pos → 生成する位置
// const D3DXVECTOR2& size → 生成するサイズ
// const LOGOTYPE& type → 生成する種類
// const ANIMTYPE& AnimType → アニメーションの種類
// const PARENT& parent →	どのプレイヤーのUIか
//-----------------------------------------------------------------------------------------------
CUi* CUi::Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& size, const TYPE& type, const ANIMTYPE& AnimType, const PARENT& parent)
{
	// ポインタクラスを宣言
	CUi* pUi = new CUi;

	if (pUi != nullptr)
	{// もしnullptrではなかったら

		// 位置設定
		pUi->SetPosition(pos);

		// サイズ設定
		pUi->SetSize(size);

		// アニメーションの種類を設定
		pUi->m_AnimType = AnimType;

		// 親の設定
		pUi->m_parent = parent;

		// 初期化
		pUi->Init();

		// テクスチャの設定
		pUi->BindTexture(m_pTexture[type]);
	}

	return pUi;
}

//-----------------------------------------------------------------------------------------------
// テクスチャの読み込み
//-----------------------------------------------------------------------------------------------
HRESULT CUi::Load()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/UI000.png", &m_pTexture[TYPE_AREA_A]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/UI001.png", &m_pTexture[TYPE_PLAYER1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/UI002.png", &m_pTexture[TYPE_PLAYER2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/UI003.png", &m_pTexture[TYPE_PRESS_ANY_BUTTON]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/UI004.png", &m_pTexture[TYPE_TO_START]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/UI005.png", &m_pTexture[TYPE_LIFE1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/UI006.png", &m_pTexture[TYPE_LIFE2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/UI007.png", &m_pTexture[TYPE_BULLET]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/UI008.png", &m_pTexture[TYPE_BULLET_OPTION]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/UI009.png", &m_pTexture[TYPE_BARRIER]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/UI011.png", &m_pTexture[TYPE_OK]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/UI012.png", &m_pTexture[TYPE_CONTINUE]);

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// テクスチャの削除
//-----------------------------------------------------------------------------------------------
void CUi::Unload()
{
	for (int nCnt = 0; nCnt >= TYPE_MAX; nCnt++)
	{
		// テクスチャの破棄
		if (m_pTexture[nCnt] != nullptr)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = nullptr;
		}
	}
}

//-----------------------------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------------------------
HRESULT CUi::Init()
{
	//初期化
	CObject2D::Init();

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------------------------
void CUi::Uninit()
{
	CObject2D::Uninit();
}

//-----------------------------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------------------------
void CUi::Update()
{
	// 親プレイヤーが参加していなければ
	if (m_parent == PLAYER_1_NOT_ENTRY || m_parent == PLAYER_2_NOT_ENTRY)
	{
		// プレイヤー情報の取得
		bool bEntry = CManager::GetEntry(m_parent - 2);

		// 親プレイヤーが参加したら
		if (bEntry == true)
		{// 破棄
			Uninit();
			return;
		}
	}
	// 親プレイヤーがいれば
	else if (m_parent != PLAYER_NONE)
	{
		// プレイヤー情報の取得
		CPlayer *pPlayer = CGame::GetPlayer(m_parent);

		// 親プレイヤーが破棄されていたら
		if (pPlayer == nullptr)
		{// 破棄
			Uninit();
			return;
		}
	}

	switch (m_AnimType)
	{
	case CUi::ANIM_FLASHING:

		// カウントを増やす
		m_nCountUninit++;

		if (m_nCountUninit >= 120)
		{
			m_nCountUninit = 0;
		}
		else if (m_nCountUninit >= 60)
		{
			CObject2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}
		else if (m_nCountUninit >= 0)
		{
			CObject2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		break;

	default:
		break;
	}

	//頂点座標の設定
	CObject2D::SetVertex();
}

//-----------------------------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------------------------
void CUi::Draw()
{
	CObject2D::Draw();
}