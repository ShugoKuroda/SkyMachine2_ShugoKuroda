//===================================================================
//
//	弾の処理[bullet.h]
//	Author:SHUGO KURODA
//
//===================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "object2D.h"

//*******************************************************************
//	弾クラスの定義
//*******************************************************************
class CBullet : public CObject2D
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

public:
	enum EType
	{//弾の種類(生成元の種類)
		TYPE_PLAYER = 0,
		TYPE_ENEMY,
		TYPE_MAX
	};

	CBullet();
	~CBullet() override;

	//メンバ関数
	static CBullet *Create(const D3DXVECTOR3& pos);	//インスタンス生成処理
	static HRESULT Load();		//テクスチャの読み込み
	static void Unload();		//テクスチャの削除

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetType(EType type) { m_nType = type; }
	bool CollisionSphere(D3DXVECTOR3 posStart);

private:
	//メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャのポインタ
	D3DXVECTOR3 m_move;				//移動量
	int m_nLife;					//寿命
	int m_nCntAnim;					//アニメーションカウンター
	int m_nPatternAnim;				//アニメーションのパターンNo.
	EType m_nType;					//弾の生成元の種類
};

#endif		// _BULLET_H_
