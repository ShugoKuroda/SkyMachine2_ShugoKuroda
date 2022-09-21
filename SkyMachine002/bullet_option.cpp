//-----------------------------------------------------------------------------------------------
//
// �e�̏���[bullet.cpp]
// Author : SHUGO kURODA
//
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------------------------
#include "bullet_option.h"
#include "bullet.h"
#include "manager.h"
#include "renderer.h"

#include "library.h"
#include "game.h"

#include "player.h"
#include "enemy.h"
#include "explosion.h"

//-----------------------------------------------------------------------------------------------
// �ÓI�����o�ϐ�
//-----------------------------------------------------------------------------------------------
// �e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 CBulletOption::m_apTexture = { nullptr };

//-----------------------------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------------------------
CBulletOption::CBulletOption() :m_fRad(0.0f), m_type(TYPE_NONE), m_nDamage(0), m_col(FADE_NONE)
{
	SetObjType(EObject::OBJ_BULLET);
}

//-----------------------------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------------------------
CBulletOption::~CBulletOption()
{

}

//-----------------------------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------------------------
CBulletOption* CBulletOption::Create(const D3DXVECTOR3& pos, const int& nSize, const int& nDamage, const EType& type)
{
	// �|�C���^�N���X��錾
	CBulletOption* pOption = new CBulletOption;

	if (pOption != nullptr)
	{// ����nullptr�ł͂Ȃ�������

		// �ʒu�ݒ�
		pOption->SetPosition(pos);

		//�_���[�W�̐ݒ�
		pOption->SetSize(D3DXVECTOR2((float)nSize, (float)nSize));

		//�_���[�W�̐ݒ�
		pOption->m_nDamage = nDamage;

		//�傫���^�C�v�̐ݒ�
		pOption->m_type = type;

		// ������
		pOption->Init();

		// �e�N�X�`���̐ݒ�
		pOption->BindTexture(m_apTexture);
	}

	return pOption;
}

//-----------------------------------------------------------------------------------------------
// �e�N�X�`���̓ǂݍ���
//-----------------------------------------------------------------------------------------------
HRESULT CBulletOption::Load()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/option000.png",
		&m_apTexture);

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// �e�N�X�`���̍폜
//-----------------------------------------------------------------------------------------------
void CBulletOption::Unload()
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
HRESULT CBulletOption::Init()
{
	// ������
	CObject2D::Init();

	// �F�ݒ�
	CObject2D::SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	// �F��Ԃ̏�����
	m_col = FADE_GREEN;

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------------------------
void CBulletOption::Uninit()
{
	CObject2D::Uninit();
}

//-----------------------------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------------------------
void CBulletOption::Update()
{
	// �F�̎擾
	D3DXCOLOR col = GetColor();

	if (m_col == FADE_RED)
	{
		col.r += 0.01f;
		col.b -= 0.01f;
		if (col.r >= 1.0f)
		{
			m_col = FADE_GREEN;
		}
	}
	else if (m_col == FADE_GREEN)
	{
		col.g += 0.01f;
		col.r -= 0.01f;
		if (col.g >= 1.0f)
		{
			m_col = FADE_BLUE;
		}
	}
	else if (m_col == FADE_BLUE)
	{
		col.b += 0.01f;
		col.g -= 0.01f;
		if (col.b >= 1.0f)
		{
			m_col = FADE_RED;
		}
	}

	// �v���C���[���̎擾
	CPlayer *pPlayer = CGame::GetPlayer();
	D3DXVECTOR3 PlayerPos = pPlayer->GetPosition();

	// �ʒu�̎擾
	D3DXVECTOR3 pos = CObject2D::GetPosition();

	m_fRad += 0.1f;
	if (m_fRad >= D3DX_PI * 2)
	{
		m_fRad = 0.0f;
	}

	// �ʒu�̍X�V(�v���C���[�𒆐S�ɉ�]������)
	pos = D3DXVECTOR3(PlayerPos.x - sinf(m_fRad) * 100,
		PlayerPos.y - cosf(m_fRad) * 100,
		0.0f);

	//�����蔻��(����)
	Collision(pos);

	// �ʒu�̍X�V
	CObject2D::SetPosition(pos);

	// �F�̐ݒ�
	SetColor(col);

	//���_���W�̐ݒ�
	CObject2D::SetVertex();
}

//-----------------------------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------------------------
void CBulletOption::Draw()
{
	CObject2D::Draw();
}

//-----------------------------------------------------------------------------------------------
// �����蔻��
//-----------------------------------------------------------------------------------------------
bool CBulletOption::Collision(D3DXVECTOR3 posStart)
{
	//�e�̃T�C�Y�擾
	float fStartLength = GetLength();

	for (int nCntObject = 0; nCntObject < CObject::MAX_OBJECT; nCntObject++)
	{
		CObject *pObject = CObject::GetObject(nCntObject);
		if (pObject != nullptr)
		{
			CObject::EObject objType = pObject->GetObjType();

			//�v���C���[�̒e�ƓG�̔���
			if (objType == OBJ_ENEMY)
			{
				//�I�u�W�F�N�g�|�C���^��G�ɃL���X�g
				CEnemy *pEnemy = (CEnemy*)pObject;

				if (LibrarySpace::SphereCollision2D(posStart, pEnemy->GetPosition(), fStartLength, pEnemy->GetLength()))
				{//�e�Ɠ���������(���̂̓����蔻��)
				 //�_���[�W����
					pEnemy->Damage(m_nDamage);
				}
			}

			//�v���C���[�̒e�ƓG�{�X�̔���
			else if (objType == OBJ_ENEMYBOSS)
			{
				//�I�u�W�F�N�g�|�C���^��G�ɃL���X�g
				CEnemy *pEnemy = (CEnemy*)pObject;

				if (LibrarySpace::SphereCollision2D(posStart, pEnemy->GetPosition(), fStartLength - 60.0f, pEnemy->GetLength()))
				{//�e�Ɠ���������(���̂̓����蔻��)
				 //�_���[�W����
					pEnemy->Damage(m_nDamage);
				}
			}
		}
	}

	return false;		//�������ĂȂ�
}
