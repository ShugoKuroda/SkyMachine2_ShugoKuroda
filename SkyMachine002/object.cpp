//=============================================================================
//
// オブジェクト処理[object.cpp]
// Author : SHUGO KURODA
//
//=============================================================================
#include "object.h"

//*****************************************************************************
//	静的メンバ変数
//*****************************************************************************
CObject* CObject::m_apObject[CObject::MAX_OBJECT] = { nullptr };
int CObject::m_nNumAll = 0;
int CObject::m_nID = 0;

//=============================================================================
// トコンストラクタ
//=============================================================================
CObject::CObject()
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (m_apObject[nCntObject] == nullptr)
		{
			m_apObject[nCntObject] = this;
			m_nNumAll++;
			m_nID = nCntObject;
			break;
		}
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CObject::~CObject()
{
}

//=============================================================================
//	リリース処理
//=============================================================================
void CObject::ReleaseAll()
{
	for (int nCntUninit = 0; nCntUninit < MAX_OBJECT; nCntUninit++)
	{
		// オブジェクトインスタンスの破棄
		if (m_apObject[nCntUninit] != nullptr)
		{
			// 終了処理
			m_apObject[nCntUninit]->Uninit();
		}
	}
}

//=============================================================================
//	更新処理
//=============================================================================
void CObject::UpdateAll()
{
	for (int nCntUpdate = 0; nCntUpdate < m_nNumAll; nCntUpdate++)
	{
		if (m_apObject[nCntUpdate] != nullptr)
		{
			// ポリゴンの更新処理
			m_apObject[nCntUpdate]->Update();
		}
	}
}

//=============================================================================
//	コンストラクタ
//=============================================================================
void CObject::DrawAll()
{
	for (int nCntDraw = 0; nCntDraw < m_nNumAll; nCntDraw++)
	{
		if (m_apObject[nCntDraw] != nullptr)
		{
			//ポリゴンの描画処理
			m_apObject[nCntDraw]->Draw();
		}
	}
}

//=============================================================================
// コンストラクタ
//=============================================================================
void CObject::Release()
{
	// オブジェクトインスタンスの破棄
	if (m_apObject[m_nID] != nullptr)
	{
		// 終了処理
		int nID = m_nID;
		delete m_apObject[nID];
		m_apObject[nID] = nullptr;
		//m_nNumAll--;
	}
}
