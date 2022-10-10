//===================================================================
//
//	背景処理[bg.h]
//	Author:SHUGO KURODA
//
//===================================================================
#ifndef _BG_H_
#define _BG_H_

#include "object.h"

//*******************************************************************
// 前方宣言
//*******************************************************************
class CObject2D;

//*******************************************************************
//	背景クラスの定義
//*******************************************************************
class CBg : public CObject
{
public:
	// 背景のセット
	enum ESet
	{
		SET_NONE = 0,
		SET_A,			// ZONE_A
		SET_B,			// ZONE_B
		SET_C,			// ZONE_C
		SET_MAX			// セットの最大数
	};

	enum EZone_A
	{//背景の種類
		BG_A_SKY = 0,		// 空
		BG_A_UNDERWATER,	// 海中
		BG_A_SEA,			// 海面
		BG_A_WAVE1,			// 波1
		BG_A_WAVE2,			// 波2
		BG_A_WAVE3,			// 波3
		BG_A_ROCK,			// 岩
		BG_A_SETWEED,		// 海藻
		BG_A_BOSS,			// ボス戦の背景
		BG_A_FLOOR,			// 海中の床
		BG_A_SETWEED2,		// 海藻
		BG_A_FADEBLACK,		// 背景フェード用
		BG_A_SEA_OTHER,		// 海の裏側(海中からみた海面)
		BG_A_MAX			// 背景の最大数
	};

	CBg();
	~CBg() override;

	//メンバ関数
	static HRESULT Load(void);		//テクスチャの読み込み
	static void Unload(void);		//テクスチャの削除
	static CBg *Create(ESet set);	//インスタンス生成処理
	static CObject2D *GetObjectBg(EZone_A index) { return m_apObject2D[index]; }

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	// 背景のフェード処理
	void FadeBack();
	// 背景全体のアニメーション処理
	void AnimationBgAll();

private:
	//メンバ変数
	static LPDIRECT3DTEXTURE9 m_apTexture[BG_A_MAX];	//テクスチャのポインタ
	static CObject2D *m_apObject2D[BG_A_MAX];			//2Dポリゴンへのポインタ
	ESet m_set;					//描画する背景(ZONE)
	int m_nCntBgChange;			//次の背景が描画されるまでのカウンター
};

#endif		// _BG_H_
