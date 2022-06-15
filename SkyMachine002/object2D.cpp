//=============================================================================
//
// オブジェクト処理[object.cpp]
// Author : SHUGO KURODA
//
//=============================================================================
#include "object2D.h"
#include "renderer.h"

#include <assert.h>

//*****************************************************************************
// 構造体定義
//*****************************************************************************
// 頂点データ
struct VERTEX_2D
{
	D3DXVECTOR3 pos;
	float rhw;
	D3DCOLOR col;
	D3DXVECTOR2 tex;
};

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
int CObject2D::m_RandPos = 0;

//=============================================================================
// コンストラクタ
//=============================================================================
CObject2D::CObject2D()
{
	//頂点バッファへのポインター初期化
	m_pVtxBuff = nullptr;
	//テクスチャへのポインター初期化
	m_pTexture = nullptr;

	//各要素の初期化
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fRot = 0.0f;
	m_fLength = 0.0f;
	m_fAngle = 0.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CObject2D::~CObject2D()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CObject2D::Init()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pD3DDevice = GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pD3DDevice, "data/TEXTURE/test001.png", &m_pTexture);

	m_RandPos++;

	//位置の初期化
	m_pos = D3DXVECTOR3(SCREEN_WIDTH / 4.0f * m_RandPos, SCREEN_HEIGHT / 2, 0.0f);
	//向きの初期化
	m_fRot = (rand() % 628) / D3DX_PI;
	//対角線の長さを算出
	m_fLength = sqrtf(400.0f * 400.0f + 200.0f * 200.0f) / 2.0f;
	//対角線の角度を算出する
	m_fAngle = atan2f(200.0f, 100.0f);

	//頂点バッファの生成
	pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = m_pos.x + sinf(m_fRot + (-D3DX_PI + m_fAngle))*m_fLength;
	pVtx[0].pos.y = m_pos.y + cosf(m_fRot + (-D3DX_PI + m_fAngle))*m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_pos.x + sinf(m_fRot + (D3DX_PI - m_fAngle))*m_fLength;
	pVtx[1].pos.y = m_pos.y + cosf(m_fRot + (D3DX_PI - m_fAngle))*m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x + sinf(m_fRot + (0.0f - m_fAngle))*m_fLength;
	pVtx[2].pos.y = m_pos.y + cosf(m_fRot + (0.0f - m_fAngle))*m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + sinf(m_fRot + (0.0f + m_fAngle))*m_fLength;
	pVtx[3].pos.y = m_pos.y + cosf(m_fRot + (0.0f + m_fAngle))*m_fLength;
	pVtx[3].pos.z = 0.0f;

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CObject2D::Uninit()
{
	//頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	//テクスチャの破棄
	if (m_pTexture != nullptr)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}

	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CObject2D::Update()
{
	m_fLength += sinf(m_fRot) * 2.0f;
	m_fRot += -D3DX_PI * 0.01f;

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = m_pos.x + sinf(m_fRot + (-D3DX_PI + m_fAngle))*m_fLength;
	pVtx[0].pos.y = m_pos.y + cosf(m_fRot + (-D3DX_PI + m_fAngle))*m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_pos.x + sinf(m_fRot + (D3DX_PI - m_fAngle))*m_fLength;
	pVtx[1].pos.y = m_pos.y + cosf(m_fRot + (D3DX_PI - m_fAngle))*m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x + sinf(m_fRot + (0.0f - m_fAngle))*m_fLength;
	pVtx[2].pos.y = m_pos.y + cosf(m_fRot + (0.0f - m_fAngle))*m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + sinf(m_fRot + (0.0f + m_fAngle))*m_fLength;
	pVtx[3].pos.y = m_pos.y + cosf(m_fRot + (0.0f + m_fAngle))*m_fLength;
	pVtx[3].pos.z = 0.0f;

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void CObject2D::Draw()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pD3DDevice = GetRenderer()->GetDevice();

	//頂点バッファをデータストリームに設定
	pD3DDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pD3DDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pD3DDevice->SetTexture(0, m_pTexture);

	//ポリゴンの描画
	pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// 生成処理
//=============================================================================
CObject2D *CObject2D::Create()
{
	CObject2D* pObject2D = nullptr;
	pObject2D = new CObject2D;

	if (pObject2D != nullptr)
	{
		pObject2D->Init();
		return pObject2D;
	}
	assert(false);
	return pObject2D;
}
