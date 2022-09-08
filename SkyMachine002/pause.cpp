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

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//=============================================================================
// CBullet�̃R���X�g���N�^
//=============================================================================
CPause::CPause():CObject()
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
HRESULT CPause::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause000.jpg", &m_apTexture[TYPE_FRAME]);		// �|�[�Y��ʘg
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause001.png", &m_apTexture[TYPE_RESUME]);		// �ĊJ
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause002.png", &m_apTexture[TYPE_RESTART]);	// ��蒼��
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause003.png", &m_apTexture[TYPE_EXIT]);		// �^�C�g���ɖ߂�

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
//	�e�N�X�`���j��
//-----------------------------------------------------------------------------------------------
void CPause::Unload(void)
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
CPause *CPause::Create(D3DXVECTOR3 pos,D3DXVECTOR2 size)
{
	// �|�C���^�ϐ��̐���
	CPause *pPause = new CPause();

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
	// �����o�ϐ��̏�����
	m_nPauseSelect = 0;
	m_nCntBg = 0;
	m_nCntMenuBg = 0;
	m_nCntMenuLogo = 0;
	m_nCntMenuLogoBg = 0;
	m_nCntLogo = 0;
	m_bPause = false;

	CManager::SetPause(m_bPause);

	return S_OK;
}

//=============================================================================
// �|�[�Y�̏I������
//=============================================================================
void CPause::Uninit(void)
{
	// �|�[�Y��Ԃ�false�ɂ���
	CManager::SetPause(false);

	// �I�u�W�F�N�g�j��
	Release();
}

//=============================================================================
// �|�[�Y�̍X�V����
//=============================================================================
void CPause::Update(void)
{
	if (m_bPause == true)
	{
		// �L�[�{�[�h���̎擾
		CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

		// �Q�[���p�b�h���̎擾
		CInputJoypad *pJoypad = CManager::GetInputJoypad();
		bool *bCrossTrigger = pJoypad->GetCrossKeyTrigger(m_nNumController);

		// �t�F�[�h���̎擾
		CFade *pFade = CManager::GetFade();

		if (pFade->GetFade() == CFade::FADE_NONE)
		{
			if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_UP) || bCrossTrigger[CInputJoypad::STICK_AND_CROSSKEY_UP])
			{
				// ��ɂ����Ƃ��ԍ������炷
				m_nPauseSelect--;

				if (m_nPauseSelect < 0)
				{
					// �R���e�B�j���[��菬�����Ȃ����Ƃ�
					m_nPauseSelect = m_nCntMenuLogoBg - 1;
				}

				// ���ʉ�
				CSound::Play(CSound::SOUND_LABEL_SE_MOVE_CURSOR);
			}
			else if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_DOWN) || bCrossTrigger[CInputJoypad::STICK_AND_CROSSKEY_DOWN])
			{
				// ���ɂ����Ƃ��ԍ��𑝂₷
				m_nPauseSelect++;

				if (m_nPauseSelect > m_nCntMenuLogoBg - 1)
				{
					// �N�C�b�g���傫���Ȃ����Ƃ�
					m_nPauseSelect = 0;
				}

				// ���ʉ�
				CSound::Play(CSound::SOUND_LABEL_SE_MOVE_CURSOR);
			}

			// �eUI�̐F��ݒ�
			for (int nCntPauseUI = 0; nCntPauseUI < m_nCntMenuLogoBg; nCntPauseUI++)
			{
				if (m_nPauseSelect == nCntPauseUI)
				{
					// ���ݑI������Ă�����̂̓��l1.0f�ɐݒ�
					m_pPauseMenuLogoBg[nCntPauseUI]->SetTex(D3DXVECTOR2(1, 2));
				}
				else
				{
					// ����ȊO�̓��l0.3f�ɐݒ�
					m_pPauseMenuLogoBg[nCntPauseUI]->SetTex(D3DXVECTOR2(1, 1));
				}
			}

			if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_OK) || pJoypad->GetButtonTrigger(m_nNumController,JOYPAD_DECISION))
			{
				// �I������Ă���UI���Q�Ƃ��A�ǂ̏��������邩����
				switch (m_nPauseSelect)
				{
					// �R���e�B�j���[�{�^��
				case 0:
					SetPause();										// �|�[�Y�����
					break;
					// ���g���C�{�^��
				case 1:
					CGame::SetGameMode(CGame::GAMEMODE_TRANSITION_GAME);
					break;
					// �I���{�^��
				case 2:
					CGame::SetGameMode(CGame::GAMEMODE_TRANSITION_TITLE);
					break;
				}
			}

			// �|�[�Y��ʂ��I����
			if (pKeyboard->GetTrigger(KEY_PAUSE) || pJoypad->GetButtonTrigger(m_nNumController, JOYPAD_PAUSE))
			{
				SetPause();
			}
		}
	}
}

//=============================================================================
// �|�[�Y�̕`�揈��
//=============================================================================
void CPause::Draw(void)
{
	// �w�i�̕`��
	for (int nCnt = 0; nCnt < m_nNumBg; nCnt++)
	{
		m_pBg[nCnt]->Draw();
	}

	// ���j���[�w�i�̕`��
	for (int nCnt = 0; nCnt < m_nCntMenuBg; nCnt++)
	{
		m_pPauseMenuBg[nCnt]->Draw();
	}

	// ���̑����S�̕`��
	for (int nCnt = 0; nCnt < m_nLogo; nCnt++)
	{
		m_pLogo[nCnt]->Draw();
	}

	// ���j���[���S�̕`��
	for (int nCnt = 0; nCnt < m_nCntMenuLogoBg; nCnt++)
	{
		m_pPauseMenuLogoBg[nCnt]->Draw();
	}

	// ���j���[���S�̕`��
	for (int nCnt = 0; nCnt < m_nCntMenuLogo; nCnt++)
	{
		m_pPauseMenuLogo[nCnt]->Draw();
	}
}

//=============================================================================
// �|�[�Y�̏���
//=============================================================================
void CPause::SetPause()
{
	m_bPause = m_bPause ? false : true;

	if (m_bPause == true)
	{
		CSound::Play(CSound::SOUND_LABEL_SE_PAUSE_IN);
	}
	else if (m_bPause == false)
	{
		CSound::Play(CSound::SOUND_LABEL_SE_PAUSE_OUT);

		// �I�����Ă��郍�S���ĊJ�ɂ���
		m_nPauseSelect = 0;
	}

	CManager::SetPause(m_bPause);
}

//=============================================================================
// �|�[�Y�̏���(�v���C���[�ԍ��ۑ��p)
//=============================================================================
void CPause::SetPause(int nNumPlayer)
{
	// �v���C���[�ԍ��ۑ�
	m_nNumController = nNumPlayer;

	// �|�[�Y����
	SetPause();
}

//=============================================================================
// �|�[�Y�f�[�^�̃��[�h
//=============================================================================
bool CPause::LoadData(void)
{
	// FILE�^�̃|�C���^�֐�
	FILE *pFile;

	// �t�@�C�����J��
	pFile = fopen("data/TEXT/02_game/pause.txt", "r");

	// NULL�`�F�b�N
	if (pFile != NULL)
	{
		// �ϐ��錾
		char aLine[128];
		char aText[128];
		bool bScript = false;
		int nCntTex = 0;
		int nNumTex = 0;
		char **pTextureFileName;

		// SCRIPT��T��
		while (fgets(aLine, 128, pFile) != NULL)
		{
			// ���s�󔒂�������܂œǂݍ���
			fscanf(pFile, "%s", &aText[0]);

			// SCRIPT���������炻��ȍ~��ǂݍ��ދ���^����
			if (strcmp(&aText[0], "SCRIPT") == 0)
			{
				bScript = true;
			}
			if (bScript == true)
			{
				if (strcmp(&aText[0], "NUM_TEXTURE") == 0)
				{
					fscanf(pFile, "%s %d", &aText[0], &nNumTex);

					// ���f���t�@�C���l�[���i�[�p�ϐ��̗̈�𓮓I�Ɋm�ۂ���
					pTextureFileName = new char*[nNumTex];

					for (int nCnt = 0; nCnt < nNumTex; nCnt++)
					{
						pTextureFileName[nCnt] = new char[128];
					}
				}
				// TEXTURE_FILENAME����������e�N�X�`���t�@�C�������i�[
				if (strcmp(&aText[0], "TEXTURE_FILENAME") == 0 && nCntTex < nNumTex)
				{
					fscanf(pFile, "%s %s", &aText[0], &pTextureFileName[nCntTex][0]);

					nCntTex++;
				}
				// LOGOSET�̂Ƃ��X�L���Q�[�W�����i�[���鋖��^����
				if (strcmp(&aText[0], "LOGOSET") == 0)
				{
					LoadLogo(pFile, nNumTex, pTextureFileName);
				}
				// BGSET�̂Ƃ��X�L���Q�[�W�����i�[���鋖��^����
				if (strcmp(&aText[0], "BGSET") == 0)
				{
					LoadBg(pFile, nNumTex, pTextureFileName);
				}
				// PAUSEMENUSET�̂Ƃ��X�L���Q�[�W�����i�[���鋖��^����
				if (strcmp(&aText[0], "PAUSEMENUSET") == 0)
				{
					LoadPauseMenu(pFile, nNumTex, pTextureFileName);
				}
				// END_SCRIPT����������ǂݍ��݂��I����
				if (strcmp(&aText[0], "END_SCRIPT") == 0)
				{
					// �������̊J��
					delete[] pTextureFileName;

					break;
				}
			}
		}
		// �t�@�C�������
		fclose(pFile);
	}
	else
	{
		return false;
	}
	return true;
}

//=============================================================================
// �w�i���̃��[�h
//=============================================================================
bool CPause::LoadBg(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// �ϐ��錾
	char aLine[128];
	char aText[128];
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };

	while (fgets(aLine, 128, pFile) != NULL)
	{
		// ���s�󔒂�������܂œǂݍ���
		fscanf(pFile, "%s", &aText[0]);

		// POS������������W�����i�[
		if (strcmp(&aText[0], "NUM_BG") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &m_nNumBg);

			// �������̊m��
			m_pBg = new CScene2D*[m_nNumBg];
		}
		// SKILLGAUGESET�̂Ƃ��X�L���Q�[�W�����i�[���鋖��^����
		if (strcmp(&aText[0], "BG") == 0)
		{
			Bg(pFile, nNumTex, pTextureFileName);
		}
		// END_PARTSSET����������e�p�[�c�̏����i�[���I����
		if (strcmp(&aText[0], "END_BGSET") == 0)
		{
			break;
		}
	}
	return false;
}

//=============================================================================
// �|�[�Y���j���[���̃��[�h
//=============================================================================
bool CPause::LoadPauseMenu(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// �ϐ��錾
	char aLine[128];
	char aText[128];
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };

	while (fgets(aLine, 128, pFile) != NULL)
	{
		// ���s�󔒂�������܂œǂݍ���
		fscanf(pFile, "%s", &aText[0]);

		// POS������������W�����i�[
		if (strcmp(&aText[0], "NUM_MENUBG") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &m_nNumMenuBg);

			// �������̊m��
			m_pPauseMenuBg = new CScene2D*[m_nNumMenuBg];
		}
		// POS������������W�����i�[
		if (strcmp(&aText[0], "NUM_MENULOGO") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &m_nNumLogo);

			// �������̊m��
			m_pPauseMenuLogo = new CScene2D*[m_nNumLogo];

			// ���S�ƃ��S�w�i�̐�����v������
			m_nNumLogoBg = m_nNumLogo;
			
			// �������̊m��
			m_pPauseMenuLogoBg = new CScene2D*[m_nNumLogoBg];
		}
		// SKILLGAUGESET�̂Ƃ��X�L���Q�[�W�����i�[���鋖��^����
		if (strcmp(&aText[0], "MENUBG") == 0)
		{
			MenuBg(pFile, nNumTex, pTextureFileName);
		}
		// SKILLGAUGESET�̂Ƃ��X�L���Q�[�W�����i�[���鋖��^����
		if (strcmp(&aText[0], "MENULOGO") == 0)
		{
			MenuLogo(pFile, nNumTex, pTextureFileName);
		}
		// SKILLGAUGESET�̂Ƃ��X�L���Q�[�W�����i�[���鋖��^����
		if (strcmp(&aText[0], "MENULOGOBG") == 0)
		{
			MenuLogoBg(pFile, nNumTex, pTextureFileName);
		}
		// END_PARTSSET����������e�p�[�c�̏����i�[���I����
		if (strcmp(&aText[0], "END_BGSET") == 0)
		{
			break;
		}
	}
	return false;
}

//=============================================================================
// �w�i���̃��[�h
//=============================================================================
bool CPause::Bg(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// �ϐ��錾
	char aLine[128];
	char aText[128];
	int nTex;
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };
	D3DXCOLOR col = { 0.0f,0.0f,0.0f,0.0f };

	while (fgets(aLine, 128, pFile) != NULL)
	{
		// ���s�󔒂�������܂œǂݍ���
		fscanf(pFile, "%s", &aText[0]);

		// POS������������W�����i�[
		if (strcmp(&aText[0], "TYPE") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &nTex);
		}
		// POS������������W�����i�[
		if (strcmp(&aText[0], "POS") == 0)
		{
			fscanf(pFile, "%s %f %f %f", &aText[0],
				&pos.x,
				&pos.y,
				&pos.z);
		}
		// SIZE����������T�C�Y�����i�[
		if (strcmp(&aText[0], "SIZE") == 0)
		{
			fscanf(pFile, "%s %f %f", &aText[0],
				&size.x,
				&size.y);
		}
		// POS������������W�����i�[
		if (strcmp(&aText[0], "COLOR") == 0)
		{
			fscanf(pFile, "%s %f %f %f %f", &aText[0],
				&col.r,
				&col.g,
				&col.b,
				&col.a);
		}
		// END_PARTSSET����������e�p�[�c�̏����i�[���I����
		if (strcmp(&aText[0], "END_BG") == 0)
		{
			if (nTex < nNumTex)
			{
				// �e�N�X�`�����̎擾
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL && m_nCntBg < m_nNumBg)
				{
					// �X�L���Q�[�W�̐���
					if (nTex != -1)
					{
						m_pBg[m_nCntBg] = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE, pTex->SetTexture(&pTextureFileName[nTex][0]));
						m_pBg[m_nCntBg]->SetCol(col);
						m_nCntBg++;
					}
					else
					{
						m_pBg[m_nCntBg] = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE, nTex);
						m_pBg[m_nCntBg]->SetCol(col);
						m_nCntBg++;
					}

					break;
				}
			}
		}
	}
	return false;
}

//=============================================================================
// ���j���[�w�i���̃��[�h
//=============================================================================
bool CPause::MenuBg(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// �ϐ��錾
	char aLine[128];
	char aText[128];
	int nTex;
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };
	D3DXCOLOR col = { 0.0f,0.0f,0.0f,0.0f };

	while (fgets(aLine, 128, pFile) != NULL)
	{
		// ���s�󔒂�������܂œǂݍ���
		fscanf(pFile, "%s", &aText[0]);

		// POS������������W�����i�[
		if (strcmp(&aText[0], "TYPE") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &nTex);
		}
		// POS������������W�����i�[
		if (strcmp(&aText[0], "POS") == 0)
		{
			fscanf(pFile, "%s %f %f %f", &aText[0],
				&pos.x,
				&pos.y,
				&pos.z);
		}
		// SIZE����������T�C�Y�����i�[
		if (strcmp(&aText[0], "SIZE") == 0)
		{
			fscanf(pFile, "%s %f %f", &aText[0],
				&size.x,
				&size.y);
		}
		// POS������������W�����i�[
		if (strcmp(&aText[0], "COLOR") == 0)
		{
			fscanf(pFile, "%s %f %f %f %f", &aText[0],
				&col.r,
				&col.g,
				&col.b,
				&col.a);
		}
		// END_PARTSSET����������e�p�[�c�̏����i�[���I����
		if (strcmp(&aText[0], "END_MENUBG") == 0)
		{
			if (nTex < nNumTex)
			{
				// �e�N�X�`�����̎擾
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL && m_nCntMenuBg < m_nNumMenuBg)
				{
					// �X�L���Q�[�W�̐���
					if (nTex != -1)
					{
						m_pPauseMenuBg[m_nCntMenuBg] = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE, pTex->SetTexture(&pTextureFileName[nTex][0]));
						m_pPauseMenuBg[m_nCntMenuBg]->SetCol(col);
						m_nCntMenuBg++;
					}
					else
					{
						m_pPauseMenuBg[m_nCntMenuBg] = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE, nTex);
						m_pPauseMenuBg[m_nCntMenuBg]->SetCol(col);
						m_nCntMenuBg++;
					}

					break;
				}
			}
		}
	}
	return false;
}

//=============================================================================
// ���j���[���S���̃��[�h
//=============================================================================
bool CPause::MenuLogo(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// �ϐ��錾
	char aLine[128];
	char aText[128];
	int nTex;
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };
	D3DXCOLOR col = { 0.0f,0.0f,0.0f,0.0f };
	D3DXVECTOR2 tex = { 0.0f,0.0f };

	while (fgets(aLine, 128, pFile) != NULL)
	{
		// ���s�󔒂�������܂œǂݍ���
		fscanf(pFile, "%s", &aText[0]);

		// POS������������W�����i�[
		if (strcmp(&aText[0], "TYPE") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &nTex);
		}
		// POS������������W�����i�[
		if (strcmp(&aText[0], "POS") == 0)
		{
			fscanf(pFile, "%s %f %f %f", &aText[0],
				&pos.x,
				&pos.y,
				&pos.z);
		}
		// SIZE����������T�C�Y�����i�[
		if (strcmp(&aText[0], "SIZE") == 0)
		{
			fscanf(pFile, "%s %f %f", &aText[0],
				&size.x,
				&size.y);
		}
		// POS������������W�����i�[
		if (strcmp(&aText[0], "COLOR") == 0)
		{
			fscanf(pFile, "%s %f %f %f %f", &aText[0],
				&col.r,
				&col.g,
				&col.b,
				&col.a);
		}
		// POS������������W�����i�[
		if (strcmp(&aText[0], "TEX") == 0)
		{
			fscanf(pFile, "%s %f %f", &aText[0],
				&tex.x,
				&tex.y);
		}
		// END_PARTSSET����������e�p�[�c�̏����i�[���I����
		if (strcmp(&aText[0], "END_MENULOGO") == 0)
		{
			if (nTex < nNumTex)
			{
				// �e�N�X�`�����̎擾
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL && m_nCntMenuLogo < m_nNumLogo)
				{
					// �X�L���Q�[�W�̐���
					if (nTex != -1)
					{
						m_pPauseMenuLogo[m_nCntMenuLogo] = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE, pTex->SetTexture(&pTextureFileName[nTex][0]));
						m_pPauseMenuLogo[m_nCntMenuLogo]->SetCol(col);
						m_nCntMenuLogo++;
					}
					else
					{
						m_pPauseMenuLogo[m_nCntMenuLogo] = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE, nTex);
						m_pPauseMenuLogo[m_nCntMenuLogo]->SetCol(col);
						m_nCntMenuLogo++;
					}

					break;
				}
			}
		}
	}
	return false;
}

//=============================================================================
// ���j���[���S�w�i���̃��[�h
//=============================================================================
bool CPause::MenuLogoBg(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// �ϐ��錾
	char aLine[128];
	char aText[128];
	int nTex;
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };
	D3DXCOLOR col = { 0.0f,0.0f,0.0f,0.0f };
	D3DXVECTOR2 tex = { 0.0f,0.0f };

	while (fgets(aLine, 128, pFile) != NULL)
	{
		// ���s�󔒂�������܂œǂݍ���
		fscanf(pFile, "%s", &aText[0]);

		// POS������������W�����i�[
		if (strcmp(&aText[0], "TYPE") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &nTex);
		}
		// POS������������W�����i�[
		if (strcmp(&aText[0], "POS") == 0)
		{
			fscanf(pFile, "%s %f %f %f", &aText[0],
				&pos.x,
				&pos.y,
				&pos.z);
		}
		// SIZE����������T�C�Y�����i�[
		if (strcmp(&aText[0], "SIZE") == 0)
		{
			fscanf(pFile, "%s %f %f", &aText[0],
				&size.x,
				&size.y);
		}
		// POS������������W�����i�[
		if (strcmp(&aText[0], "COLOR") == 0)
		{
			fscanf(pFile, "%s %f %f %f %f", &aText[0],
				&col.r,
				&col.g,
				&col.b,
				&col.a);
		}
		// POS������������W�����i�[
		if (strcmp(&aText[0], "TEX") == 0)
		{
			fscanf(pFile, "%s %f %f", &aText[0],
				&tex.x,
				&tex.y);
		}
		// END_PARTSSET����������e�p�[�c�̏����i�[���I����
		if (strcmp(&aText[0], "END_MENULOGOBG") == 0)
		{
			if (nTex < nNumTex)
			{
				// �e�N�X�`�����̎擾
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL && m_nCntMenuLogoBg < m_nNumLogoBg)
				{
					// �X�L���Q�[�W�̐���
					if (nTex != -1)
					{
						m_pPauseMenuLogoBg[m_nCntMenuLogoBg] = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE, pTex->SetTexture(&pTextureFileName[nTex][0]));
						m_pPauseMenuLogoBg[m_nCntMenuLogoBg]->SetCol(col);
						m_pPauseMenuLogoBg[m_nCntMenuLogoBg]->SetTexSheet(tex);
						m_pPauseMenuLogoBg[m_nCntMenuLogoBg]->SetTex(D3DXVECTOR2(1,0.5));
						m_nCntMenuLogoBg++;
					}
					else
					{
						m_pPauseMenuLogoBg[m_nCntMenuLogoBg] = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE, nTex);
						m_pPauseMenuLogoBg[m_nCntMenuLogoBg]->SetCol(col);
						m_pPauseMenuLogoBg[m_nCntMenuLogoBg]->SetTexSheet(tex);
						m_pPauseMenuLogoBg[m_nCntMenuLogoBg]->SetTex(D3DXVECTOR2(1, 0.5));
						m_nCntMenuLogoBg++;
					}

					break;
				}
			}
		}
	}
	return false;
}

//=============================================================================
// ���S���̃��[�h
//=============================================================================
bool CPause::LoadLogo(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// �ϐ��錾
	char aLine[128];
	char aText[128];
	int nTex;
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };
	D3DXCOLOR col = { 0.0f,0.0f,0.0f,0.0f };

	while (fgets(aLine, 128, pFile) != NULL)
	{
		// ���s�󔒂�������܂œǂݍ���
		fscanf(pFile, "%s", &aText[0]);

		// POS������������W�����i�[
		if (strcmp(&aText[0], "NUM_LOGO") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &m_nLogo);

			// �������̊m��
			m_pLogo = new CScene2D*[m_nLogo];
		}
		// END_PARTSSET����������e�p�[�c�̏����i�[���I����
		if (strcmp(&aText[0], "LOGO") == 0)
		{
			while (fgets(aLine, 128, pFile) != NULL)
			{
				// ���s�󔒂�������܂œǂݍ���
				fscanf(pFile, "%s", &aText[0]);

				// POS������������W�����i�[
				if (strcmp(&aText[0], "TYPE") == 0)
				{
					fscanf(pFile, "%s %d", &aText[0], &nTex);
				}
				// POS������������W�����i�[
				if (strcmp(&aText[0], "POS") == 0)
				{
					fscanf(pFile, "%s %f %f %f", &aText[0],
						&pos.x,
						&pos.y,
						&pos.z);
				}
				// SIZE����������T�C�Y�����i�[
				if (strcmp(&aText[0], "SIZE") == 0)
				{
					fscanf(pFile, "%s %f %f", &aText[0],
						&size.x,
						&size.y);
				}
				// POS������������W�����i�[
				if (strcmp(&aText[0], "COLOR") == 0)
				{
					fscanf(pFile, "%s %f %f %f %f", &aText[0],
						&col.r,
						&col.g,
						&col.b,
						&col.a);
				}
				// END_PARTSSET����������e�p�[�c�̏����i�[���I����
				if (strcmp(&aText[0], "END_LOGO") == 0)
				{
					if (nTex < nNumTex)
					{
						// �e�N�X�`�����̎擾
						CTexture *pTex = CManager::GetTexture();

						if (pTex != NULL && m_nCntLogo < m_nLogo)
						{
							// �X�L���Q�[�W�̐���
							m_pLogo[m_nCntLogo] = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE, pTex->SetTexture(&pTextureFileName[nTex][0]));
							m_pLogo[m_nCntLogo]->SetCol(col);
							m_nCntLogo++;

							break;
						}
						else if (pTex == NULL && m_nCntLogo < m_nLogo)
						{
							// �X�L���Q�[�W�̐���
							m_pLogo[m_nCntLogo] = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE,nTex);
							m_pLogo[m_nCntLogo]->SetCol(col);
							m_nCntLogo++;

							break;
						}
					}
				}
			}
		}
		if (strcmp(&aText[0], "END_LOGOSET") == 0)
		{
			break;
		}
	}
	return false;
}