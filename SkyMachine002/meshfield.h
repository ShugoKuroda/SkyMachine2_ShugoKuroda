//===================================================================
//
//	���b�V���t�B�[���h�����̃w�b�_�[[meshfield.h]
//	Author:SHUGO KURODA
//
//===================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "object.h"

//-------------------------------------------
// ���b�V���t�B�[���h�N���X
//-------------------------------------------
class CMeshField : public CObject
{
public:
	// �R���X�g���N�^
	CMeshField();
	// �f�X�g���N�^
	~CMeshField() override;

	// ����
	static CMeshField *Create();
	// �e�N�X�`���̓ǂݍ���
	static HRESULT Load();
	// �e�N�X�`���̍폜
	static void Unload();

	// ������
	HRESULT Init() override;
	// �I��
	void Uninit() override;
	// �X�V
	void Update() override;
	// �`��
	void Draw() override;

	// �F�ω�
	void ChangeCol();
	// �A�j���[�V����
	void Animation();
	// ���_���W�̍X�V
	void UpdateVtx();

	// �ړ��ʂ̐ݒ�
	void SetMove(const D3DXVECTOR3& move) { m_move = move; }
	// �I���t���O�̐ݒ�
	void SetUninit(bool bUninit) { m_bUninit = bUninit; }

private:
	// �e�N�X�`���̃|�C���^
	static LPDIRECT3DTEXTURE9 m_pTexture;	// �e�N�X�`���̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;		// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_move;		// �ړ���(��ʃV�F�C�N�p)
	D3DXCOLOR m_col;		// �F
	int m_nCounterAnim;		// �A�j���[�V�����p�J�E���^�[
	int m_nPatternAnim;		// ���݂̃A�j���[�V�����ԍ�
	bool m_bCol;			// �F�ύX�t���O
	bool m_bUninit;			// �I�����邩�ǂ���
};

#endif		// _MESHFIELD_H_
