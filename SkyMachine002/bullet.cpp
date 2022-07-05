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

#include "player.h"
#include "enemy.h"
#include "explosion.h"

//-----------------------------------------------------------------------------------------------
// �ϐ��ϐ�
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
LPDIRECT3DTEXTURE9 CBullet::m_pTexture = nullptr;

//-----------------------------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------------------------
CBullet::CBullet() :
	m_move(0.0f, 0.0f, 0.0f), m_nLife(0), m_nCntAnim(0), m_nPatternAnim(0)
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
CBullet* CBullet::Create(const D3DXVECTOR3& pos)
{
	// �|�C���^�N���X��錾
	CBullet* pBullet = new CBullet;

	if (pBullet != nullptr)
	{// ����nullptr�ł͂Ȃ�������
		// �ʒu�ݒ�
		pBullet->SetPosition(pos);

		// ������
		pBullet->Init();

		// �e�N�X�`���̐ݒ�
		pBullet->BindTexture(m_pTexture);
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
		&m_pTexture);

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// �e�N�X�`���̍폜
//-----------------------------------------------------------------------------------------------
void CBullet::Unload()
{
	// �e�N�X�`���̔j��
	if (m_pTexture != nullptr)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}
}

//-----------------------------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------------------------
HRESULT CBullet::Init()
{
	// �ړ���
	m_move.x = MOVE_DEFAULT;
	// ����
	m_nLife = LIFE;
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

	// ���C�t�����炵�Ă���
	m_nLife--;

	if (m_nLife == 0)
	{// ���C�t��0
		// �����̐���
		CExplosion::Create(pos);
		// �e�̔j��
		Uninit();
	}
	else
	{
		//�����蔻��(����)
		bool bHit = CollisionSphere(pos);

		if (!bHit)
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
}

//-----------------------------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------------------------
void CBullet::Draw()
{
	CObject2D::Draw();
}

//-----------------------------------------------------------------------------------------------
// �����蔻��(����)
//-----------------------------------------------------------------------------------------------
bool CBullet::CollisionSphere(D3DXVECTOR3 posStart)
{
	//�e�̃T�C�Y�擾
	float fStartLength = GetLength();
	int nMaxObj = CObject::GetObjectNumAll();

	for (int nCntObject = 0; nCntObject < nMaxObj; nCntObject++)
	{
		CObject *pObject = CObject::GetObject(nCntObject);
		if (pObject != nullptr)
		{
			CObject::EObject objType = pObject->GetObjType();
			if (objType == OBJ_ENEMY && m_nType == TYPE_PLAYER)
			{
				CEnemy *pEnemy = (CEnemy*)pObject;

				//�����̈ʒu,�T�C�Y�̎擾
				D3DXVECTOR3 posEnd = pEnemy->GetPosition();
				float fEndLength = pEnemy->GetLength();

				//2�̍��W���������߂�
				D3DXVECTOR3 Length = posStart - posEnd;

				//���W�������狗�������߂�
				float fLength = D3DXVec3Length(&Length);

				if (fStartLength + fEndLength > fLength)
				{//���݂��̋��̃T�C�Y�̍��v��苗����������������

					//�_���[�W����
					pEnemy->Damage();
					// �e�̔j��
					Uninit();
					return true;	//��������
				}
			}
		}
	}

	return false;		//�������ĂȂ�
}
