//-----------------------------------------------------------------------------------------------
//
// �e�̏���[bullet.cpp]
// Author : SHUGO kURODA
//
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------------------------
#include "item.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"

#include "library.h"

#include "player.h"

//-----------------------------------------------------------------------------------------------
// �萔��`
//-----------------------------------------------------------------------------------------------
// ��
const float CItem::SIZE_WIDTH = 40.0f;
// ����
const float CItem::SIZE_HEIGHT = 40.0f;
// ��{�ړ���
const float CItem::MOVE_DEFAULT = -5.0f;
// �A�j���[�V�����Ԋu
const int CItem::ANIM_INTERVAL = 5;
// �A�j���[�V�����ő吔
const int CItem::MAX_ANIM = 4;
// U���W�̍ő啪����
const int CItem::DIVISION_U = 4;
// V���W�̍ő啪����
const int CItem::DIVISION_V = 1;

//-----------------------------------------------------------------------------------------------
// �ÓI�����o�ϐ�
//-----------------------------------------------------------------------------------------------
// �e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 CItem::m_apTexture[TYPE_MAX] = { nullptr };

//-----------------------------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------------------------
CItem::CItem() :
	m_move(0.0f, 0.0f, 0.0f), m_fRot(0.0f), m_nCntAnim(0), m_nPatternAnim(0), m_type(TYPE_NONE)
{
	SetObjType(EObject::OBJ_ITEM);
}

//-----------------------------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------------------------
CItem::~CItem()
{

}

//-----------------------------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------------------------
CItem* CItem::Create(const D3DXVECTOR3& pos, const EType type)
{
	// �|�C���^�N���X��錾
	CItem* pBullet = new CItem;

	if (pBullet != nullptr)
	{// ����nullptr�ł͂Ȃ�������

		// �ʒu�̕ۑ�
		pBullet->m_move = pos;

		// �ʒu�ݒ�(pos�̉~����ɔz�u)
		pBullet->SetPosition(D3DXVECTOR3(pBullet->m_move.x - sinf(pBullet->m_fRot) * 150,
			pBullet->m_move.y - cosf(pBullet->m_fRot) * 150,
			0.0f));

		//�e�N�X�`����ނ̐ݒ�
		pBullet->m_type = type;

		// ������
		pBullet->Init();

		// �e�N�X�`���̐ݒ�
		pBullet->BindTexture(m_apTexture[pBullet->m_type]);
	}

	return pBullet;
}

//-----------------------------------------------------------------------------------------------
// �e�N�X�`���̓ǂݍ���
//-----------------------------------------------------------------------------------------------
HRESULT CItem::Load()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bullet000.png",
		&m_apTexture[TYPE_RED]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bullet001.png",
		&m_apTexture[TYPE_BLUE]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bullet002.png",
		&m_apTexture[TYPE_GREEN]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bullet003.png",
		&m_apTexture[TYPE_ORANGE]);

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// �e�N�X�`���̍폜
//-----------------------------------------------------------------------------------------------
void CItem::Unload()
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		// �e�N�X�`���̔j��
		if (m_apTexture[nCnt] != nullptr)
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = nullptr;
		}
	}
}

//-----------------------------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------------------------
HRESULT CItem::Init()
{
	// �T�C�Y
	CObject2D::SetSize(D3DXVECTOR2(SIZE_WIDTH, SIZE_HEIGHT));
	// ������
	CObject2D::Init();

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------------------------
void CItem::Uninit()
{
	CObject2D::Uninit();
}

//-----------------------------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------------------------
void CItem::Update()
{
	// �ړ��ʂ̉��Z
	m_move.x += MOVE_DEFAULT;
	// ��]�ʂ̉��Z
	m_fRot += 0.1f;
	if (m_fRot >= D3DX_PI * 2)
	{
		m_fRot = 0.0f;
	}

	// �ړ��_�𒆐S�ɉ�]������
	D3DXVECTOR3 pos = D3DXVECTOR3(m_move.x - sinf(m_fRot) * 150,
		m_move.y - cosf(m_fRot) * 150,
		0.0f);

	if (LibrarySpace::OutScreen2D(&pos, CObject2D::GetSize()))
	{//��ʊO�ɏo����I��
		Uninit();
		return;
	}

	//�����蔻��
	if (Collision(pos))
	{// ����������I��	
		Uninit();
		return;
	}

	// �ʒu�̍X�V
	CObject2D::SetPosition(pos);

	// �J�E���g�𑝂₷
	m_nCntAnim++;

	// ���t���[�����o������
	if (m_nCntAnim % ANIM_INTERVAL == 0)
	{
		// �A�j���[�V����������1�i�߂�
		m_nPatternAnim++;
	}

	// �A�j���[�V�������I�������
	if (m_nPatternAnim == MAX_ANIM)
	{
		// �A�j���[�V����������������
		m_nPatternAnim = 0;
	}
	else
	{
		//���_���W�̐ݒ�
		CObject2D::SetVertex();

		//�e�N�X�`���A�j���[�V����
		CObject2D::SetAnimation(m_nPatternAnim, 1, DIVISION_U, DIVISION_V);
	}
}

//-----------------------------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------------------------
void CItem::Draw()
{
	CObject2D::Draw();
}

//-----------------------------------------------------------------------------------------------
// �����蔻��
//-----------------------------------------------------------------------------------------------
bool CItem::Collision(D3DXVECTOR3 posStart)
{
	//�A�C�e���̃T�C�Y�擾
	float fStartLength = GetLength();

	for (int nCntPlayer = 0; nCntPlayer < CPlayer::PLAYER_MAX; nCntPlayer++)
	{
		// �v���C���[���̎擾
		CPlayer *pPlayer = CGame::GetPlayer(nCntPlayer);

		if (pPlayer != nullptr)
		{
			if (LibrarySpace::SphereCollision2D(posStart, pPlayer->GetPosition(), fStartLength, pPlayer->GetLength()))
			{//�A�C�e���Ɠ���������(���̂̓����蔻��)

				//�p���[�A�b�v����
				//pPlayer->Damage(1);

				return true;	//��������
			}
		}
	}

	return false;		//�������ĂȂ�
}
