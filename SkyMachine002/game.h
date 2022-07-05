//=============================================================================
//
// ゲーム画面処理 [game.h]
// Author : SHUGO KURODA
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "base.h"

//-----------------------------------------------------------------------------
// ベースクラス(派生クラス)
//-----------------------------------------------------------------------------
class CGame : public CBase
{
public:
	CGame();
	~CGame();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};

#endif
