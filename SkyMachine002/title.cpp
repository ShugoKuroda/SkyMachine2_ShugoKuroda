//===============================================================================================
//
// �^�C�g����ʏ��� [title.cpp]
// Author : SHUGO KURODA
//
//===============================================================================================
#include "title.h"
#include "sound.h"

#include "manager.h"
#include "input_keyboard.h"
#include "input_joypad.h"
#include "object.h"
#include "object2D.h"
#include "library.h"
#include "fade.h"

#include "cloud.h"
#include "fade_scene.h"
#include "library.h"
#include "ui.h"

//-----------------------------------------------------------------------------------------------
// using�錾
//-----------------------------------------------------------------------------------------------
using namespace LibrarySpace;

//-----------------------------------------------------------------------------------------------
// �ÓI�����o�ϐ�
//-----------------------------------------------------------------------------------------------
// �e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 CTitle::m_apTexture[OBJ_MAX] = { nullptr };

//-----------------------------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------------------------
CTitle::CTitle() :m_nCountMoveBg(0), m_bTitleDraw(false), m_bPressFade(false), m_nCntLoop(0), m_bEntry{ false }, m_bTutorial(false), m_move(0.0f,0.0f,0.0f)
{
	for (int nCnt = 0; nCnt < OBJ_MAX - 1; nCnt++)
	{
		m_apObject2D[nCnt] = nullptr;
	}

	// �G���g���[����������
	CManager::SetEntry(0, false);
	CManager::SetEntry(1, false);

	//�_�̐�������������
	ZeroMemory(&m_CloudInfo, sizeof(m_CloudInfo));
}

//-----------------------------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------------------------
CTitle::~CTitle()
{
}

//-----------------------------------------------------------------------------------------------
// �e�N�X�`���̓ǂݍ��ݏ���
//-----------------------------------------------------------------------------------------------
HRESULT CTitle::Load()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title000.jpg", &m_apTexture[BG_SKY]);			// �w�i�̋�
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title001.png", &m_apTexture[LOGO_TITLE]);		// �^�C�g�����S
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title002.png", &m_apTexture[LOGO_PLAYER]);		// �v���C���[���S
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title004.png", &m_apTexture[LOGO_PRESS]);		// PRESS���S
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/tutorial000.png", &m_apTexture[LOGO_TUTORIAL]);// �`���[�g���A��
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title003.png", &m_apTexture[LOGO_SHADOW]);		// �v���C���[���S(��)

	// UI�e�N�X�`���̃��[�h
	CUi::Load();

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// �e�N�X�`���̔j��
//-----------------------------------------------------------------------------------------------
void CTitle::Unload()
{
	for (int nCnt = 0; nCnt < OBJ_MAX; nCnt++)
	{
		// �e�N�X�`���̔j��
		if (m_apTexture[nCnt] != nullptr)
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = nullptr;
		}
	}

	// UI�e�N�X�`���̔j��
	CUi::Unload();
}

//-----------------------------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------------------------
HRESULT CTitle::Init()
{
	// �e�N�X�`���̃��[�h
	CTitle::Load();
	// �_
	CCloud::Load();

	// �X�N���[���T�C�Y�̕ۑ�
	D3DXVECTOR2 ScreenSize = D3DXVECTOR2((float)CRenderer::SCREEN_WIDTH, (float)CRenderer::SCREEN_HEIGHT);

	for (int nCnt = 0; nCnt < OBJ_MAX - 1; nCnt++)
	{// ����
		m_apObject2D[nCnt] = new CObject2D;
	}

	// ��
	m_apObject2D[BG_SKY]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, -ScreenSize.y / 2, 0.0f));
	m_apObject2D[BG_SKY]->SetSize(D3DXVECTOR2(ScreenSize.x, ScreenSize.y * 3));
	// �^�C�g�����S
	m_apObject2D[LOGO_TITLE]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, ScreenSize.y - 450.0f, 0.0f));
	m_apObject2D[LOGO_TITLE]->SetSize(D3DXVECTOR2(ScreenSize.x - 550.0f, ScreenSize.y / 2.5f));
	// �v���C���[���S
	m_apObject2D[LOGO_PLAYER]->SetPosition(D3DXVECTOR3(-ScreenSize.x, 160.0f, 0.0f));
	m_apObject2D[LOGO_PLAYER]->SetSize(D3DXVECTOR2(300.0f, 150.0f));
	// Press���S
	m_apObject2D[LOGO_PRESS]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, ScreenSize.y - 150.0f, 0.0f));
	m_apObject2D[LOGO_PRESS]->SetSize(D3DXVECTOR2(ScreenSize.x - 550.0f, 90.0f));
	// �t�F�[�h
	m_apObject2D[BG_FADE]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, ScreenSize.y / 2, 0.0f));
	m_apObject2D[BG_FADE]->SetSize(D3DXVECTOR2(ScreenSize.x, ScreenSize.y));
	// �`���[�g���A��
	m_apObject2D[LOGO_TUTORIAL]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, ScreenSize.y / 2, 0.0f));
	m_apObject2D[LOGO_TUTORIAL]->SetSize(D3DXVECTOR2(ScreenSize.x, ScreenSize.y));

	for (int nCnt = 0; nCnt < OBJ_MAX - 1; nCnt++)
	{// �������ƃe�N�X�`���̐ݒ�
		m_apObject2D[nCnt]->Init();
		m_apObject2D[nCnt]->BindTexture(m_apTexture[nCnt]);
	}

	//�w�i�ƃv���C���[���S�ȊO��O�ɕ`�悷��(�^�C�v��ݒ肷��)
	for (int nCnt = LOGO_TITLE; nCnt < OBJ_MAX - 1; nCnt++)
	{
		m_apObject2D[nCnt]->SetObjType(CObject::OBJ_TITLE);
	}
	//�v���C���[���S��w�i�̎��ɕ`�悷��
	m_apObject2D[LOGO_PLAYER]->SetObjType(CObject::OBJ_TITLE_LOGO);
	m_apObject2D[BG_FADE]->SetObjType(CObject::OBJ_BLACK);
	m_apObject2D[LOGO_TUTORIAL]->SetObjType(CObject::OBJ_LOGO);

	//�^�C�g����Press���S�𓧖��ɂ���
	m_apObject2D[LOGO_TITLE]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	m_apObject2D[LOGO_PRESS]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	m_apObject2D[BG_FADE]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
	m_apObject2D[LOGO_TUTORIAL]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	// �_�𕡐��A�����z�u����
	for (int nCntCloud = 0; nCntCloud < 25; nCntCloud++)
	{
		// �ʒu��ݒ�
		D3DXVECTOR3 pos;
		pos.x = (float)GetRandNum(CRenderer::SCREEN_WIDTH, 0);
		pos.y = (float)GetRandNum(CRenderer::SCREEN_HEIGHT - 300, CRenderer::SCREEN_HEIGHT * -4);
		pos.z = 0.0f;

		// �_�̐���
		CCloud::Create(pos);
	}

	// �v���C���[����
	for (int nCntPlayer = 0; nCntPlayer < CPlayer::PLAYER_MAX; nCntPlayer++)
	{
		// �v���C���[ENTRY���̎擾
		bool bEntry = CManager::GetEntry(nCntPlayer);

		// �G���g���[���Ă��Ȃ����
		if (bEntry == false)
		{
			switch (nCntPlayer)
			{
			case CPlayer::PLAYER_1:
				// �G���g���[�҂���UI1�𐶐�
				CUi::Create(D3DXVECTOR3((CRenderer::SCREEN_WIDTH / 4) - 50.0f, 40.0f, 0.0f), D3DXVECTOR2(400.0f, 50.0f),
					CUi::TYPE_PRESS_ANY_BUTTON, CUi::ANIM_FLASHING, CUi::PLAYER_1_NOT_ENTRY);
				// �G���g���[�҂���UI2�𐶐�
				CUi::Create(D3DXVECTOR3((CRenderer::SCREEN_WIDTH / 4) - 50.0f, 80.0f, 0.0f), D3DXVECTOR2(400.0f, 50.0f),
					CUi::TYPE_TO_START, CUi::ANIM_NONE, CUi::PLAYER_1_NOT_ENTRY);
				break;

			case CPlayer::PLAYER_2:
				// �G���g���[�҂���UI1�𐶐�
				CUi::Create(D3DXVECTOR3(CRenderer::SCREEN_WIDTH - 300.0f, 40.0f, 0.0f), D3DXVECTOR2(400.0f, 50.0f),
					CUi::TYPE_PRESS_ANY_BUTTON, CUi::ANIM_FLASHING, CUi::PLAYER_2_NOT_ENTRY);
				// �G���g���[�҂���UI2�𐶐�
				CUi::Create(D3DXVECTOR3(CRenderer::SCREEN_WIDTH - 300.0f, 80.0f, 0.0f), D3DXVECTOR2(400.0f, 50.0f),
					CUi::TYPE_TO_START, CUi::ANIM_NONE, CUi::PLAYER_2_NOT_ENTRY);
				break;
			default:
				break;
			}
		}
	}

	// �^�C�g��BGM
	CSound::Play(CSound::SOUND_LABEL_TITLE);

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------------------------
void CTitle::Uninit()
{
	// �|�C���^�̔j��
	for (int nCnt = 0; nCnt < OBJ_MAX; nCnt++)
	{
		if (m_apObject2D[nCnt] != nullptr)
		{
			m_apObject2D[nCnt] = nullptr;
		}
	}

	//�^�C�g���e�N�X�`���̔j��
	CTitle::Unload();
	//�_�e�N�X�`���̔j��
	CCloud::Unload();

	// �^�C�g��BGM
	CSound::Stop();
}

//-----------------------------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------------------------
void CTitle::Update()
{
	// �w�i�A�j���[�V�����p�J�E���^�[�̉��Z
	m_nCountMoveBg++;

	//�w�i�ʒu�̎擾
	D3DXVECTOR3 aPosBg[OBJ_MAX - 1];
	for (int nCnt = 0; nCnt < OBJ_MAX - 1; nCnt++)
	{
		aPosBg[nCnt] = m_apObject2D[nCnt]->GetPosition();
	}

	{
		// �v���C���[��ENTRY���擾
		bool bEntry = CManager::GetEntry(CPlayer::PLAYER_1);

		// �G���g���[���Ă����
		if (bEntry == true)
		{
			// �L�[�{�[�h���̎擾
			CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

			for (int nCnt = CInputKeyboard::KEYINFO_OK; nCnt < CInputKeyboard::KEYINFO_MAX; nCnt++)
			{
				//�L�[�������ꂽ��
				if (pKeyboard->GetTrigger(nCnt) == true)
				{
					// ���艹
					CSound::Play(CSound::SOUND_LABEL_SE_MENU_OK);

					if (m_bTitleDraw == false)
					{
						aPosBg[BG_SKY].y = CRenderer::SCREEN_HEIGHT;
						aPosBg[LOGO_PLAYER].x = 960.0f;
					}
					else
					{
						if (m_bTutorial == true)
						{
							// ���[�h�̐ݒ�
							CManager::GetFade()->SetFade(CFade::FADE_OUT, CManager::MODE::MODE_GAME);
							return;
						}

						m_bTutorial = true;
						m_apObject2D[BG_FADE]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.7f));

						// �F�̎擾
						D3DXCOLOR col = m_apObject2D[LOGO_TUTORIAL]->GetColor();

						if (col.a <= 1.0f)
						{
							col.a += 0.05f;
							m_apObject2D[LOGO_TUTORIAL]->SetColor(col);
						}
					}
				}
			}
		}
	}

	// �W���C�p�b�h���̎擾
	CInputJoypad *pJoypad = CManager::GetInputJoypad();

	for (int nCntPlayer = 0; nCntPlayer < CPlayer::PLAYER_MAX; nCntPlayer++)
	{
		// �v���C���[��ENTRY���擾
		bool bEntry = CManager::GetEntry(nCntPlayer);

		// �G���g���[���Ă����
		if (bEntry == true)
		{
			for (int nCnt = CInputJoypad::JOYKEY_UP; nCnt < CInputJoypad::JOYKEY_MAX; nCnt++)
			{
				// �L�[�������ꂽ��
				if (pJoypad->GetTrigger((CInputJoypad::JOYKEY)nCnt, nCntPlayer) == true)
				{
					// ���艹
					CSound::Play(CSound::SOUND_LABEL_SE_MENU_OK);

					if (m_bTitleDraw == false)
					{
						aPosBg[BG_SKY].y = CRenderer::SCREEN_HEIGHT;
						aPosBg[LOGO_PLAYER].x = 960.0f;
					}
					else
					{
						if (m_bTutorial == true)
						{
							// ���[�h�̐ݒ�
							CManager::GetFade()->SetFade(CFade::FADE_OUT, CManager::MODE::MODE_GAME);
							return;
						}

						m_bTutorial = true;
						m_apObject2D[BG_FADE]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.7f));
					}
				}
			}
		}
	}

	if (m_bTutorial == true)
	{
		// �F�̎擾
		D3DXCOLOR col = m_apObject2D[LOGO_TUTORIAL]->GetColor();

		if (col.a <= 1.0f)
		{
			col.a += 0.05f;
			m_apObject2D[LOGO_TUTORIAL]->SetColor(col);
		}
	}

	// �Q�[���X�^�[�g���\�ɂȂ�����
	if (m_bTitleDraw == true)
	{
		// PRESS���S�̐F���擾
		D3DXCOLOR col = m_apObject2D[LOGO_PRESS]->GetColor();

		// PRESS���S��_�ł�����
		if (m_bPressFade == false)
		{
			// a�l�����Z
			col.a += 0.02f;
			// a�l�̉��Z���I�������
			if (col.a >= 1.0f)
			{// a�l�̌��Z���n�߂�
				m_bPressFade = true;
			}
		}
		else if (m_bPressFade == true)
		{
			// a�l�����Z
			col.a -= 0.02f;
			// a�l�̌��Z���I�������
			if (col.a <= 0.0f)
			{// a�l�̉��Z���n�߂�
				m_bPressFade = false;
			}
		}

		// PRESS���S�̐F��ݒ�
		m_apObject2D[LOGO_PRESS]->SetColor(col);
	}
	// �w�i�̈ړ����I������
	else if (aPosBg[BG_SKY].y >= CRenderer::SCREEN_HEIGHT)
	{
		// �v���C���[�̃��S��������o��(�ړ�)������
		aPosBg[LOGO_PLAYER].x += 25.0f;

		// �v���C���[���S�̈ړ����I�������
		if (aPosBg[LOGO_PLAYER].x >= 960.0f)
		{
			// �^�C�g�����S���o��������
			m_apObject2D[LOGO_TITLE]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			//�v���C���[���S������̈ʒu�ɐݒ�
			aPosBg[LOGO_PLAYER].x = 960.0f;
			//�v���C���[���S�̃e�N�X�`����ύX
			m_apObject2D[LOGO_PLAYER]->BindTexture(m_apTexture[LOGO_SHADOW]);

			//���t�F�[�h�̐ݒ�
			CFadeScene::Create(CFadeScene::TYPE_WHITE);

			// �Q�[���X�^�[�g�\�ɂ���
			m_bTitleDraw = true;

			//�J�E���^�[�̃��Z�b�g
			m_nCountMoveBg = 0;
		}
	}
	// �J�E���^�[����萔�ȏ�
	else if (m_nCountMoveBg >= 60)
	{// �w�i�̈ړ����J�n����

		//�_�̈ړ�����
		CCloud::Move(5.0f);

		//������Ɉړ�������
		aPosBg[BG_SKY].y += 5.0f;
	}

	//�w�i���̐ݒ�
	for (int nCnt = 0; nCnt < OBJ_MAX - 1; nCnt++)
	{
		//�ʒu�ݒ�
		m_apObject2D[nCnt]->SetPosition(aPosBg[nCnt]);
		//���_���W�̐ݒ�
		m_apObject2D[nCnt]->SetVertex();
	}

	//�_�̐���
	CreateCloud();

	if (m_bTutorial == false)
	{
		// �^�C�g����ʂ����[�v������܂ł̎���
		m_nCntLoop++;

		// 50�b�o�߂Ń^�C�g����ʃ��Z�b�g
		if (m_nCntLoop >= 5580)
		{
			//�J�E���^�[���Z�b�g
			m_nCntLoop = 0;

			// ���[�h�̐ݒ�
			CManager::GetFade()->SetFade(CFade::FADE_OUT, CManager::MODE::MODE_TITLE);
		}
	}

	// �v���C���[����
	for (int nCntPlayer = 0; nCntPlayer < CPlayer::PLAYER_MAX; nCntPlayer++)
	{
		// �v���C���[ENTRY���̎擾
		bool bEntry = CManager::GetEntry(nCntPlayer);

		// �G���g���[���Ă����
		if (bEntry == true && m_bEntry[nCntPlayer] == false)
		{
			// �G���g���[��Ԃɂ���
			m_bEntry[nCntPlayer] = true;

			switch (nCntPlayer)
			{
			case CPlayer::PLAYER_1:
				// PLAYER1��UI
				CUi::Create(D3DXVECTOR3(90.0f, 30.0f, 0.0f), D3DXVECTOR2(120.0f, 50.0f),
					CUi::TYPE_PLAYER1, CUi::ANIM_NONE, CUi::PLAYER_NONE);
				// OK��UI
				CUi::Create(D3DXVECTOR3(190.0f, 30.0f, 0.0f), D3DXVECTOR2(60.0f, 50.0f),
					CUi::TYPE_OK, CUi::ANIM_NONE, CUi::PLAYER_NONE);
				break;

			case CPlayer::PLAYER_2:
				// PLAYER2��UI
				CUi::Create(D3DXVECTOR3(CRenderer::SCREEN_WIDTH - 170.0f, 30.0f, 0.0f), D3DXVECTOR2(120.0f, 50.0f),
					CUi::TYPE_PLAYER2, CUi::ANIM_NONE, CUi::PLAYER_NONE);
				// OK��UI
				CUi::Create(D3DXVECTOR3(CRenderer::SCREEN_WIDTH - 60.0f, 30.0f, 0.0f), D3DXVECTOR2(60.0f, 50.0f),
					CUi::TYPE_OK, CUi::ANIM_NONE, CUi::PLAYER_NONE);
				break;
			default:
				break;
			}
		}
	}
}

//-----------------------------------------------------------------------------------------------
// �_�̐���
//-----------------------------------------------------------------------------------------------
void CTitle::CreateCloud()
{
	m_CloudInfo.nCount++;
	if (m_CloudInfo.nCount >= m_CloudInfo.nRandTime)
	{
		//�_�̐���
		CCloud::Create();
		//�J�E���^�[���Z�b�g
		m_CloudInfo.nCount = 0;

		//�_�̍ďo�����Ԃ𗐐��Őݒ�
		m_CloudInfo.nRandTime = LibrarySpace::GetRandNum(300, 60);
	}
}
