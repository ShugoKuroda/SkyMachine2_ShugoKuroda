//=============================================================================
//
// リザルト画面処理 [result.cpp]
// Author : SHUGO KURODA
//
//=============================================================================
#include "result.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************

//=============================================================================
// CGameのコンストラクタ
//=============================================================================
CResult::CResult()
{

}

//=============================================================================
// CManagerのデストラクタ
//=============================================================================
CResult::~CResult()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CResult::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	//// メンバ変数の初期化
	//m_resultMode = RESULTMODE_NORMAL;

	//// データ保存用オブジェクトの取得
	//CKeepObject *pKeepObject = CManager::GetKeepObject();

	//CTexture *pTex = CManager::GetTexture();
	//CBg::Create({ 640.0f,360.0f,0.0f }, { 1280.0f,720.0f }, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f), pTex->SetTexture("data/TEXTURE/field.jpg"));
	//CScore *pScore = CScore::Create({ 640.0f,360.0f,0.0f }, { 50.0f,100.0f }, 50.0f, CScore::SCOREPLACEMENT_LEFT, pTex->SetTexture("data/TEXTURE/number000.png"));
	//pScore->SetScore(CManager::GetKeepObject()->GetScore());
	//CScene2D::Create({ 500.0f,300.0f,0.0f }, { 150.0f,70.0f,0.0f }, CScene::PRIORITY_UI, pTex->SetTexture("data/TEXTURE/MyScore000.png"));
	//CScene2D::Create({ 640.0f,180.0f,0.0f }, { 300.0f,140.0f,0.0f }, CScene::PRIORITY_UI, pTex->SetTexture("data/TEXTURE/Result000.png"));

	//// リザルト管理の生成
	//m_pResultManager = new CResultManager;

	//if (m_pResultManager != NULL)
	//{
	//	m_pResultManager->Init({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f });
	//}

	//// サウンド
	//CSound::Play(CSound::SOUND_LABEL_RESULT);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CResult::Uninit(void)
{
	// サウンド
	//CSound::Stop();
}

//=============================================================================
// 更新処理
//=============================================================================
void CResult::Update(void)
{
	//CBase *pBase = NULL;

	//switch (m_resultMode)
	//{
	//	// 通常
	//case RESULTMODE_NORMAL:
	//	break;
	//	// タイトルに遷移
	//case RESULTMODE_RESTART:
	//	pBase = new CGame;

	//	if (pBase != NULL)
	//	{
	//		CManager::GetFade()->SetFade(CFade::FADE_OUT, pBase);
	//	}
	//	break;
	//	// ゲームに遷移
	//case RESULTMODE_TRANSITION_MODESELECT:
	//	pBase = new CModeSelect;

	//	if (pBase != NULL)
	//	{
	//		CManager::GetFade()->SetFade(CFade::FADE_OUT, pBase);
	//	}
	//	break;
	//	// リザルトに遷移
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
// 描画処理
//=============================================================================
void CResult::Draw(void)
{

}