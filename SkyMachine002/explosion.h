//-------------------------------------------
//
// 爆発の情報[explosion.h]
// Author : SHUGO KURODA
//
//-------------------------------------------
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

//-------------------------------------------
// インクルードファイル
//-------------------------------------------
#include "object2D.h"	// 2Dオブジェクト

//-------------------------------------------
// 爆発クラス
//-------------------------------------------
class CExplosion : public CObject2D
{
private:		// 定数
				// 幅
	static const float SIZE_WIDTH;
	// 高さ
	static const float SIZE_HEIGHT;
	// アニメーション間隔
	static const int ANIM_INTERVAL;
	// アニメーション最大数
	static const int ANIM_MAXANIM;
	// U座標の最大分割数
	static const int DIVISION_U;
	// V座標の最大分割数
	static const int DIVISION_V;

public:
	// コンストラクタ
	CExplosion();
	// デストラクタ
	~CExplosion() override;

	// 生成
	static CExplosion* Create(const D3DXVECTOR3& pos);
	// テクスチャの読み込み
	static HRESULT Load();
	// テクスチャの削除
	static void Unload();

	// 初期化
	HRESULT Init(const D3DXVECTOR3& pos) override;
	// 終了
	void Uninit() override;
	// 更新
	void Update() override;
	// 描画
	void Draw() override;

private:
	// テクスチャのポインタ
	static LPDIRECT3DTEXTURE9 m_pTexture;
	// アニメーション用カウンター
	int m_nCntAnim;
	// 現在のアニメーション番号
	int m_nPatterAnim;
};

#endif		// _EXPLOSION_H_
