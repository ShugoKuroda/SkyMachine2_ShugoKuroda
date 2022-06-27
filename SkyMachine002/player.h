//===================================================================
//
//	プレイヤーの処理[player.h]
//	Author:SHUGO KURODA
//
//===================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "object2D.h"

//*******************************************************************
//	プレイヤークラスの定義
//*******************************************************************
class CPlayer : public CObject2D
{
private:
	//テクスチャ数
	static const int MAX_TEX = 2;
	//プレイヤーのサイズ(X)
	static const float SIZE_X;
	//プレイヤーのサイズ(Y)
	static const float SIZE_Y;
	//プレイヤーの基本移動量
	static const float MOVE_DEFAULT;

public:
	//プレイヤーの状態
	enum STATE
	{
		STATE_NORMAL = 0,	//通常
		STATE_RESPAWN,		//無敵(リスポーン)状態
		STATE_MAX
	};

	CPlayer();
	~CPlayer() override;

	//メンバ関数
	static CPlayer *Create(const D3DXVECTOR3& pos);	//インスタンス生成処理
	static HRESULT Load();		// テクスチャの読み込み
	static void Unload();		// テクスチャの削除

	HRESULT Init(const D3DXVECTOR3& pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void Damage();
	void Die();

private:
	// テクスチャのポインタ
	static LPDIRECT3DTEXTURE9 m_pTexture[2];

	//メンバ変数
	D3DXVECTOR3 m_move;				//移動量
	STATE m_state;					//状態
	int m_nCntState;				//状態カウンター
	int m_nCntAttack;				//攻撃カウンター
	int m_nCounterAnimPlayer;		//アニメーションカウンター
	int m_nPatternAnimPlayer;		//アニメーションのパターンNo.
};

#endif //_PLAYER_H_