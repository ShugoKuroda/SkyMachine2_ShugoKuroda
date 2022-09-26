//===================================================================
//
//	プレイヤーの処理[player.h]
//	Author:SHUGO KURODA
//
//===================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "object2D.h"
#include "bullet_option.h"
#include "barrier.h"
#include "item.h"

//*******************************************************************
//	前方宣言
//*******************************************************************
class CLife;
class CScore;

//*******************************************************************
//	プレイヤークラスの定義
//*******************************************************************
class CPlayer : public CObject2D
{
private:
	//プレイヤーのサイズ(X)
	static const float SIZE_X;
	// プレイヤーのサイズ(Y)
	static const float SIZE_Y;
	// 登場時のサイズ(X)
	static const float ENTRY_SIZE_X;
	// 登場時のサイズ(Y)
	static const float ENTRY_SIZE_Y;
	// プレイヤーの基本移動量
	static const float MOVE_DEFAULT;
	// アニメーション間隔
	static const int ANIM_INTERVAL;
	// アニメーション最大数
	static const int MAX_ANIM;
	// U座標の最大分割数
	static const int DIVISION_U;
	// V座標の最大分割数
	static const int DIVISION_V;
	// プレイヤーのデフォルトカラー
	static const D3DXCOLOR DEFAULT_COL;
	// 最大オプション数
	static const int MAX_OPTION = 4;
	// デフォルト残機数
	static const int DEFAULT_LIFE;

public:
	// プレイヤー
	enum PLAYER
	{
		PLAYER_1 = 0,	//1P
		PLAYER_2,		//2P
		PLAYER_MAX
	};

	// メイン弾の強化状態
	enum LEVEL
	{
		LEVEL_1 = 0,	//1段階目
		LEVEL_2,		//2段階目
		LEVEL_3,		//3段階目
	};

	//状態
	enum STATE
	{
		STATE_NORMAL = 0,	//通常
		STATE_ENTRY,		//登場
		STATE_RESPAWN,		//無敵(リスポーン)状態
		STATE_DIE,			//死亡状態
		STATE_MAX
	};

	//アニメーション番号
	enum ANIMTYPE
	{
		TYPE_NEUTRAL = 0,
		TYPE_UP,
		TYPE_DOWN,
		TYPE_MAX
	};

	CPlayer();
	~CPlayer() override;

	//メンバ関数
	static CPlayer *Create(const D3DXVECTOR3& pos,const int& nNum);	//インスタンス生成処理
	static HRESULT Load();		// テクスチャの読み込み
	static void Unload();		// テクスチャの削除

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	D3DXVECTOR3 Move(D3DXVECTOR3 pos);
	void SetSpray();
	void SetAnimNum(ANIMTYPE AnimIn, ANIMTYPE AnimOut);
	void SetLevel(CItem::EType type);

	STATE GetState() { return m_state; }
	int GetBulletLevel() { return m_BulletLevel; }
	int GetOptionLevel() { return m_OptionLevel; }
	int GetBarrierLevel() { return m_BarrierLevel; }
	// スコア情報の取得
	CScore *GetScore() { return m_pScore; }
	// 死亡状態の取得
	bool GetDie() { return m_bDie; }

	void State();
	void Damage();
	void Die();

private:	//メンバ変数
	// テクスチャのポインタ
	static LPDIRECT3DTEXTURE9 m_apTexture[PLAYER_MAX];

	// 弾オプションのポインタ
	CBulletOption *m_pOption[MAX_OPTION];
	// バリアのポインタ
	CBarrier *m_pBarrier;
	// ライフのポインタ
	CLife *m_pLife;
	// スコアのポインタ
	CScore *m_pScore;

	// 弾オプションの強化段階
	CBulletOption::LEVEL m_OptionLevel;
	// バリアの強化段階
	CBarrier::LEVEL m_BarrierLevel;
	// メイン弾の強化状態
	CPlayer::LEVEL m_BulletLevel;

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
	//アニメーション遷移までの硬直カウンター
	int m_nCntAnimMove;
	//現在のアニメーションV番号
	int m_nPatterAnimV;
	//テクスチャアニメーションの種類
	ANIMTYPE m_nTexRotType;
	//プレイヤーの番号
	int m_nPlayerNum;
	//弾の発射位置
	D3DXVECTOR2 posBullet;
	//プレイヤーが操作できるかどうか
	bool m_bControl;
	//海に入ったかどうか
	bool m_bInSea;

	// 死亡したかどうか
	bool m_bDie;
};

#endif //_PLAYER_H_