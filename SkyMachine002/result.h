//=============================================================================
//
// リザルト画面処理 [result.h]
// Author : itsuki takaiwa
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
};

#endif
