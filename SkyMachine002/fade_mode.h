////=============================================================================
////
//// フェードヘッダー (fade.h)
//// Author:Itsuki Takaiwa
////
////=============================================================================
//#ifndef _FADE_H_
//#define _FADE_H_
//
//#include "manager.h"
//
////*****************************************************************************
//// 前方宣言
////*****************************************************************************
//class CBase;			// ベースシーンクラス
//
////*****************************************************************************
//// フェードクラス(基本クラス)
////*****************************************************************************
//class CFadeMode
//{
//public:
//	// フェードコマンド
//	typedef enum
//	{
//		FADE_NONE = 0,	// 何もしていない
//		FADE_IN,		// フェードイン
//		FADE_OUT,		// フェードアウト
//		FADE_MAX,
//	}FADE;
//
//	CFadeMode();
//	~CFadeMode();
//
//	HRESULT Init(CBase *pBase);
//	void Uninit(void);
//	void Update(void);
//	void Draw(void);
//
//	static void SetFade(FADE fade, CBase *pBase);
//	static FADE GetFade(void);
//
//private:
//	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffFade;	// 頂点バッファのポインタ
//	static FADE m_fade;						// フェードの状態
//	static CBase *m_pBaseNext;				// 次のモード
//	D3DXCOLOR m_colorFade;					// フェード色
//};
//
//#endif
