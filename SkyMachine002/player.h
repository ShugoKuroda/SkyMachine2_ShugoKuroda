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
	// アニメーション間隔
	static const int ANIM_INTERVAL;
	// アニメーション最大数
	static const int MAX_ANIM;
	// U座標の最大分割数
	static const int DIVISION_U;
	// V座標の最大分割数
	static const int DIVISION_V;

public:
	//プレイヤーの状態
	enum STATE
	{
		STATE_NORMAL = 0,	//通常
		STATE_RESPAWN,		//無敵(リスポーン)状態
		STATE_MAX
	};

	enum ROTTYPE
	{
		TYPE_NEUTRAL = 0,
		TYPE_UP,
		TYPE_DOWN,
		TYPE_MAX
	};

	CPlayer();
	~CPlayer() override;

	//メンバ関数
	static CPlayer *Create(const D3DXVECTOR3& pos);	//インスタンス生成処理
	static HRESULT Load();		// テクスチャの読み込み
	static void Unload();		// テクスチャの削除

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	D3DXVECTOR3 Move(D3DXVECTOR3 pos);
	void Damage();
	void Die();

private:	//メンバ変数
	// テクスチャのポインタ
	static LPDIRECT3DTEXTURE9 m_pTexture[2];

	//移動量
	D3DXVECTOR3 m_move;
	//状態
	STATE m_state;
	//状態カウンター
	int m_nCntState;
	//攻撃カウンター
	int m_nCntAttack;
	//アニメーションカウンター
	int m_nCntAnim;
	//アニメーションのパターンNo.
	int m_nPatternAnim;
	// 現在のアニメーションV番号
	int m_nPatterAnimV;
	//テクスチャアニメーションの種類
	ROTTYPE m_nTexRotType;
};

#endif //_PLAYER_H_