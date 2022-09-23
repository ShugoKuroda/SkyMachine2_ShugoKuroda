//===============================================================================================
//
// �Q�[����ʏ��� [game.cpp]
// Author : SHUGO KURODA
//
//===============================================================================================
#include "manager.h"
#include "game.h"
#include "object.h"

#include "library.h"
#include "load.h"
#include "fade_scene.h"
#include "input_keyboard.h"
#include "input_joypad.h"

#include "score.h"
#include "number.h"
#include "logo.h"
#include "pause.h"

#include "bg.h"
#include "cloud.h"
#include "enemy.h"
#include "enemy_boss.h"
#include "bullet.h"
#include "bullet_option.h"
#include "explosion.h"
#include "spray.h"
#include "bubble.h"
#include "effect.h"
#include "bg_move.h"
#include "meshfield.h"

//-----------------------------------------------------------------------------------------------
// using�錾
//-----------------------------------------------------------------------------------------------
using namespace LibrarySpace;

//-----------------------------------------------------------------------------------------------
// �ÓI�����o�ϐ�
//-----------------------------------------------------------------------------------------------
bool CGame::m_bCreateCloud = true; 
bool CGame::m_bCreateBubble = false;
bool CGame::m_bDieBoss = false;
CPlayer *CGame::m_pPlayer[CPlayer::PLAYER_MAX] = {};
CScore *CGame::m_pScore = nullptr;
CMeshField *CGame::m_pMeshField = nullptr;

//-----------------------------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------------------------
CGame::CGame() :m_nCntBubble(0), m_nRandBubble(0)
{
	//�G�̐�������������
	ZeroMemory(&m_EnemyInfo, sizeof(m_EnemyInfo));
	//�_�̐�������������
	ZeroMemory(&m_CloudInfo, sizeof(m_CloudInfo));
}

//-----------------------------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------------------------
CGame::~CGame()
{
}

//-----------------------------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------------------------
HRESULT CGame::Init()
{
	//�G���ǂݍ���
	m_EnemyInfo.pCreate = LoadSpace::GetEnemy();
	//�E�F�[�u���̓ǂݍ���
	m_EnemyInfo.nWave = LoadSpace::GetWave();
	// �e�N�X�`���ǂݍ���
	LoadAll();

	// �v���C���[����
	for (int nCntPlayer = 0; nCntPlayer < CPlayer::PLAYER_MAX; nCntPlayer++)
	{
		// �v���C���[ENTRY���̎擾
		bool bEntry = CManager::GetEntry(nCntPlayer);

		// �G���g���[���Ă����
		if (bEntry == true)
		{// �v���C���[����
			m_pPlayer[nCntPlayer] = CPlayer::Create(D3DXVECTOR3(-630.0f, CRenderer::SCREEN_HEIGHT, 0.0f), nCntPlayer);
		}
	}

	// �w�i�̐���
	CBg::Create(CBg::SET_A);
	// �X�R�A�̐���
	m_pScore = CScore::Create();

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------------------------
void CGame::Uninit()
{
	//�e�N�X�`���̔j��
	UnloadAll();
}

//-----------------------------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------------------------
void CGame::Update()
{
	// �v���C���[����
	for (int nCntPlayer = 0; nCntPlayer < CPlayer::PLAYER_MAX; nCntPlayer++)
	{
		if (m_pPlayer[nCntPlayer] == nullptr)
		{
			// �v���C���[ENTRY���̎擾
			bool bEntry = CManager::GetEntry(nCntPlayer);

			// �G���g���[���Ă����
			if (bEntry == true)
			{// �v���C���[����
				m_pPlayer[nCntPlayer] = CPlayer::Create(D3DXVECTOR3(-630.0f, CRenderer::SCREEN_HEIGHT, 0.0f), nCntPlayer);
			}
		}
	}

	//�_�𐶐����邩�ǂ���
	if (m_bCreateCloud == true)
	{
		//�_�̐�������
		CreateCloud();
	}
	//�A�G�t�F�N�g�𐶐����邩�ǂ���
	if (m_bCreateBubble == true)
	{
		//�A�G�t�F�N�g�̐�������
		CreateBubble();
	}

	// �L�[�{�[�h���̎擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	// �Q�[���p�b�h���̎擾
	CInputJoypad *pJoypad = CManager::GetInputJoypad();

	// �{�X���S�t���O�������Ă��Ȃ����
	if (m_bDieBoss == false)
	{
		// �|�[�Y����
		if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_PAUSE) == true || pJoypad->GetTrigger(CInputJoypad::JOYKEY_START, 0) == true)
		{
			// �|�[�Y��Ԃ̎擾
			bool bPause = CManager::GetPause();
			// �|�[�Y���łȂ����
			if (bPause == false)
			{//�|�[�Y��ʂ𐶐�
				CPause::Create();
			}
		}
	}

	//�G�̐�������	
	CreateEnemy();
}

//-----------------------------------------------------------------------------------------------
// �_�̃����_������
//-----------------------------------------------------------------------------------------------
void CGame::CreateCloud()
{
	m_CloudInfo.nCount++;
	if (m_CloudInfo.nCount >= m_CloudInfo.nRandTime)
	{
		//�_�̐���
		CCloud::Create();
		//�J�E���^�[���Z�b�g
		m_CloudInfo.nCount = 0;

		//�_�̍ďo�����Ԃ𗐐��Őݒ�
		m_CloudInfo.nRandTime = GetRandNum(250, 180);
	}
}

//-----------------------------------------------------------------------------------------------
// �A�G�t�F�N�g�̐���
//-----------------------------------------------------------------------------------------------
void CGame::CreateBubble()
{
	m_nCntBubble++;

	if (m_nCntBubble >= m_nRandBubble)
	{
		D3DXVECTOR3 posBubble = D3DXVECTOR3((float)GetRandNum(CRenderer::SCREEN_WIDTH, 0), (float)GetRandNum(CRenderer::SCREEN_HEIGHT, CRenderer::SCREEN_HEIGHT - 250), 0.0f);

		for (int nCntBubble = 0; nCntBubble < 3; nCntBubble++)
		{
			CBubble::Create(D3DXVECTOR3(posBubble.x, posBubble.y - (nCntBubble * 20), posBubble.z), D3DXVECTOR2((float)CBubble::MIN_SIZE * (nCntBubble + 1), (float)CBubble::MIN_SIZE * (nCntBubble + 1)));
		}

		//�J�E���^�[���Z�b�g
		m_nCntBubble = 0;

		//�A�G�t�F�N�g�̍ďo�����Ԃ𗐐��Őݒ�
		m_nRandBubble = GetRandNum(120, 30);
	}
}

//-----------------------------------------------------------------------------------------------
// �G�̐���(�O���X�N���v�g�g�p)
//-----------------------------------------------------------------------------------------------
void CGame::CreateEnemy()
{
	// �G�����J�E���^�[�̉��Z
	m_EnemyInfo.nCreatenCount++;

	//�G�̃E�F�[�u��������������
	for (int nCntEnemy = 0; nCntEnemy < m_EnemyInfo.nWave; nCntEnemy++)
	{
		// �G���o������t���[�����ɒB������ && �G�̏o�����I����Ă��Ȃ��Ȃ�
		if (m_EnemyInfo.pCreate[nCntEnemy].nFrame <= m_EnemyInfo.nCreatenCount && m_EnemyInfo.count[nCntEnemy].bCreate == false)
		{
			//�G���A���ŏo������܂ł̃J�E���^�[�����Z
			m_EnemyInfo.count[nCntEnemy].nCreate++;
			//�G�̘A���o���Ԋu�J�E���^�[���ő�ɒB������
			if (m_EnemyInfo.count[nCntEnemy].nCreate >= m_EnemyInfo.pCreate[nCntEnemy].nInterval)
			{
				//�G�̐���(�ʒu�A��ށA�̗́A�ړ����������ɐݒ�)
				CEnemy *pEnemy = CEnemy::Create(m_EnemyInfo.pCreate[nCntEnemy].pos, (CEnemy::TYPE)m_EnemyInfo.pCreate[nCntEnemy].nType, m_EnemyInfo.pCreate[nCntEnemy].nLife, &m_EnemyInfo.pCreate[nCntEnemy].move[0]);

				//�G�̐������J�E���^�[�����Z
				m_EnemyInfo.count[nCntEnemy].nNum++;
				//�G�̘A���o���Ԋu�J�E���^�[�̃��Z�b�g
				m_EnemyInfo.count[nCntEnemy].nCreate = 0;

				//�G���ő吔�܂Ő���������
				if (m_EnemyInfo.pCreate[nCntEnemy].nNum <= m_EnemyInfo.count[nCntEnemy].nNum)
				{
					// �F�̐ݒ肪����Ă�����
					if (m_EnemyInfo.pCreate[nCntEnemy].nColor > 0)
					{// �Ō���̓G�̐F��ݒ�
						pEnemy->SetItemColor((CEnemy::COLORITEM)m_EnemyInfo.pCreate[nCntEnemy].nColor);
					}
					// ���̃E�F�[�u�̐�������߂�
					m_EnemyInfo.count[nCntEnemy].bCreate = true;
				}
			}
		}
	}

	// �{�X���o��������
	if (m_EnemyInfo.nCreatenCount == 60)//420
	{
		CEnemyBoss::Create(D3DXVECTOR3((float)CRenderer::SCREEN_WIDTH, (float)CRenderer::SCREEN_HEIGHT + CEnemyBoss::SIZE_HEIGHT, 0.0f), CEnemy::TYPE_DARK_BOSS);
	}

	if (m_EnemyInfo.nCreatenCount == 100)
	{
		CFadeScene::Create(CFadeScene::TYPE_BLACK);
	}

	// �{�X��p�w�i�̐���
	if (m_EnemyInfo.nCreatenCount == 520)
	{
		//CBgMove::Create();
		m_pMeshField = CMeshField::Create();
	}

	//���S�̐���
	CreateLogo(m_EnemyInfo.nCreatenCount);
}

//-----------------------------------------------------------------------------------------------
// ���S�̐���
//-----------------------------------------------------------------------------------------------
void CGame::CreateLogo(int nCounter)
{
	if (nCounter == 100)
	{
		// �{�X�ڋߒ��̃��S
		CLogo::Create(D3DXVECTOR3(CRenderer::SCREEN_WIDTH / 2, 300.0f, 0.0f), D3DXVECTOR2(CRenderer::SCREEN_WIDTH - 400.0f, 90.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, CLogo::TYPE_WARNING, CLogo::ANIM_LENGTHWISE, 420);
	}

	if (nCounter == 220)
	{
		// �{�X�ڋߒ��̐������S
		CLogo::Create(D3DXVECTOR3(CRenderer::SCREEN_WIDTH / 2, 500.0f, 0.0f), D3DXVECTOR2(CRenderer::SCREEN_WIDTH - 760.0f, 270.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, CLogo::TYPE_WARNING_SUB, CLogo::ANIM_HORIZONTALLY, 300);
	}
}

//-----------------------------------------------------------------------------------------------
// �{�X�̎��S�t���O�ݒ�
//-----------------------------------------------------------------------------------------------
void CGame::SetDieBoss(bool bDie)
{
	// �{�X�̎��S��Ԃ�ݒ�
	m_bDieBoss = bDie;
	// ��ʂ��~�߂�
	CManager::SetPause(true);
}

//-----------------------------------------------------------------------------------------------
// �e�N�X�`���ǂݍ���
//-----------------------------------------------------------------------------------------------
void CGame::LoadAll()
{
	// �G�t�F�N�g
	CEffect::Load();
	// �_
	CCloud::Load();
	// �v���C���[
	CPlayer::Load();
	// �G
	CEnemy::Load();
	// �e
	CBullet::Load();
	// ����
	CExplosion::Load();
	// �����Ԃ�
	CSpray::Load();
	// �A�G�t�F�N�g
	CBubble::Load();
	// �w�i
	CBg::Load();
	// �X�R�A
	CNumber::Load();
	// ���S
	CLogo::Load();
	// �|�[�Y���
	CPause::Load();
	// �����ړ����̔w�i
	CBgMove::Load();
	// �{�X�w�i
	CMeshField::Load();
	// �e�I�v�V����
	CBulletOption::Load();
}

//-----------------------------------------------------------------------------------------------
// �e�N�X�`���j���@
//-----------------------------------------------------------------------------------------------
void CGame::UnloadAll()
{
	// �G�t�F�N�g
	CEffect::Unload();
	// �_
	CCloud::Unload();
	// �v���C���[
	CPlayer::Unload();
	// �G
	CEnemy::Unload();
	// �e
	CBullet::Unload();
	// ����
	CExplosion::Unload();
	// �����Ԃ�
	CSpray::Unload();
	// �A�G�t�F�N�g
	CBubble::Unload();
	// �w�i
	CBg::Unload();
	// �X�R�A
	CNumber::Unload();
	// ���S
	CLogo::Unload();
	// �|�[�Y���
	CPause::Unload();
	// �����ړ����̔w�i
	CBgMove::Unload();
	// �{�X�w�i
	CMeshField::Unload();
	// �e�I�v�V����
	CBulletOption::Unload();
}
