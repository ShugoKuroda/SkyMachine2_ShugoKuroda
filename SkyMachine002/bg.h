//===================================================================
//
//	背景処理[bg.h]
//	Author:SHUGO KURODA
//
//===================================================================
#ifndef _BG_H_
#define _BG_H_

#include "object.h"
#include "object2D.h"

//*******************************************************************
//	背景クラスの定義
//*******************************************************************
class CBg : public CObject
{
private:
	// 寿命
	static const int LIFE = 50;
	//プレイヤーのサイズ(X)
	static const float SIZE_WIDTH;
	//プレイヤーのサイズ(Y)
	static const float SIZE_HEIGHT;
	//プレイヤーの基本移動量
	static const float MOVE_DEFAULT;
	// アニメーション間隔
	static const int ANIM_INTERVAL;
	// アニメーション最大数
	static const int MAX_ANIM;
	// U座標の最大分割数
	static const int DIVISION_U;
	// V座標の最大分割数
	static const int DIVISION_V;

	//最大背景数
	static const int MAX_BG = 5;
public:
	// 背景のセット
	enum SET
	{
		SET_NONE = 0,	// なし
		SET_a,			// α
		SET_b,			// β
		SET_r,			// γ
		SET_MAX			// セットの最大数
	};

	enum EType
	{//背景の種類
		TYPE_PLAYER = 0,
		TYPE_ENEMY,
		TYPE_MAX
	};

	CBg();
	~CBg() override;

	//メンバ関数
	static CBg *Create(SET set);	//インスタンス生成処理
	static HRESULT Load();		//テクスチャの読み込み
	static void Unload();		//テクスチャの削除

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

private:
	//メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture[TYPE_MAX];		//テクスチャのポインタ
	CObject2D *m_apObject2D[MAX_BG];					// 2Dポリゴンへのポインタ
	D3DXVECTOR3 m_move;			//移動量
	int m_nLife;				//寿命
	int m_nCntAnim;				//アニメーションカウンター
	int m_nPatternAnim;			//アニメーションのパターンNo.
};

#endif		// _BG_H_
