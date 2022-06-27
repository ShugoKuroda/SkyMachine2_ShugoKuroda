//===================================================================
//
//	�v���C���[�̏���[player.h]
//	Author:SHUGO KURODA
//
//===================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "object2D.h"

//*******************************************************************
//	�v���C���[�N���X�̒�`
//*******************************************************************
class CPlayer : public CObject2D
{
private:
	//�e�N�X�`����
	static const int MAX_TEX = 2;
	//�v���C���[�̃T�C�Y(X)
	static const float SIZE_X;
	//�v���C���[�̃T�C�Y(Y)
	static const float SIZE_Y;
	//�v���C���[�̊�{�ړ���
	static const float MOVE_DEFAULT;

public:
	//�v���C���[�̏��
	enum STATE
	{
		STATE_NORMAL = 0,	//�ʏ�
		STATE_RESPAWN,		//���G(���X�|�[��)���
		STATE_MAX
	};

	CPlayer();
	~CPlayer() override;

	//�����o�֐�
	static CPlayer *Create(const D3DXVECTOR3& pos);	//�C���X�^���X��������
	static HRESULT Load();		// �e�N�X�`���̓ǂݍ���
	static void Unload();		// �e�N�X�`���̍폜

	HRESULT Init(const D3DXVECTOR3& pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void Damage();
	void Die();

private:
	// �e�N�X�`���̃|�C���^
	static LPDIRECT3DTEXTURE9 m_pTexture[2];

	//�����o�ϐ�
	D3DXVECTOR3 m_move;				//�ړ���
	STATE m_state;					//���
	int m_nCntState;				//��ԃJ�E���^�[
	int m_nCntAttack;				//�U���J�E���^�[
	int m_nCounterAnimPlayer;		//�A�j���[�V�����J�E���^�[
	int m_nPatternAnimPlayer;		//�A�j���[�V�����̃p�^�[��No.
};

#endif //_PLAYER_H_