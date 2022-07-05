//=============================================================================
//
// オブジェクト処理[object.cpp]
// Author : SHUGO KURODA
//
//=============================================================================
#include "object2D.h"

#include "manager.h"
#include "renderer.h"

#include <assert.h>

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CObject2D::CObject2D()
	: m_pTexture(nullptr), m_pVtxBuff(nullptr), m_pos(0.0f, 0.0f, 0.0f), m_fRot(0.0f), m_fLength(0.0f), m_fAngle(0.0f)
{
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CObject2D::~CObject2D()
{
}

//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
HRESULT CObject2D::Init()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = m_pos.x + sinf(m_fRot + (-D3DX_PI + m_fAngle)) * m_fLength;
	pVtx[0].pos.y = m_pos.y + cosf(m_fRot + (-D3DX_PI + m_fAngle)) * m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_pos.x + sinf(m_fRot + (D3DX_PI - m_fAngle)) * m_fLength;
	pVtx[1].pos.y = m_pos.y + cosf(m_fRot + (D3DX_PI - m_fAngle)) * m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x + sinf(m_fRot + (0.0f - m_fAngle)) * m_fLength;
	pVtx[2].pos.y = m_pos.y + cosf(m_fRot + (0.0f - m_fAngle)) * m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + sinf(m_fRot + (0.0f + m_fAngle)) * m_fLength;
	pVtx[3].pos.y = m_pos.y + cosf(m_fRot + (0.0f + m_fAngle)) * m_fLength;
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

//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void CObject2D::Uninit()
{
	//頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// オブジェクトの破棄
	Release();
}

//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void CObject2D::Update()
{
}

//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void CObject2D::Draw()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//-----------------------------------------------------------------------------
// サイズの設定
//-----------------------------------------------------------------------------
void CObject2D::SetSize(D3DXVECTOR2 size)
{
	m_size = size;
	//対角線の長さを算出
	m_fLength = sqrtf(size.x * size.x + size.y * size.y) / 2.0f;
	//対角線の角度を算出する
	m_fAngle = atan2f(size.x, size.y);
}

//-----------------------------------------------------------------------------
// 頂点座標の設定
//-----------------------------------------------------------------------------
void CObject2D::SetVertex()
{
	VERTEX_2D *pVtx;

	//頂点バッファの設定
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

	//頂点バッファの解放
	m_pVtxBuff->Unlock();
}

//-----------------------------------------------------------------------------
// 頂点カラーの設定
//-----------------------------------------------------------------------------
void CObject2D::SetColor(D3DXCOLOR col)
{
	VERTEX_2D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(col);
	pVtx[1].col = D3DXCOLOR(col);
	pVtx[2].col = D3DXCOLOR(col);
	pVtx[3].col = D3DXCOLOR(col);

	m_pVtxBuff->Unlock();
}

//-----------------------------------------------------------------------------
// テクスチャアニメーション処理
//-----------------------------------------------------------------------------
void CObject2D::SetAnimation(int nAnimU, int nAnimV, int nPartU, int nPartV)
{
	VERTEX_2D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(0.0f + (1.0f / nPartU) * nAnimU, 0.0f + (1.0f / nPartV) * nAnimV);
	pVtx[1].tex = D3DXVECTOR2((1.0f / nPartU) * (nAnimU + 1), 0.0f + (1.0f / nPartV) * nAnimV);
	pVtx[2].tex = D3DXVECTOR2(0.0f + (1.0f / nPartU) * nAnimU, (1.0f / nPartV) * (nAnimV + 1));
	pVtx[3].tex = D3DXVECTOR2((1.0f / nPartU) * (nAnimU + 1), (1.0f / nPartV) * (nAnimV + 1));

	m_pVtxBuff->Unlock();
}

//-----------------------------------------------------------------------------
// 生成処理
//-----------------------------------------------------------------------------
//CObject2D *CObject2D::Create()
//{
//	CObject2D* pObject2D = nullptr;
//	pObject2D = new CObject2D;
//
//	if (pObject2D != nullptr)
//	{
//		pObject2D->Init();
//		return pObject2D;
//	}
//
//	assert(false);
//	return pObject2D;
//}
