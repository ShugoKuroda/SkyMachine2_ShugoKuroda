//=============================================================================
//
// オブジェクト処理[object.h]
// Author : SHUGO KURODA
//
//=============================================================================
#ifndef _OBJECT_2D_
#define _OBJECT_2D_

#include "main.h"
#include "object.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CObject2D : public CObject
{
public:
	CObject2D();
	//CObject2D(CObject2D* pObject);
	~CObject2D() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	static CObject2D *Create();

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

	// 頂点フォーマット
	static const DWORD FVF_VERTEX_2D = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	static int m_RandPos;
};

#endif // _OBJECT_2D_
