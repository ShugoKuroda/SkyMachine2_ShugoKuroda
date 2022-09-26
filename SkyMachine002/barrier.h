//===================================================================
//
//	オプション弾の処理[bullet_option.h]
//	Author:SHUGO KURODA
//
//===================================================================
#ifndef _BARRIER_H_
#define _BARRIER_H_

#include "object2D.h"

//*******************************************************************
//	弾クラスの定義
//*******************************************************************
class CBarrier : public CObject2D
{
private:
	//サイズ(X)
	static const float SIZE_X;
	//サイズ(Y)
	static const float SIZE_Y;
	// アニメーション間隔
	static const int ANIM_INTERVAL;
	// アニメーション最大数
	static const int MAX_ANIM;
	// U座標の最大分割数
	static const int DIVISION_U;
	// V座標の最大分割数
	static const int DIVISION_V;

public:
	// バリアの最大強化数
	static const int MAX_NUM = 4;

	// 親プレイヤー
	enum PARENT
	{
		PLAYER_1 = 0,	//1P
		PLAYER_2,		//2P
	};

	enum LEVEL
	{//強化(色の変化)状態
		LEVEL_NONE = 0,
		LEVEL_GREEN,	//1段階目
		LEVEL_SILVER,	//2段階目
		LEVEL_GOLD,		//3段階目
		LEVEL_MAX
	};

	CBarrier();
	~CBarrier() override;

	//メンバ関数
	static CBarrier *Create(const D3DXVECTOR3& pos, const PARENT& parent);		//インスタンス生成処理
	static HRESULT Load();		//テクスチャの読み込み
	static void Unload();		//テクスチャの削除

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetBarrier(LEVEL col);

private:
	//メンバ変数
	static LPDIRECT3DTEXTURE9 m_apTexture[LEVEL_MAX];		// テクスチャのポインタ
	float m_fRad;				// 角度
	LEVEL m_col;				// 色の種類
	int m_nDamage;				// ダメージ量
	PARENT m_parent;	// 所有するプレイヤー

	// アニメーション用カウンター
	int m_nCntAnim;
	// 現在のアニメーション番号
	int m_nPatternAnim;
	// 現在のアニメーションV番号
	int m_nPatterAnimV;
};

#endif		// _BARRIER_H_
