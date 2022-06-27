//=============================================================================
//
// �I�u�W�F�N�g����[object.h]
// Author : SHUGO KURODA
//
//=============================================================================
#ifndef _OBJECT_2D_
#define _OBJECT_2D_

#include "object.h"
#include "renderer.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CObject2D : public CObject
{
public:
	// ���_�t�H�[�}�b�g
	static const DWORD FVF_VERTEX_2D = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	// ���_�f�[�^
	struct VERTEX_2D
	{
		D3DXVECTOR3 pos;
		float rhw;
		D3DCOLOR col;
		D3DXVECTOR2 tex;
	};

public:
	CObject2D();
	//CObject2D(CObject2D* pObject);
	virtual ~CObject2D() override;

	virtual HRESULT Init(const D3DXVECTOR3& pos) override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;
	void SetPosition(const D3DXVECTOR3& pos) override;
	void SetSize(float fSizeX, float fSizeY) override;
	void SetRot(float fRot) override;

	void SetVertex();
	void SetColor(D3DXCOLOR col);
	void SetAnimation(int nAnimU, int nAnimV, int nPartU, int nPartV);

	//static CObject2D *Create();

	// �ʒu�̎擾
	const D3DXVECTOR3& GetPosition() const override;
	// �����̎擾
	const float& GetRot() const;

protected:
	// �e�N�X�`���̐ݒ�
	void BindTexture(LPDIRECT3DTEXTURE9 Texture);

private:	//�����o�ϐ�
	//���_�o�b�t�@�ւ̃|�C���^�[
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	//�e�N�X�`���ւ̃|�C���^�[
	LPDIRECT3DTEXTURE9 m_pTexture;
	//�ʒu
	D3DXVECTOR3 m_pos;
	//����
	float m_fRot;
	//�Ίp���̒���
	float m_fLength;
	//�Ίp���̊p�x
	float m_fAngle;
	//���_�J���[�̐ݒ�
	D3DXCOLOR m_col;
};

#endif // _OBJECT_2D_
