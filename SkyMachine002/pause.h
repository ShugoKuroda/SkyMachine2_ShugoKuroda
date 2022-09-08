//=============================================================================
//
// �|�[�Y��ʏ�� [pause.h]
// Author:SHUGO KURODA
//
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "object.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CObject;			// �I�u�W�F�N�g�N���X

//*****************************************************************************
// �|�[�Y��ʃN���X(�h���N���X)
//*****************************************************************************
class CPause : public CObject
{
public:
	enum TYPE
	{//�|�[�Y�̎��
		TYPE_FRAME = 0,		// �|�[�Y��ʘg
		TYPE_RESUME,		// �ĊJ
		TYPE_RESTART,		// ��蒼��
		TYPE_EXIT,			// �^�C�g���ɖ߂�
		TYPE_MAX			// �|�[�Y�̍ő吔
	};

	CPause();
	~CPause();

	static HRESULT Load(void);		//�e�N�X�`���̓ǂݍ���
	static void Unload(void);		//�e�N�X�`���̍폜
	static CPause *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

private:
	bool m_bPause;
	int m_nNumController;

	int m_nPauseSelect;					// �|�[�Y���j���[����J�ڂ���ꏊ��ۑ�

	static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];	//�e�N�X�`���̃|�C���^
	static CObject2D *m_apObject2D[TYPE_MAX];			//2D�|���S���ւ̃|�C���^
};
#endif