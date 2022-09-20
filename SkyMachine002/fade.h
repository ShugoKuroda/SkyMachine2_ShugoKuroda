//=============================================================================
//
// フェード情報 [fade.h]
// Author:SHUGO KURODA
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "manager.h"
#include "renderer.h"
#include "object2D.h"

//*****************************************************************************
// フェードクラス(基本クラス)
//*****************************************************************************
class CFade
{
public:
	// フェードコマンド
	typedef enum
	{
		FADE_NONE = 0,	// 何もしていない
		FADE_IN,		// フェードイン
		FADE_OUT,		// フェードアウト
		FADE_MAX,
	}FADE;

	CFade();
	~CFade();

	// フェード生成
	static CFade* Create(CManager::MODE modeNext);
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	static void SetFade(FADE fade, CManager::MODE modeNext);
	static FADE GetFade() { return m_fade; }

private:
	//頂点バッファへのポインター
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	// フェードの状態
	static FADE m_fade;
	// 次の画面(モード)
	static CManager::MODE m_modeNext;
	// フェード色
	D3DXCOLOR m_colorFade;
};

#endif
