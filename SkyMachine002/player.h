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
	// �A�j���[�V�����Ԋu
	static const int ANIM_INTERVAL;
	// �A�j���[�V�����ő吔
	static const int MAX_ANIM;
	// U���W�̍ő啪����
	static const int DIVISION_U;
	// V���W�̍ő啪����
	static const int DIVISION_V;
	//�v���C���[�̃f�t�H���g�J���[
	static const D3DXCOLOR DEFAULT_COL;

public:
	//���
	enum STATE
	{
		STATE_NORMAL = 0,	//�ʏ�
		STATE_RESPAWN,		//���G(���X�|�[��)���
		STATE_DIE,			//���S���
		STATE_MAX
	};

	//�A�j���[�V�����ԍ�
	enum ANIMTYPE
	{
		TYPE_NEUTRAL = 0,
		TYPE_UP,
		TYPE_DOWN,
		TYPE_MAX
	};

	CPlayer();
	~CPlayer() override;

	//�����o�֐�
	static CPlayer *Create(const D3DXVECTOR3& pos);	//�C���X�^���X��������
	static HRESULT Load();		// �e�N�X�`���̓ǂݍ���
	static void Unload();		// �e�N�X�`���̍폜

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	D3DXVECTOR3 Move(D3DXVECTOR3 pos);
	void SetSpray();
	void SetAnimNum(ANIMTYPE AnimIn, ANIMTYPE AnimOut);

	STATE GetState() { return m_state; }

	void State();
	void Damage(int nDamage);
	void Die();

private:	//�����o�ϐ�
	// �e�N�X�`���̃|�C���^
	static LPDIRECT3DTEXTURE9 m_apTexture[2];

	//�ړ���
	D3DXVECTOR3 m_move;
	//���
	STATE m_state;
	//��ԃJ�E���^�[
	int m_nCntState;
	//�U���J�E���^�[
	int m_nCntAttack;
	//�A�j���[�V�����J�E���^�[
	int m_nCntAnim;
	//�A�j���[�V�����̃p�^�[��No.
	int m_nPatternAnim;
	//�A�j���[�V�����J�ڂ܂ł̍d���J�E���^�[
	int m_nCntAnimMove;
	//���݂̃A�j���[�V����V�ԍ�
	int m_nPatterAnimV;
	//�e�N�X�`���A�j���[�V�����̎��
	ANIMTYPE m_nTexRotType;
	//�v���C���[�̔ԍ�
	int m_nPlayerNum;
	//�e�̔��ˈʒu
	D3DXVECTOR2 posBullet;
	//�v���C���[������ł��邩�ǂ���
	bool m_bControl;
	//�C�ɓ��������ǂ���
	bool m_bInSea;
};

#endif //_PLAYER_H_