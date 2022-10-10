//=============================================================================
//
// リザルト画面処理 [result.h]
// Author : SHUGO KURODA
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "base.h"

//*****************************************************************************
// リザルト画面クラス(派生クラス)
//*****************************************************************************
class CResult : public CBase
{
public:
	CResult();
	~CResult();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;

private:
	// テクスチャ読み込み
	static void LoadAll();
	// テクスチャ破棄
	static void UnloadAll();
};

#endif		// _RESULT_H_
