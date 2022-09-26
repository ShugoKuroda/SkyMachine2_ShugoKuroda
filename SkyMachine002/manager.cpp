//=============================================================================
//
// �}�l�[�W���[���� [manager.cpp]
// Author : 
//
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "base.h"
#include "fade.h"
#include "sound.h"
#include "load.h"

#include "input_keyboard.h"
#include "input_joypad.h"
#include "input_mouse.h"

#include "object.h"
#include "title.h"
#include "game.h"
#include "result.h"

#include "continue.h"

#include <time.h>

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
CBase *CManager::m_pBase = nullptr;
CRenderer *CManager::m_pRenderer = nullptr;
CInputKeyboard *CManager::m_pInputKeyboard = nullptr;
CInputJoypad *CManager::m_pInputJoypad = nullptr;
CInputMouse *CManager::m_pInputMouse = nullptr;
CSound *CManager::m_pSound = nullptr;	
// �t�F�[�h�N���X
CFade* CManager::m_pFade = nullptr;// �T�E���h���̃|�C���^
bool CManager::m_bPause = false;
bool CManager::m_bEntry[CPlayer::PLAYER_MAX] = { false };

CManager::MODE CManager::m_mode = MODE_TITLE;

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CManager::CManager()
{
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CManager::~CManager()
{
}

//-----------------------------------------------------------------------------
// ����������
//-----------------------------------------------------------------------------
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	// ���ݎ����ɂ���ă����_���ɐݒ肷��
	srand((unsigned)time(NULL));

	// �����_���[�̏���������
	m_pRenderer = new CRenderer;

	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Init(hWnd, bWindow);
	}

	// �L�[�{�[�h�̏���������
	m_pInputKeyboard = new CInputKeyboard;

	if (m_pInputKeyboard != nullptr)
	{
		m_pInputKeyboard->Init(hInstance, hWnd);
	}

	// �W���C�p�b�h���̏���������
	m_pInputJoypad = new CInputJoypad;

	if (m_pInputJoypad != nullptr)
	{
		m_pInputJoypad->Init();
	}

	// �}�E�X���̏���������
	m_pInputMouse = new CInputMouse;

	if (m_pInputMouse != nullptr)
	{
		m_pInputMouse->Init(hInstance, hWnd);
	}

	// �T�E���h�̏���������
	m_pSound = new CSound;

	if (m_pSound != nullptr)
	{
		m_pSound->Init(hWnd);
	}

	// �t�F�[�h�̐���
	if (m_pFade == nullptr)
	{
		m_pFade = CFade::Create(m_mode);
	}

	//�G�z�u���̃��[�h
	LoadSpace::LoadEnemy(hWnd);

	// ���[�h�̐ݒ�
	SetMode(MODE_TITLE);

	return S_OK;
}

//-----------------------------------------------------------------------------
// �I������
//-----------------------------------------------------------------------------
void CManager::Uninit()
{
	// �T�E���h�̏I������
	if (m_pSound != nullptr)
	{
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = nullptr;
	}

	// �����_���̏I������
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	// �t�F�[�h�̔j��
	if (m_pFade != nullptr)
	{
		m_pFade->Uninit();
		m_pFade = nullptr;
	}

	// �I�u�W�F�N�g�̏I������
	CObject::ReleaseAll();

	// �x�[�X�̔j��
	if (m_pBase != nullptr)
	{
		m_pBase->Uninit();
		delete m_pBase;
		m_pBase = nullptr;
	}

	// �L�[�{�[�h�̏I������
	if (m_pInputKeyboard != nullptr)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = nullptr;
	}

	// �W���C�p�b�h�̏I������
	if (m_pInputJoypad != nullptr)
	{
		m_pInputJoypad->Uninit();
		delete m_pInputJoypad;
		m_pInputJoypad = nullptr;
	}

	// �}�E�X�̏I������
	if (m_pInputMouse != nullptr)
	{
		m_pInputMouse->Uninit();
		delete m_pInputMouse;
		m_pInputMouse = nullptr;
	}
}

//-----------------------------------------------------------------------------
// �X�V����
//-----------------------------------------------------------------------------
void CManager::Update()
{
	// �L�[�{�[�h�̍X�V����(���ŏ��ɍs��)
	if (m_pInputKeyboard != nullptr)
	{
		m_pInputKeyboard->Update();
	}

	// �W���C�p�b�h�̍X�V����
	if (m_pInputJoypad != nullptr)
	{
		m_pInputJoypad->Update();
	}

	// �}�E�X�̍X�V����
	if (m_pInputMouse != nullptr)
	{
		m_pInputMouse->Update();
	}

	// �t�F�[�h�̍X�V
	if (m_pFade != nullptr)
	{
		m_pFade->Update();
	}

	//�|�[�Y���łȂ��Ȃ�
	if (m_bPause == false)
	{
		// �x�[�X�̍X�V����
		if (m_pBase != nullptr)
		{
			m_pBase->Update();
		}
	}

	// �����_���̍X�V����
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Update();
	}

	// �L�[�{�[�h���̎擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	// �W���C�p�b�h���̎擾
	CInputJoypad *pJoypad = CManager::GetInputJoypad();

	// �R���e�B�j���[���o��Ԃ̎擾
	bool bContinue = CContinue::GetContinue();

	// �R���e�B�j���[���o���ł͂Ȃ��Ȃ�
	if (bContinue == false)
	{
		for (int nCnt = CInputKeyboard::KEYINFO_OK; nCnt < CInputKeyboard::KEYINFO_MAX; nCnt++)
		{
			// �L�[�{�[�h��ENTRY����
			if (pKeyboard->GetTrigger(nCnt) && m_bEntry[CPlayer::PLAYER_1] == false)
			{
				// �{�^�����������R���g���[���[��ENTRY��Ԃɂ���
				SetEntry(CPlayer::PLAYER_1, true);
				// �G���g���[��
				CSound::Play(CSound::SOUND_LABEL_SE_MENU_IN);

				break;
			}
		}

		// �v���C���[�̃G���g���[����
		for (int nCntController = 0; nCntController < CPlayer::PLAYER_MAX; nCntController++)
		{
			for (int nCnt = CInputJoypad::JOYKEY_UP; nCnt < CInputJoypad::JOYKEY_MAX; nCnt++)
			{
				if (pJoypad->GetTrigger((CInputJoypad::JOYKEY)nCnt, nCntController) && m_bEntry[nCntController] == false)
				{
					// �{�^�����������R���g���[���[��ENTRY��Ԃɂ���
					SetEntry(nCntController, true);
					// �G���g���[��
					CSound::Play(CSound::SOUND_LABEL_SE_MENU_IN);
					break;
				}
			}
		}
	}
	// �R���e�B�j���[���o���Ȃ�
	if (bContinue == true)
	{
		// �L�[�{�[�h��ENTRY����
		if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_OK) && m_bEntry[CPlayer::PLAYER_1] == false)
		{
			// �{�^�����������R���g���[���[��ENTRY��Ԃɂ���
			SetEntry(CPlayer::PLAYER_1, true);
			// �G���g���[��
			CSound::Play(CSound::SOUND_LABEL_SE_MENU_IN);
		}

		// �v���C���[�̃G���g���[����
		for (int nCntController = 0; nCntController < CPlayer::PLAYER_MAX; nCntController++)
		{
			if (pJoypad->GetTrigger(CInputJoypad::JOYKEY_START, nCntController) && m_bEntry[nCntController] == false)
			{
				// �{�^�����������R���g���[���[��ENTRY��Ԃɂ���
				SetEntry(nCntController, true);
				// �G���g���[��
				CSound::Play(CSound::SOUND_LABEL_SE_MENU_IN);
			}
		}
	}
}

//-----------------------------------------------------------------------------
// �`�揈��
//-----------------------------------------------------------------------------
void CManager::Draw()
{
	// �t�F�[�h�̍X�V
	if (m_pFade != nullptr)
	{
		m_pFade->Draw();
	}

	// �����_���̕`�揈��
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Draw();
	}
}

//-----------------------------------------------------------------------------
// ���[�h�̐ݒ�
//-----------------------------------------------------------------------------
void CManager::SetMode(MODE mode)
{
	if (m_pBase != nullptr)
	{// ���݂̉�ʂ�j��
		m_pBase->Uninit();
		delete m_pBase;
		m_pBase = nullptr;
	}

	// �I�u�W�F�N�g�̍폜
	CObject::ReleaseAll();

	// ���[�h�̐ݒ�
	m_mode = mode;

	// ���[�h�̊m��
	switch (m_mode)
	{
	case MODE_TITLE:
		m_pBase = new CTitle;
		break;

	case MODE_GAME:
		m_pBase = new CGame;
		break;

	case MODE_RESULT:
		m_pBase = new CResult;
		break;
	}

	if (m_pBase != nullptr)
	{//����������
		m_pBase->Init();
	}
}
