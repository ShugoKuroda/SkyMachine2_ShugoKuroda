//====================================================================
//
// 白フェードの処理 (whiteFade.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _FADE_WHITE_H_
#define _FADE_WHITE_H_

//================================================
// インクルードファイル
//================================================
#include "renderer.h"
#include "object2D.h"

//================================================
// クラス宣言
//================================================
// 白フェードクラス
class CFadeWhite : public CObject2D
{
public:
	CFadeWhite();
	~CFadeWhite();
	static CFadeWhite *Create();
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};

#endif	//_FADE_WHITE_H_
