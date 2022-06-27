//=============================================================================
//
//	�v���C���[����[player.cpp]
//	Author : SHUGO KURODA
//
//=============================================================================
#include "manager.h"
#include "bullet.h"
#include "player.h"
#include "input_keyboard.h"
#include "sound.h"

//*****************************************************************************
// �萔�錾
//*****************************************************************************
const float CPlayer::SIZE_X = 40.0f;
const float CPlayer::SIZE_Y = 20.0f;
const float CPlayer::MOVE_DEFAULT = 10.0f;

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
// �e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture[2] = { nullptr };

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CPlayer::CPlayer()
	:m_move(0.0f, 0.0f, 0.0f), m_state(STATE_NORMAL), m_nCntState(0), m_nCntAttack(0), 
	m_nCounterAnimPlayer(0), m_nPatternAnimPlayer(0)
{
	//�I�u�W�F�N�g�̎�ސݒ�
	SetObjectType(EObject::TYPE_PLAYER);
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CPlayer::~CPlayer()
{
}

//-----------------------------------------------------------------------------
// �C���X�^���X��������
//-----------------------------------------------------------------------------
CPlayer *CPlayer::Create(const D3DXVECTOR3& pos)
{
	//�C���X�^���X����
	CPlayer *pCPlayer = new CPlayer;

	if (pCPlayer != nullptr)
	{
		//��������
		pCPlayer->Init(pos);

		// �e�N�X�`���̐ݒ�
		pCPlayer->BindTexture(m_pTexture[0]);
	}

	return pCPlayer;
}

//-----------------------------------------------------------------------------
// �e�N�X�`���̓ǂݍ���
//-----------------------------------------------------------------------------
HRESULT CPlayer::Load()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/player000.png",
		&m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/player001.png",
		&m_pTexture[1]);

	return S_OK;
}

//-----------------------------------------------------------------------------
//	�e�N�X�`���̍폜
//-----------------------------------------------------------------------------
void CPlayer::Unload()
{
	for (int nCnt = 0; nCnt < MAX_TEX; nCnt++)
	{
		// �e�N�X�`���̔j��
		if (m_pTexture[nCnt] != nullptr)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = nullptr;
		}
	}
}

//-----------------------------------------------------------------------------
// ����������
//-----------------------------------------------------------------------------
HRESULT CPlayer::Init(const D3DXVECTOR3& pos)
{
	CObject2D::SetSize(SIZE_X, SIZE_Y);

	CObject2D::Init(pos);
	return S_OK;
}

//-----------------------------------------------------------------------------
// �I������
//-----------------------------------------------------------------------------
void CPlayer::Uninit()
{
	CObject2D::Uninit();
}

//-----------------------------------------------------------------------------
// �X�V����
//-----------------------------------------------------------------------------
void CPlayer::Update()
{
	// �ʒu�����擾
	D3DXVECTOR3 pos = CObject2D::GetPosition();

	// �L�[�{�[�h���̎擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_LEFT) == true)
	{//A�L�[�������ꂽ
		if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_UP) == true)
		{
			pos.x += sinf(-D3DX_PI * 0.75f) * MOVE_DEFAULT;
			pos.y += cosf(-D3DX_PI * 0.75f) * MOVE_DEFAULT;
		}
		else if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_DOWN) == true)
		{
			pos.x += sinf(-D3DX_PI * 0.25f) * MOVE_DEFAULT;
			pos.y += cosf(-D3DX_PI * 0.25f) * MOVE_DEFAULT;
		}
		else
		{
			pos.x += sinf(-D3DX_PI * 0.5f) * MOVE_DEFAULT;
			pos.y += cosf(-D3DX_PI * 0.5f) * MOVE_DEFAULT;
		}
	}
	else if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_RIGHT) == true)
	{//D�L�[�������ꂽ
		if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_UP) == true)
		{
			pos.x += sinf(D3DX_PI *0.75f) * MOVE_DEFAULT;
			pos.y += cosf(D3DX_PI *0.75f) * MOVE_DEFAULT;
		}
		else if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_DOWN) == true)
		{
			pos.x += sinf(D3DX_PI * 0.25f) * MOVE_DEFAULT;
			pos.y += cosf(D3DX_PI * 0.25f) * MOVE_DEFAULT;
		}
		else
		{
			pos.x += sinf(D3DX_PI * 0.5f) * MOVE_DEFAULT;
			pos.y += cosf(D3DX_PI * 0.5f) * MOVE_DEFAULT;
		}
	}
	else if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_UP) == true)
	{//W�L�[�������ꂽ
		pos.x += sinf(D3DX_PI * 1.0f) * MOVE_DEFAULT;
		pos.y += cosf(D3DX_PI * 1.0f) * MOVE_DEFAULT;
	}
	else if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_DOWN) == true)
	{//S�L�[�������ꂽ
		pos.x += sinf(D3DX_PI * 0.0f) * MOVE_DEFAULT;
		pos.y += cosf(D3DX_PI * 0.0f) * MOVE_DEFAULT;
	}

	if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_ATTACK) == true)
	{//SPACE�L�[�������ꂽ
		m_nCntAttack++;		//�U���Ԋu�̃J�E���^�[
		if (m_nCntAttack > 10)
		{
			//�e�̐ݒ�
			CBullet::Create(pos);
			m_nCntAttack = 0;

			//�T�E���h�Đ�
			//CSound::Play(CSound::SOUND_LABEL_TITLE);
		}
	}
	else
	{
		m_nCntAttack = 10;
	}

	//�ʒu���X�V
	CObject2D::SetPosition(pos);

	//�T�C�Y�X�V
	//CObject2D::SetSize(m_Scale.x, m_Scale.y);

	//�e�N�X�`���A�j���[�V����
	//CObject2D::SetAnimation();
}

//-----------------------------------------------------------------------------
// �`�揈��
//-----------------------------------------------------------------------------
void CPlayer::Draw()
{
	CObject2D::Draw();
}

//-----------------------------------------------------------------------------
// �_���[�W����
//-----------------------------------------------------------------------------
void CPlayer::Damage()
{
}

//-----------------------------------------------------------------------------
// ���S����
//-----------------------------------------------------------------------------
void CPlayer::Die()
{
}
