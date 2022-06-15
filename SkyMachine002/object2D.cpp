//=============================================================================
//
// �I�u�W�F�N�g����[object.cpp]
// Author : SHUGO KURODA
//
//=============================================================================
#include "object2D.h"
#include "renderer.h"

#include <assert.h>

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
// ���_�f�[�^
struct VERTEX_2D
{
	D3DXVECTOR3 pos;
	float rhw;
	D3DCOLOR col;
	D3DXVECTOR2 tex;
};

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
int CObject2D::m_RandPos = 0;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CObject2D::CObject2D()
{
	//���_�o�b�t�@�ւ̃|�C���^�[������
	m_pVtxBuff = nullptr;
	//�e�N�X�`���ւ̃|�C���^�[������
	m_pTexture = nullptr;

	//�e�v�f�̏�����
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fRot = 0.0f;
	m_fLength = 0.0f;
	m_fAngle = 0.0f;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CObject2D::~CObject2D()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CObject2D::Init()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pD3DDevice = GetRenderer()->GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pD3DDevice, "data/TEXTURE/test001.png", &m_pTexture);

	m_RandPos++;

	//�ʒu�̏�����
	m_pos = D3DXVECTOR3(SCREEN_WIDTH / 4.0f * m_RandPos, SCREEN_HEIGHT / 2, 0.0f);
	//�����̏�����
	m_fRot = (rand() % 628) / D3DX_PI;
	//�Ίp���̒������Z�o
	m_fLength = sqrtf(400.0f * 400.0f + 200.0f * 200.0f) / 2.0f;
	//�Ίp���̊p�x���Z�o����
	m_fAngle = atan2f(200.0f, 100.0f);

	//���_�o�b�t�@�̐���
	pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
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

//=============================================================================
// �I������
//=============================================================================
void CObject2D::Uninit()
{
	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	//�e�N�X�`���̔j��
	if (m_pTexture != nullptr)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}

	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CObject2D::Update()
{
	m_fLength += sinf(m_fRot) * 2.0f;
	m_fRot += -D3DX_PI * 0.01f;

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
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

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CObject2D::Draw()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pD3DDevice = GetRenderer()->GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pD3DDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pD3DDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pD3DDevice->SetTexture(0, m_pTexture);

	//�|���S���̕`��
	pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// ��������
//=============================================================================
CObject2D *CObject2D::Create()
{
	CObject2D* pObject2D = nullptr;
	pObject2D = new CObject2D;

	if (pObject2D != nullptr)
	{
		pObject2D->Init();
		return pObject2D;
	}
	assert(false);
	return pObject2D;
}
