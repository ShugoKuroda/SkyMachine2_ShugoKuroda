//===============================================================================================
//
// �Q�[����ʏ��� [game.cpp]
// Author : SHUGO KURODA
//
//===============================================================================================
#include "game.h"
#include "object.h"

#include "library.h"
#include "load.h"

#include "bg.h"
#include "cloud.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "explosion.h"
#include "spray.h"
#include "bubble.h"

//-----------------------------------------------------------------------------------------------
// �ÓI�����o�ϐ�
//-----------------------------------------------------------------------------------------------
bool CGame::m_bCreateCloud = true; 
bool CGame::m_bCreateBubble = false;
CPlayer *CGame::m_pPlayer = nullptr;

//-----------------------------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------------------------
CGame::CGame() :m_nCntBubble(0), m_nRandBubble(0)
{
	//�G�̐�������������
	ZeroMemory(&m_EnemySetInfo, sizeof(m_EnemySetInfo));
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
	m_EnemySetInfo.pCreateInfo = LoadSpace::GetEnemy();
	//�E�F�[�u���̓ǂݍ���
	m_EnemySetInfo.nWave = LoadSpace::GetWave();
	// �e�N�X�`���ǂݍ���
	LoadAll();
	// �v���C���[����
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(300.0f, 300.0f, 0.0f));
	// �w�i�̐���
	CBg::Create(CBg::SET_A);

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
		m_CloudInfo.nRandTime = LibrarySpace::GetRandNum(250, 180);
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
		D3DXVECTOR3 posBubble = D3DXVECTOR3((float)LibrarySpace::GetRandNum(CRenderer::SCREEN_WIDTH, 0), (float)LibrarySpace::GetRandNum(CRenderer::SCREEN_HEIGHT, CRenderer::SCREEN_HEIGHT - 250), 0.0f);

		for (int nCntBubble = 0; nCntBubble < 3; nCntBubble++)
		{
			CBubble::Create(D3DXVECTOR3(posBubble.x, posBubble.y - (nCntBubble * 20), posBubble.z), D3DXVECTOR2((float)CBubble::MIN_SIZE * (nCntBubble + 1), (float)CBubble::MIN_SIZE * (nCntBubble + 1)));
		}

		//�J�E���^�[���Z�b�g
		m_nCntBubble = 0;

		//�A�G�t�F�N�g�̍ďo�����Ԃ𗐐��Őݒ�
		m_nRandBubble = LibrarySpace::GetRandNum(120, 30);
	}
}

//-----------------------------------------------------------------------------------------------
// �G�̐���(�O���X�N���v�g�g�p)
//-----------------------------------------------------------------------------------------------
void CGame::CreateEnemy()
{
	// �G�����J�E���^�[�̉��Z
	m_EnemySetInfo.nCreatenCount++;

	//�G�̃E�F�[�u��������������
	for (int nCntEnemy = 0; nCntEnemy < m_EnemySetInfo.nWave; nCntEnemy++)
	{
		// �G���o������t���[�����ɒB������ && �G�̏o�����I����Ă��Ȃ��Ȃ�
		if (m_EnemySetInfo.pCreateInfo[nCntEnemy].nFrame <= m_EnemySetInfo.nCreatenCount && m_EnemySetInfo.CountInfo[nCntEnemy].bCreate == false)
		{
			//�G���A���ŏo������܂ł̃J�E���^�[�����Z
			m_EnemySetInfo.CountInfo[nCntEnemy].nCreateTime++;
			//�G�̘A���o���Ԋu�J�E���^�[���ő�ɒB������
			if (m_EnemySetInfo.CountInfo[nCntEnemy].nCreateTime >= m_EnemySetInfo.pCreateInfo[nCntEnemy].nInterval)
			{
				//�G�̐���(�ʒu�A��ށA�ړ����������ɐݒ�)
				CEnemy::Create(m_EnemySetInfo.pCreateInfo[nCntEnemy].pos, (CEnemy::TYPE)m_EnemySetInfo.pCreateInfo[nCntEnemy].nType, m_EnemySetInfo.pCreateInfo[nCntEnemy].nLife, &m_EnemySetInfo.pCreateInfo[nCntEnemy].MoveInfo[0]);

				//�G�̐������J�E���^�[�����Z
				m_EnemySetInfo.CountInfo[nCntEnemy].nCreateNum++;
				//�G�̘A���o���Ԋu�J�E���^�[�̃��Z�b�g
				m_EnemySetInfo.CountInfo[nCntEnemy].nCreateTime = 0;

				//�G���ő吔�܂Ő���������
				if (m_EnemySetInfo.pCreateInfo[nCntEnemy].nNum <= m_EnemySetInfo.CountInfo[nCntEnemy].nCreateNum)
				{//���̃E�F�[�u�̐�������߂�
					m_EnemySetInfo.CountInfo[nCntEnemy].bCreate = true;
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------------------------
// �e�N�X�`���ǂݍ���
//-----------------------------------------------------------------------------------------------
void CGame::LoadAll()
{
	//�w�i
	CBg::Load();
	//�_
	CCloud::Load();
	// �v���C���[
	CPlayer::Load();
	//�G
	CEnemy::Load();
	// �e
	CBullet::Load();
	// ����
	CExplosion::Load();
	//�����Ԃ�
	CSpray::Load();
	// �A�G�t�F�N�g
	CBubble::Load();
}

//-----------------------------------------------------------------------------------------------
// �e�N�X�`���j���@
//-----------------------------------------------------------------------------------------------
void CGame::UnloadAll()
{
	//�_
	CCloud::Unload();
	// �v���C���[
	CPlayer::Unload();
	//�G
	CEnemy::Unload();
	// �e
	CBullet::Unload();
	// ����
	CExplosion::Unload();
	//�����Ԃ�
	CSpray::Unload();
	// �A�G�t�F�N�g
	CBubble::Unload();
	//�w�i
	CBg::Unload();
}
