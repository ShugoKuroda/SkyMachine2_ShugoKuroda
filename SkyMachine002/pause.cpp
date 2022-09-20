//=============================================================================
//
// �|�[�Y��ʏ�� [pause.cpp]
// Author:SHUGO KURODA
//
//=============================================================================
#include "pause.h"
#include "game.h"
#include "fade.h"
#include "input_keyboard.h"
#include "sound.h"
#include "manager.h"
#include "renderer.h"
#include "object2D.h"
#include "input_joypad.h"

//-----------------------------------------------------------------------------
// �ÓI�����o�ϐ�
//-----------------------------------------------------------------------------
// �e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 CPause::m_apTexture[TYPE_MAX] = {};
CObject2D *CPause::m_apObject2D[TYPE_MAX] = {};

//=============================================================================
// CBullet�̃R���X�g���N�^
//=============================================================================
CPause::CPause() :m_bPause(false), m_nPauseSelect(0)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_apObject2D[nCnt] = nullptr;
	}

	// �I�u�W�F�N�g�^�C�v�̐ݒ�
	SetObjType(OBJ_PAUSE);
}

//=============================================================================
// CPlayer�̃f�X�g���N�^
//=============================================================================
CPause::~CPause()
{

}

//-----------------------------------------------------------------------------------------------
//	�e�N�X�`���ǂݍ���
//-----------------------------------------------------------------------------------------------
HRESULT CPause::Load()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause000.jpg", &m_apTexture[TYPE_FRAME]);		// �|�[�Y��ʘg
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause001.png", &m_apTexture[TYPE_RESUME]);		// �ĊJ
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause002.png", &m_apTexture[TYPE_RESTART]);	// ��蒼��
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause003.png", &m_apTexture[TYPE_EXIT]);		// �^�C�g���ɖ߂�
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause003.png", &m_apTexture[TYPE_SELECTOR]);	// �I���J�[�\��

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
//	�e�N�X�`���j��
//-----------------------------------------------------------------------------------------------
void CPause::Unload()
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

//=============================================================================
// ��������
//=============================================================================
CPause *CPause::Create()
{
	// �|�C���^�ϐ��̐���
	CPause *pPause = new CPause;

	// NULL�`�F�b�N
	if (pPause != nullptr)
	{
		// ����������
		pPause->Init();
	}

	return pPause;
}

//=============================================================================
// �|�[�Y�̏���������
//=============================================================================
HRESULT CPause::Init()
{
	SetPause();

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{// �������ƃe�N�X�`���̐ݒ�
		m_apObject2D[nCnt]->Init();
		m_apObject2D[nCnt]->BindTexture(m_apTexture[nCnt]);
	}

	return S_OK;
}

//=============================================================================
// �|�[�Y�̏I������
//=============================================================================
void CPause::Uninit()
{
	// �|�[�Y��Ԃ�false�ɂ���
	CManager::SetPause(false);

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_apObject2D[nCnt] != nullptr)
		{
			m_apObject2D[nCnt]->Uninit();
			m_apObject2D[nCnt] = nullptr;
		}
	}

	// �I�u�W�F�N�g�j��
	Release();
}

//=============================================================================
// �|�[�Y�̍X�V����
//=============================================================================
void CPause::Update()
{
	if (m_bPause == true)
	{
		// �L�[�{�[�h���̎擾
		CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
		// �Q�[���p�b�h���̎擾
		CInputJoypad *pJoypad = CManager::GetInputJoypad();

		if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_UP) || pJoypad->GetTrigger(CInputJoypad::JOYKEY_UP, 0))
		{
			// ���ݑI������Ă��鍀�ڂ̐F�����ɖ߂�
			m_apObject2D[m_nPauseSelect]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			// ��ɂ����Ƃ��ԍ������炷
			m_nPauseSelect--;

			// �|�[�Y���ڂ̍ŏ㕔�𒴂����Ƃ�
			if (m_nPauseSelect < TYPE_RESUME)
			{
				m_nPauseSelect = TYPE_EXIT;
			}

			// ���ʉ�
			CSound::Play(CSound::SOUND_LABEL_SE_MENU_SELECT);
		}
		else if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_DOWN) || pJoypad->GetTrigger(CInputJoypad::JOYKEY_DOWN, 0))
		{
			// ���ɂ����Ƃ��ԍ��𑝂₷
			m_nPauseSelect++;

			// �|�[�Y���ڂ̍ŉ����𒴂����Ƃ�
			if (m_nPauseSelect > TYPE_EXIT)
			{
				m_nPauseSelect = TYPE_RESUME;
			}

			// ���ʉ�
			CSound::Play(CSound::SOUND_LABEL_SE_MENU_SELECT);
		}

		// ���ݑI������Ă��鍀�ڂ͐F�����F�ɐݒ�
		m_apObject2D[m_nPauseSelect]->SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

		if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_OK) || pJoypad->GetTrigger(CInputJoypad::JOYKEY_A, 0))
		{
			// �I������Ă���UI���Q�Ƃ��A�ǂ̏��������邩����
			switch (m_nPauseSelect)
			{
				// �ĊJ�{�^��
			case TYPE_RESUME:
				// �|�[�Y�����
				CManager::SetPause(false);
				Uninit();
				return;
				// ���g���C�{�^��
			case TYPE_RESTART:
				CFade::SetFade(CFade::FADE_OUT, CManager::MODE_GAME);
				Uninit();
				return;
				// �I���{�^��
			case TYPE_EXIT:
				CFade::SetFade(CFade::FADE_OUT, CManager::MODE_TITLE);
				Uninit();
				return;
			default:
				break;
			}
		}

		// �|�[�Y��ʂ��I����
		if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_PAUSE) || pJoypad->GetTrigger(CInputJoypad::JOYKEY_START, 0))
		{
			SetPause();
			Uninit();
		}
	}
}

//=============================================================================
// �|�[�Y�̕`�揈��
//=============================================================================
void CPause::Draw(void)
{

}

//=============================================================================
// �|�[�Y�̏���
//=============================================================================
void CPause::SetPause()
{
	m_bPause = m_bPause ? false : true;

	if (m_bPause == true)
	{
		CSound::Play(CSound::SOUND_LABEL_SE_MENU_IN);
	}
	else if (m_bPause == false)
	{
		CSound::Play(CSound::SOUND_LABEL_SE_MENU_OUT);

		// �I�����Ă��郍�S���ĊJ�ɂ���
		m_nPauseSelect = 0;
	}

	CManager::SetPause(m_bPause);
}
