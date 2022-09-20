//-----------------------------------------------------------------------------------------------
//
// ロゴ処理[logo.cpp]
// Author : SHUGO KURODA
//
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------------------------
#include "logo.h"
#include "manager.h"	// アプリケーション
#include "renderer.h"	// レンダリング

//-----------------------------------------------------------------------------------------------
// 定数宣言
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// 静的メンバ変数
//-----------------------------------------------------------------------------------------------
// テクスチャのポインタ
LPDIRECT3DTEXTURE9 CLogo::m_pTexture[ANIM_MAX] = {};

//-----------------------------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------------------------
CLogo::CLogo()
	:m_nCountUninit(0), m_AnimType(ANIM_NONE)
{
	SetObjType(EObject::OBJ_LOGO);
}

//-----------------------------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------------------------
CLogo::~CLogo()
{

}

//-----------------------------------------------------------------------------------------------
// 生成
// const D3DXVECTOR3& pos → 生成する位置
// const D3DXVECTOR2& size → 生成するサイズ
// const float& fRot → 生成する角度
// const D3DXCOLOR& col → 生成する色
// const LOGOTYPE& type → 生成する種類
// const LOGOTYPE& AnimType → アニメーションの種類
// const int& nCount → 破棄するまでの時間
//-----------------------------------------------------------------------------------------------
CLogo* CLogo::Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& size, const D3DXCOLOR& col, const float& fRot, const LOGOTYPE& type, const ANIMTYPE& AnimType, const int& nCount)
{
	// ポインタクラスを宣言
	CLogo* pLogo = new CLogo;

	if (pLogo != nullptr)
	{// もしnullptrではなかったら

		// 位置設定
		pLogo->SetPosition(pos);

		// 角度設定
		pLogo->SetRot(fRot);

		// サイズの保存
		pLogo->m_DefaultSize = size;

		// アニメーションの種類を設定
		pLogo->m_AnimType = AnimType;

		// 破棄カウンターを設定
		pLogo->m_nCountUninit = nCount;

		// 初期化
		pLogo->Init();

		// 色設定
		pLogo->SetColor(col);

		if (type != TYPE_NONE)
		{
			// テクスチャの設定
			pLogo->BindTexture(m_pTexture[type]);
		}
	}

	return pLogo;
}

//-----------------------------------------------------------------------------------------------
// テクスチャの読み込み
//-----------------------------------------------------------------------------------------------
HRESULT CLogo::Load()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/logo_000.png",
		&m_pTexture[TYPE_WARNING]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/logo_001.png",
		&m_pTexture[TYPE_WARNING_SUB]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/logo_002.png",
		&m_pTexture[TYPE_REMINDER]);

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// テクスチャの削除
//-----------------------------------------------------------------------------------------------
void CLogo::Unload()
{
	for (int nCnt = 0; nCnt >= ANIM_MAX; nCnt++)
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
HRESULT CLogo::Init()
{
	switch (m_AnimType)
	{
	case CLogo::ANIM_LENGTHWISE:
		SetSize(D3DXVECTOR2(m_DefaultSize.x, 0.0f));
		break;
	case CLogo::ANIM_HORIZONTALLY:
		SetSize(D3DXVECTOR2(0.0f, m_DefaultSize.y));
		break;
	default:
		SetSize(D3DXVECTOR2(m_DefaultSize));
		break;
	}

	//初期化
	CObject2D::Init();

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------------------------
void CLogo::Uninit()
{
	CObject2D::Uninit();
}

//-----------------------------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------------------------
void CLogo::Update()
{
	// カウントを減らす
	m_nCountUninit--;

	// 表示カウンターが0以下
	if (m_nCountUninit <= 0)
	{// 破棄
		Uninit();
		return;
	}

	// サイズの取得
	D3DXVECTOR2 size = GetSize();

	switch (m_AnimType)
	{
	case CLogo::ANIM_LENGTHWISE:

		size.y += 1.0f;

		if (m_DefaultSize.y <= size.y)
		{
			size.y = m_DefaultSize.y;
		}

		break;

	case CLogo::ANIM_HORIZONTALLY:
		size.x += 10.0f;
		if (m_DefaultSize.x <= size.x)
		{
			size.x = m_DefaultSize.x;
		}
		break;
	case CLogo::ANIM_FLASHING:
		if (m_nCountUninit % 3 == 0)
		{
			CObject2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{
			CObject2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}
		break;
	default:
		break;
	}

	//サイズの設定
	CObject2D::SetSize(size);

	//頂点座標の設定
	CObject2D::SetVertex();
}

//-----------------------------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------------------------
void CLogo::Draw()
{
	CObject2D::Draw();
}