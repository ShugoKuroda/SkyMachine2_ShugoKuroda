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
	enum EParent
	{//弾の生成元(所有者)
		PARENT_PLAYER1 = 0,
		PARENT_PLAYER2,
		PARENT_ENEMY,
		PARENT_MAX
	};

	enum EType
	{//弾の種類
		TYPE_PLAYER_BLUE = 0,
		TYPE_ENEMY_ORANGE,
		TYPE_ENEMY_LASER,
		TYPE_ENEMY_RED,
		TYPE_PLAYER_GREEN,
		TYPE_MAX
	};

	CBullet();
	~CBullet() override;

	//メンバ関数
	static CBullet *Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const int& nDamage, const EType type);	//インスタンス生成処理
	static HRESULT Load();		//テクスチャの読み込み
	static void Unload();		//テクスチャの削除

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetParent(EParent parent) { m_parent = parent; }
	bool Collision(D3DXVECTOR3 posStart);

private:
	//メンバ変数
	static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];	//テクスチャのポインタ
	D3DXVECTOR3 m_move;				//移動量
	int m_nDamage;					//弾のダメージ
	int m_nCntAnim;					//アニメーションカウンター
	int m_nPatternAnim;				//アニメーションのパターンNo.
	EParent m_parent;				//弾の生成元の種類
	EType m_type;					//弾の種類
};

#endif		// _BULLET_H_
