//=============================================================================
//
// 入力処理 [input.h]
// Author : SHUGO KURODA
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

//*****************************************************************************
//	ヘッダファイルのインクルード
//*****************************************************************************
#define DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>					// 入力処理に必要

//*****************************************************************************
//	ライブラリファイルのリンク
//*****************************************************************************
#pragma comment(lib,"dinput8.lib")	// 入力処理に必要

//-----------------------------------------------------------------------------
// 入力クラス(基本クラス)
//-----------------------------------------------------------------------------
class CInput
{
public:
	CInput();
	virtual ~CInput();

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;

protected:
	LPDIRECTINPUTDEVICE8 m_pDevice;
	static LPDIRECTINPUT8 m_pInput;
};

#endif
