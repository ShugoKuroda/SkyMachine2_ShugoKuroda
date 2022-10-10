//-----------------------------------------------------------------------------------------------
//
// �w�i�̏���[bg.cpp]
// Author : SHUGO kURODA
//
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------------------------
#include "bg.h"

#include "manager.h"
#include "renderer.h"
#include "object.h"
#include "object2D.h"
#include "input_keyboard.h"

#include "game.h"
#include "cloud.h"
#include "spray.h"

//-----------------------------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------------------------
#define DEFAULT_COL				(D3DXCOLOR(0.7f, 0.7f, 1.0f, 1.0f))		// �w�i�̏����F
#define DEFAULT_FADE_COL		(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f))		// �t�F�[�h�w�i�̏����F

#define BG_INTERVAL_IN_SEA		(2180)		// �C�ɓ��鉉�o�̊J�n����
#define BG_INTERVAL_SEA_WEED	(2360)		// �C���̕`��J�n����
#define BG_INTERVAL_BUBBLE		(2540)		// ���A�̕`��J�n����
#define BG_INTERVAL_OUT_SEA		(2720)		// �C�ɓ��鉉�o�̏I������

//-----------------------------------------------------------------------------------------------
// �ÓI�����o�ϐ�
//-----------------------------------------------------------------------------------------------
// �e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 CBg::m_apTexture[BG_A_MAX] = {};
CObject2D *CBg::m_apObject2D[BG_A_MAX] = {};

//-----------------------------------------------------------------------------------------------
//	�R���X�g���N�^
//-----------------------------------------------------------------------------------------------
CBg::CBg() :m_set(SET_NONE), m_nCntBgChange(0)
{
	//�I�u�W�F�N�g�̎�ސݒ�
	SetObjType(EObject::OBJ_BG_MOVE);
}

//-----------------------------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------------------------
CBg::~CBg()
{
}

//-----------------------------------------------------------------------------------------------
//	�e�N�X�`���ǂݍ���
//-----------------------------------------------------------------------------------------------
HRESULT CBg::Load()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg000_00.jpg", &m_apTexture[BG_A_SKY]);		// ��
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg000_02.jpg", &m_apTexture[BG_A_UNDERWATER]);	// �C��
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg000_01.jpg", &m_apTexture[BG_A_SEA]);		// �C��
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg000_03.png", &m_apTexture[BG_A_WAVE1]);		// �g1
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg000_04.png", &m_apTexture[BG_A_WAVE2]);		// �g2
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg000_05.png", &m_apTexture[BG_A_WAVE3]);		// �g3
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg000_06.png", &m_apTexture[BG_A_FLOOR]);		// �C���̏�
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg000_11.jpg", &m_apTexture[BG_A_BOSS]);		// �{�X��̔w�i
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg000_07.png", &m_apTexture[BG_A_ROCK]);		// ��
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg000_09.png", &m_apTexture[BG_A_SETWEED]);	// �C��
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg000_10.png", &m_apTexture[BG_A_SETWEED2]);	// �C��2
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg000_08.png", &m_apTexture[BG_A_SEA_OTHER]);	// �C�̗���

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
//	�e�N�X�`���j��
//-----------------------------------------------------------------------------------------------
void CBg::Unload()
{
	for (int nCnt = 0; nCnt < BG_A_MAX; nCnt++)
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
//	��������
//-----------------------------------------------------------------------------------------------
CBg *CBg::Create(ESet set)
{
	// �|�C���^�N���X��錾
	CBg* pBg = new CBg;

	if (pBg != nullptr)
	{
		// �w�i�Q���̐ݒ�
		pBg->m_set = set;

		// ������
		pBg->Init();
	}

	return pBg;
}

//-----------------------------------------------------------------------------------------------
//	������
//-----------------------------------------------------------------------------------------------
HRESULT CBg::Init()
{
	//�X�N���[���T�C�Y�̕ۑ�
	D3DXVECTOR2 ScreenSize = D3DXVECTOR2((float)CRenderer::SCREEN_WIDTH, (float)CRenderer::SCREEN_HEIGHT);

	// �e�w�i�̐����Ə�����
	switch (m_set)
	{
	case SET_A:		// ZONE_A

		for (int nCnt = 0; nCnt < BG_A_MAX; nCnt++)
		{// ����
			m_apObject2D[nCnt] = new CObject2D;
			//�I�u�W�F�N�g�̎�ސݒ�
			m_apObject2D[nCnt]->SetObjType(EObject::OBJ_BG);
		}
		
		//�g�̔w�i�̂ݑO�ŕ`�悷��
		m_apObject2D[BG_A_WAVE1]->SetObjType(CObject::OBJ_WAVE1);
		m_apObject2D[BG_A_WAVE2]->SetObjType(CObject::OBJ_WAVE2);
		m_apObject2D[BG_A_WAVE3]->SetObjType(CObject::OBJ_WAVE3);

		/* �e�w�i�̈ʒu�ƃT�C�Y��ݒ� */
		//��
		m_apObject2D[BG_A_SKY]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, (ScreenSize.y / 2) - 100.0f, 0.0f));
		m_apObject2D[BG_A_SKY]->SetSize(D3DXVECTOR2(ScreenSize.x, ScreenSize.y - 200.0f));
		//�C
		m_apObject2D[BG_A_SEA]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, ScreenSize.y - 75.0f, 0.0f));
		m_apObject2D[BG_A_SEA]->SetSize(D3DXVECTOR2(ScreenSize.x, 250.0f));
		//�g1
		m_apObject2D[BG_A_WAVE1]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, ScreenSize.y + 40.0f, 0.0f));
		m_apObject2D[BG_A_WAVE1]->SetSize(D3DXVECTOR2(ScreenSize.x, 40.0f));
		//�g2
		m_apObject2D[BG_A_WAVE2]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, ScreenSize.y + 40.0f, 0.0f));
		m_apObject2D[BG_A_WAVE2]->SetSize(D3DXVECTOR2(ScreenSize.x, 40.0f));
		//�g3
		m_apObject2D[BG_A_WAVE3]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, ScreenSize.y + 40.0f, 0.0f));
		m_apObject2D[BG_A_WAVE3]->SetSize(D3DXVECTOR2(ScreenSize.x, 40.0f));
		//�C��
		m_apObject2D[BG_A_UNDERWATER]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, ((ScreenSize.y / 2) - 83.5f) * 3, 0.0f));
		m_apObject2D[BG_A_UNDERWATER]->SetSize(D3DXVECTOR2(ScreenSize.x, ScreenSize.y - 100.0f));
		//�C���̏�
		m_apObject2D[BG_A_FLOOR]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, (ScreenSize.y - 50.0f) * 2.5f, 0.0f));
		m_apObject2D[BG_A_FLOOR]->SetSize(D3DXVECTOR2(ScreenSize.x, 100.0f));
		//��
		m_apObject2D[BG_A_ROCK]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, (ScreenSize.y - 110.0f) * 2.5f, 0.0f));
		m_apObject2D[BG_A_ROCK]->SetSize(D3DXVECTOR2(ScreenSize.x, 300.0f));
		//�C��
		m_apObject2D[BG_A_SETWEED]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, (ScreenSize.y - 110.0f) * 2.5f, 0.0f));
		m_apObject2D[BG_A_SETWEED]->SetSize(D3DXVECTOR2(ScreenSize.x, 250.0f));
		//�C��2
		m_apObject2D[BG_A_SETWEED2]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, (ScreenSize.y - 100.0f) * 2.5f, 0.0f));
		m_apObject2D[BG_A_SETWEED2]->SetSize(D3DXVECTOR2(ScreenSize.x, 200.0f));
		//�w�i�t�F�[�h�p
		m_apObject2D[BG_A_FADEBLACK]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, ScreenSize.y / 2, 0.0f));
		m_apObject2D[BG_A_FADEBLACK]->SetSize(D3DXVECTOR2(ScreenSize.x, ScreenSize.y));

		// �������ƃe�N�X�`���̐ݒ�
		for (int nCnt = 0; nCnt < BG_A_MAX; nCnt++)
		{
			m_apObject2D[nCnt]->Init();
			m_apObject2D[nCnt]->BindTexture(m_apTexture[nCnt]);
		}

		// �F�̐ݒ�
		for (int nCnt = BG_A_ROCK; nCnt < BG_A_FADEBLACK; nCnt++)
		{
			m_apObject2D[nCnt]->SetColor(DEFAULT_COL);
		}
		// �t�F�[�h�w�i�����ʐF�ɐݒ�
		m_apObject2D[BG_A_FADEBLACK]->SetColor(DEFAULT_FADE_COL);

		break;

	case SET_B:		// ZONE_B
		break;

	case SET_C:		// ZONE_C
		break;

	default:
		break;
	}

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
//	�I��
//-----------------------------------------------------------------------------------------------
void CBg::Uninit()
{
	// �w�i�|���S���̏I��
	for (int nCnt = 0; nCnt < BG_A_MAX; nCnt++)
	{
		if (m_apObject2D[nCnt] != nullptr)
		{
			m_apObject2D[nCnt]->Uninit();
			m_apObject2D[nCnt] = nullptr;
		}
	}

	// �w�i�I�u�W�F�N�g�̏I��
	Release();
}

//-----------------------------------------------------------------------------------------------
//	�X�V
//-----------------------------------------------------------------------------------------------
void CBg::Update()
{
	// �w�i�t�F�[�h
	FadeBack();

	// �w�i�S�̂̃A�j���[�V��������
	AnimationBgAll();
}

//-----------------------------------------------------------------------------------------------
//	�`��
//-----------------------------------------------------------------------------------------------
void CBg::Draw()
{
}

//-----------------------------------------------------------------------------------------------
//	�w�i�t�F�[�h
//-----------------------------------------------------------------------------------------------
void CBg::FadeBack()
{
	// �t�F�[�h�w�i�̐F���擾
	D3DXCOLOR col = m_apObject2D[BG_A_FADEBLACK]->GetColor();
	// �����ɂ���
	col.a -= 0.01f;

	if (col.a > 0.0f)
	{// ���S�����ɂȂ�܂ŐF�ݒ�
		m_apObject2D[BG_A_FADEBLACK]->SetColor(col);
	}
}

//-----------------------------------------------------------------------------------------------
//	�w�i�S�̂̃A�j���[�V��������
//-----------------------------------------------------------------------------------------------
void CBg::AnimationBgAll()
{
	// �C����ɉ��A�j���[�V��������
	m_apObject2D[BG_A_SEA]->SetAnimBg(1, 1000, true);

	// �w�i�ړ��܂ł̃J�E���^�[�����Z
	m_nCntBgChange++;

	// ��莞�Ԍo�߂ŊC�ɓ��鉉�o���~�߂�
	if (m_nCntBgChange >= BG_INTERVAL_OUT_SEA)
	{
		// �J�E���^�[���~�߂�
		m_nCntBgChange = m_nCntBgChange;
		// ���̃A�j���[�V����
		m_apObject2D[BG_A_FLOOR]->SetAnimBg(1, 1000, true);
		// ��̃A�j���[�V����
		m_apObject2D[BG_A_ROCK]->SetAnimBg(1, 2000, true);
		// �C���̃A�j���[�V����
		m_apObject2D[BG_A_SETWEED]->SetAnimBg(1, 1500, true);
		// �C��2�̃A�j���[�V����
		m_apObject2D[BG_A_SETWEED2]->SetAnimBg(1, 1000, true);
	}
	// ��莞�Ԍo�߂ŊC�ɓ��鉉�o���J�n����
	else if (m_nCntBgChange >= BG_INTERVAL_IN_SEA)
	{
		// �w�i�ʒu�̕ۑ��p
		D3DXVECTOR3 aPosBg[BG_A_MAX];
		for (int nCnt = 0; nCnt < BG_A_MAX; nCnt++)
		{// �w�i�ʒu�̎擾
			aPosBg[nCnt] = m_apObject2D[nCnt]->GetPosition();
		}

		// �C�̃T�C�Y���擾
		D3DXVECTOR2 sizeSea = m_apObject2D[BG_A_SEA]->GetSize();

		// �w�i�|���S���̈ړ��{����ݒ�
		float fMul = 1.5f;

		// ��莞�Ԍo�߂ŖA�̐������J�n
		if (m_nCntBgChange >= BG_INTERVAL_BUBBLE)
		{
			//�A�G�t�F�N�g�̐������J�n����
			CGame::SetCreateBubble(true);

			// �w�i�|���S���̈ړ��{����ݒ�
			fMul = 0.75f;

			/* �e�w�i�|���S���̍�����ς��� */
			// �C
			aPosBg[BG_A_SEA].y += 0.5f;
			// �T�C�Y���ς���
			sizeSea.y += 0.5f;
			// �C��
			aPosBg[BG_A_UNDERWATER].y += 0.25f;
			// ��
			aPosBg[BG_A_SKY].y += 0.25f;

			//���̃A�j���[�V����
			m_apObject2D[BG_A_FLOOR]->SetAnimBg(1, 1000, true);
			//��̃A�j���[�V����
			m_apObject2D[BG_A_ROCK]->SetAnimBg(1, 2000, true);
			//�C���̃A�j���[�V����
			m_apObject2D[BG_A_SETWEED]->SetAnimBg(1, 1500, true);
			//�C��2�̃A�j���[�V����
			m_apObject2D[BG_A_SETWEED2]->SetAnimBg(1, 1000, true);

			//�_�̐������~�߂�
			CGame::SetCreateCloud(false);
		}
		// ��莞�Ԍo�߂ŊC���w�i�̃A�j���[�V�������J�n
		else if (m_nCntBgChange >= BG_INTERVAL_SEA_WEED)
		{
			// �w�i�|���S���̈ړ��{����ݒ�
			fMul = 3.0f;

			//��̃A�j���[�V����
			m_apObject2D[BG_A_ROCK]->SetAnimBg(1, 2000, true);
			//�C���̃A�j���[�V����
			m_apObject2D[BG_A_SETWEED]->SetAnimBg(1, 1500, true);
			//�C��2�̃A�j���[�V����
			m_apObject2D[BG_A_SETWEED2]->SetAnimBg(1, 1000, true);
		}

		//�_��w�i�ɍ��킹�ē�����
		CCloud::AddPos(fMul);

		//�w�i�ɍ��킹�đ��I�u�W�F�N�g���ړ�
		CCloud::Move(-0.5f * fMul);		//�_
		CSpray::Move(-1.0f * fMul);		//�����Ԃ�

		/* �e�w�i�|���S���̍�����ς��� */
		// ��
		aPosBg[BG_A_SKY].y -= 0.5f * fMul;
		// �C��
		aPosBg[BG_A_UNDERWATER].y -= 0.5f * fMul;

		// �C���̏�
		aPosBg[BG_A_FLOOR].y -= 1.0f * fMul;
		// �C���̊�
		aPosBg[BG_A_ROCK].y -= 1.0f * fMul;
		// �C��1
		aPosBg[BG_A_SETWEED].y -= 1.0f * fMul;
		// �C��2
		aPosBg[BG_A_SETWEED2].y -= 1.0f * fMul;

		// �g1
		aPosBg[BG_A_WAVE1].y -= 1.0f * fMul;
		// �g2
		aPosBg[BG_A_WAVE2].y -= 1.0f * fMul;
		// �g3
		aPosBg[BG_A_WAVE3].y -= 1.0f * fMul;

		// �C
		aPosBg[BG_A_SEA].y -= 0.75f * fMul;
		// �T�C�Y���ς���
		sizeSea.y -= 0.5f * fMul;

		if (sizeSea.y <= 0.0f)
		{// �C�ʂ̃e�N�X�`���ݒ�
			m_apObject2D[BG_A_SEA]->BindTexture(m_apTexture[BG_A_SEA_OTHER]);
		}
		// �C�ʂ̃T�C�Y�ݒ�
		m_apObject2D[BG_A_SEA]->SetSize(sizeSea);

		//�w�i�ʒu�̐ݒ�
		for (int nCnt = 0; nCnt < BG_A_MAX; nCnt++)
		{
			m_apObject2D[nCnt]->SetPosition(aPosBg[nCnt]);
			m_apObject2D[nCnt]->SetVertex();
		}

		//�g�̃A�j���[�V����
		m_apObject2D[BG_A_WAVE1]->SetAnimBg(3, 200, true);
		m_apObject2D[BG_A_WAVE2]->SetAnimBg(4, 200, true);
		m_apObject2D[BG_A_WAVE3]->SetAnimBg(2, 200, true);
	}
}
