//=============================================================================
//
// スコア処理[score.cpp]
// Author : SHUGO KURODA
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object2D.h"
#include "score.h"
#include "number.h"

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CScore::m_pTexture = nullptr;

//=============================================================================
// コンストラクタ
//=============================================================================
CScore::CScore() :m_nScore(0)
{
	//オブジェクトの種類設定
	SetObjType(EObject::OBJ_PAUSE);
}

//=============================================================================
// デストラクタ
//=============================================================================
CScore::~CScore()
{
}

//=============================================================================
// 生成
// const D3DXVECTOR3& pos → 生成する位置
// const D3DXVECTOR2& size → 生成するサイズ
// const int& nNumSpace → 数字の間隔
//=============================================================================
CScore *CScore::Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& size,const int& nNumSpace)
{
	CScore *pScore = new CScore;

	if (pScore != nullptr)
	{
		for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
		{
			// 数字ポリゴン生成
			pScore->m_apNumber[nCntScore] = new CNumber;

			if (pScore->m_apNumber[nCntScore] != nullptr)
			{
				// 位置設定
				pScore->m_apNumber[nCntScore]->SetPosition(D3DXVECTOR3(pos.x + (nCntScore * nNumSpace), pos.y, pos.z));
				// サイズ設定
				pScore->m_apNumber[nCntScore]->SetSize(size);
				// 初期化
				pScore->m_apNumber[nCntScore]->Init();
			}
		}
	}

	return pScore;
}

//=============================================================================
// ポリゴンの初期化
//=============================================================================
HRESULT CScore::Init()
{
	CObject::SetObjType(CObject::OBJ_SCORE);

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CScore::Uninit()
{
	for (int nCntNumber = 0; nCntNumber < MAX_SCORE; nCntNumber++)
	{
		// 中身があるなら
		if (m_apNumber[nCntNumber] != nullptr)
		{
			// 数字情報の終了処理
			m_apNumber[nCntNumber]->Uninit();
			m_apNumber[nCntNumber] = nullptr;
		}
	}

	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新
//=============================================================================
void CScore::Update()
{
}

//=============================================================================
// 描画
//=============================================================================
void CScore::Draw()
{
}

//=============================================================================
// スコアの加算
//=============================================================================
void CScore::Add(const int& nScore)
{
	m_nScore += nScore;
	Set();
}

//=============================================================================
// スコアの設定
//=============================================================================
void CScore::Set(const int & nScore)
{
	m_nScore = nScore;
	Set();
}

//=============================================================================
// 色の設定
//=============================================================================
void CScore::SetColor(D3DXCOLOR col)
{
	for (int nCntNumber = 0; nCntNumber < MAX_SCORE; nCntNumber++)
	{
		// 中身があるなら
		if (m_apNumber[nCntNumber] != nullptr)
		{// 数字情報の色を設定
			m_apNumber[nCntNumber]->SetColor(col);
		}
	}
}

D3DXCOLOR CScore::GetColor()
{
	return m_apNumber[0]->GetColor();
}

//=============================================================================
// スコアの設定
//=============================================================================
void CScore::Set()
{
	//各桁の数値を格納(pPostexU[桁ごとの数値])
	int aPosTexU[MAX_SCORE];

	aPosTexU[0] = (m_nScore % 100000000) / 10000000;
	aPosTexU[1] = (m_nScore % 10000000) / 1000000;
	aPosTexU[2] = (m_nScore % 1000000) / 100000;
	aPosTexU[3] = (m_nScore % 100000) / 10000;
	aPosTexU[4] = (m_nScore % 10000) / 1000;
	aPosTexU[5] = (m_nScore % 1000) / 100;
	aPosTexU[6] = (m_nScore % 100) / 10;
	aPosTexU[7] = m_nScore % 10;

	//テクスチャを更新する
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		m_apNumber[nCntScore]->SetAnimation(aPosTexU[nCntScore], 0, CNumber::DIVISION_U, CNumber::DIVISION_V);
	}
}