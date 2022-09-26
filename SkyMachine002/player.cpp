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
#include "ui.h"
#include "gauge.h"
#include "life.h"
#include "score.h"

#include "rank.h"

//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------
#define PLAYER_UI_SIZE		(D3DXVECTOR2(200.0f, 50.0f))
#define LIFE_UI_SIZE		(D3DXVECTOR2(120.0f, 30.0f))
#define LEVEL_UI_SIZE		(D3DXVECTOR2(50.0f, 50.0f))
#define ATTACK_INTERVAL		(7)

//-----------------------------------------------------------------------------
// using�錾
//-----------------------------------------------------------------------------
using namespace LibrarySpace;

//*****************************************************************************
// �萔�錾
//*****************************************************************************
const float CPlayer::SIZE_X = 90.0f;
const float CPlayer::SIZE_Y = 40.0f;
const float CPlayer::ENTRY_SIZE_X = 630.0f;
const float CPlayer::ENTRY_SIZE_Y = 280.0f;
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
// �f�t�H���g�c�@
const int CPlayer::DEFAULT_LIFE = 2;

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
	m_nTexRotType(TYPE_NEUTRAL), m_nPlayerNum(0), posBullet(0.0f, 0.0f), m_bControl(false), m_bInSea(false), m_pOption{ nullptr }, m_pBarrier(nullptr),
	m_OptionLevel(CBulletOption::LEVEL_NONE),m_BarrierLevel(CBarrier::LEVEL_NONE),m_BulletLevel(CPlayer::LEVEL_1), m_pLife(nullptr),m_pScore(nullptr),
	m_bDie(false)
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
CPlayer *CPlayer::Create(const D3DXVECTOR3& pos, const int& nNum)
{
	//�C���X�^���X����
	CPlayer *pPlayer = new CPlayer;

	if (pPlayer != nullptr)
	{
		float fPos = 0.0f;
		// 2P�̂݁A���炵�Ĕz�u
		if (nNum == PLAYER_2)
		{
			fPos = -150.0f;
		}

		// �ʒu�ݒ�
		pPlayer->SetPosition(D3DXVECTOR3(pos.x + fPos, pos.y, pos.z));
		// �v���C���[���̐ݒ�
		pPlayer->m_nPlayerNum = nNum;
		// ��������
		pPlayer->Init();
		// �e�N�X�`���̐ݒ�
		pPlayer->BindTexture(m_apTexture[nNum]);
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
	for (int nCnt = 0; nCnt < PLAYER_MAX; nCnt++)
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
	// �e�̔��ˈʒu��ݒ�
	posBullet = D3DXVECTOR2(20.0f, 10.0f);
	// �o���Ԃɂ���
	m_state = STATE_ENTRY;

	// �T�C�Y�̐ݒ�
	CObject2D::SetSize(D3DXVECTOR2(ENTRY_SIZE_X, ENTRY_SIZE_Y));
	// ������
	CObject2D::Init();
	// �F�̐ݒ�
	CObject2D::SetColor(DEFAULT_COL);
	// �e�N�X�`�����W�̐ݒ�
	CObject2D::SetAnimation(m_nPatternAnim, m_nTexRotType, DIVISION_U, DIVISION_V);

	// �eUI�̐���
	switch (m_nPlayerNum)
	{
		// �v���C���[1�̏ꍇ
	case PLAYER_1:
		// PLAYER1��UI
		CUi::Create(D3DXVECTOR3(120.0f, 25.0f, 0.0f), PLAYER_UI_SIZE, CUi::TYPE_PLAYER1, CUi::ANIM_NONE, (CUi::PARENT)m_nPlayerNum);
		// �c�@UI
		CUi::Create(D3DXVECTOR3(CRenderer::SCREEN_WIDTH / 2 - 150.0f, 30.0f, 0.0f), LIFE_UI_SIZE, CUi::TYPE_LIFE1, CUi::ANIM_NONE, (CUi::PARENT)m_nPlayerNum);
		// ���C���eUI
		CUi::Create(D3DXVECTOR3(50.0f, 80.0f, 0.0f), LEVEL_UI_SIZE, CUi::TYPE_BULLET, CUi::ANIM_NONE, (CUi::PARENT)m_nPlayerNum);
		// �I�v�V�����eUI
		CUi::Create(D3DXVECTOR3(240.0f, 80.0f, 0.0f), LEVEL_UI_SIZE, CUi::TYPE_BULLET_OPTION, CUi::ANIM_NONE, (CUi::PARENT)m_nPlayerNum);
		// �o���AUI
		CUi::Create(D3DXVECTOR3(430.0f, 80.0f, 0.0f), LEVEL_UI_SIZE, CUi::TYPE_BARRIER, CUi::ANIM_NONE, (CUi::PARENT)m_nPlayerNum);

		// ���C���e�Q�[�WUI
		CGauge::Create(D3DXVECTOR3(140.0f, 80.0f, 0.0f), CGauge::TYPE_BULLET, (CGauge::PARENT)m_nPlayerNum);
		// �I�v�V�����e�Q�[�WUI
		CGauge::Create(D3DXVECTOR3(330.0f, 80.0f, 0.0f), CGauge::TYPE_BULLET_OPTION, (CGauge::PARENT)m_nPlayerNum);
		// �o���A�Q�[�WUI
		CGauge::Create(D3DXVECTOR3(520.0f, 80.0f, 0.0f), CGauge::TYPE_BARRIER, (CGauge::PARENT)m_nPlayerNum);

		// �X�R�A�̐���
		m_pScore = CScore::Create(D3DXVECTOR3(250.0f, 25.0f, 0.0f), D3DXVECTOR2(30.0f, 30.0f), 20);
		// ���C�t�̐���
		m_pLife = CLife::Create(D3DXVECTOR3(CRenderer::SCREEN_WIDTH / 2 - 100.0f, 30.0f, 0.0f), D3DXVECTOR2(30.0f, 30.0f));

		break;

	case PLAYER_2:
		// PLAYER2��UI
		CUi::Create(D3DXVECTOR3(820.0f, 25.0f, 0.0f), PLAYER_UI_SIZE, CUi::TYPE_PLAYER2, CUi::ANIM_NONE, (CUi::PARENT)m_nPlayerNum);
		// �c�@UI
		CUi::Create(D3DXVECTOR3(CRenderer::SCREEN_WIDTH - 100.0f, 30.0f, 0.0f), LIFE_UI_SIZE, CUi::TYPE_LIFE2, CUi::ANIM_NONE, (CUi::PARENT)m_nPlayerNum);
		// ���C���eUI
		CUi::Create(D3DXVECTOR3(730.0f, 80.0f, 0.0f), LEVEL_UI_SIZE, CUi::TYPE_BULLET, CUi::ANIM_NONE, (CUi::PARENT)m_nPlayerNum);
		// �I�v�V�����eUI
		CUi::Create(D3DXVECTOR3(920.0f, 80.0f, 0.0f), LEVEL_UI_SIZE, CUi::TYPE_BULLET_OPTION, CUi::ANIM_NONE, (CUi::PARENT)m_nPlayerNum);
		// �o���AUI
		CUi::Create(D3DXVECTOR3(1110.0f, 80.0f, 0.0f), LEVEL_UI_SIZE, CUi::TYPE_BARRIER, CUi::ANIM_NONE, (CUi::PARENT)m_nPlayerNum);

		// ���C���e�Q�[�WUI
		CGauge::Create(D3DXVECTOR3(830.0f, 80.0f, 0.0f), CGauge::TYPE_BULLET, (CGauge::PARENT)m_nPlayerNum);
		// �I�v�V�����e�Q�[�WUI
		CGauge::Create(D3DXVECTOR3(1020.0f, 80.0f, 0.0f), CGauge::TYPE_BULLET_OPTION, (CGauge::PARENT)m_nPlayerNum);
		// �o���A�Q�[�WUI
		CGauge::Create(D3DXVECTOR3(1210.0f, 80.0f, 0.0f), CGauge::TYPE_BARRIER, (CGauge::PARENT)m_nPlayerNum);

		// �X�R�A�̐���
		m_pScore = CScore::Create(D3DXVECTOR3(950.0f, 25.0f, 0.0f), D3DXVECTOR2(30.0f, 30.0f), 20);
		// ���C�t�̐���
		m_pLife = CLife::Create(D3DXVECTOR3(CRenderer::SCREEN_WIDTH - 50.0f, 30.0f, 0.0f), D3DXVECTOR2(30.0f, 30.0f));

		break;

	default:
		break;
	}

	// ���C�t�̐ݒ�
	m_pLife->Add(DEFAULT_LIFE);

	// �v���C���[���S��
	CSound::Play(CSound::SOUND_LABEL_SE_ENTRY);

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

		if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_ATTACK) == true ||
			pJoypad->GetPress(CInputJoypad::JOYKEY_A, m_nPlayerNum) ||
			pJoypad->GetPress(CInputJoypad::JOYKEY_B, m_nPlayerNum))
		{//�U���L�[����

			//�U���J�E���^�[�̉��Z
			m_nCntAttack++;

			if (m_nCntAttack > ATTACK_INTERVAL)
			{
				// �v���C���[�̋�����Ԃɂ���ďo���e��ݒ�
				switch (m_BulletLevel)
				{
				case CPlayer::LEVEL_1:
					//�e�̐ݒ�
					CBullet::Create(D3DXVECTOR3(pos.x + posBullet.x, pos.y + posBullet.y, pos.z), D3DXVECTOR3(15.0f, 0.0f, 0.0f),
						5, CBullet::TYPE_PLAYER_BLUE)->SetParent((CBullet::EParent)m_nPlayerNum);
					break;

				case CPlayer::LEVEL_2:
					//�e1�̐ݒ�
					CBullet::Create(D3DXVECTOR3(pos.x + posBullet.x, pos.y + posBullet.y - 15.0f, pos.z), D3DXVECTOR3(15.0f, 0.0f, 0.0f),
						5, CBullet::TYPE_PLAYER_BLUE)->SetParent((CBullet::EParent)m_nPlayerNum);
					//�e2�̐ݒ�
					CBullet::Create(D3DXVECTOR3(pos.x + posBullet.x, pos.y + posBullet.y + 15.0f, pos.z), D3DXVECTOR3(15.0f, 0.0f, 0.0f),
						5, CBullet::TYPE_PLAYER_BLUE)->SetParent((CBullet::EParent)m_nPlayerNum);
					break;

				case CPlayer::LEVEL_3:
					//�e1�̐ݒ�
					CBullet::Create(D3DXVECTOR3(pos.x + posBullet.x, pos.y + posBullet.y, pos.z), D3DXVECTOR3(15.0f, 0.0f, 0.0f),
						5, CBullet::TYPE_PLAYER_BLUE)->SetParent((CBullet::EParent)m_nPlayerNum);
					//�e2�̐ݒ�
					CBullet::Create(D3DXVECTOR3(pos.x + posBullet.x, pos.y + posBullet.y - 30.0f, pos.z), D3DXVECTOR3(15.0f, 0.0f, 0.0f),
						5, CBullet::TYPE_PLAYER_BLUE)->SetParent((CBullet::EParent)m_nPlayerNum);
					//�e3�̐ݒ�
					CBullet::Create(D3DXVECTOR3(pos.x + posBullet.x, pos.y + posBullet.y + 30.0f, pos.z), D3DXVECTOR3(15.0f, 0.0f, 0.0f),
						5, CBullet::TYPE_PLAYER_BLUE)->SetParent((CBullet::EParent)m_nPlayerNum);
					break;

				default:
					break;
				}

				// �I�v�V�����e�̍U������
				for (int nCnt = 0; nCnt < MAX_OPTION; nCnt++)
				{
					if (m_pOption[nCnt] != nullptr)
					{
						m_pOption[nCnt]->Attack();
					}
				}

				//�U���J�E���^�[�����Z�b�g
				m_nCntAttack = 0;

				//�T�E���h�Đ�
				CSound::Play(CSound::SOUND_LABEL_SE_SHOT);
			}
		}
		else
		{
			m_nCntAttack = 10;
		}

		//���͈͂���ʓ��ɐ���
		SteyInScreen2D(&pos, CObject2D::GetSize());
	}

	//�����Ԃ���ݒ�
	SetSpray();

	//�ʒu���X�V
	CObject2D::SetPosition(pos);

	//��ԊǗ�
	State();

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

	if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_LEFT) == true ||
		pJoypad->GetPress(CInputJoypad::JOYKEY_LEFT, m_nPlayerNum) == true ||
		pJoypad->GetStick(CInputJoypad::JOYKEY_LEFT_STICK, m_nPlayerNum).x <= -0.2f)
	{//���L�[����
		if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_UP) == true ||
			pJoypad->GetPress(CInputJoypad::JOYKEY_UP, m_nPlayerNum) == true ||
			pJoypad->GetStick(CInputJoypad::JOYKEY_LEFT_STICK, m_nPlayerNum).y <= -0.2f)
		{//��L�[����
			//�ړ��ʉ��Z
			pos.x += GetSinVec(-0.75f, MOVE_DEFAULT);
			pos.y += GetCosVec(-0.75f, MOVE_DEFAULT);
			//�A�j���[�V�����ύX
			SetAnimNum(TYPE_DOWN, TYPE_UP);
			m_nCntAnimMove++;
			//�e�̔��ˈʒu��ݒ�
			posBullet = D3DXVECTOR2(20.0f, 10.0f);
		}
		else if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_DOWN) == true ||
			pJoypad->GetPress(CInputJoypad::JOYKEY_DOWN, m_nPlayerNum) == true ||
			pJoypad->GetStick(CInputJoypad::JOYKEY_LEFT_STICK, m_nPlayerNum).y >= 0.2f)
		{//���L�[����
			pos.x += GetSinVec(-0.25f, MOVE_DEFAULT);
			pos.y += GetCosVec(-0.25f, MOVE_DEFAULT);
			SetAnimNum(TYPE_UP, TYPE_DOWN);
			m_nCntAnimMove++;
			//�e�̔��ˈʒu��ݒ�
			posBullet = D3DXVECTOR2(20.0f, -5.0f);
		}
		else
		{
			pos.x += GetSinVec(-0.5f, MOVE_DEFAULT);
			pos.y += GetCosVec(-0.5f, MOVE_DEFAULT);
			m_nTexRotType = TYPE_NEUTRAL;
			m_nCntAnimMove = 0;
			//�e�̔��ˈʒu��ݒ�
			posBullet = D3DXVECTOR2(20.0f, 10.0f);
		}
	}
	else if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_RIGHT) == true ||
		pJoypad->GetPress(CInputJoypad::JOYKEY_RIGHT, m_nPlayerNum) == true ||
		pJoypad->GetStick(CInputJoypad::JOYKEY_LEFT_STICK, m_nPlayerNum).x >= 0.2f)
	{//�E�L�[����
		if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_UP) == true ||
			pJoypad->GetPress(CInputJoypad::JOYKEY_UP, m_nPlayerNum) == true ||
			pJoypad->GetStick(CInputJoypad::JOYKEY_LEFT_STICK, m_nPlayerNum).y <= -0.2f)
		{//��L�[����
			pos.x += GetSinVec(0.75f, MOVE_DEFAULT);
			pos.y += GetCosVec(0.75f, MOVE_DEFAULT);
			SetAnimNum(TYPE_DOWN, TYPE_UP);
			m_nCntAnimMove++;
			//�e�̔��ˈʒu��ݒ�
			posBullet = D3DXVECTOR2(20.0f, 10.0f);
		}
		else if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_DOWN) == true ||
			pJoypad->GetPress(CInputJoypad::JOYKEY_DOWN, m_nPlayerNum) == true ||
			pJoypad->GetStick(CInputJoypad::JOYKEY_LEFT_STICK, m_nPlayerNum).y >= 0.2f)
		{//���L�[����
			pos.x += GetSinVec(0.25f, MOVE_DEFAULT);
			pos.y += GetCosVec(0.25f, MOVE_DEFAULT);
			SetAnimNum(TYPE_UP, TYPE_DOWN);
			m_nCntAnimMove++;
			//�e�̔��ˈʒu��ݒ�
			posBullet = D3DXVECTOR2(20.0f, -5.0f);
		}
		else
		{
			pos.x += GetSinVec(0.5f, MOVE_DEFAULT);
			pos.y += GetCosVec(0.5f, MOVE_DEFAULT);
			m_nTexRotType = TYPE_NEUTRAL;
			m_nCntAnimMove = 0;
			//�e�̔��ˈʒu��ݒ�
			posBullet = D3DXVECTOR2(20.0f, 10.0f);
		}
	}
	else if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_UP) == true ||
		pJoypad->GetPress(CInputJoypad::JOYKEY_UP, m_nPlayerNum) == true ||
		pJoypad->GetStick(CInputJoypad::JOYKEY_LEFT_STICK, m_nPlayerNum).y <= -0.2f)
	{//��L�[����
		pos.x += GetSinVec(1.0f, MOVE_DEFAULT);
		pos.y += GetCosVec(1.0f, MOVE_DEFAULT);
		SetAnimNum(TYPE_DOWN, TYPE_UP);
		m_nCntAnimMove++;
		//�e�̔��ˈʒu��ݒ�
		posBullet = D3DXVECTOR2(20.0f, 10.0f);
	}
	else if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_DOWN) == true ||
		pJoypad->GetPress(CInputJoypad::JOYKEY_DOWN, m_nPlayerNum) == true ||
		pJoypad->GetStick(CInputJoypad::JOYKEY_LEFT_STICK, m_nPlayerNum).y >= 0.2f)
	{//���L�[����
		pos.x += GetSinVec(0.0f, MOVE_DEFAULT);
		pos.y += GetCosVec(0.0f, MOVE_DEFAULT);
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
// ������Ԃ̐ݒ�
//-----------------------------------------------------------------------------
void CPlayer::SetLevel(CItem::EType type)
{
	// �ʒu�̎擾
	D3DXVECTOR3 pos = GetPosition();

	// �擾�����A�C�e���ɂ���ċ�����Ԃ�ύX
	switch (type)
	{
		// ���C���e����
	case CItem::TYPE_RED:

		// �I�v�V�����e��Ԃ�LEVEL_3(�ő勭��)�ȊO�̏ꍇ
		if (m_BulletLevel != CPlayer::LEVEL_3)
		{// ������Ԃ�i�߂�
			m_BulletLevel = (LEVEL)(m_BulletLevel + 1);
		}

		break;
		// �I�v�V�����e����
	case CItem::TYPE_BLUE:

		switch (m_OptionLevel)
		{
		case CBulletOption::LEVEL_NONE:
			m_pOption[0] = CBulletOption::Create(0.0f, (CBulletOption::PARENT)m_nPlayerNum);
			break;

		case CBulletOption::LEVEL_1:
			m_pOption[0]->SetRad(0.0f);
			m_pOption[1] = CBulletOption::Create(D3DX_PI, (CBulletOption::PARENT)m_nPlayerNum);
			break;

		case CBulletOption::LEVEL_2:
			m_pOption[0]->SetRad(0.0f);
			m_pOption[1]->SetRad(D3DX_PI);
			m_pOption[2] = CBulletOption::Create(D3DX_PI / 2, (CBulletOption::PARENT)m_nPlayerNum);
			m_pOption[3] = CBulletOption::Create(D3DX_PI * 1.5f, (CBulletOption::PARENT)m_nPlayerNum);
			break;
		case CBulletOption::LEVEL_3:
			break;
		default:
			break;
		}

		// �I�v�V�����e��Ԃ�LEVEL_3(�ő勭��)�ȊO�̏ꍇ
		if (m_OptionLevel != CBulletOption::LEVEL_3)
		{// ������Ԃ�i�߂�
			m_OptionLevel = (CBulletOption::LEVEL)(m_OptionLevel + 1);
		}
		break;

		// �o���A����
	case CItem::TYPE_GREEN:

		// ���݂̒i�K�ɂ���ċ�����Ԃ�ω�
		switch (m_BarrierLevel)
		{
		case CBarrier::LEVEL_NONE:
			// �o���A�̐���
			m_pBarrier = CBarrier::Create(pos, (CBarrier::PARENT)m_nPlayerNum);
			break;
		case CBarrier::LEVEL_GREEN:
			m_pBarrier->SetBarrier(CBarrier::LEVEL_SILVER);
			break;
		case CBarrier::LEVEL_SILVER:
			m_pBarrier->SetBarrier(CBarrier::LEVEL_GOLD);
			break;
		case CBarrier::LEVEL_GOLD:
			break;
		default:
			break;
		}
		
		// �o���A��Ԃ�GOLD(�ő勭��)�ȊO�̏ꍇ
		if (m_BarrierLevel != CBarrier::LEVEL_GOLD)
		{// ������Ԃ�i�߂�
			m_BarrierLevel = (CBarrier::LEVEL)(m_BarrierLevel + 1);
		}
		break;

	case CItem::TYPE_ORANGE:
		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------------
// ��ԊǗ�
//-----------------------------------------------------------------------------
void CPlayer::State()
{
	// �Ίp��(�T�C�Y)�̎擾
	D3DXVECTOR2 size = CObject2D::GetSize();
	// �ʒu�̎擾
	D3DXVECTOR3 pos = CObject2D::GetPosition();

	switch (m_state)
	{
	case STATE_NORMAL:			//�v���C���[�̏�Ԃ��ʏ�̏ꍇ
		break;
	case STATE_ENTRY:			//�o���Ԃ̏ꍇ

		m_nCntState++;

		// �Ίp��(�T�C�Y)�̌��Z
		size -= D3DXVECTOR2(4.46f, 1.98f);
		// �Ίp���̎擾
		CObject2D::SetSize(size);

		if (pos.y <= CRenderer::SCREEN_HEIGHT / 2)
		{
			m_bControl = true;
			m_state = STATE_RESPAWN;
			m_nCntState = 150;
		}
		else if (pos.y <= CRenderer::SCREEN_HEIGHT / 1.5f)
		{
			pos += D3DXVECTOR3(-20.0f, -3.0f, 0.0f);
		}
		else
		{
			pos += D3DXVECTOR3(22.0f, -3.0f, 0.0f);
		}

		if (m_nCntState >= 96)
		{
			m_nTexRotType = TYPE_NEUTRAL;
		}
		else if (m_nCntState >= 72)
		{
			m_nTexRotType = TYPE_DOWN;
		}
		else if (m_nCntState >= 48)
		{
			m_nTexRotType = TYPE_NEUTRAL;
		}
		else if (m_nCntState >= 24)
		{
			m_nTexRotType = TYPE_UP;
		}

		CObject2D::SetPosition(pos);

		break;
	case STATE_RESPAWN:			//�v���C���[���_���[�W��Ԃ̏ꍇ
		//��ԃJ�E���^�[�̌��Z
		m_nCntState--;
		//�E����o��������
		if (m_nCntState >= 150)
		{
			// �ʒu�̉��Z
			pos.x += 10.0f;
			// �ʒu�̐ݒ�
			CObject2D::SetPosition(pos);

			// �J�E���^�[�����
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
void CPlayer::Damage()
{
	if (m_BarrierLevel == CBarrier::LEVEL_NONE)
	{
		// ���S����
		Die();

		// �v���C���[���S��
		CSound::Play(CSound::SOUND_LABEL_SE_DIE_PLAYER);
	}
	else
	{
		m_BarrierLevel = (CBarrier::LEVEL)(m_BarrierLevel - 1);

		if (m_BarrierLevel == CBarrier::LEVEL_NONE)
		{
			if (m_pBarrier != nullptr)
			{
				m_pBarrier->Uninit();
				m_pBarrier = nullptr;
			}
		}
		else
		{
			m_pBarrier->SetBarrier(m_BarrierLevel);
		}
		m_state = STATE_RESPAWN;
		m_nCntState = 150;
	}
}

//-----------------------------------------------------------------------------
// ���S����
//-----------------------------------------------------------------------------
void CPlayer::Die()
{
	// ���C�t���j������Ă��Ȃ����
	if (m_pLife != nullptr)
	{
		// ���C�t�����炷
		m_pLife->Add(-1);

		// ���C�t��0����
		if (m_pLife->GetLife() < 0)
		{
			// �X�R�A�̔j��
			if (m_pScore != nullptr)
			{
				m_pScore->Uninit();
				m_pScore = nullptr;
			}

			// ���C�t�̔j��
			m_pLife->Uninit();
			m_pLife = nullptr;

			// �v���C���[�����S��Ԃɂ���
			m_bDie = true;

			// �T�C�Y�̎擾
			D3DXVECTOR2 size = GetSize();
			// �����̐���
			CExplosion::Create(CObject2D::GetPosition(), D3DXVECTOR2(size.x, size.y * 2));

			return;
		}

		// ����s�\�ɂ���
		m_bControl = false;
		// ���X�|�[����Ԃɂ���
		m_state = STATE_DIE;
		m_nCntState = 60;

		// �ϐ��̃��Z�b�g
		m_nTexRotType = TYPE_NEUTRAL;		//�A�j���[�V�����ԍ������Z�b�g����

		// �o���A�̔j��
		if (m_pBarrier != nullptr)
		{
			m_pBarrier->Uninit();
			m_pBarrier = nullptr;
		}

		// �I�v�V�����̔j��
		for (int nCnt = 0; nCnt < MAX_OPTION; nCnt++)
		{
			if (m_pOption[nCnt] != nullptr)
			{
				m_pOption[nCnt]->Uninit();
				m_pOption[nCnt] = nullptr;
			}
		}

		// �e������Ԃ�����������
		m_OptionLevel = CBulletOption::LEVEL_NONE;
		m_BarrierLevel = CBarrier::LEVEL_NONE;
		m_BulletLevel = CPlayer::LEVEL_1;

		// �T�C�Y�̎擾
		D3DXVECTOR2 size = GetSize();
		// �����̐���
		CExplosion::Create(CObject2D::GetPosition(), D3DXVECTOR2(size.x, size.y * 2));

		//�v���C���[�̈ʒu�����[�ɐݒ肷��
		if (m_nPlayerNum == PLAYER_1)
		{// 1P�̏ꍇ
			CObject2D::SetPosition(D3DXVECTOR3(-SIZE_X, CRenderer::SCREEN_HEIGHT / 2, 0.0f));
		}
		else
		{// 2P�̏ꍇ
			CObject2D::SetPosition(D3DXVECTOR3(-SIZE_X, CRenderer::SCREEN_HEIGHT / 2 + SIZE_Y, 0.0f));
		}

	}
}
