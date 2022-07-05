//===================================================================
//
//	�e�̏���[bullet.h]
//	Author:SHUGO KURODA
//
//===================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "object2D.h"

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
	typedef enum
	{
		STATE_NORMAL = 0,		//�ʏ�
		STATE_DAMAGE,			//�_���[�W���
		STATE_MAX
	}STATE;

	CEnemy();
	~CEnemy() override;

	//�����o�֐�
	static CEnemy *Create(const D3DXVECTOR3& pos);	//�C���X�^���X��������
	static HRESULT Load();		//�e�N�X�`���̓ǂݍ���
	static void Unload();		//�e�N�X�`���̍폜

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void Damage();
	void SetMove(D3DXVECTOR3 move) { m_move = move; }

private:
	//�����o�ϐ�
	static LPDIRECT3DTEXTURE9 m_pTexture;	//�e�N�X�`���̃|�C���^
	D3DXVECTOR3 m_move;				//�ړ���
	int m_nLife;					//����
	int m_nCounterAnimPlayer;		//�A�j���[�V�����J�E���^�[
	int m_nPatternAnimPlayer;		//�A�j���[�V�����̃p�^�[��No.
};

#endif		// _ENEMY_H_
