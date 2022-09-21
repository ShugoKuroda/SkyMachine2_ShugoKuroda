//====================================================================
//
// フェードの処理 (fade.h)
// Author : SHUGO KURODA
//
//====================================================================
#ifndef _FADE_SCENE_H_
#define _FADE_SCENE_H_

//================================================
// インクルードファイル
//================================================
#include "renderer.h"
#include "object2D.h"

//================================================
// クラス宣言
//================================================
// フェードクラス
class CFadeScene : public CObject2D
{
public:
	enum FADETYPE
	{//フェードの種類
		TYPE_NONE = 0,
		TYPE_BLACK,		// 薄い黒
		TYPE_WHITE		// 白
	};

public:
	CFadeScene();
	~CFadeScene();
	static CFadeScene *Create(FADETYPE type);
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

private:
	FADETYPE m_type;	//フェードの種類
	int m_nCounter;		//フェード解除までのカウンター
};

#endif	//_FADE_SCENE_H_
