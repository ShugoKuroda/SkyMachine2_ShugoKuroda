//===================================================================
//
//	�I�v�V�����e�̏���[bullet_option.h]
//	Author:SHUGO KURODA
//
//===================================================================
#ifndef _BULLET_OPTION_H_
#define _BULLET_OPTION_H_

#include "object2D.h"
#include "player.h"

//*******************************************************************
//	�e�N���X�̒�`
//*******************************************************************
class CBulletOption : public CObject2D
{
public:
	// �I�v�V�����̒��a�T�C�Y
	static const int SIZE = 30;
	// �I�v�V�����̃_���[�W��
	static const int DAMAGE = 10;

public:
	enum EColor
	{//�F�̕ω����
		FADE_NONE = 0,
		FADE_RED,
		FADE_GREEN,
		FADE_BLUE,
	};

	CBulletOption();
	~CBulletOption() override;

	//�����o�֐�
	static CBulletOption *Create(const D3DXVECTOR3& pos, const CPlayer::PLAYER& parent);		//�C���X�^���X��������
	static HRESULT Load();		//�e�N�X�`���̓ǂݍ���
	static void Unload();		//�e�N�X�`���̍폜

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	bool Collision(D3DXVECTOR3 posStart);

private:
	//�����o�ϐ�
	static LPDIRECT3DTEXTURE9 m_apTexture;		//�e�N�X�`���̃|�C���^
	float m_fRad;				//�p�x
	EColor m_col;				//�F�̎��
	int m_nDamage;				//�_���[�W��
	CPlayer::PLAYER m_parent;	//���L����v���C���[
};

#endif		// _BULLET_H_
