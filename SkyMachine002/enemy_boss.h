//===================================================================
//
//	�G�̏���[enemy.h]
//	Author:SHUGO KURODA
//
//===================================================================
#ifndef _ENEMY_BOSS_H_
#define _ENEMY_BOSS_H_

#include "enemy.h"

//*******************************************************************
//	�G�N���X�̒�`
//*******************************************************************
class CEnemyBoss : public CEnemy
{
private:
	// ����
	static const int LIFE;
	// ��
	static const float SIZE_WIDTH;
	// ����
	static const float SIZE_HEIGHT;

public:
	//�G�̏��
	enum STATE
	{
		STATE_NORMAL = 0,	//�ʏ�
		STATE_DAMAGE,		//�_���[�W���
		STATE_DIE,
		STATE_MAX
	};

	CEnemyBoss();
	~CEnemyBoss() override;

	//�����o�֐�
	static CEnemyBoss *Create(const D3DXVECTOR3& pos, CEnemy::TYPE type);	//�C���X�^���X��������

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
