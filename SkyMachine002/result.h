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
	// モード
	typedef enum
	{
		RESULTMODE_NORMAL = 0,
		RESULTMODE_RESTART,
		RESULTMODE_TRANSITION_MODESELECT,
		RESULTMODE_TRANSITION_TITLE,
		RESULTMODE_MAX,
	}ResultMode;

	CResult();
	~CResult();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void SetResultMode(ResultMode resultMode) { m_resultMode = resultMode; }

private:
	static ResultMode m_resultMode;
};
#endif