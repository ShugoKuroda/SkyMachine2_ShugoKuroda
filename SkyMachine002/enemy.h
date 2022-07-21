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
//	�G�N���X�̒�`
//*******************************************************************
class CEnemy : public CObject2D
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
		STATE_NORMAL = 0,		//�ʏ�
		STATE_DAMAGE,			//�_���[�W���
		STATE_MAX
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
		TYPE_RING_BOSS,		// �����O�^�̒��{�X
		TYPE_DARK_BOSS,		// ��{�X
		TYPE_MAX			// �G�̍ő吔
	};

	CEnemy();
	~CEnemy() override;

	//�����o�֐�
	static CEnemy *Create(const D3DXVECTOR3& pos, TYPE type, EnemyMove *pEnemyMove);	//�C���X�^���X��������
	static HRESULT Load();		//�e�N�X�`���̓ǂݍ���
	static void Unload();		//�e�N�X�`���̍폜

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void SetMove();
	void Damage(int nDamage);
	void State();

private:
	//�����o�ϐ�
	static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];	//�e�N�X�`���̃|�C���^
	D3DXVECTOR3 m_move;		//�ړ���
	EnemyMove *m_pMoveInfo;	//�ړ����̃|�C���^

	STATE m_state;			//���
	TYPE m_type;			//���
	int m_nLife;			//����
	int m_nCntState;		//��ԃJ�E���^�[
	int m_nCntAnim;			//�A�j���[�V�����J�E���^�[
	int m_nPatternAnim;		//�A�j���[�V�����̃p�^�[��No.
	int m_nRestTime;		//�G�̈ړ��^�C�}�[
};

#endif	// _ENEMY_H_
