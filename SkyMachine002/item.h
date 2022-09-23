//===================================================================
//
//	アイテム処理[item.h]
//	Author:SHUGO KURODA
//
//===================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

#include "object2D.h"

//*******************************************************************
//	アイテムクラスの定義
//*******************************************************************
class CItem : public CObject2D
{
private:
	// サイズ(X)
	static const float SIZE_WIDTH;
	// サイズ(Y)
	static const float SIZE_HEIGHT;
	// 基本移動量
	static const float MOVE_DEFAULT;
	// アニメーション間隔
	static const int ANIM_INTERVAL;
	// アニメーション最大数
	static const int MAX_ANIM;
	// U座標の最大分割数
	static const int DIVISION_U;
	// V座標の最大分割数
	static const int DIVISION_V;

public:
	enum EType
	{//アイテムの種類
		TYPE_NONE = 0,
		TYPE_RED,
		TYPE_BLUE,
		TYPE_GREEN,
		TYPE_ORANGE,
		TYPE_MAX,
	};

	CItem();
	~CItem() override;

	//メンバ関数
	static CItem *Create(const D3DXVECTOR3& pos, const EType type);		//インスタンス生成処理
	//テクスチャの読み込み
	static HRESULT Load();
	//テクスチャの削除
	static void Unload();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//当たり判定
	bool Collision(D3DXVECTOR3 posStart);

private:
	//メンバ変数
	static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];	//テクスチャのポインタ
	D3DXVECTOR3 m_move;				//移動量
	float m_fRot;					//回転の角度
	int m_nCntAnim;					//アニメーションカウンター
	int m_nPatternAnim;				//アニメーションのパターンNo.
	EType m_type;					//アイテムの種類
};

#endif		// _ITEM_H_
