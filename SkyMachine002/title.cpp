//===============================================================================================
//
// �Q�[����ʏ��� [title.cpp]
// Author : SHUGO KURODA
//
//===============================================================================================
#include "title.h"

#include "manager.h"
#include "input_keyboard.h"
#include "object2D.h"

#include "cloud.h"
#include "library.h"

//-----------------------------------------------------------------------------------------------
// �ÓI�����o�ϐ�
//-----------------------------------------------------------------------------------------------
// �e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 CTitle::m_apTexture[OBJ_MAX] = { nullptr };

//-----------------------------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------------------------
CTitle::CTitle() :m_nCountMoveBg(0)
{
	for (int nCnt = 0; nCnt < OBJ_MAX; nCnt++)
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
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title003.png", &m_apTexture[LOGO_SHADOW]);		// �v���C���[���S(��)
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title004.png", &m_apTexture[LOGO_PRESS]);		// PRESS���S

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
	//�e�N�X�`���̃��[�h
	CTitle::Load();
	//�_
	CCloud::Load();

	//�X�N���[���T�C�Y�̕ۑ�
	D3DXVECTOR2 ScreenSize = D3DXVECTOR2((float)CRenderer::SCREEN_WIDTH, (float)CRenderer::SCREEN_HEIGHT);

	for (int nCnt = 0; nCnt < OBJ_MAX; nCnt++)
	{// ����
		m_apObject2D[nCnt] = new CObject2D;
	}

	//��
	m_apObject2D[BG_SKY]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, -ScreenSize.y / 2, 0.0f));
	m_apObject2D[BG_SKY]->SetSize(D3DXVECTOR2(ScreenSize.x, ScreenSize.y * 3));

	for (int nCnt = 0; nCnt < OBJ_MAX; nCnt++)
	{// �������ƃe�N�X�`���̐ݒ�
		m_apObject2D[nCnt]->Init();
		m_apObject2D[nCnt]->BindTexture(m_apTexture[nCnt]);
	}

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------------------------
void CTitle::Uninit()
{
	// �I������
	for (int nCntBg = 0; nCntBg < OBJ_MAX; nCntBg++)
	{
		if (m_apObject2D[nCntBg] != nullptr)
		{
			m_apObject2D[nCntBg]->Uninit();
			m_apObject2D[nCntBg] = nullptr;
		}
	}

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
		m_CloudInfo.nRandTime = LibrarySpace::GetRandNum(300, 240);
	}
}
