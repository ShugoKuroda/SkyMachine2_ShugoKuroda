//=============================================================================
//
// �I�u�W�F�N�g����[object.h]
// Author : SHUGO KURODA
//
//=============================================================================
#ifndef _OBJECT_2D_
#define _OBJECT_2D_

#include "main.h"
#include "object.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CObject2D : public CObject
{
public:
	CObject2D();
	//CObject2D(CObject2D* pObject);
	~CObject2D() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	static CObject2D *Create();

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

	// ���_�t�H�[�}�b�g
	static const DWORD FVF_VERTEX_2D = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	static int m_RandPos;
};

#endif // _OBJECT_2D_
