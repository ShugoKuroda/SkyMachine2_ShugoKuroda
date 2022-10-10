//====================================================================
//
// エフェクトの処理[effect.h]
// Author : SHUGO KURODA
//
//====================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//================================================
// インクルードファイル
//================================================
#include "object2D.h"

//================================================
// クラス宣言
//================================================
// エフェクトクラス
class CEffect : public CObject2D
{
private:
	//　エフェクトの寿命
	int MAX_LIFE = 30;

public:
	// エフェクトの種類
	enum TYPE
	{
		TYPE_SPHERE = 0,		// 球体
		TYPE_AFTERIMAGE,		// 残像
		TYPE_MAX				// 最大数
	};

	// テクスチャの種類
	enum TEX
	{
		TEX_NONE = 0,
		TEX_SPHERE,				// 球体
		TEX_BOSS,				// ボス
		TEX_MAX
	};

	CEffect();
	~CEffect() override;

	static CEffect *Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& size, const TYPE& type, const TEX& tex);
	static HRESULT Load();		//テクスチャの読み込み
	static void Unload();		//テクスチャの削除

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[TEX_MAX];   //テクスチャのポインタ
	D3DXVECTOR3 m_move;		// 移動量
	int m_nLife;			// 寿命
	TYPE m_type;			// エフェクトの種類
};

#endif