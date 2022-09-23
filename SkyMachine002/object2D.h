//=============================================================================
//
// オブジェクト処理[object.h]
// Author : SHUGO KURODA
//
//=============================================================================
#ifndef _OBJECT_2D_
#define _OBJECT_2D_

#include "object.h"
#include "renderer.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CObject2D : public CObject
{
public:
	// 頂点フォーマット
	static const DWORD FVF_VERTEX_2D = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	// 頂点の数
	static const int MAX_VERTEX = 4;

	// 頂点データ
	struct VERTEX_2D
	{
		D3DXVECTOR3 pos;
		float rhw;
		D3DCOLOR col;
		D3DXVECTOR2 tex;
	};

public:
	CObject2D();
	CObject2D(EObject type);
	virtual ~CObject2D() override;

	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;
	void SetPosition(const D3DXVECTOR3& pos) { m_pos = pos; }
	void SetSize(D3DXVECTOR2 size);
	void SetRot(float fRot) { m_fRot = fRot; }
	void SetMove(const D3DXVECTOR3& move){ m_move = move; }

	void SetVertex();
	void SetColor(D3DXCOLOR col);
	void SetAnimation(int nAnimU, int nAnimV, int nPartU, int nPartV);
	void SetAnimation(int nAnimU, int nAnimV, float fPartU, float fPartV);
	void SetAnimBg(int nSpeed, int nPattern, bool bRightToLeft);
	// テクスチャの設定
	void BindTexture(LPDIRECT3DTEXTURE9 Texture) { m_pTexture = Texture; }

	// 位置の取得
	const D3DXVECTOR3& GetPosition() const { return m_pos; }
	// 向きの取得
	const float& GetRot() const { return m_fRot; }
	// サイズの取得
	const D3DXVECTOR2& GetSize() const { return m_size; }
	// 対角線の長さ取得
	const float& GetLength() const { return m_fLength; }
	// 色の取得
	const D3DXCOLOR& GetColor() const { return m_col; }

private:	//メンバ変数
	//頂点バッファへのポインター
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	//テクスチャへのポインター
	LPDIRECT3DTEXTURE9 m_pTexture;
	//位置
	D3DXVECTOR3 m_pos;
	//サイズ
	D3DXVECTOR2 m_size;
	//向き
	float m_fRot;
	//対角線の長さ
	float m_fLength;
	//対角線の角度
	float m_fAngle;
	//頂点カラー
	D3DXCOLOR m_col;
	//アニメーションカウンター
	int m_nCounterAnim;
	//アニメーションパターン
	int m_nPatternAnim;

	// 移動量(画面シェイク用)
	D3DXVECTOR3 m_move;
};

#endif // _OBJECT_2D_
