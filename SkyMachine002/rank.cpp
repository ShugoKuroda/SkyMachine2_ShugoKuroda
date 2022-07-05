//=============================================================================
//
// �����N���� [rank.cpp]
// Author : SHUGO KURODA
//
//=============================================================================
#include "rank.h"

#include "manager.h"
#include "renderer.h"

//=============================================================================
// CRank�̃R���X�g���N�^
//=============================================================================
CRank::CRank()
{

}

//=============================================================================
// CRank�̃f�X�g���N�^
//=============================================================================
CRank::~CRank()
{

}

//=============================================================================
// ��������
//=============================================================================
CRank *CRank::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXVECTOR2 texSheet, int nTex)
{
	// �|�C���^�ϐ��̐���
	CRank *Rank;
	Rank = new CRank;

	// NULL�`�F�b�N
	if (Rank != nullptr)
	{
		// ����������
		Rank->Init(pos, size, texSheet, nTex);

		// �I�u�W�F�N�g�̎�ނ�ݒ�
		//Rank->SetObjType(CScene::OBJTYPE_PLAYER_MARKER);
	}

	return Rank;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CRank::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXVECTOR2 texSheet, int nTex)
{
	//// CScene2D�̏���������
	//CObject2D::Init(pos, size);

	//// �e�N�X�`���̓\��t��
	//CTexture *pTex = CManager::GetTexture();

	//if (pTex != NULL)
	//{
	//	CObject2D::BindTexture(pTex->GetAddress(nTex));
	//}

	//// �e�N�X�`���̕����ʂ̐ݒ�
	//CScene2D::SetTexSheet(texSheet);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CRank::Uninit(void)
{
	// CScene2D�̏I������
	CObject2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CRank::Update(void)
{
	// CScene2D�̍X�V����
	CObject2D::Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CRank::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �A���t�@�e�X�g
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 120);

	// CScene2D�̕`�揈��
	CObject2D::Draw();

	// �A���t�@�e�X�g�����̐ݒ�ɖ߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);
}