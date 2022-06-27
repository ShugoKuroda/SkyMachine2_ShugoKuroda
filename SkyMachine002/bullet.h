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

public:
	CBullet();
	~CBullet() override;

	//メンバ関数
	static CBullet *Create(const D3DXVECTOR3& pos);	//インスタンス生成処理
	static HRESULT Load();		//テクスチャの読み込み
	static void Unload();		//テクスチャの削除

	HRESULT Init(const D3DXVECTOR3& pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	bool Collision(D3DXVECTOR3 posStart, D3DXVECTOR3 posEnd, int StartLength, int EndLength);

private:
	//メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャのポインタ
	D3DXVECTOR3 m_move;				//移動量
	int m_nLife;					//寿命
	int m_nCounterAnimPlayer;		//アニメーションカウンター
	int m_nPatternAnimPlayer;		//アニメーションのパターンNo.
};

#endif		// _BULLET_H_
