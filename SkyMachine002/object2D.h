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
	//CObject2D(CObject2D* pObject);
	virtual ~CObject2D() override;

	virtual HRESULT Init(const D3DXVECTOR3& pos) override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;
	void SetPosition(const D3DXVECTOR3& pos) override;
	void SetSize(float fSizeX, float fSizeY) override;
	void SetRot(float fRot) override;

	void SetVertex();
	void SetColor(D3DXCOLOR col);
	void SetAnimation(int nAnimU, int nAnimV, int nPartU, int nPartV);

	//static CObject2D *Create();

	// 位置の取得
	const D3DXVECTOR3& GetPosition() const override;
	// 向きの取得
	const float& GetRot() const;

protected:
	// テクスチャの設定
	void BindTexture(LPDIRECT3DTEXTURE9 Texture);

private:	//メンバ変数
	//頂点バッファへのポインター
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	//テクスチャへのポインター
	LPDIRECT3DTEXTURE9 m_pTexture;
	//位置
	D3DXVECTOR3 m_pos;
	//向き
	float m_fRot;
	//対角線の長さ
	float m_fLength;
	//対角線の角度
	float m_fAngle;
	//頂点カラーの設定
	D3DXCOLOR m_col;
};

#endif // _OBJECT_2D_
