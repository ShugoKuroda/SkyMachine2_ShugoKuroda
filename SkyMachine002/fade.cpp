//=============================================================================
//
// フェード [fade.cpp]
// Author:SHUGO KURODA
//
//=============================================================================
#include "fade.h"
#include "base.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define REMOVE_ALFHA		(0.05f)				// α値の増減量

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CFade::FADE CFade::m_fade = CFade::FADE_IN;					// フェードの状態
CManager::MODE CFade::m_modeNext = CManager::MODE_TITLE;	// フェードの状態

//=============================================================================
// CFadeのコンストラクタ
//=============================================================================
CFade::CFade() :m_pVtxBuff(nullptr)
{
	
}

//=============================================================================
// CFadeのデストラクタ
//=============================================================================
CFade::~CFade()
{
}

//=============================================================================
// フェード生成
//=============================================================================
CFade* CFade::Create(CManager::MODE modeNext)
{
	CFade* pFade = new CFade;

	if (pFade != nullptr)
	{
		// 次の画面(モード)設定
		pFade->m_modeNext = modeNext;
		// 初期化処理
		pFade->Init();
	}

	return pFade;
}

//=============================================================================
// フェードの初期化処理
//=============================================================================
HRESULT CFade::Init()
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 変数の初期化
	m_fade = CFade::FADE_IN;
	m_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(CObject2D::VERTEX_2D) * CObject2D::MAX_VERTEX, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		CObject2D::FVF_VERTEX_2D,		// 頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	CObject2D::VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, (float)CRenderer::SCREEN_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3((float)CRenderer::SCREEN_WIDTH, (float)CRenderer::SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3((float)CRenderer::SCREEN_WIDTH, 0.0f, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = m_colorFade;
	pVtx[1].col = m_colorFade;
	pVtx[2].col = m_colorFade;
	pVtx[3].col = m_colorFade;

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// フェードの終了処理
//=============================================================================
void CFade::Uninit()
{
	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
// フェードの更新処理
//=============================================================================
void CFade::Update()
{
	// 構造体のポインタ変数
	CObject2D::VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	pVtx[0].col = m_colorFade;
	pVtx[1].col = m_colorFade;
	pVtx[2].col = m_colorFade;
	pVtx[3].col = m_colorFade;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// フェード情報の分岐
	if (m_fade == FADE_IN)
	{// フェードインの時
		m_colorFade.a -= REMOVE_ALFHA;

		if (m_colorFade.a <= 0.0f)
		{// 0.0fになったとき何もしない
			m_fade = FADE_NONE;
		}
	}
	else if (m_fade == FADE_OUT)
	{// フェードアウトのとき
		m_colorFade.a += REMOVE_ALFHA;

		if (m_colorFade.a >= 1.0f)
		{// 1.0fになったときフェードイン
			m_fade = FADE_IN;

			// モードの設定
			CManager::SetMode(m_modeNext);
		}
	}
	else if (m_fade == FADE_NONE)
	{// 何もしていないとき
		m_colorFade.a = 0.0f;
	}
}

//=============================================================================
// フェードの描画処理
//=============================================================================
void CFade::Draw()
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(CObject2D::VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(CObject2D::FVF_VERTEX_2D);

	// ポリゴンの描画
	pDevice->SetTexture(0, NULL);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// フェード設定
//=============================================================================
void CFade::SetFade(FADE fade, CManager::MODE modeNext)
{
	m_fade = fade;
	m_modeNext = modeNext;
}
