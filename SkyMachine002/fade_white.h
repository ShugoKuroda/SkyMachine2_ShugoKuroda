//====================================================================
//
// ���t�F�[�h�̏��� (whiteFade.h)
// Author : �㓡�T�V��
//
//====================================================================
#ifndef _FADE_WHITE_H_
#define _FADE_WHITE_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "renderer.h"
#include "object2D.h"

//================================================
// �N���X�錾
//================================================
// ���t�F�[�h�N���X
class CFadeWhite : public CObject2D
{
public:
	CFadeWhite();
	~CFadeWhite();
	static CFadeWhite *Create();
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};

#endif	//_FADE_WHITE_H_
