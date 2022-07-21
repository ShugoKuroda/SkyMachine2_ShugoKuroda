//=============================================================================
//
// �����N���� [rank.h]
// Author : SHUGO KURODA
//
//=============================================================================
#ifndef _RANK_H_
#define _RANK_H_

#include "object2D.h"

//*****************************************************************************
// �����L���O�N���X(�h���N���X)
//*****************************************************************************
class CRank : public CObject2D
{
public:
	CRank();
	~CRank();

	static CRank *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXVECTOR2 texSheet, int nTex);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXVECTOR2 texSheet, int nTex);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
};
#endif