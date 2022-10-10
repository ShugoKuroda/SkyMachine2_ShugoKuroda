//===================================================================
//
//	�G�̏���[enemy.h]
//	Author:SHUGO KURODA
//
//===================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "object2D.h"
#include "enemy_data.h"

//*******************************************************************
//	�O���錾
//*******************************************************************
class CPlayer;

//*******************************************************************
//	�G�N���X�̒�`
//*******************************************************************
class CEnemy : public CObject2D
{
private:
	// ��
	static const float SIZE_WIDTH;
	// ����
	static const float SIZE_HEIGHT;

public:
	//�G�̏��
	enum STATE
	{
		STATE_NORMAL = 0,		// �ʏ�
		STATE_DAMAGE,			// �_���[�W���
		STATE_DIE,				// ���S���
		STATE_MAX
	};

	//�G�̐F(���Ƃ��A�C�e��)
	enum COLORITEM
	{
		COLOR_NONE = 0,		// �Ȃɂ�����
		COLOR_RED,			// ��(���C���e����)
		COLOR_BLUE,			// ��(�I�v�V�����e����)
		COLOR_GREEN,		// ��(�o���A����)
	};

	enum TYPE
	{//�G�̎��
		TYPE_NONE = 0,		// �����Ȃ�
		TYPE_STARFISH,		// �q�g�f�^�̓G
		TYPE_MOSQUITO,		// �H���^�̓G
		TYPE_SPHERE,		// ���̌^�̓G
		TYPE_ASSAULT,		// �ˌ��^�̓G(�o�g�~���g���̋ʂ݂����ȓG)
		TYPE_SEAURCHIN,		// �E�j�^�̓G
		TYPE_ROWLING,		// ��]�^�̓G
		TYPE_FREEFALL,		// ���R�����^�̓G
		TYPE_SHOT,			// �ˌ��^�̓G
		TYPE_SENTRYGUN,		// �Œ�C��̓G
		TYPE_FLYINGFISH,	// ���ʂ��ԓG
		TYPE_RING_BOSS,		// �����O�^�̒��{�X
		TYPE_DARK_BOSS,		// ��{�X
		TYPE_MAX			// �G�̍ő吔
	};

	CEnemy();
	~CEnemy() override;

	//�����o�֐�
	static CEnemy *Create(const D3DXVECTOR3& pos, TYPE type, int nLife, EnemyMove *pEnemyMove);	//�C���X�^���X��������
	static HRESULT Load();		//�e�N�X�`���̓ǂݍ���
	static void Unload();		//�e�N�X�`���̍폜
	static LPDIRECT3DTEXTURE9 GetTexture(TYPE type) { return m_apTexture[type]; }

	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	// ��ԊǗ�
	virtual void State();
	// �����蔻��
	virtual bool Collision(D3DXVECTOR3 posStart);
	// �_���[�W����
	virtual void Damage(int nDamage, CPlayer* pPlayer);
	// �����Ǘ�
	void SetAnim();
	// �ړ����̐ݒ�
	void SetMove();
	// �x�N�g���Ǘ�
	D3DXVECTOR3 SetVector();
	// �F�̐ݒ�(���Ƃ��A�C�e���̎�ނ�ݒ�)
	void SetItemColor(COLORITEM color);

	// ��ނ̐ݒ�
	void SetType(TYPE type) { m_type = type; }
	// ���C�t�̐ݒ�
	void SetLife(int nLife) { m_nLife = nLife; }
	// �ړ��ʂ̐ݒ�
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	// ��ނ̎擾
	TYPE GetType() { return m_type; }
	// ���C�t�̎擾
	int GetLife() { return m_nLife; }
	// �ړ��ʂ̎擾
	D3DXVECTOR3 GetMove() { return m_move; }
	// �e�N�X�`���|�C���^�̎擾
	LPDIRECT3DTEXTURE9 GetTexture(int nNum) { return m_apTexture[nNum]; }

private:
	//�����o�ϐ�
	static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];	//�e�N�X�`���̃|�C���^

	D3DXVECTOR3 m_move;		//�ړ���
	EnemyMove *m_pMoveInfo;	//�ړ����̃|�C���^
	STATE m_state;			//���
	TYPE m_type;			//���
	COLORITEM m_col;		//�F(���Ƃ��A�C�e���̎��)
	int m_nLife;			//����
	int m_nCntState;		//��ԃJ�E���^�[
	int m_nPattern;			//�p�^�[���ԍ�
	int m_nNumPatten;		//�p�^�[���ԍ��̉��Z��
	int m_nCounter;			//�l�X�ȏ������s�����߂̃J�E���^�[
	int m_nCountAttack;		//�U�����s���܂ł̃J�E���^�[
	int m_nRestTime;		//�G�̈ړ��^�C�}�[
};

#endif	// _ENEMY_H_
