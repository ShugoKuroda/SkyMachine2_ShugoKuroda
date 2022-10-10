//===================================================================
//
//	敵の処理[enemy.h]
//	Author:SHUGO KURODA
//
//===================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "object2D.h"
#include "enemy_data.h"

//*******************************************************************
//	前方宣言
//*******************************************************************
class CPlayer;

//*******************************************************************
//	敵クラスの定義
//*******************************************************************
class CEnemy : public CObject2D
{
private:
	// 幅
	static const float SIZE_WIDTH;
	// 高さ
	static const float SIZE_HEIGHT;

public:
	//敵の状態
	enum STATE
	{
		STATE_NORMAL = 0,		// 通常
		STATE_DAMAGE,			// ダメージ状態
		STATE_DIE,				// 死亡状態
		STATE_MAX
	};

	//敵の色(落とすアイテム)
	enum COLORITEM
	{
		COLOR_NONE = 0,		// なにも無し
		COLOR_RED,			// 赤(メイン弾強化)
		COLOR_BLUE,			// 青(オプション弾強化)
		COLOR_GREEN,		// 緑(バリア強化)
	};

	enum TYPE
	{//敵の種類
		TYPE_NONE = 0,		// 何もない
		TYPE_STARFISH,		// ヒトデ型の敵
		TYPE_MOSQUITO,		// 羽虫型の敵
		TYPE_SPHERE,		// 球体型の敵
		TYPE_ASSAULT,		// 突撃型の敵(バトミントンの玉みたいな敵)
		TYPE_SEAURCHIN,		// ウニ型の敵
		TYPE_ROWLING,		// 回転型の敵
		TYPE_FREEFALL,		// 自由落下型の敵
		TYPE_SHOT,			// 射撃型の敵
		TYPE_SENTRYGUN,		// 固定砲台の敵
		TYPE_FLYINGFISH,	// 水面を飛ぶ敵
		TYPE_RING_BOSS,		// リング型の中ボス
		TYPE_DARK_BOSS,		// 大ボス
		TYPE_MAX			// 敵の最大数
	};

	CEnemy();
	~CEnemy() override;

	//メンバ関数
	static CEnemy *Create(const D3DXVECTOR3& pos, TYPE type, int nLife, EnemyMove *pEnemyMove);	//インスタンス生成処理
	static HRESULT Load();		//テクスチャの読み込み
	static void Unload();		//テクスチャの削除
	static LPDIRECT3DTEXTURE9 GetTexture(TYPE type) { return m_apTexture[type]; }

	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	// 状態管理
	virtual void State();
	// 当たり判定
	virtual bool Collision(D3DXVECTOR3 posStart);
	// ダメージ処理
	virtual void Damage(int nDamage, CPlayer* pPlayer);
	// 挙動管理
	void SetAnim();
	// 移動情報の設定
	void SetMove();
	// ベクトル管理
	D3DXVECTOR3 SetVector();
	// 色の設定(落とすアイテムの種類を設定)
	void SetItemColor(COLORITEM color);

	// 種類の設定
	void SetType(TYPE type) { m_type = type; }
	// ライフの設定
	void SetLife(int nLife) { m_nLife = nLife; }
	// 移動量の設定
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	// 種類の取得
	TYPE GetType() { return m_type; }
	// ライフの取得
	int GetLife() { return m_nLife; }
	// 移動量の取得
	D3DXVECTOR3 GetMove() { return m_move; }
	// テクスチャポインタの取得
	LPDIRECT3DTEXTURE9 GetTexture(int nNum) { return m_apTexture[nNum]; }

private:
	//メンバ変数
	static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];	//テクスチャのポインタ

	D3DXVECTOR3 m_move;		//移動量
	EnemyMove *m_pMoveInfo;	//移動情報のポインタ
	STATE m_state;			//状態
	TYPE m_type;			//種類
	COLORITEM m_col;		//色(落とすアイテムの種類)
	int m_nLife;			//寿命
	int m_nCntState;		//状態カウンター
	int m_nPattern;			//パターン番号
	int m_nNumPatten;		//パターン番号の加算数
	int m_nCounter;			//様々な処理を行うためのカウンター
	int m_nCountAttack;		//攻撃を行うまでのカウンター
	int m_nRestTime;		//敵の移動タイマー
};

#endif	// _ENEMY_H_
