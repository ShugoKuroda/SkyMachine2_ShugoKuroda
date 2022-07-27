//===============================================================================================
//
// �Q�[����ʏ��� [title.cpp]
// Author : SHUGO KURODA
//
//===============================================================================================
#include "title.h"

#include "manager.h"
#include "input_keyboard.h"
#include "object.h"
#include "object2D.h"
#include "library.h"

#include "cloud.h"
#include "fade_white.h"
#include "library.h"

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
CTitle::CTitle() :m_nCountMoveBg(0), m_bTitleDraw(false), m_bPressFade(false)
{
	for (int nCnt = 0; nCnt < OBJ_MAX - 1; nCnt++)
	{
		m_apObject2D[nCnt] = nullptr;
	}

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
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title003.png", &m_apTexture[LOGO_SHADOW]);		// �v���C���[���S(��)

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

	for (int nCnt = 0; nCnt < OBJ_MAX - 1; nCnt++)
	{// �������ƃe�N�X�`���̐ݒ�
		m_apObject2D[nCnt]->Init();
		m_apObject2D[nCnt]->BindTexture(m_apTexture[nCnt]);
	}

	//�w�i�ȊO��O�ɕ`�悷��(�^�C�v��ݒ肷��)
	for (int nCnt = LOGO_PLAYER; nCnt < OBJ_MAX - 1; nCnt++)
	{
		m_apObject2D[nCnt]->SetObjectType(CObject::OBJ_TITLE);
	}

	//�^�C�g����Press���S�𓧖��ɂ���
	m_apObject2D[LOGO_TITLE]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	m_apObject2D[LOGO_PRESS]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------------------------
void CTitle::Uninit()
{
	//�e�N�X�`���̔j��
	CTitle::Unload();
	//�_
	CCloud::Unload();
}

//-----------------------------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------------------------
void CTitle::Update()
{
	m_nCountMoveBg++;

	//�w�i�ʒu�̎擾
	D3DXVECTOR3 aPosBg[OBJ_MAX - 1];
	for (int nCnt = 0; nCnt < OBJ_MAX - 1; nCnt++)
	{
		aPosBg[nCnt] = m_apObject2D[nCnt]->GetPosition();
	}

	// �w�i���ړ�����������
	if (m_bTitleDraw == true && m_nCountMoveBg >= 40)
	{
		D3DXCOLOR col = m_apObject2D[LOGO_PRESS]->GetColor();
		if (m_bPressFade == false)
		{
			col.a += 0.02f;
			if (col.a >= 1.0f)
			{
				m_bPressFade = true;
			}
		}
		if (m_bPressFade == true)
		{
			col.a -= 0.02f;
			if (col.a <= 0.0f)
			{
				m_bPressFade = false;
			}
		}
		m_apObject2D[LOGO_PRESS]->SetColor(col);
	}
	else if (m_nCountMoveBg >= 540)
	{
		aPosBg[LOGO_PLAYER].x += 25.0f;
		if (aPosBg[LOGO_PLAYER].x >= 960.0f)
		{
			m_apObject2D[LOGO_TITLE]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			aPosBg[LOGO_PLAYER].x = 960.0f;
			m_apObject2D[LOGO_PLAYER]->BindTexture(m_apTexture[LOGO_SHADOW]);

			//�^�C�g����`�悷��
			m_bTitleDraw = true;

			//���t�F�[�h�̐ݒ�
			CFadeWhite::Create();

			//�J�E���^�[�̃��Z�b�g
			m_nCountMoveBg = 0;
		}
	}
	// �w�i�̈ړ����J�n����
	else if (m_nCountMoveBg >= 120)
	{
		//�_�̈ړ�����
		CCloud::MoveCloud(3.0f);

		aPosBg[BG_SKY].y += 3.0f;
	}

	//�w�i�ʒu�̐ݒ�
	for (int nCnt = 0; nCnt < OBJ_MAX - 1; nCnt++)
	{
		m_apObject2D[nCnt]->SetPosition(aPosBg[nCnt]);
		m_apObject2D[nCnt]->SetVertex();
	}

	// �L�[�{�[�h���̎擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_ATTACK) == true)
	{//SPACE�L�[�������ꂽ
		// ���[�h�̐ݒ�
		CManager::SetMode(CManager::MODE_GAME);
	}

	//�_�̐���
	CreateCloud();
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
