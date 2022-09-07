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
#include "renderer.h"

#include "library.h"

#include "bg.h"
#include "spray.h"
#include "bullet.h"
#include "player.h"
#include "explosion.h"

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
// �v���C���[�̃f�t�H���g�J���[
const D3DXCOLOR CPlayer::DEFAULT_COL = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
// �e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 CPlayer::m_apTexture[2] = { nullptr };

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CPlayer::CPlayer() :
	m_move(0.0f, 0.0f, 0.0f), m_state(STATE_NORMAL), m_nCntState(0), m_nCntAttack(0), m_nCntAnim(0), m_nPatternAnim(0), m_nCntAnimMove(0),
	m_nTexRotType(TYPE_NEUTRAL), m_nPlayerNum(0), posBullet(0.0f, 0.0f), m_bControl(false), m_bInSea(false)
{
	//�I�u�W�F�N�g�̎�ސݒ�
	SetObjType(EObject::OBJ_PLAYER);
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
	//�e�̔��ˈʒu��ݒ�
	posBullet = D3DXVECTOR2(20.0f, 10.0f);
	//�v���C���[�𑀍�\�ɂ���
	m_bControl = true;

	//�T�C�Y�̐ݒ�
	CObject2D::SetSize(D3DXVECTOR2(SIZE_X, SIZE_Y));
	//������
	CObject2D::Init();
	//�F�̐ݒ�
	CObject2D::SetColor(DEFAULT_COL);
	//�e�N�X�`�����W�̐ݒ�
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

	//����ł����ԂȂ�
	if (m_bControl == true)
	{
		//�ړ�����
		pos = Move(pos);

		// �L�[�{�[�h���̎擾
		CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
		CInputJoypad *pJoypad = CManager::GetInputJoypad();

		if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_ATTACK) == true)
		{//SPACE�L�[�������ꂽ

			//�U���J�E���^�[�̉��Z
			m_nCntAttack++;		

			if (m_nCntAttack > 5)
			{
				//�e�̐ݒ�
				CBullet::Create(D3DXVECTOR3(pos.x + posBullet.x, pos.y + posBullet.y, pos.z), D3DXVECTOR3(15.0f, 0.0f, 0.0f),
					5, CBullet::TYPE_PLAYER_BLUE)->SetParent(CBullet::PARENT_PLAYER);

				//�U���J�E���^�[�����Z�b�g
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
	}

	//�����Ԃ���ݒ�
	SetSpray();

	//�ʒu���X�V
	CObject2D::SetPosition(pos);

	//��ԊǗ�
	State();
	
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
		// �A�j���[�V�����p�^�[��������������
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
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�e�N�X�`���u�����f�B���O�����Z�����ɂ���
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	//�`��
	CObject2D::Draw();

	//�e�N�X�`���u�����f�B���O�̐ݒ�����ɖ߂�
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
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
			SetAnimNum(TYPE_DOWN, TYPE_UP);
			m_nCntAnimMove++;
			//�e�̔��ˈʒu��ݒ�
			posBullet = D3DXVECTOR2(20.0f, 10.0f);
		}
		else if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_DOWN) == true)
		{
			pos.x += sinf(-D3DX_PI * 0.25f) * MOVE_DEFAULT;
			pos.y += cosf(-D3DX_PI * 0.25f) * MOVE_DEFAULT;
			SetAnimNum(TYPE_UP, TYPE_DOWN);
			m_nCntAnimMove++;
			//�e�̔��ˈʒu��ݒ�
			posBullet = D3DXVECTOR2(20.0f, -5.0f);
		}
		else
		{
			pos.x += sinf(-D3DX_PI * 0.5f) * MOVE_DEFAULT;
			pos.y += cosf(-D3DX_PI * 0.5f) * MOVE_DEFAULT;
			m_nTexRotType = TYPE_NEUTRAL;
			m_nCntAnimMove = 0;
			//�e�̔��ˈʒu��ݒ�
			posBullet = D3DXVECTOR2(20.0f, 10.0f);
		}
	}
	else if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_RIGHT) == true)
	{//D�L�[�������ꂽ
		if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_UP) == true)
		{
			pos.x += sinf(D3DX_PI * 0.75f) * MOVE_DEFAULT;
			pos.y += cosf(D3DX_PI * 0.75f) * MOVE_DEFAULT;
			SetAnimNum(TYPE_DOWN, TYPE_UP);
			m_nCntAnimMove++;
			//�e�̔��ˈʒu��ݒ�
			posBullet = D3DXVECTOR2(20.0f, 10.0f);
		}
		else if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_DOWN) == true)
		{
			pos.x += sinf(D3DX_PI * 0.25f) * MOVE_DEFAULT;
			pos.y += cosf(D3DX_PI * 0.25f) * MOVE_DEFAULT;
			SetAnimNum(TYPE_UP, TYPE_DOWN);
			m_nCntAnimMove++;
			//�e�̔��ˈʒu��ݒ�
			posBullet = D3DXVECTOR2(20.0f, -5.0f);
		}
		else
		{
			pos.x += sinf(D3DX_PI * 0.5f) * MOVE_DEFAULT;
			pos.y += cosf(D3DX_PI * 0.5f) * MOVE_DEFAULT;
			m_nTexRotType = TYPE_NEUTRAL;
			m_nCntAnimMove = 0;
			//�e�̔��ˈʒu��ݒ�
			posBullet = D3DXVECTOR2(20.0f, 10.0f);
		}
	}
	else if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_UP) == true)
	{//W�L�[�������ꂽ
		pos.x += sinf(D3DX_PI * 1.0f) * MOVE_DEFAULT;
		pos.y += cosf(D3DX_PI * 1.0f) * MOVE_DEFAULT;
		SetAnimNum(TYPE_DOWN, TYPE_UP);
		m_nCntAnimMove++;
		//�e�̔��ˈʒu��ݒ�
		posBullet = D3DXVECTOR2(20.0f, 10.0f);
	}
	else if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_DOWN) == true)
	{//S�L�[�������ꂽ
		pos.x += sinf(D3DX_PI * 0.0f) * MOVE_DEFAULT;
		pos.y += cosf(D3DX_PI * 0.0f) * MOVE_DEFAULT;
		SetAnimNum(TYPE_UP, TYPE_DOWN);
		m_nCntAnimMove++;
		//�e�̔��ˈʒu��ݒ�
		posBullet = D3DXVECTOR2(20.0f, -5.0f);
	}
	else
	{
		//�e�̔��ˈʒu��ݒ�
		posBullet = D3DXVECTOR2(20.0f, 10.0f);
		m_nCntAnimMove = 0;
		m_nTexRotType = TYPE_NEUTRAL;
	}

	return pos;
}

//-----------------------------------------------------------------------------
// �����Ԃ��̐ݒ�
//-----------------------------------------------------------------------------
void CPlayer::SetSpray()
{
	//�C�̐��ʃI�u�W�F�N�g���̎擾
	CObject2D *pObject2D = CBg::GetObjectBg(CBg::BG_A_WAVE1);

	// �ʒu�����擾
	D3DXVECTOR3 posWave = pObject2D->GetPosition();		//���ʂ̈ʒu���擾
	D3DXVECTOR3 pos = CObject2D::GetPosition();			//�v���C���[�̈ʒu���擾

	// �C�ɓ����Ă��Ȃ��Ȃ�
	if (m_bInSea == false)
	{
		//���@�̍��������ʂ�艺
		if (posWave.y <= pos.y)
		{
			//�����t���O�𗧂Ă�
			m_bInSea = true;
			//�����Ԃ��̐���
			CSpray::Create(D3DXVECTOR3(pos.x, pos.y - 30.0f, pos.z));
			//���@������������
			CObject2D::SetColor(D3DXCOLOR(0.0f, 0.0f, 0.3f, 1.0f));
		}
	}
	// �C�ɓ����Ă���Ȃ�
	else if (m_bInSea == true)
	{
		//���@�̍��������ʂ���
		if (posWave.y > pos.y)
		{
			//�����t���O��܂�
			m_bInSea = false;
			//���@�̐F�����ɖ߂�
			CObject2D::SetColor(DEFAULT_COL);
		}
		//�C�ɓ����Ă�̂ɐF�������l��������
		else if (GetColor() == DEFAULT_COL)
		{
			//���@������������
			CObject2D::SetColor(D3DXCOLOR(0.0f, 0.0f, 0.3f, 1.0f));
		}
	}
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
// ��ԊǗ�
//-----------------------------------------------------------------------------
void CPlayer::State()
{
	switch (m_state)
	{
	case STATE_NORMAL:			//�v���C���[�̏�Ԃ��ʏ�̏ꍇ
		break;
	case STATE_RESPAWN:			//�v���C���[���_���[�W��Ԃ̏ꍇ
		//��ԃJ�E���^�[�̌��Z
		m_nCntState--;
		//�E����o��������
		if (m_nCntState >= 150)
		{
			D3DXVECTOR3 pos = CObject2D::GetPosition();
			pos.x += 10.0f;
			CObject2D::SetPosition(pos);
			if (m_nCntState == 150)
			{
				//����\�ɂ���
				m_bControl = true;
			}
		}

		//�_�ł�����
		if (m_nCntState % 4 == 0)
		{//���F
			CObject2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
		}
		else
		{//�F�Ȃ�
			CObject2D::SetColor(DEFAULT_COL);
		}

		//��Ԃ�ʏ�ɖ߂�
		if (m_nCntState <= 0)
		{
			m_state = STATE_NORMAL;

			CObject2D::SetColor(DEFAULT_COL);
		}
		break;
	case STATE_DIE:			//�v���C���[�����S��Ԃ̏ꍇ
		//��ԃJ�E���^�[�̌��Z
		m_nCntState--;

		if (m_nCntState <= 0)
		{
			m_state = STATE_RESPAWN;
			m_nCntState = 180;
		}
		break;
	}
}

//-----------------------------------------------------------------------------
// �_���[�W����
//-----------------------------------------------------------------------------
void CPlayer::Damage(int nDamage)
{
	// ���S����
	Die();
}

//-----------------------------------------------------------------------------
// ���S����
//-----------------------------------------------------------------------------
void CPlayer::Die()
{
	// ����s�\�ɂ���
	m_bControl = false;
	// ���X�|�[����Ԃɂ���
	m_state = STATE_DIE;
	m_nCntState = 60;
	//�A�j���[�V�����ԍ������Z�b�g����
	m_nTexRotType = TYPE_NEUTRAL;
	D3DXVECTOR2 size = GetSize();
	// �����̐���
	CExplosion::Create(CObject2D::GetPosition(), D3DXVECTOR2(size.x, size.y * 2));
	//�v���C���[�̈ʒu�����[�ɐݒ肷��
	CObject2D::SetPosition(D3DXVECTOR3(-SIZE_X, CRenderer::SCREEN_HEIGHT / 2, 0.0f));
}
