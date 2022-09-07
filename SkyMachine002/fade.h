//====================================================================
//
// �t�F�[�h�̏��� (fade.h)
// Author : SHUGO KURODA
//
//====================================================================
#ifndef _FADE_H_
#define _FADE_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "renderer.h"
#include "object2D.h"

//================================================
// �N���X�錾
//================================================
// �t�F�[�h�N���X
class CFade : public CObject2D
{
public:
	enum FADETYPE
	{//�t�F�[�h�̎��
		TYPE_NONE = 0,
		TYPE_BLACK,		// ������(�{�X�O���o)
		TYPE_WHITE,		// ��
	};

public:
	CFade();
	~CFade();
	static CFade *Create(FADETYPE type);
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

private:
	FADETYPE m_type;	//�t�F�[�h�̎��
	int m_nCounter;		//�t�F�[�h�����܂ł̃J�E���^�[
};

#endif	//_FADE_H_
