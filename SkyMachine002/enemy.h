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
	static const EObject TYPE = EObject::TYPE_ENEMY;

	// ����
	static const int LIFE = 50;
	//�v���C���[�̃T�C�Y(X)
	static const float SIZE;
	//�v���C���[�̊�{�ړ���
	static const float MOVE_DEFAULT;

public:
	CEnemy();
	~CEnemy() override;

	//�����o�֐�
	static CEnemy *Create(const D3DXVECTOR3& pos);	//�C���X�^���X��������
	static HRESULT Load();		//�e�N�X�`���̓ǂݍ���
	static void Unload();		//�e�N�X�`���̍폜

	HRESULT Init(const D3DXVECTOR3& pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

private:
	//�����o�ϐ�
	static LPDIRECT3DTEXTURE9 m_pTexture;	//�e�N�X�`���̃|�C���^
	D3DXVECTOR3 m_move;				//�ړ���
	int m_nLife;					//����
	int m_nCounterAnimPlayer;		//�A�j���[�V�����J�E���^�[
	int m_nPatternAnimPlayer;		//�A�j���[�V�����̃p�^�[��No.
};

#endif		// _ENEMY_H_
