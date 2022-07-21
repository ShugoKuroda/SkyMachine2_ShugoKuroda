//====================================================================
//
// 泡エフェクト処理 (bubble.h)
// Author : SHUGO KURODA
//
//====================================================================
#ifndef _BUBBLE_H_
#define _BUBBLE_H_

#include "object2D.h"

//================================================
// クラス宣言
//================================================
// 泡クラス
class CBubble : public CObject2D
{
public:	//定数
	// 泡サイズの最小値
	static const int MIN_SIZE = 10;
public:
	CBubble();
	~CBubble() override;

	//メンバ関数
	static CBubble *Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& size);
	static HRESULT Load();
	static void Unload();
	
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;   // 読み込んだテクスチャを保存する
	static int m_nReverseCount;				// 泡の揺れを反転させる用
	int m_nReverse;
	D3DXVECTOR3 m_move;     // 移動量
};

#endif	//_BUBBLE_H_
