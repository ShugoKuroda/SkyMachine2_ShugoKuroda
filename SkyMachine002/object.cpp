//=============================================================================
//
// オブジェクト処理[object.cpp]
// Author : SHUGO KURODA
//
//=============================================================================
#include "object.h"
#include "manager.h"
#include "renderer.h"

#include "object2D.h"
#include "library.h"
#include "meshfield.h"
#include "game.h"

//*****************************************************************************
//	静的メンバ変数
//*****************************************************************************
CObject* CObject::m_apObject[MAX_OBJECT] = { nullptr };
int CObject::m_nNumAll = 0;
int CObject::m_nShakeInterval = 0;
bool CObject::m_bShake = false;

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CObject::CObject()
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (m_apObject[nCntObject] == nullptr)
		{
			// オブジェクト情報を格納
			m_apObject[nCntObject] = this;
			// オブジェクト番号の保存
			m_nID = nCntObject;
			// 合計オブジェクト数を加算
			m_nNumAll++;
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
			// オブジェクトの終了処理
			m_apObject[nCntUninit]->Uninit();

			// メモリの開放
			delete m_apObject[nCntUninit];
			m_apObject[nCntUninit] = nullptr;
		}
	}
}

//=============================================================================
//	更新処理
//=============================================================================
void CObject::UpdateAll()
{
	// ポーズ情報の取得
	bool bPause = CManager::GetPause();

	// ポーズ中でないなら
	if (bPause == false)
	{
		for (int nCntUpdate = 0; nCntUpdate < MAX_OBJECT; nCntUpdate++)
		{
			if (m_apObject[nCntUpdate] != nullptr)
			{
				// ポリゴンの更新処理
				m_apObject[nCntUpdate]->Update();
			}
		}

		// 画面シェイクフラグが立っているなら
		if (m_bShake == true)
		{//画面シェイクする
			ShakeAll();
		}
	}
	// ポーズ中なら
	else if (bPause == true)
	{
		for (int nCntUpdate = 0; nCntUpdate < MAX_OBJECT; nCntUpdate++)
		{
			if (m_apObject[nCntUpdate] != nullptr)
			{
				//ポーズ画面のみ更新
				if (m_apObject[nCntUpdate]->m_nType == OBJ_PAUSE || m_apObject[nCntUpdate]->m_nType == OBJ_PAUSE_MENU)
				{
					// 更新処理
					m_apObject[nCntUpdate]->Update();
				}
				//ボスが倒されたらボスのみ更新
				else if (CGame::GetDieBoss() == true && m_apObject[nCntUpdate]->m_nType == OBJ_ENEMYBOSS ||
					CGame::GetDieBoss() == true && m_apObject[nCntUpdate]->m_nType == OBJ_FADE)
				{
					// 更新処理
					m_apObject[nCntUpdate]->Update();
				}
			}
		}
	}
}

//=============================================================================
//	描画処理
//=============================================================================
void CObject::DrawAll()
{
	//オブジェクト情報の保存用
	CObject *apObject[MAX_OBJECT];
	CObject *pObject;

	// オブジェクト情報を保存
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		apObject[nCntObject] = m_apObject[nCntObject];
	}

	// 選択ソートで描画順の並び替え(昇順)
	for (int nNum01 = 0; nNum01 < MAX_OBJECT - 1; nNum01++)
	{
		for (int nNum02 = nNum01 + 1; nNum02 < MAX_OBJECT; nNum02++)
		{
			// 比較対象にオブジェクト情報があったら
			if (apObject[nNum01] != nullptr && apObject[nNum02] != nullptr)
			{
				// オブジェクトを優先順に並び替え
				if (apObject[nNum01]->m_nType > apObject[nNum02]->m_nType)
				{
					pObject = apObject[nNum01];
					apObject[nNum01] = apObject[nNum02];
					apObject[nNum02] = pObject;
				}
			}
		}
	}

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 描画
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (apObject[nCntObject] != nullptr)
		{
			if (apObject[nCntObject]->m_nType == OBJ_BG)
			{
				//カリングをOFFにする
				pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

				// 描画処理
				apObject[nCntObject]->Draw();

				//カリングをONに戻す
				pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			}
			else
			{
				// 描画処理
				apObject[nCntObject]->Draw();
			}
		}
	}
}

//=============================================================================
// オブジェクトを揺らす
//=============================================================================
void CObject::ShakeAll()
{
	if (m_nShakeInterval < 0)
	{//シェイクを終える
		m_bShake = false;
		return;
	}

	// 突進する角度を決める
	float fRot = LibrarySpace::GetRandFloat(3, 0, 100);

	// シェイク(移動)量の設定
	D3DXVECTOR3 move = D3DXVECTOR3(sinf(fRot) * m_nShakeInterval, cosf(fRot) * m_nShakeInterval, 0);

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (m_apObject[nCntObject] != nullptr)
		{
			if (m_apObject[nCntObject]->m_nType != OBJ_PLAYER &&
				m_apObject[nCntObject]->m_nType != OBJ_BG_MOVE)
			{
				//オブジェクトポインタを敵にキャスト
				CObject2D *pObject2D = (CObject2D*)m_apObject[nCntObject];

				// シェイク(移動)量の加算
				pObject2D->SetMove(D3DXVECTOR3(sinf(fRot) * m_nShakeInterval,
					cosf(fRot) * m_nShakeInterval, 0));
			}
		}
	}

	// ボス戦背景情報の取得
	CMeshField* pMeshField= CGame::GetMeshField();

	// シェイク(移動)量の加算
	pMeshField->SetMove(D3DXVECTOR3(sinf(fRot) * m_nShakeInterval,
		cosf(fRot) * m_nShakeInterval, 0));

	// シェイクする秒数(振れ幅)を減らす
	m_nShakeInterval--;
}

void CObject::SetShake(int nShakeNum)
{
	m_bShake = true;
	m_nShakeInterval = nShakeNum;
}

//=============================================================================
// 特定のオブジェクト破棄
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
		m_nNumAll--;
	}
}
