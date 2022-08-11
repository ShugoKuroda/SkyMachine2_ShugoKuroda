//===================================================================
//
//	敵の処理[enemy.h]
//	Author:SHUGO KURODA
//
//===================================================================
#ifndef _ENEMY_BOSS_H_
#define _ENEMY_BOSS_H_

#include "enemy.h"

//*******************************************************************
//	敵クラスの定義
//*******************************************************************
class CEnemyBoss : public CEnemy
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
	enum STATE
	{
		STATE_NORMAL = 0,	//通常
		STATE_DAMAGE,		//ダメージ状態
		STATE_DIE,
		STATE_MAX
	};

	CEnemyBoss();
	~CEnemyBoss() override;

	//メンバ関数
	static CEnemyBoss *Create(const D3DXVECTOR3& pos, CEnemy::TYPE type);	//インスタンス生成処理

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	bool Collision(D3DXVECTOR3 posStart) override;
	void Damage(int nDamage) override;
	void State() override;
	void SetAnim() override;

private:
};

#endif	// _ENEMY_BOSS_H_
