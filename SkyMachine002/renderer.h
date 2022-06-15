//=============================================================================
//
// レンダラー処理[renderer.h]
// Author : SHUGO KURODA
//
//=============================================================================
#ifndef _RENDERER_
#define _RENDERER_

#include "main.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	LPDIRECT3DDEVICE9 GetDevice();

#ifdef _DEBUG
	void DrawFPS();
#endif // _DEBUG

private:	//メンバ変数
	// Direct3Dオブジェクト
	LPDIRECT3D9 m_pD3D = nullptr;
	// Deviceオブジェクト
	LPDIRECT3DDEVICE9 m_pD3DDevice = nullptr;

#ifdef _DEBUG
	// フォント
	LPD3DXFONT m_pFont = nullptr;
#endif // _DEBUG

};

#endif // _RENDERER_
