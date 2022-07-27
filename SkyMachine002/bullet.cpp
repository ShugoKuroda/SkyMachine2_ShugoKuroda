//-----------------------------------------------------------------------------------------------
//
// �e�̏���[bullet.cpp]
// Author : SHUGO kURODA
//
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------------------------
#include "bullet.h"
#include "manager.h"
#include "renderer.h"

#include "library.h"

#include "player.h"
#include "enemy.h"
#include "explosion.h"

//-----------------------------------------------------------------------------------------------
// �萔��`
//-----------------------------------------------------------------------------------------------
// ��
const float CBullet::SIZE_WIDTH = 30.0f;
// ����
const float CBullet::SIZE_HEIGHT = 30.0f;
// ��{�ړ���
const float CBullet::MOVE_DEFAULT = 15.0f;
// �A�j���[�V�����Ԋu
const int CBullet::ANIM_INTERVAL = 5;
// �A�j���[�V�����ő吔
const int CBullet::MAX_ANIM = 4;
// U���W�̍ő啪����
const int CBullet::DIVISION_U = 4;
// V���W�̍ő啪����
const int CBullet::DIVISION_V = 1;

//-----------------------------------------------------------------------------------------------
// �ÓI�����o�ϐ�
//-----------------------------------------------------------------------------------------------
// �e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 CBullet::m_apTexture = nullptr;

//-----------------------------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------------------------
CBullet::CBullet() :
	m_move(0.0f, 0.0f, 0.0f), m_nDamage(0), m_nCntAnim(0), m_nPatternAnim(0)
{
	SetObjectType(EObject::OBJ_BULLET);
}

//-----------------------------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------------------------
CBullet::~CBullet()
{

}

//-----------------------------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------------------------
CBullet* CBullet::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const int& nDamage)
{
	// �|�C���^�N���X��錾
	CBullet* pBullet = new CBullet;

	if (pBullet != nullptr)
	{// ����nullptr�ł͂Ȃ�������
		// �ʒu�ݒ�
		pBullet->SetPosition(pos);

		//�ړ��ʂ̐ݒ�
		pBullet->m_move = move;

		//�e�̃_���[�W�ʂ̐ݒ�
		pBullet->m_nDamage = nDamage;

		// ������
		pBullet->Init();

		// �e�N�X�`���̐ݒ�
		pBullet->BindTexture(m_apTexture);
	}

	return pBullet;
}

//-----------------------------------------------------------------------------------------------
// �e�N�X�`���̓ǂݍ���
//-----------------------------------------------------------------------------------------------
HRESULT CBullet::Load()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bullet000.png",
		&m_apTexture);

	if (m_apTexture != nullptr)
	{
		return S_OK;
	}

	return S_FALSE;
}

//-----------------------------------------------------------------------------------------------
// �e�N�X�`���̍폜
//-----------------------------------------------------------------------------------------------
void CBullet::Unload()
{
	// �e�N�X�`���̔j��
	if (m_apTexture != nullptr)
	{
		m_apTexture->Release();
		m_apTexture = nullptr;
	}
}

//-----------------------------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------------------------
HRESULT CBullet::Init()
{
	// �T�C�Y
	CObject2D::SetSize(D3DXVECTOR2(SIZE_WIDTH, SIZE_HEIGHT));

	CObject2D::Init();

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------------------------
void CBullet::Uninit()
{
	CObject2D::Uninit();
}

//-----------------------------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------------------------
void CBullet::Update()
{
	// �ʒu�̎擾
	D3DXVECTOR3 pos = CObject2D::GetPosition();

	// �ړ��ʂ̍X�V
	pos += m_move;

	if (LibrarySpace::OutScreen2D(&pos, CObject2D::GetSize()))
	{//��ʊO
		// �e�̔j��
		Uninit();
		return;
	}

	//�����蔻��(����)
	if (!Collision(pos))
	{
		// �ʒu�̍X�V
		CObject2D::SetPosition(pos);

		// �J�E���g�𑝂₷
		m_nCntAnim++;
		if (m_nCntAnim % ANIM_INTERVAL == 0)
		{
			// ���̃A�j���[�V������1�i�߂�
			m_nPatternAnim++;
		}

		if (m_nPatternAnim == MAX_ANIM)
		{// �A�j���[�V�������I�������
			// �I������
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
}

//-----------------------------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------------------------
void CBullet::Draw()
{
	CObject2D::Draw();
}

//-----------------------------------------------------------------------------------------------
// �����蔻��
//-----------------------------------------------------------------------------------------------
bool CBullet::Collision(D3DXVECTOR3 posStart)
{
	//�e�̃T�C�Y�擾
	float fStartLength = GetLength();

	for (int nCntObject = 0; nCntObject < CObject::MAX_OBJECT; nCntObject++)
	{
		CObject *pObject = CObject::GetObject(nCntObject);
		if (pObject != nullptr)
		{
			CObject::EObject objType = pObject->GetObjType();
			if (objType == OBJ_ENEMY && m_nType == TYPE_PLAYER)
			{
				CEnemy *pEnemy = (CEnemy*)pObject;

				if (LibrarySpace::SphereCollision2D(posStart, pEnemy->GetPosition(), fStartLength, pEnemy->GetLength()))
				{//�e�Ɠ���������(���̂̓����蔻��)

					//�_���[�W����
					pEnemy->Damage(m_nDamage);
					// �e�̔j��
					Uninit();
					return true;	//��������
				}
			}
		}
	}

	return false;		//�������ĂȂ�
}
