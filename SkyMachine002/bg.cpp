//-----------------------------------------------------------------------------------------------
//
// 背景の処理[bg.cpp]
// Author : SHUGO kURODA
//
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------------------------
#include "bg.h"

#include "manager.h"
#include "renderer.h"
#include "object.h"
#include "object2D.h"
#include "input_keyboard.h"

#include "game.h"
#include "cloud.h"
#include "spray.h"

//-----------------------------------------------------------------------------------------------
// 静的メンバ変数
//-----------------------------------------------------------------------------------------------
// テクスチャのポインタ
LPDIRECT3DTEXTURE9 CBg::m_apTexture[BG_A_MAX] = {};
CObject2D *CBg::m_apObject2D[BG_A_MAX] = {};

//-----------------------------------------------------------------------------------------------
//	コンストラクタ
//-----------------------------------------------------------------------------------------------
CBg::CBg() :m_set(SET_NONE), m_nCntBgChange(0)
{
	for (int nCnt = 0; nCnt < BG_A_MAX; nCnt++)
	{
		m_apObject2D[nCnt] = nullptr;
	}

	//オブジェクトの種類設定
	SetObjType(EObject::OBJ_BG);
}

//-----------------------------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------------------------
CBg::~CBg()
{
}

//-----------------------------------------------------------------------------------------------
//	テクスチャ読み込み
//-----------------------------------------------------------------------------------------------
HRESULT CBg::Load()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg000_00.jpg", &m_apTexture[BG_A_SKY]);		// 空
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg000_02.jpg", &m_apTexture[BG_A_UNDERWATER]);	// 海中
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg000_01.jpg", &m_apTexture[BG_A_SEA]);		// 海面
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg000_03.png", &m_apTexture[BG_A_WAVE1]);		// 波1
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg000_04.png", &m_apTexture[BG_A_WAVE2]);		// 波2
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg000_05.png", &m_apTexture[BG_A_WAVE3]);		// 波3
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg000_06.png", &m_apTexture[BG_A_FLOOR]);		// 海中の床
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg000_11.jpg", &m_apTexture[BG_A_BOSS]);		// ボス戦の背景
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg000_07.png", &m_apTexture[BG_A_ROCK]);		// 岩
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg000_09.png", &m_apTexture[BG_A_SETWEED]);	// 海藻
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg000_10.png", &m_apTexture[BG_A_SETWEED2]);	// 海藻2
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg000_08.png", &m_apTexture[BG_A_SEA_OTHER]);	// 海の裏側

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
//	テクスチャ破棄
//-----------------------------------------------------------------------------------------------
void CBg::Unload()
{
	for (int nCnt = 0; nCnt < BG_A_MAX; nCnt++)
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
//	生成処理
//-----------------------------------------------------------------------------------------------
CBg *CBg::Create(ESet set)
{
	// ポインタクラスを宣言
	CBg* pBg = new CBg;

	if (pBg != nullptr)
	{
		// 背景群情報の設定
		pBg->m_set = set;

		// 初期化
		pBg->Init();
	}

	return pBg;
}

//-----------------------------------------------------------------------------------------------
//	初期化
//-----------------------------------------------------------------------------------------------
HRESULT CBg::Init()
{
	//スクリーンサイズの保存
	D3DXVECTOR2 ScreenSize = D3DXVECTOR2((float)CRenderer::SCREEN_WIDTH, (float)CRenderer::SCREEN_HEIGHT);

	// 各背景の生成と初期化
	switch (m_set)
	{
	case SET_A:		// ZONE_A

		for (int nCnt = 0; nCnt < BG_A_MAX; nCnt++)
		{// 生成
			m_apObject2D[nCnt] = new CObject2D;
		}
		
		//波の背景のみ前で描画する
		m_apObject2D[BG_A_WAVE1]->SetObjType(CObject::OBJ_WAVE1);
		m_apObject2D[BG_A_WAVE2]->SetObjType(CObject::OBJ_WAVE2);
		m_apObject2D[BG_A_WAVE3]->SetObjType(CObject::OBJ_WAVE3);

		//空
		m_apObject2D[BG_A_SKY]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, (ScreenSize.y / 2) - 100.0f, 0.0f));
		m_apObject2D[BG_A_SKY]->SetSize(D3DXVECTOR2(ScreenSize.x, ScreenSize.y - 200.0f));
		//海
		m_apObject2D[BG_A_SEA]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, ScreenSize.y - 75.0f, 0.0f));
		m_apObject2D[BG_A_SEA]->SetSize(D3DXVECTOR2(ScreenSize.x, 250.0f));
		//波1
		m_apObject2D[BG_A_WAVE1]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, ScreenSize.y + 40.0f, 0.0f));
		m_apObject2D[BG_A_WAVE1]->SetSize(D3DXVECTOR2(ScreenSize.x, 40.0f));
		//波2
		m_apObject2D[BG_A_WAVE2]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, ScreenSize.y + 40.0f, 0.0f));
		m_apObject2D[BG_A_WAVE2]->SetSize(D3DXVECTOR2(ScreenSize.x, 40.0f));
		//波3
		m_apObject2D[BG_A_WAVE3]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, ScreenSize.y + 40.0f, 0.0f));
		m_apObject2D[BG_A_WAVE3]->SetSize(D3DXVECTOR2(ScreenSize.x, 40.0f));
		//海中
		m_apObject2D[BG_A_UNDERWATER]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, ((ScreenSize.y / 2) - 83.5f) * 3, 0.0f));
		m_apObject2D[BG_A_UNDERWATER]->SetSize(D3DXVECTOR2(ScreenSize.x, ScreenSize.y - 100.0f));
		//海中の床
		m_apObject2D[BG_A_FLOOR]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, (ScreenSize.y - 50.0f) * 2.5f, 0.0f));
		m_apObject2D[BG_A_FLOOR]->SetSize(D3DXVECTOR2(ScreenSize.x, 100.0f));
		//岩
		m_apObject2D[BG_A_ROCK]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, (ScreenSize.y - 110.0f) * 2.5f, 0.0f));
		m_apObject2D[BG_A_ROCK]->SetSize(D3DXVECTOR2(ScreenSize.x, 300.0f));
		//海藻
		m_apObject2D[BG_A_SETWEED]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, (ScreenSize.y - 110.0f) * 2.5f, 0.0f));
		m_apObject2D[BG_A_SETWEED]->SetSize(D3DXVECTOR2(ScreenSize.x, 250.0f));
		//海藻2
		m_apObject2D[BG_A_SETWEED2]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, (ScreenSize.y - 100.0f) * 2.5f, 0.0f));
		m_apObject2D[BG_A_SETWEED2]->SetSize(D3DXVECTOR2(ScreenSize.x, 200.0f));
		//背景フェード用
		m_apObject2D[BG_A_FADEBLACK]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, ScreenSize.y / 2, 0.0f));
		m_apObject2D[BG_A_FADEBLACK]->SetSize(D3DXVECTOR2(ScreenSize.x, ScreenSize.y));

		for (int nCnt = 0; nCnt < BG_A_MAX; nCnt++)
		{// 初期化とテクスチャの設定
			m_apObject2D[nCnt]->Init();
			m_apObject2D[nCnt]->BindTexture(m_apTexture[nCnt]);
		}

		for (int nCnt = BG_A_ROCK; nCnt < BG_A_FADEBLACK; nCnt++)
		{// 初期化とテクスチャの設定
			//色の設定
			m_apObject2D[nCnt]->SetColor(D3DXCOLOR(0.7f, 0.7f, 1.0f, 1.0f));
		}
		m_apObject2D[BG_A_FADEBLACK]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

		break;

	case SET_B:		// ZONE_B

		break;

	case SET_C:		// ZONE_C

		break;

	default:

		break;
	}

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
//	終了
//-----------------------------------------------------------------------------------------------
void CBg::Uninit()
{
	for (int nCnt = 0; nCnt < BG_A_MAX; nCnt++)
	{
		if (m_apObject2D[nCnt] != nullptr)
		{
			m_apObject2D[nCnt]->Uninit();
			m_apObject2D[nCnt] = nullptr;
		}
	}

	Release();
}

//-----------------------------------------------------------------------------------------------
//	更新
//-----------------------------------------------------------------------------------------------
void CBg::Update()
{
	D3DXCOLOR col = m_apObject2D[BG_A_FADEBLACK]->GetColor();
	col.a -= 0.01f;
	if (col.a > 0.0f)
	{
		m_apObject2D[BG_A_FADEBLACK]->SetColor(col);
	}

	//海を常に横アニメーションする
	m_apObject2D[BG_A_SEA]->SetAnimBg(1, 1000, true);

	//背景移動までのカウンターを加算
	m_nCntBgChange++;

	if (m_nCntBgChange >= 2720)
	{
		//カウンターを止める
		m_nCntBgChange = m_nCntBgChange;
		//床のアニメーション
		m_apObject2D[BG_A_FLOOR]->SetAnimBg(1, 1000, true);
		//岩のアニメーション
		m_apObject2D[BG_A_ROCK]->SetAnimBg(1, 2000, true);
		//海藻のアニメーション
		m_apObject2D[BG_A_SETWEED]->SetAnimBg(1, 1500, true);
		//海藻2のアニメーション
		m_apObject2D[BG_A_SETWEED2]->SetAnimBg(1, 1000, true);
	}
	//一定時間経過で海に入る演出を開始する
	else if (m_nCntBgChange >= 2180)
	{
		//背景位置の取得
		D3DXVECTOR3 aPosBg[BG_A_MAX];
		for (int nCnt = 0; nCnt < BG_A_MAX; nCnt++)
		{
			aPosBg[nCnt] = m_apObject2D[nCnt]->GetPosition();
		}
		//海のサイズを取得
		D3DXVECTOR2 sizeSea = m_apObject2D[BG_A_SEA]->GetSize();

		float fMul = 1.5f;

		if (m_nCntBgChange >= 2540)
		{
			//泡エフェクトの生成を開始する
			CGame::SetCreateBubble(true);

			fMul = 0.75f;
			sizeSea.y += 0.5f;
			aPosBg[BG_A_SEA].y += 0.5f;
			aPosBg[BG_A_UNDERWATER].y += 0.25f;
			aPosBg[BG_A_SKY].y += 0.25f;

			//床のアニメーション
			m_apObject2D[BG_A_FLOOR]->SetAnimBg(1, 1000, true);
			//岩のアニメーション
			m_apObject2D[BG_A_ROCK]->SetAnimBg(1, 2000, true);
			//海藻のアニメーション
			m_apObject2D[BG_A_SETWEED]->SetAnimBg(1, 1500, true);
			//海藻2のアニメーション
			m_apObject2D[BG_A_SETWEED2]->SetAnimBg(1, 1000, true);

			//雲の生成を止める
			CGame::SetCreateCloud(false);
		}
		else if (m_nCntBgChange >= 2360)
		{
			fMul = 3.0f;

			//岩のアニメーション
			m_apObject2D[BG_A_ROCK]->SetAnimBg(1, 2000, true);
			//海藻のアニメーション
			m_apObject2D[BG_A_SETWEED]->SetAnimBg(1, 1500, true);
			//海藻2のアニメーション
			m_apObject2D[BG_A_SETWEED2]->SetAnimBg(1, 1000, true);
		}

		//雲を背景に合わせて動かす
		CCloud::AddPos(fMul);
		
		//背景に合わせて他オブジェクトを移動
		CCloud::Move(-0.5f * fMul);		//雲
		CSpray::Move(-1.0f * fMul);		//水しぶき

		aPosBg[BG_A_SKY].y -= 0.5f * fMul;
		aPosBg[BG_A_UNDERWATER].y -= 0.5f * fMul;
		//海のみサイズも変える
		aPosBg[BG_A_SEA].y -= 0.75f * fMul;
		sizeSea.y -= 0.5f * fMul;
		aPosBg[BG_A_FLOOR].y -= 1.0f * fMul;
		aPosBg[BG_A_ROCK].y -= 1.0f * fMul;
		aPosBg[BG_A_SETWEED].y -= 1.0f * fMul;
		aPosBg[BG_A_SETWEED2].y -= 1.0f * fMul;

		aPosBg[BG_A_WAVE1].y -= 1.0f * fMul;
		aPosBg[BG_A_WAVE2].y -= 1.0f * fMul;
		aPosBg[BG_A_WAVE3].y -= 1.0f * fMul;

		if (sizeSea.y <= 0.0f)
		{
			m_apObject2D[BG_A_SEA]->BindTexture(m_apTexture[BG_A_SEA_OTHER]);
		}

		m_apObject2D[BG_A_SEA]->SetSize(sizeSea);

		//背景位置の設定
		for (int nCnt = 0; nCnt < BG_A_MAX; nCnt++)
		{
			m_apObject2D[nCnt]->SetPosition(aPosBg[nCnt]);
			m_apObject2D[nCnt]->SetVertex();
		}

		//波のアニメーション
		m_apObject2D[BG_A_WAVE1]->SetAnimBg(3, 200, true);
		m_apObject2D[BG_A_WAVE2]->SetAnimBg(4, 200, true);
		m_apObject2D[BG_A_WAVE3]->SetAnimBg(2, 200, true);
	}
}

//-----------------------------------------------------------------------------------------------
//	描画
//-----------------------------------------------------------------------------------------------
void CBg::Draw()
{
}
