//=============================================================================
//
// �x�[�X�V�[������ [base.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _BASE_H_
#define _BASE_H_

#include "renderer.h"

//-----------------------------------------------------------------------------
// �x�[�X�N���X(��{�N���X)
//-----------------------------------------------------------------------------
class CBase
{
public:
	CBase();
	~CBase();

	virtual HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size) = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
};
#endif