//=============================================================================
//
// ���U���g��ʏ��� [result.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "base.h"

//*****************************************************************************
// ���U���g��ʃN���X(�h���N���X)
//*****************************************************************************
class CResult : public CBase
{
public:
	// ���[�h
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