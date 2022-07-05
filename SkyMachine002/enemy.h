//===================================================================
//
//	弾の処理[bullet.h]
//	Author:SHUGO KURODA
//
//===================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "object2D.h"

//*******************************************************************
//	敵クラスの定義
//*******************************************************************
class CEnemy : public CObject2D
{
private:
	// 寿命
	static const int LIFE;
	// 幅
	static const float SIZE_WIDTH;
	// 高さ
	static const float SIZE_HEIGHT;

public:
	//敵の状態
	typedef enum
	{
		STATE_NORMAL = 0,		//通常
		STATE_DAMAGE,			//ダメージ状態
		STATE_MAX
	}STATE;

	CEnemy();
	~CEnemy() override;

	//メンバ関数
	static CEnemy *Create(const D3DXVECTOR3& pos);	//インスタンス生成処理
	static HRESULT Load();		//テクスチャの読み込み
	static void Unload();		//テクスチャの削除

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void Damage();
	void SetMove(D3DXVECTOR3 move) { m_move = move; }

private:
	//メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャのポインタ
	D3DXVECTOR3 m_move;				//移動量
	int m_nLife;					//寿命
	int m_nCounterAnimPlayer;		//アニメーションカウンター
	int m_nPatternAnimPlayer;		//アニメーションのパターンNo.
};

#endif		// _ENEMY_H_
