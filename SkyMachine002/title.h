//=============================================================================
//
// ゲーム画面処理 [game.h]
// Author : SHUGO KURODA
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "base.h"
#include "cloud_data.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CObject2D;

//-----------------------------------------------------------------------------
// ベースクラス(派生クラス)
//-----------------------------------------------------------------------------
class CTitle : public CBase
{
public:
	enum TITLE_OBJ
	{//背景の種類
		BG_SKY = 0,		// 背景の空
		LOGO_TITLE,		// タイトルロゴ
		LOGO_PLAYER,	// プレイヤーロゴ
		LOGO_SHADOW,	// プレイヤーロゴ(黒)
		LOGO_PRESS,		// PRESSロゴ
		OBJ_MAX			// 背景の最大数
	};

public:
	CTitle();
	~CTitle() override;

	//メンバ関数
	static HRESULT Load();		//テクスチャの読み込み
	static void Unload();		//テクスチャの削除

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void CreateCloud();

private:
	//メンバ変数
	static LPDIRECT3DTEXTURE9 m_apTexture[OBJ_MAX];	//テクスチャのポインタ
	CObject2D *m_apObject2D[OBJ_MAX];
	CloudInfo m_CloudInfo;
	int m_nCountMoveBg;
};

#endif		// _TITLE_H_
