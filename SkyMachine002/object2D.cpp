//=============================================================================
//
// �I�u�W�F�N�g����[object.cpp]
// Author : SHUGO KURODA
//
//=============================================================================
#include "object2D.h"

#include "manager.h"
#include "renderer.h"

#include <assert.h>

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CObject2D::CObject2D()
	: m_pTexture(nullptr), m_pVtxBuff(nullptr), m_pos(0.0f, 0.0f, 0.0f), m_fRot(0.0f), m_fLength(0.0f), m_fAngle(0.0f)
{
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CObject2D::~CObject2D()
{
}

//-----------------------------------------------------------------------------
// ����������
//-----------------------------------------------------------------------------
HRESULT CObject2D::Init()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos.x = m_pos.x + sinf(m_fRot + (-D3DX_PI + m_fAngle)) * m_fLength;
	pVtx[0].pos.y = m_pos.y + cosf(m_fRot + (-D3DX_PI + m_fAngle)) * m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_pos.x + sinf(m_fRot + (D3DX_PI - m_fAngle)) * m_fLength;
	pVtx[1].pos.y = m_pos.y + cosf(m_fRot + (D3DX_PI - m_fAngle)) * m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x + sinf(m_fRot + (0.0f - m_fAngle)) * m_fLength;
	pVtx[2].pos.y = m_pos.y + cosf(m_fRot + (0.0f - m_fAngle)) * m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + sinf(m_fRot + (0.0f + m_fAngle)) * m_fLength;
	pVtx[3].pos.y = m_pos.y + cosf(m_fRot + (0.0f + m_fAngle)) * m_fLength;
	pVtx[3].pos.z = 0.0f;

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
// �I������
//-----------------------------------------------------------------------------
void CObject2D::Uninit()
{
	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// �I�u�W�F�N�g�̔j��
	Release();
}

//-----------------------------------------------------------------------------
// �X�V����
//-----------------------------------------------------------------------------
void CObject2D::Update()
{
}

//-----------------------------------------------------------------------------
// �`�揈��
//-----------------------------------------------------------------------------
void CObject2D::Draw()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//-----------------------------------------------------------------------------
// �T�C�Y�̐ݒ�
//-----------------------------------------------------------------------------
void CObject2D::SetSize(D3DXVECTOR2 size)
{
	m_size = size;
	//�Ίp���̒������Z�o
	m_fLength = sqrtf(size.x * size.x + size.y * size.y) / 2.0f;
	//�Ίp���̊p�x���Z�o����
	m_fAngle = atan2f(size.x, size.y);
}

//-----------------------------------------------------------------------------
// ���_���W�̐ݒ�
//-----------------------------------------------------------------------------
void CObject2D::SetVertex()
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�̐ݒ�
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos.x = m_pos.x + sinf(m_fRot + (-D3DX_PI + m_fAngle))*m_fLength;
	pVtx[0].pos.y = m_pos.y + cosf(m_fRot + (-D3DX_PI + m_fAngle))*m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_pos.x + sinf(m_fRot + (D3DX_PI - m_fAngle))*m_fLength;
	pVtx[1].pos.y = m_pos.y + cosf(m_fRot + (D3DX_PI - m_fAngle))*m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x + sinf(m_fRot + (0.0f - m_fAngle))*m_fLength;
	pVtx[2].pos.y = m_pos.y + cosf(m_fRot + (0.0f - m_fAngle))*m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + sinf(m_fRot + (0.0f + m_fAngle))*m_fLength;
	pVtx[3].pos.y = m_pos.y + cosf(m_fRot + (0.0f + m_fAngle))*m_fLength;
	pVtx[3].pos.z = 0.0f;

	//���_�o�b�t�@�̉��
	m_pVtxBuff->Unlock();
}

//-----------------------------------------------------------------------------
// ���_�J���[�̐ݒ�
//-----------------------------------------------------------------------------
void CObject2D::SetColor(D3DXCOLOR col)
{
	VERTEX_2D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(col);
	pVtx[1].col = D3DXCOLOR(col);
	pVtx[2].col = D3DXCOLOR(col);
	pVtx[3].col = D3DXCOLOR(col);

	m_pVtxBuff->Unlock();
}

//-----------------------------------------------------------------------------
// �e�N�X�`���A�j���[�V��������
//-----------------------------------------------------------------------------
void CObject2D::SetAnimation(int nAnimU, int nAnimV, int nPartU, int nPartV)
{
	VERTEX_2D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(0.0f + (1.0f / nPartU) * nAnimU, 0.0f + (1.0f / nPartV) * nAnimV);
	pVtx[1].tex = D3DXVECTOR2((1.0f / nPartU) * (nAnimU + 1), 0.0f + (1.0f / nPartV) * nAnimV);
	pVtx[2].tex = D3DXVECTOR2(0.0f + (1.0f / nPartU) * nAnimU, (1.0f / nPartV) * (nAnimV + 1));
	pVtx[3].tex = D3DXVECTOR2((1.0f / nPartU) * (nAnimU + 1), (1.0f / nPartV) * (nAnimV + 1));

	m_pVtxBuff->Unlock();
}

//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
//CObject2D *CObject2D::Create()
//{
//	CObject2D* pObject2D = nullptr;
//	pObject2D = new CObject2D;
//
//	if (pObject2D != nullptr)
//	{
//		pObject2D->Init();
//		return pObject2D;
//	}
//
//	assert(false);
//	return pObject2D;
//}
