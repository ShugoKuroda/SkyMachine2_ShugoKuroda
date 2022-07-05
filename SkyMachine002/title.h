//=============================================================================
//
// �Q�[����ʏ��� [game.h]
// Author : SHUGO KURODA
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "base.h"

//-----------------------------------------------------------------------------
// �x�[�X�N���X(�h���N���X)
//-----------------------------------------------------------------------------
class CTitle : public CBase
{
public:
	CTitle();
	~CTitle();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};

#endif
