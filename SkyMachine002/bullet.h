//===================================================================
//
//	�e�̏���[bullet.h]
//	Author:SHUGO KURODA
//
//===================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "object2D.h"

//*******************************************************************
//	�e�N���X�̒�`
//*******************************************************************
class CBullet : public CObject2D
{
private:
	// ����
	static const int LIFE = 50;
	//�v���C���[�̃T�C�Y(X)
	static const float SIZE_WIDTH;
	//�v���C���[�̃T�C�Y(Y)
	static const float SIZE_HEIGHT;
	//�v���C���[�̊�{�ړ���
	static const float MOVE_DEFAULT;

public:
	CBullet();
	~CBullet() override;

	//�����o�֐�
	static CBullet *Create(const D3DXVECTOR3& pos);	//�C���X�^���X��������
	static HRESULT Load();		//�e�N�X�`���̓ǂݍ���
	static void Unload();		//�e�N�X�`���̍폜

	HRESULT Init(const D3DXVECTOR3& pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	bool Collision(D3DXVECTOR3 posStart, D3DXVECTOR3 posEnd, int StartLength, int EndLength);

private:
	//�����o�ϐ�
	static LPDIRECT3DTEXTURE9 m_pTexture;	//�e�N�X�`���̃|�C���^
	D3DXVECTOR3 m_move;				//�ړ���
	int m_nLife;					//����
	int m_nCounterAnimPlayer;		//�A�j���[�V�����J�E���^�[
	int m_nPatternAnimPlayer;		//�A�j���[�V�����̃p�^�[��No.
};

#endif		// _BULLET_H_
