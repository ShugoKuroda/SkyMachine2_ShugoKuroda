//=============================================================================
//
// レンダラー処理[renderer.cpp]
// Author : SHUGO KURODA
//
//=============================================================================
#include <tchar.h> // _T
#include <stdio.h>

#include "main.h"		//スクリーンサイズの取得
#include "renderer.h"
#include "manager.h"
#include "object.h"
#include "object2D.h"

#include <assert.h>

//=============================================================================
// コンストラクタ
//=============================================================================
CRenderer::CRenderer() :m_pD3D(nullptr), m_pD3DDevice(nullptr)
{
#ifdef _DEBUG
	m_pFont = nullptr;
#endif // _DEBUG
}

//=============================================================================
// デストラクタ
//=============================================================================
CRenderer::~CRenderer()
{
	assert(CManager::GetRenderer() != nullptr);
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CRenderer::Init(HWND hWnd, bool bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pD3D == nullptr)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));			// ワークをゼロクリア
	d3dpp.BackBufferCount = 1;					// バックバッファの数
	d3dpp.BackBufferWidth = SCREEN_WIDTH;		// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;		// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;		// カラーモードの指定
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// 映像信号に同期してフリップする
	d3dpp.EnableAutoDepthStencil = TRUE;		// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;	// デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;					// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル

	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if ((FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice))) &&
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice))) &&
		// 上記の設定が失敗したら
		// 描画と頂点処理をCPUで行なう
		(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice))))
	{
		// 生成失敗
		return E_FAIL;
	}

	//レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);					//カリング
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);					// αブレンドを行う
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);			// αソースカラーの指定
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);		// αデスティネーションカラー

	//サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);			// テクスチャU値の繰り返し設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);			// テクスチャV値の繰り返し設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);		// テクスチャ拡大時の補間設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);		// テクスチャ縮小時の補間設定

	//テクスチャステージステートパラメータの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);		//αブレンディング処理設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);		//最初のα引数
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);		//２番目のα引数

#ifdef _DEBUG
	// デバッグ情報表示用フォントの生成
	D3DXCreateFont(m_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Terminal"), &m_pFont);
#endif

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CRenderer::Uninit()
{
#ifdef _DEBUG
	// デバッグ情報表示用フォントの破棄
	if (m_pFont != nullptr)
	{
		m_pFont->Release();
		m_pFont = nullptr;
	}
#endif // _DEBUG

	// デバイスの破棄
	if (m_pD3DDevice != nullptr)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}

	// Direct3Dオブジェクトの破棄
	if (m_pD3D != nullptr)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CRenderer::Update()
{
	//オブジェクトの更新処理
	CObject::UpdateAll();
}

//=============================================================================
// 描画処理
//=============================================================================
void CRenderer::Draw()
{
	// バックバッファ＆Ｚバッファのクリア
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3Dによる描画の開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{//描画開始が成功した場合

		//オブジェクトの描画処理
		CObject::DrawAll();

#ifdef _DEBUG
		// FPS表示
		DrawFPS();
#endif // _DEBUG

		//描画終了
		m_pD3DDevice->EndScene();
	}

	//バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

#ifdef _DEBUG
//=============================================================================
// FPS描画処理
//=============================================================================
void CRenderer::DrawFPS()
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	RECT rectObj[CObject::MAX_OBJECT];
	char str[256];
	char strObj[CObject::MAX_OBJECT][256];

	int nCntFPS = GetCounterFPS();
	wsprintf(str, _T("FPS : %d\n"), nCntFPS);
	// FPS描画
	m_pFont->DrawTextA(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	//オブジェクト情報の取得
	for (int nCntObj = 0; nCntObj < CObject::MAX_OBJECT; nCntObj++)
	{
		CObject *pObject = CObject::GetObject(nCntObj);
		if (pObject != nullptr && nCntObj != 1)
		{
			if (nCntObj > 35)
			{
				rectObj[nCntObj] = { 300, 20 * (nCntObj - 35), SCREEN_WIDTH, SCREEN_HEIGHT };
			}
			else
			{
				rectObj[nCntObj] = { 0, 20 * (nCntObj + 1), SCREEN_WIDTH, SCREEN_HEIGHT };
			}

			D3DXVECTOR3 pos = ((CObject2D*)pObject)->GetPosition();

			sprintf(strObj[nCntObj], _T("OBJ[%d] : X=%.2f Y=%.2f Z=%.2f"), nCntObj, pos.x, pos.y, pos.z);

			//オブジェクトの位置描画
			m_pFont->DrawTextA(NULL, strObj[nCntObj], -1, &rectObj[nCntObj], DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
		}
	}
}

#endif // _DEBUG
