//===================================================================
//
//	�G�{�X�̏���[enemy_boss.h]
//	Author:SHUGO KURODA
//
//===================================================================
#ifndef _ENEMY_BOSS_H_
#define _ENEMY_BOSS_H_

#include "enemy.h"

//*******************************************************************
//	�O���錾
//*******************************************************************
class CPlayer;

//*******************************************************************
//	�G�N���X�̒�`
//*******************************************************************
class CEnemyBoss : public CEnemy
{
public:		//�萔
	// ��
	static const float SIZE_WIDTH;
	// ����
	static const float SIZE_HEIGHT;
	// U���W�̍ő啪����
	static const int DIVISION_U;
	// V���W�̍ő啪����
	static const int DIVISION_V;
private:
	// ����
	static const int LIFE;
	// �U���O�̗\������Ԋu
	static const int RUSH_OPERATION;

public:
	//���
	enum STATE
	{
		STATE_NORMAL = 0,	//�ʏ�
		STATE_DAMAGE,		//�_���[�W���
		STATE_MAX
	};

	//�s���p�^�[��
	enum PATTERN
	{
		PATTERN_ENTRY = 0,	//�o��
		PATTERN_NORMAL,		//�ʏ�
		PATTERN_RUSH,		//�A���ːi�U��
		PATTERN_CREATE,		//�G���G�̐����U��
		PATTERN_BARRAGE,	//�e���U��
		PATTERN_DIE,		//���S
		PATTERN_MAX
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
	void Damage(int nDamage, CPlayer* pPlayer) override;
	void State() override;
	void SetAnim() override;
	bool Pattern(D3DXVECTOR3& pos, D3DXVECTOR2& size, D3DXVECTOR3& move);
	void ChangeSize(D3DXVECTOR2 *pSize, const float& fSize);
	void StateReset();

	PATTERN GetPattern() { return m_pattern; }

private:
	D3DXVECTOR3 m_PosOld;	//�O��̈ʒu
	PATTERN m_pattern;		//�s���p�^�[��
	int m_nCounter;			//�s���ω��܂ł̃J�E���^�[
	int m_nCountOperation;	//�ːi�O�̗\������̊Ԋu
	float m_fAttackRot;		//�ːi�U���A�e���˂̊p�x
	bool m_bSizeChange;		//�傫���̕ύX
	int m_nCountObject;		//�I�u�W�F�N�g�𐶐�������
};

#endif	// _ENEMY_BOSS_H_
