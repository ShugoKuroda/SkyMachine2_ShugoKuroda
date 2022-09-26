//=============================================================================
//
// マネージャー処理[manager.h]
// Author : SHUGO KURODA
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include <Windows.h>
#include "player.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CRenderer;
class CBase;
class CSound;				// サウンドクラス
class CFade;				// フェードクラス
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
	static CBase *GetBase() { return m_pBase; }
	static CFade *GetFade() { return m_pFade; }
	static CSound *GetSound() { return m_pSound; }
	static CInputKeyboard *GetInputKeyboard() { return m_pInputKeyboard; }
	static CInputJoypad *GetInputJoypad() { return m_pInputJoypad; }
	static CInputMouse *GetInputMouse() { return m_pInputMouse; }

	static bool GetPause() { return m_bPause; }
	static void SetPause(bool bPause) { m_bPause = bPause; }
	static bool GetEntry(int nNum) { return m_bEntry[nNum]; }
	static void SetEntry(int nNum, bool bEntry) { m_bEntry[nNum] = bEntry; }

private:
	static CRenderer *m_pRenderer;				// レンダラー情報のポインタ
	static CInputKeyboard *m_pInputKeyboard;	// キーボード情報のポインタ
	static CInputJoypad *m_pInputJoypad;		// ジョイパッド情報のポインタ
	static CInputMouse *m_pInputMouse;			// マウス情報のポインタ
	static CSound *m_pSound;					// サウンド情報のポインタ
	static CBase *m_pBase;						// ベースシーン情報のポインタ
	static CFade *m_pFade;						// 画面フェード情報のポインタ
	static MODE m_mode;							// モード情報
	static bool m_bPause;						// ポーズするかどうか
	static bool m_bEntry[CPlayer::PLAYER_MAX];	// 参加しているかどうか
};

#endif // _MANAGER_
