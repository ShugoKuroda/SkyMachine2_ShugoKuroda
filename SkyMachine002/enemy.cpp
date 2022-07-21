//-----------------------------------------------------------------------------------------------
//
// �e�̏���[bullet.cpp]
// Author : SHUGO kURODA
//
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------------------------
#include "enemy.h"
#include "manager.h"
#include "renderer.h"

#include "library.h"
#include "explosion.h"

//-----------------------------------------------------------------------------------------------
// �萔�ϐ�
//-----------------------------------------------------------------------------------------------
// �̗�
const int CEnemy::LIFE = 50;
// ��
const float CEnemy::SIZE_WIDTH = 50.0f;
// ����
const float CEnemy::SIZE_HEIGHT = 50.0f;

//-----------------------------------------------------------------------------------------------
// �ÓI�����o�ϐ�
//-----------------------------------------------------------------------------------------------
// �e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 CEnemy::m_apTexture[TYPE_MAX] = { nullptr };

//-----------------------------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------------------------
CEnemy::CEnemy() :
	m_move(0.0f, 0.0f, 0.0f), m_state(STATE_NORMAL), m_type(TYPE_NONE), m_nLife(0), m_nCntState(0), m_nCntAnim(0), m_nPatternAnim(0)
{
	SetObjectType(EObject::OBJ_ENEMY);
}

//-----------------------------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------------------------
CEnemy::~CEnemy()
{
}

//-----------------------------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------------------------
CEnemy *CEnemy::Create(const D3DXVECTOR3& pos, TYPE type, EnemyMove *pEnemyMove)
{
	// �|�C���^�N���X��錾
	CEnemy* pEnemy = new CEnemy;

	if (pEnemy != nullptr)
	{
		//�ʒu�ݒ�
		pEnemy->SetPosition(pos);

		//�ړ����̐ݒ�
		pEnemy->m_pMoveInfo = pEnemyMove;

		//��ނ̐ݒ�
		pEnemy->m_type = type;

		// ������
		pEnemy->Init();

		// �e�N�X�`���̐ݒ�
		pEnemy->BindTexture(m_apTexture[type]);
	}

	return pEnemy;
}

//-----------------------------------------------------------------------------------------------
// �e�N�X�`���̓ǂݍ���
//-----------------------------------------------------------------------------------------------
HRESULT CEnemy::Load()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy000.png", &m_apTexture[TYPE_STARFISH]);			// �q�g�f�^�̓G
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy001.png", &m_apTexture[TYPE_MOSQUITO]);			// �H���^�̓G
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy002.png", &m_apTexture[TYPE_SPHERE]);				// ���̌^�̓G
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy003.png", &m_apTexture[TYPE_ASSAULT]);			// �ˌ��^�̓G(�o�g�~���g���̋ʂ݂����ȓG)
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy004.png", &m_apTexture[TYPE_SEAURCHIN]);			// �E�j�^�̓G
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy005.png", &m_apTexture[TYPE_ROWLING]);			// ��]�^�̓G
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy006.png", &m_apTexture[TYPE_FREEFALL]);			// ���R�����^�̓G
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy007.png", &m_apTexture[TYPE_SHOT]);				// �ˌ��^�̓G
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/EnemyBoss000.png", &m_apTexture[TYPE_RING_BOSS]);		// �����O�^�̒��{�X
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/EnemyBoss001.png", &m_apTexture[TYPE_DARK_BOSS]);		// ��{�X

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// �e�N�X�`���̍폜
//-----------------------------------------------------------------------------------------------
void CEnemy::Unload()
{
	for (int nCntEnemy = 0; nCntEnemy < TYPE_MAX; nCntEnemy++)
	{
		// �e�N�X�`���̔j��
		if (m_apTexture[nCntEnemy] != nullptr)
		{
			m_apTexture[nCntEnemy]->Release();
			m_apTexture[nCntEnemy] = nullptr;
		}
	}
}

//-----------------------------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------------------------
HRESULT CEnemy::Init()
{
	//�ړ��ʂ̌v�Z
	m_nRestTime = m_pMoveInfo->nFrameMove;
	// �����̐ݒ�
	m_nLife = LIFE;
	// �T�C�Y�ݒ�
	CObject2D::SetSize(D3DXVECTOR2(SIZE_WIDTH, SIZE_HEIGHT));
	// �I�u�W�F�N�g���̏�����
	CObject2D::Init();
	// ���_�J���[�̐ݒ�
	CObject2D::SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------------------------
void CEnemy::Uninit()
{
	CObject2D::Uninit();
}

//-----------------------------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------------------------
void CEnemy::Update()
{
	// �ʒu�̎擾
	D3DXVECTOR3 pos = CObject2D::GetPosition();
	//�����̎擾
	float fRot = CObject2D::GetRot();
	//�ړ��ʂ̐ݒ�
	m_move = (m_pMoveInfo->posEnd - pos) / (float)m_nRestTime;
	//�ړ��܂ł̎��Ԃ�����
	m_nRestTime--;

	// �ړ��ʂ̍X�V
	pos += m_move;
	fRot += 0.1f;

	if (m_nLife <= 0)
	{// ���C�t��0
		// �����̐���
		CExplosion::Create(pos);
		// �j��
		Uninit();
		return;
	}
	else if (LibrarySpace::OutScreen2D(&pos, CObject2D::GetSize()))
	{//��ʊO�ɏo����
		// �j��
		Uninit();
		return;
	}

	//�ړ��ʂ̍Đݒ�
	if (m_nRestTime <= 0)
	{//�ړI�̈ʒu�ɓ��B������
		SetMove();
	}

	//��ԏ���
	State();
	//�����̍X�V
	CObject2D::SetRot(fRot);
	// �ʒu�̍X�V
	CObject2D::SetPosition(pos);
	//���_���W�̐ݒ�
	CObject2D::SetVertex();
}

//-----------------------------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------------------------
void CEnemy::Draw()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�e�N�X�`���u�����f�B���O�����Z�����ɂ���
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	//�`�揈��
	CObject2D::Draw();

	//�e�N�X�`���u�����f�B���O�̐ݒ�����ɖ߂�
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
}

//-----------------------------------------------------------------------------------------------
// �ړ��ʂ̐ݒ�(�v�Z)
//-----------------------------------------------------------------------------------------------
void CEnemy::SetMove()
{
	m_pMoveInfo++;
	m_nRestTime = m_pMoveInfo->nFrameMove;
}

//-----------------------------------------------------------------------------------------------
// �_���[�W����
//-----------------------------------------------------------------------------------------------
void CEnemy::Damage(int nDamage)
{
	m_nLife -= nDamage;
	m_state = STATE_DAMAGE;
	m_nCntState = 5;
}

//-----------------------------------------------------------------------------------------------
// ���
//-----------------------------------------------------------------------------------------------
void CEnemy::State()
{
	switch (m_state)
	{
	case STATE_NORMAL:			//�G�l�~�[�̏�Ԃ��ʏ�̏ꍇ
		break;
	case STATE_DAMAGE:			//�G�l�~�[���_���[�W��Ԃ̏ꍇ
		//��ԃJ�E���^�[�̌��Z
		m_nCntState--;
		//�_�ł�����
		if (m_nCntState % 2 == 0)
		{//���F
			CObject2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
		}
		else
		{//�s���N
			CObject2D::SetColor(D3DXCOLOR(1.0f, 0.0f, 0.5f, 1.0f));
		}

		//��Ԃ�ʏ�ɖ߂�
		if (m_nCntState <= 0)
		{
			m_state = STATE_NORMAL;

			CObject2D::SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		}
		break;
	}
}
