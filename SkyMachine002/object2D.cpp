//=============================================================================
//
// オブジェクト処理[object2D.cpp]
// Author : SHUGO KURODA
//
//=============================================================================
#include "object2D.h"

#include "manager.h"
#include "renderer.h"

#include <assert.h>

//*****************************************************************************
// テンプレート関数                       
//*****************************************************************************

//*****************************************************************************
// 割り算
//
// 引数   :data0,data1 
// 戻り値 :計算結果
//*****************************************************************************
template<class T> T Divide(const T data0, const T data1)
{
	T answer = data0 / data1;
	return answer;
}

//-----------------------------------------------------------------------------
// デフォルトコンストラクタ
//-----------------------------------------------------------------------------
CObject2D::CObject2D()
	: m_pTexture(nullptr), m_pVtxBuff(nullptr), m_pos(0.0f, 0.0f, 0.0f), m_size(0.0f, 0.0f), m_fRot(0.0f), m_fLength(0.0f), m_fAngle(0.0f),
	m_col(1.0f, 1.0f, 1.0f, 1.0f), m_nCounterAnim(0), m_nPatternAnim(0), m_move(0.0f, 0.0f, 0.0f)
{
}

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CObject2D::CObject2D(EObject type)
{
	//オブジェクトの種類設定
	SetObjType(type);
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
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAX_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//頂点情報ポインタを宣言
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
	pVtx[0].col = D3DXCOLOR(m_col);
	pVtx[1].col = D3DXCOLOR(m_col);
	pVtx[2].col = D3DXCOLOR(m_col);
	pVtx[3].col = D3DXCOLOR(m_col);

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
	//大きさの設定
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
	pVtx[0].pos.x = (m_pos.x + m_move.x) + sinf(m_fRot + (-D3DX_PI + m_fAngle))*m_fLength;
	pVtx[0].pos.y = (m_pos.y + m_move.y) + cosf(m_fRot + (-D3DX_PI + m_fAngle))*m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = (m_pos.x + m_move.x) + sinf(m_fRot + (D3DX_PI - m_fAngle))*m_fLength;
	pVtx[1].pos.y = (m_pos.y + m_move.y) + cosf(m_fRot + (D3DX_PI - m_fAngle))*m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = (m_pos.x + m_move.x) + sinf(m_fRot + (0.0f - m_fAngle))*m_fLength;
	pVtx[2].pos.y = (m_pos.y + m_move.y) + cosf(m_fRot + (0.0f - m_fAngle))*m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = (m_pos.x + m_move.x) + sinf(m_fRot + (0.0f + m_fAngle))*m_fLength;
	pVtx[3].pos.y = (m_pos.y + m_move.y) + cosf(m_fRot + (0.0f + m_fAngle))*m_fLength;
	pVtx[3].pos.z = 0.0f;

	//頂点バッファの解放
	m_pVtxBuff->Unlock();
}

//-----------------------------------------------------------------------------
// 頂点カラーの設定
//-----------------------------------------------------------------------------
void CObject2D::SetColor(D3DXCOLOR col)
{
	// カラーの設定
	m_col = col;

	VERTEX_2D *pVtx;

	//頂点バッファの設定
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(m_col);
	pVtx[1].col = D3DXCOLOR(m_col);
	pVtx[2].col = D3DXCOLOR(m_col);
	pVtx[3].col = D3DXCOLOR(m_col);

	//頂点バッファの解放
	m_pVtxBuff->Unlock();
}

//-----------------------------------------------------------------------------
// テクスチャアニメーション処理(int)
//
// nAnimU → 現在のアニメーションU座標
// nAnimV → 現在のアニメーションV座標
// nPartU → U座標の最大分割数
// nPartV → V座標の最大分割数
//-----------------------------------------------------------------------------
void CObject2D::SetAnimation(int nAnimU, int nAnimV, int nPartU, int nPartV)
{
	VERTEX_2D *pVtx;

	//頂点バッファの設定
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f + (1.0f / nPartU) * nAnimU, 0.0f + (1.0f / nPartV) * nAnimV);
	pVtx[1].tex = D3DXVECTOR2((1.0f / nPartU) * (nAnimU + 1), 0.0f + (1.0f / nPartV) * nAnimV);
	pVtx[2].tex = D3DXVECTOR2(0.0f + (1.0f / nPartU) * nAnimU, (1.0f / nPartV) * (nAnimV + 1));
	pVtx[3].tex = D3DXVECTOR2((1.0f / nPartU) * (nAnimU + 1), (1.0f / nPartV) * (nAnimV + 1));

	//頂点バッファの解放
	m_pVtxBuff->Unlock();
}

//-----------------------------------------------------------------------------
// テクスチャアニメーション処理(float)
//
// nAnimU → 現在のアニメーションU座標
// nAnimV → 現在のアニメーションV座標
// fPartU → U座標の最大分割数
// fPartV → V座標の最大分割数
//-----------------------------------------------------------------------------
void CObject2D::SetAnimation(int nAnimU, int nAnimV, float fPartU, float fPartV)
{
	VERTEX_2D *pVtx;

	//頂点バッファの設定
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f + (1.0f / fPartU) * nAnimU, 0.0f + (1.0f / fPartV) * nAnimV);
	pVtx[1].tex = D3DXVECTOR2((1.0f / fPartU) * (nAnimU + 1), 0.0f + (1.0f / fPartV) * nAnimV);
	pVtx[2].tex = D3DXVECTOR2(0.0f + (1.0f / fPartU) * nAnimU, (1.0f / fPartV) * (nAnimV + 1));
	pVtx[3].tex = D3DXVECTOR2((1.0f / fPartU) * (nAnimU + 1), (1.0f / fPartV) * (nAnimV + 1));

	//頂点バッファの解放
	m_pVtxBuff->Unlock();
}

//-----------------------------------------------------------------------------
// 背景アニメーション処理
//-----------------------------------------------------------------------------
void CObject2D::SetAnimBg(int nSpeed, int nPattern, bool bRightToLeft)
{
	// アニメーション
	m_nCounterAnim++;	//カウンタ加算

	if (m_nCounterAnim == nSpeed)//速さ
	{
		// オーバーフロー防止
		m_nCounterAnim = 0;  // カウンタを0に戻す

		// アニメーションを切り替える
		m_nPatternAnim = (m_nPatternAnim + 1) % nPattern;  // 枚数
	}

	// 何等分するか計算
	float fEqualDivision = Divide(1.0f, (float)nPattern);

	// 左から右なら、-1をかける
	if (bRightToLeft == false)
	{
		fEqualDivision *= -1;
	}

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャの座標を反映
	pVtx[0].tex = D3DXVECTOR2(m_nPatternAnim * fEqualDivision, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(m_nPatternAnim * fEqualDivision + 1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(m_nPatternAnim * fEqualDivision, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(m_nPatternAnim * fEqualDivision + 1.0f, 1.0f);

	//頂点バッファの解放
	m_pVtxBuff->Unlock();
}

//-----------------------------------------------------------------------------
// 背景アニメーション処理(右肩上がり)
//-----------------------------------------------------------------------------
void CObject2D::SetAnimBgLeftUp(int nSpeed, int nPattern, bool bRightToLeft)
{
	// アニメーション
	m_nCounterAnim++;	//カウンタ加算

	if (m_nCounterAnim == nSpeed)//速さ
	{
		// オーバーフロー防止
		m_nCounterAnim = 0;  // カウンタを0に戻す

		// アニメーションを切り替える
		m_nPatternAnim = (m_nPatternAnim + 1) % nPattern;  // 枚数
	}

	// 何等分するか計算
	float fEqualDivision = Divide(1.0f, (float)nPattern);

	// 左から右なら、-1をかける
	if (bRightToLeft == false)
	{
		fEqualDivision *= -1;
	}

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャの座標を反映
	pVtx[0].tex = D3DXVECTOR2(m_nPatternAnim * fEqualDivision, m_nPatternAnim * (fEqualDivision * (-1)));
	pVtx[1].tex = D3DXVECTOR2(m_nPatternAnim * fEqualDivision + 1.0f, m_nPatternAnim * (fEqualDivision * (-1)));
	pVtx[2].tex = D3DXVECTOR2(m_nPatternAnim * fEqualDivision, m_nPatternAnim * (fEqualDivision * (-1)) + 1.0f);
	pVtx[3].tex = D3DXVECTOR2(m_nPatternAnim * fEqualDivision + 1.0f, m_nPatternAnim * (fEqualDivision * (-1)) + 1.0f);

	//頂点バッファの解放
	m_pVtxBuff->Unlock();
}

//-----------------------------------------------------------------------------
// テクスチャの描画範囲の設定
//-----------------------------------------------------------------------------
void CObject2D::SetTextureRange(int nRange, int nPattern)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標の設定
	float fEqualDivision = 0.0f;   // テクスチャを等分する

	// 何等分するか計算
	fEqualDivision = Divide((float)nRange, (float)nPattern);

	// テクスチャの座標を反映
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(fEqualDivision, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(fEqualDivision, 1.0f);

	//頂点データをアンロックする
	m_pVtxBuff->Unlock();
}
