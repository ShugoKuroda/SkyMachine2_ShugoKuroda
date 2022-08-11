//=============================================================================
//
// キーボード処理 [input_keyboard.h]
// Author : ITSUKI TAKAIWA
//
//=============================================================================
#ifndef _INPUTKEYBOARD_H_
#define _INPUTKEYBOARD_H_

#include "input.h"

//-----------------------------------------------------------------------------
// キーボードクラス(派生クラス)
//-----------------------------------------------------------------------------
class CInputKeyboard : public CInput
{
private:
	static const int MAX_KEY = 256;

public:
	// キー入力情報
	enum KEYINFO
	{
		KEYINFO_OK = 0,				// 決定
		KEYINFO_UP,					// 上選択
		KEYINFO_DOWN,				// 下選択
		KEYINFO_RIGHT,				// 右移動
		KEYINFO_LEFT,				// 左移動
		KEYINFO_PAUSE,				// ポーズ
		KEYINFO_ATTACK,				// 攻撃
		KEYINFO_1,					// 1
		KEYINFO_MAX,
	};

	CInputKeyboard();
	~CInputKeyboard();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit();
	void Update();
	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetReleace(int nKey);

private:
	BYTE m_aKeyState[MAX_KEY];
	BYTE m_aKeyStateTrigger[MAX_KEY];
	BYTE m_aKeyStateReleace[MAX_KEY];

	int m_aKeyInfo[KEYINFO_MAX] =
	{
		DIK_RETURN,		// 決定
		DIK_W,			// 上選択
		DIK_S,			// 下選択 
		DIK_D,			// 右移動
		DIK_A,			// 左移動
		DIK_P,			// ポーズ
		DIK_SPACE,		// 攻撃
		DIK_1			// 1
	};
};
#endif