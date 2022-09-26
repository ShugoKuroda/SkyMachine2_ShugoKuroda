//=============================================================================
//
// �Q�[����ʏ��� [game.h]
// Author : SHUGO KURODA
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "base.h"
#include "cloud_data.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CObject2D;

//-----------------------------------------------------------------------------
// �x�[�X�N���X(�h���N���X)
//-----------------------------------------------------------------------------
class CTitle : public CBase
{
public:
	enum TITLE_OBJ
	{//�w�i�̎��
		BG_SKY = 0,		// �w�i�̋�
		LOGO_PLAYER,	// �v���C���[���S
		LOGO_TITLE,		// �^�C�g�����S
		LOGO_PRESS,		// PRESS���S
		BG_FADE,		// ��ʂ𔖈Â�����p
		LOGO_TUTORIAL,	// �`���[�g���A��
		LOGO_SHADOW,	// �v���C���[���S(��)
		OBJ_MAX			// �w�i�̍ő吔
	};

public:
	CTitle();
	~CTitle() override;

	//�����o�֐�
	static HRESULT Load();		//�e�N�X�`���̓ǂݍ���
	static void Unload();		//�e�N�X�`���̍폜

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void CreateCloud();

private:
	//�����o�ϐ�
	static LPDIRECT3DTEXTURE9 m_apTexture[OBJ_MAX];	//�e�N�X�`���̃|�C���^
	CObject2D *m_apObject2D[OBJ_MAX - 1];
	CloudInfo m_CloudInfo;
	int m_nCountMoveBg;
	bool m_bTitleDraw;
	bool m_bPressFade;
	int m_nCntLoop;		// �^�C�g�����o�����[�v������܂ł̎���
	bool m_bEntry[2];
	bool m_bTutorial;
	D3DXVECTOR3 m_move;		// �ړ���
};

#endif		// _TITLE_H_
