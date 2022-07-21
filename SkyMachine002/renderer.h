//=============================================================================
//
// レンダラー処理[renderer.h]
// Author : SHUGO KURODA
//
//=============================================================================
#ifndef _RENDERER_
#define _RENDERER_

//*****************************************************************************
//	ヘッダファイルのインクルード
//*****************************************************************************
#include <d3dx9.h>		// 描画処理に必要

//*****************************************************************************
//	ライブラリファイルのリンク
//*****************************************************************************
#pragma comment(lib,"d3d9.lib")   // 描画処理に必要

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CRenderer
{
public:	//定数
	// スクリーンの幅
	static const int SCREEN_WIDTH = 1280;
	// スクリーンの高さ
	static const int SCREEN_HEIGHT = 720;

public:
	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, bool bWindow);
	void Uninit();
	void Update();
	void Draw();

	LPDIRECT3DDEVICE9 GetDevice() { return m_pD3DDevice; }

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
