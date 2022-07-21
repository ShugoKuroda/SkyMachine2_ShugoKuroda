//=============================================================================
//
//	�v���C���[����[player.cpp]
//	Author : SHUGO KURODA
//
//=============================================================================
#include "manager.h"
#include "input_keyboard.h"
#include "input_joypad.h"
#include "sound.h"

#include "library.h"

#include "bullet.h"
#include "player.h"

//*****************************************************************************
// �萔�錾
//*****************************************************************************
const float CPlayer::SIZE_X = 90.0f;
const float CPlayer::SIZE_Y = 40.0f;
const float CPlayer::MOVE_DEFAULT = 10.0f;
// �A�j���[�V�����Ԋu
const int CPlayer::ANIM_INTERVAL = 5;
// �A�j���[�V�����ő吔
const int CPlayer::MAX_ANIM = 2;
// U���W�̍ő啪����
const int CPlayer::DIVISION_U = 2;
// V���W�̍ő啪����
const int CPlayer::DIVISION_V = 4;

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
// �e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 CPlayer::m_apTexture[2] = { nullptr };

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CPlayer::CPlayer() :
	m_move(0.0f, 0.0f, 0.0f), m_state(STATE_NORMAL), m_nCntState(0), m_nCntAttack(0),
	m_nCntAnim(0), m_nPatternAnim(0), m_nCntAnimMove(0), m_nTexRotType(TYPE_NEUTRAL), m_nPlayerNum(0)
{
	//�I�u�W�F�N�g�̎�ސݒ�
	SetObjectType(EObject::OBJ_PLAYER);
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
	CPlayer *pPlayer = new CPlayer;

	if (pPlayer != nullptr)
	{
		//�ʒu�ݒ�
		pPlayer->SetPosition(pos);

		//��������
		pPlayer->Init();

		// �e�N�X�`���̐ݒ�
		pPlayer->BindTexture(m_apTexture[0]);
	}

	return pPlayer;
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
		&m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/player001.png",
		&m_apTexture[1]);

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
		if (m_apTexture[nCnt] != nullptr)
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = nullptr;
		}
	}
}

//-----------------------------------------------------------------------------
// ����������
//-----------------------------------------------------------------------------
HRESULT CPlayer::Init()
{
	CObject2D::SetSize(D3DXVECTOR2(SIZE_X, SIZE_Y));

	CObject2D::Init();

	//�e�N�X�`���A�j���[�V����
	CObject2D::SetAnimation(m_nPatternAnim, m_nTexRotType, DIVISION_U, DIVISION_V);

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

	//�ړ�����
	pos = Move(pos);

	// �L�[�{�[�h���̎擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	CInputJoypad *pJoypad = CManager::GetInputJoypad();

	if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_ATTACK) == true)
	{//SPACE�L�[�������ꂽ
		m_nCntAttack++;		//�U���Ԋu�̃J�E���^�[
		if (m_nCntAttack > 5)
		{
			//�e�̐ݒ�
			CBullet::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), 5)->SetType(CBullet::EType::TYPE_PLAYER);
			m_nCntAttack = 0;

			//�T�E���h�Đ�
			//CSound::Play(CSound::SOUND_LABEL_TITLE);
		}
	}
	else
	{
		m_nCntAttack = 10;
	}

	//���͈͂���ʓ��ɐ���
	LibrarySpace::SteyInScreen2D(&pos, CObject2D::GetSize());

	//�ʒu���X�V
	CObject2D::SetPosition(pos);
	
	//�T�C�Y�X�V
	//CObject2D::SetSize(m_Scale.x, m_Scale.y);

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
		CObject2D::SetAnimation(m_nPatternAnim, m_nTexRotType, DIVISION_U, DIVISION_V);
	}
}

//-----------------------------------------------------------------------------
// �`�揈��
//-----------------------------------------------------------------------------
void CPlayer::Draw()
{
	CObject2D::Draw();
}

//-----------------------------------------------------------------------------
// �ړ�����
//-----------------------------------------------------------------------------
D3DXVECTOR3 CPlayer::Move(D3DXVECTOR3 pos)
{
	// �L�[�{�[�h���̎擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	// �W���C�p�b�h���̎擾
	CInputJoypad *pJoypad = CManager::GetInputJoypad();

	if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_LEFT) == true /*||
		pJoypad->GetStick(CInputJoypad::JOYKEY_LEFT_STICK, m_nPlayerNum).x*/)
	{//A�L�[�������ꂽ
		if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_UP) == true)
		{
			pos.x += sinf(-D3DX_PI * 0.75f) * MOVE_DEFAULT;
			pos.y += cosf(-D3DX_PI * 0.75f) * MOVE_DEFAULT;
			SetAnimNum(TYPE_DOWN,TYPE_UP);
			m_nCntAnimMove++;
		}
		else if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_DOWN) == true)
		{
			pos.x += sinf(-D3DX_PI * 0.25f) * MOVE_DEFAULT;
			pos.y += cosf(-D3DX_PI * 0.25f) * MOVE_DEFAULT;
			SetAnimNum(TYPE_UP, TYPE_DOWN);
			m_nCntAnimMove++;
		}
		else
		{
			pos.x += sinf(-D3DX_PI * 0.5f) * MOVE_DEFAULT;
			pos.y += cosf(-D3DX_PI * 0.5f) * MOVE_DEFAULT;
			m_nTexRotType = TYPE_NEUTRAL;
			m_nCntAnimMove = 0;
		}
	}
	else if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_RIGHT) == true)
	{//D�L�[�������ꂽ
		if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_UP) == true)
		{
			pos.x += sinf(D3DX_PI *0.75f) * MOVE_DEFAULT;
			pos.y += cosf(D3DX_PI *0.75f) * MOVE_DEFAULT;
			SetAnimNum(TYPE_DOWN, TYPE_UP);
			m_nCntAnimMove++;
		}
		else if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_DOWN) == true)
		{
			pos.x += sinf(D3DX_PI * 0.25f) * MOVE_DEFAULT;
			pos.y += cosf(D3DX_PI * 0.25f) * MOVE_DEFAULT;
			SetAnimNum(TYPE_UP, TYPE_DOWN);
			m_nCntAnimMove++;
		}
		else
		{
			pos.x += sinf(D3DX_PI * 0.5f) * MOVE_DEFAULT;
			pos.y += cosf(D3DX_PI * 0.5f) * MOVE_DEFAULT;
			m_nTexRotType = TYPE_NEUTRAL;
			m_nCntAnimMove = 0;
		}
	}
	else if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_UP) == true)
	{//W�L�[�������ꂽ
		pos.x += sinf(D3DX_PI * 1.0f) * MOVE_DEFAULT;
		pos.y += cosf(D3DX_PI * 1.0f) * MOVE_DEFAULT;
		SetAnimNum(TYPE_DOWN, TYPE_UP);
		m_nCntAnimMove++;
	}
	else if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_DOWN) == true)
	{//S�L�[�������ꂽ
		pos.x += sinf(D3DX_PI * 0.0f) * MOVE_DEFAULT;
		pos.y += cosf(D3DX_PI * 0.0f) * MOVE_DEFAULT;
		SetAnimNum(TYPE_UP, TYPE_DOWN);
		m_nCntAnimMove++;
	}
	else
	{
		m_nCntAnimMove = 0;
		m_nTexRotType = TYPE_NEUTRAL;
	}

	return pos;
}

//-----------------------------------------------------------------------------
// �A�j���[�V�����ԍ��̐ݒ�
//-----------------------------------------------------------------------------
void CPlayer::SetAnimNum(ANIMTYPE AnimIn, ANIMTYPE AnimOut)
{
	if (m_nTexRotType == AnimIn)
	{
		m_nTexRotType = TYPE_NEUTRAL;
		m_nCntAnimMove = 0;
	}
	else if (m_nCntAnimMove >= 10)
	{
		m_nTexRotType = AnimOut;
	}
}

//-----------------------------------------------------------------------------
// �_���[�W����
//-----------------------------------------------------------------------------
void CPlayer::Damage()
{
	m_move.x += 123.0f;
}

//-----------------------------------------------------------------------------
// ���S����
//-----------------------------------------------------------------------------
void CPlayer::Die()
{
}
