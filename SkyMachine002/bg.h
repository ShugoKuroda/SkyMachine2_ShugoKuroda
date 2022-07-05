//===================================================================
//
//	�w�i����[bg.h]
//	Author:SHUGO KURODA
//
//===================================================================
#ifndef _BG_H_
#define _BG_H_

#include "object.h"
#include "object2D.h"

//*******************************************************************
//	�w�i�N���X�̒�`
//*******************************************************************
class CBg : public CObject
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

	//�ő�w�i��
	static const int MAX_BG = 5;
public:
	// �w�i�̃Z�b�g
	enum SET
	{
		SET_NONE = 0,	// �Ȃ�
		SET_a,			// ��
		SET_b,			// ��
		SET_r,			// ��
		SET_MAX			// �Z�b�g�̍ő吔
	};

	enum EType
	{//�w�i�̎��
		TYPE_PLAYER = 0,
		TYPE_ENEMY,
		TYPE_MAX
	};

	CBg();
	~CBg() override;

	//�����o�֐�
	static CBg *Create(SET set);	//�C���X�^���X��������
	static HRESULT Load();		//�e�N�X�`���̓ǂݍ���
	static void Unload();		//�e�N�X�`���̍폜

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

private:
	//�����o�ϐ�
	static LPDIRECT3DTEXTURE9 m_pTexture[TYPE_MAX];		//�e�N�X�`���̃|�C���^
	CObject2D *m_apObject2D[MAX_BG];					// 2D�|���S���ւ̃|�C���^
	D3DXVECTOR3 m_move;			//�ړ���
	int m_nLife;				//����
	int m_nCntAnim;				//�A�j���[�V�����J�E���^�[
	int m_nPatternAnim;			//�A�j���[�V�����̃p�^�[��No.
};

#endif		// _BG_H_
