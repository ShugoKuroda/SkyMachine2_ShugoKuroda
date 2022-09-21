//===================================================================
//
//	�I�v�V�����e�̏���[bullet_option.h]
//	Author:SHUGO KURODA
//
//===================================================================
#ifndef _BULLET_OPTION_H_
#define _BULLET_OPTION_H_

#include "object2D.h"

//*******************************************************************
//	�e�N���X�̒�`
//*******************************************************************
class CBulletOption : public CObject2D
{
public:
	// �I�v�V����(��)�̃T�C�Y
	static const int SIZE_SMALL = 30;
	// �I�v�V����(��)�̃T�C�Y
	static const int SIZE_MEDIUM = 60;
	// �I�v�V����(��)�̃T�C�Y
	static const int SIZE_LARGE = 90;
	// �I�v�V����(��)�̃_���[�W��
	static const int DAMAGE_SMALL = 10;
	// �I�v�V����(��)�̃_���[�W��
	static const int DAMAGE_MEDIUM = 20;
	// �I�v�V����(��)�̃_���[�W��
	static const int DAMAGE_LARGE = 30;

public:
	enum EType
	{//�I�v�V�����̎��
		TYPE_NONE = 0,
		TYPE_SMALL,
		TYPE_MEDIUM,
		TYPE_LARGE,
		TYPE_MAX
	};

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
	static CBulletOption *Create(const D3DXVECTOR3& pos, const int& nSize, const int& nDamage, const EType& type);		//�C���X�^���X��������
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
	float m_fRad;			//�p�x
	EType m_type;			//�e�̎��
	EColor m_col;			//�F�̎��
	int m_nDamage;			//�_���[�W��
};

#endif		// _BULLET_H_
