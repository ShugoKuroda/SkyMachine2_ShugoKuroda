//=============================================================================
//
// ���U���g��ʏ��� [result.cpp]
// Author : SHUGO KURODA
//
//=============================================================================
#include "result.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************

//=============================================================================
// CGame�̃R���X�g���N�^
//=============================================================================
CResult::CResult()
{

}

//=============================================================================
// CManager�̃f�X�g���N�^
//=============================================================================
CResult::~CResult()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CResult::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	//// �����o�ϐ��̏�����
	//m_resultMode = RESULTMODE_NORMAL;

	//// �f�[�^�ۑ��p�I�u�W�F�N�g�̎擾
	//CKeepObject *pKeepObject = CManager::GetKeepObject();

	//CTexture *pTex = CManager::GetTexture();
	//CBg::Create({ 640.0f,360.0f,0.0f }, { 1280.0f,720.0f }, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f), pTex->SetTexture("data/TEXTURE/field.jpg"));
	//CScore *pScore = CScore::Create({ 640.0f,360.0f,0.0f }, { 50.0f,100.0f }, 50.0f, CScore::SCOREPLACEMENT_LEFT, pTex->SetTexture("data/TEXTURE/number000.png"));
	//pScore->SetScore(CManager::GetKeepObject()->GetScore());
	//CScene2D::Create({ 500.0f,300.0f,0.0f }, { 150.0f,70.0f,0.0f }, CScene::PRIORITY_UI, pTex->SetTexture("data/TEXTURE/MyScore000.png"));
	//CScene2D::Create({ 640.0f,180.0f,0.0f }, { 300.0f,140.0f,0.0f }, CScene::PRIORITY_UI, pTex->SetTexture("data/TEXTURE/Result000.png"));

	//// ���U���g�Ǘ��̐���
	//m_pResultManager = new CResultManager;

	//if (m_pResultManager != NULL)
	//{
	//	m_pResultManager->Init({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f });
	//}

	//// �T�E���h
	//CSound::Play(CSound::SOUND_LABEL_RESULT);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CResult::Uninit(void)
{
	// �T�E���h
	//CSound::Stop();
}

//=============================================================================
// �X�V����
//=============================================================================
void CResult::Update(void)
{
	//CBase *pBase = NULL;

	//switch (m_resultMode)
	//{
	//	// �ʏ�
	//case RESULTMODE_NORMAL:
	//	break;
	//	// �^�C�g���ɑJ��
	//case RESULTMODE_RESTART:
	//	pBase = new CGame;

	//	if (pBase != NULL)
	//	{
	//		CManager::GetFade()->SetFade(CFade::FADE_OUT, pBase);
	//	}
	//	break;
	//	// �Q�[���ɑJ��
	//case RESULTMODE_TRANSITION_MODESELECT:
	//	pBase = new CModeSelect;

	//	if (pBase != NULL)
	//	{
	//		CManager::GetFade()->SetFade(CFade::FADE_OUT, pBase);
	//	}
	//	break;
	//	// ���U���g�ɑJ��
	//case RESULTMODE_TRANSITION_TITLE:
	//	pBase = new CTitle;

	//	if (pBase != NULL)
	//	{
	//		CManager::GetFade()->SetFade(CFade::FADE_OUT, pBase);
	//	}
	//	break;
	//}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CResult::Draw(void)
{

}