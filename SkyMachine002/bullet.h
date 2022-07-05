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
	// �A�j���[�V�����Ԋu
	static const int ANIM_INTERVAL;
	// �A�j���[�V�����ő吔
	static const int MAX_ANIM;
	// U���W�̍ő啪����
	static const int DIVISION_U;
	// V���W�̍ő啪����
	static const int DIVISION_V;

public:
	enum EType
	{//�e�̎��(�������̎��)
		TYPE_PLAYER = 0,
		TYPE_ENEMY,
		TYPE_MAX
	};

	CBullet();
	~CBullet() override;

	//�����o�֐�
	static CBullet *Create(const D3DXVECTOR3& pos);	//�C���X�^���X��������
	static HRESULT Load();		//�e�N�X�`���̓ǂݍ���
	static void Unload();		//�e�N�X�`���̍폜

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetType(EType type) { m_nType = type; }
	bool CollisionSphere(D3DXVECTOR3 posStart);

private:
	//�����o�ϐ�
	static LPDIRECT3DTEXTURE9 m_pTexture;	//�e�N�X�`���̃|�C���^
	D3DXVECTOR3 m_move;				//�ړ���
	int m_nLife;					//����
	int m_nCntAnim;					//�A�j���[�V�����J�E���^�[
	int m_nPatternAnim;				//�A�j���[�V�����̃p�^�[��No.
	EType m_nType;					//�e�̐������̎��
};

#endif		// _BULLET_H_
