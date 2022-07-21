//=============================================================================
//
// マネージャー処理[manager.h]
// Author : SHUGO KURODA
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include <Windows.h>

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CRenderer;
class CBase;
class CSound;				// サウンドクラス
//class CFade;				// フェードクラス
class CInputKeyboard;		// キーボードクラス
class CInputJoypad;			// ジョイパッドクラス
class CInputMouse;			// マウスクラス

//*****************************************************************************
// クラス定義(基本クラス)
//*****************************************************************************
class CManager
{
public:
	// モードの種類
	enum MODE
	{
		MODE_TITLE = 0,		// タイトル
		MODE_TUTORIAL,		// チュートリアル
		MODE_GAME,			// ゲーム
		MODE_RESULT,		// リザルト
		MODE_RANKING,		// ランキング
		MODE_MAX,
	};

	CManager();
	~CManager();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
	void Uninit();
	void Update();
	void Draw();
	
	static void SetMode(MODE mode);
	static MODE GetMode() { return m_mode; }

	static CRenderer *GetRenderer() { return m_pRenderer; }
	static CBase *GetBase(void) { return m_pBase; }
	//static CFade *GetFade(void) { return m_pFade; }
	//static MODE GetMode(void) { return m_mode; }
	static CSound *GetSound(void) { return m_pSound; }
	static CInputKeyboard *GetInputKeyboard() { return m_pInputKeyboard; }
	static CInputJoypad *GetInputJoypad() { return m_pInputJoypad; }
	static CInputMouse *GetInputMouse() { return m_pInputMouse; }

private:
	static CRenderer *m_pRenderer;
	static CInputKeyboard *m_pInputKeyboard;	// キーボード情報のポインタ
	static CInputJoypad *m_pInputJoypad;		// ジョイパッド情報のポインタ
	static CInputMouse *m_pInputMouse;			// マウス情報のポインタ
	static CSound *m_pSound;					// サウンド情報のポインタ
	static CBase *m_pBase;						// ベースシーン情報のポインタ
	//static CFade *m_pFade;
	static MODE m_mode;							// モード情報
};

#endif // _MANAGER_
