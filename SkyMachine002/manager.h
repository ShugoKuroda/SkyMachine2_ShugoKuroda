//=============================================================================
//
// オブジェクト処理[object.h]
// Author : SHUGO KURODA
//
//=============================================================================
#ifndef _MANAGER_
#define _MANAGER_

#include "main.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CRenderer;
class CInputKeyboard;		// キーボードクラス
class CInputJoypad;			// ジョイパッドクラス
class CInputMouse;			// マウスクラス

//*****************************************************************************
// クラス定義(基本クラス)
//*****************************************************************************
class CManager
{
public:
	CManager();
	~CManager();

	HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
	void Uninit();
	void Update();
	void Draw();

	static CRenderer *GetRenderer() { return m_pRenderer; }
	static CInputKeyboard *GetInputKeyboard() { return m_pInputKeyboard; }
	static CInputJoypad *GetInputJoypad() { return m_pInputJoypad; }
	static CInputMouse *GetInputMouse() { return m_pInputMouse; }

private:
	static CRenderer *m_pRenderer;
	static CInputKeyboard *m_pInputKeyboard;	// キーボード情報のポインタ
	static CInputJoypad *m_pInputJoypad;		// ジョイパッド情報のポインタ
	static CInputMouse *m_pInputMouse;			// マウス情報のポインタ
};

#endif // _MANAGER_
