//=============================================================================
//
// �X�R�A�w�b�_
// Author : 
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

//�O���錾
class CNumber;

//*****************************************************************************
// �w�i�N���X��`
//*****************************************************************************
class CScore : public CObject
{
// �萔��` //
	static const int SCORE_SIZE = 30;
	static const int MAX_SCORE = 8;

// �����o�֐� //
public:
	CScore();										//�R���X�g���N�^
	~CScore() override;								//�f�X�g���N�^

	static CScore *Create();						//����
	HRESULT Init() override;						//������
	void Uninit() override;							//�I��
	void Update() override;							//�X�V
	void Draw() override;							//�`��
	void Add(const int& nScore);					//�X�R�A�̉��Z

private:
	void Set();										//�X�R�A�̐ݒ�

// �����o�ϐ� //
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = nullptr;	//���_�o�b�t�@�̃|�C���^
	static LPDIRECT3DTEXTURE9 m_pTexture;			//�e�N�X�`���̃|�C���^
	CNumber *pNumber[MAX_SCORE];					//�X�R�A�Ɏg�p���鐔��
	int m_nScore;									//�X�R�A���i�[
};

#endif // !_SCORE_H_