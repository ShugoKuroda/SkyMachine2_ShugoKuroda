//=============================================================================
//
// ランク処理 [rank.cpp]
// Author : SHUGO KURODA
//
//=============================================================================
#include "rank.h"

#include "manager.h"
#include "renderer.h"

//=============================================================================
// CRankのコンストラクタ
//=============================================================================
CRank::CRank()
{

}

//=============================================================================
// CRankのデストラクタ
//=============================================================================
CRank::~CRank()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CRank *CRank::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXVECTOR2 texSheet, int nTex)
{
	// ポインタ変数の生成
	CRank *Rank;
	Rank = new CRank;

	// NULLチェック
	if (Rank != nullptr)
	{
		// 初期化処理
		Rank->Init(pos, size, texSheet, nTex);

		// オブジェクトの種類を設定
		//Rank->SetObjType(CScene::OBJTYPE_PLAYER_MARKER);
	}

	return Rank;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CRank::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXVECTOR2 texSheet, int nTex)
{
	//// CScene2Dの初期化処理
	//CObject2D::Init(pos, size);

	//// テクスチャの貼り付け
	//CTexture *pTex = CManager::GetTexture();

	//if (pTex != NULL)
	//{
	//	CObject2D::BindTexture(pTex->GetAddress(nTex));
	//}

	//// テクスチャの分割量の設定
	//CScene2D::SetTexSheet(texSheet);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CRank::Uninit(void)
{
	// CScene2Dの終了処理
	CObject2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CRank::Update(void)
{
	// CScene2Dの更新処理
	CObject2D::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CRank::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// アルファテスト
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 120);

	// CScene2Dの描画処理
	CObject2D::Draw();

	// アルファテストを元の設定に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);
}