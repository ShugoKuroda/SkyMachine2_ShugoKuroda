//=============================================================================
//
//	プレイヤー処理[player.cpp]
//	Author : SHUGO KURODA
//
//=============================================================================
#include "manager.h"
#include "input_keyboard.h"
#include "input_joypad.h"
#include "sound.h"
#include "renderer.h"

#include "library.h"

#include "bg.h"
#include "spray.h"
#include "bullet.h"
#include "player.h"
#include "explosion.h"
#include "ui.h"
#include "gauge.h"
#include "life.h"
#include "score.h"

#include "rank.h"

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define PLAYER_UI_SIZE		(D3DXVECTOR2(200.0f, 50.0f))
#define LIFE_UI_SIZE		(D3DXVECTOR2(120.0f, 30.0f))
#define LEVEL_UI_SIZE		(D3DXVECTOR2(50.0f, 50.0f))
#define ATTACK_INTERVAL		(7)

//-----------------------------------------------------------------------------
// using宣言
//-----------------------------------------------------------------------------
using namespace LibrarySpace;

//*****************************************************************************
// 定数宣言
//*****************************************************************************
const float CPlayer::SIZE_X = 90.0f;
const float CPlayer::SIZE_Y = 40.0f;
const float CPlayer::ENTRY_SIZE_X = 630.0f;
const float CPlayer::ENTRY_SIZE_Y = 280.0f;
const float CPlayer::MOVE_DEFAULT = 10.0f;
// アニメーション間隔
const int CPlayer::ANIM_INTERVAL = 5;
// アニメーション最大数
const int CPlayer::MAX_ANIM = 2;
// U座標の最大分割数
const int CPlayer::DIVISION_U = 2;
// V座標の最大分割数
const int CPlayer::DIVISION_V = 4;
// プレイヤーのデフォルトカラー
const D3DXCOLOR CPlayer::DEFAULT_COL = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
// デフォルト残機
const int CPlayer::DEFAULT_LIFE = 2;

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
// テクスチャのポインタ
LPDIRECT3DTEXTURE9 CPlayer::m_apTexture[2] = { nullptr };

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CPlayer::CPlayer() :
	m_move(0.0f, 0.0f, 0.0f), m_state(STATE_NORMAL), m_nCntState(0), m_nCntAttack(0), m_nCntAnim(0), m_nPatternAnim(0), m_nCntAnimMove(0),
	m_nTexRotType(TYPE_NEUTRAL), m_nPlayerNum(0), posBullet(0.0f, 0.0f), m_bControl(false), m_bInSea(false), m_pOption{ nullptr }, m_pBarrier(nullptr),
	m_OptionLevel(CBulletOption::LEVEL_NONE),m_BarrierLevel(CBarrier::LEVEL_NONE),m_BulletLevel(CPlayer::LEVEL_1), m_pLife(nullptr),m_pScore(nullptr),
	m_bDie(false)
{
	//オブジェクトの種類設定
	SetObjType(EObject::OBJ_PLAYER);
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CPlayer::~CPlayer()
{
}

//-----------------------------------------------------------------------------
// インスタンス生成処理
//-----------------------------------------------------------------------------
CPlayer *CPlayer::Create(const D3DXVECTOR3& pos, const int& nNum)
{
	//インスタンス生成
	CPlayer *pPlayer = new CPlayer;

	if (pPlayer != nullptr)
	{
		float fPos = 0.0f;
		// 2Pのみ、ずらして配置
		if (nNum == PLAYER_2)
		{
			fPos = -150.0f;
		}

		// 位置設定
		pPlayer->SetPosition(D3DXVECTOR3(pos.x + fPos, pos.y, pos.z));
		// プレイヤー数の設定
		pPlayer->m_nPlayerNum = nNum;
		// 生成処理
		pPlayer->Init();
		// テクスチャの設定
		pPlayer->BindTexture(m_apTexture[nNum]);
	}

	return pPlayer;
}

//-----------------------------------------------------------------------------
// テクスチャの読み込み
//-----------------------------------------------------------------------------
HRESULT CPlayer::Load()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/player000.png",
		&m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/player001.png",
		&m_apTexture[1]);

	return S_OK;
}

//-----------------------------------------------------------------------------
//	テクスチャの削除
//-----------------------------------------------------------------------------
void CPlayer::Unload()
{
	for (int nCnt = 0; nCnt < PLAYER_MAX; nCnt++)
	{
		// テクスチャの破棄
		if (m_apTexture[nCnt] != nullptr)
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = nullptr;
		}
	}
}

//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
HRESULT CPlayer::Init()
{
	// 弾の発射位置を設定
	posBullet = D3DXVECTOR2(20.0f, 10.0f);
	// 登場状態にする
	m_state = STATE_ENTRY;

	// サイズの設定
	CObject2D::SetSize(D3DXVECTOR2(ENTRY_SIZE_X, ENTRY_SIZE_Y));
	// 初期化
	CObject2D::Init();
	// 色の設定
	CObject2D::SetColor(DEFAULT_COL);
	// テクスチャ座標の設定
	CObject2D::SetAnimation(m_nPatternAnim, m_nTexRotType, DIVISION_U, DIVISION_V);

	// 各UIの生成
	switch (m_nPlayerNum)
	{
		// プレイヤー1の場合
	case PLAYER_1:
		// PLAYER1のUI
		CUi::Create(D3DXVECTOR3(120.0f, 25.0f, 0.0f), PLAYER_UI_SIZE, CUi::TYPE_PLAYER1, CUi::ANIM_NONE, (CUi::PARENT)m_nPlayerNum);
		// 残機UI
		CUi::Create(D3DXVECTOR3(CRenderer::SCREEN_WIDTH / 2 - 150.0f, 30.0f, 0.0f), LIFE_UI_SIZE, CUi::TYPE_LIFE1, CUi::ANIM_NONE, (CUi::PARENT)m_nPlayerNum);
		// メイン弾UI
		CUi::Create(D3DXVECTOR3(50.0f, 80.0f, 0.0f), LEVEL_UI_SIZE, CUi::TYPE_BULLET, CUi::ANIM_NONE, (CUi::PARENT)m_nPlayerNum);
		// オプション弾UI
		CUi::Create(D3DXVECTOR3(240.0f, 80.0f, 0.0f), LEVEL_UI_SIZE, CUi::TYPE_BULLET_OPTION, CUi::ANIM_NONE, (CUi::PARENT)m_nPlayerNum);
		// バリアUI
		CUi::Create(D3DXVECTOR3(430.0f, 80.0f, 0.0f), LEVEL_UI_SIZE, CUi::TYPE_BARRIER, CUi::ANIM_NONE, (CUi::PARENT)m_nPlayerNum);

		// メイン弾ゲージUI
		CGauge::Create(D3DXVECTOR3(140.0f, 80.0f, 0.0f), CGauge::TYPE_BULLET, (CGauge::PARENT)m_nPlayerNum);
		// オプション弾ゲージUI
		CGauge::Create(D3DXVECTOR3(330.0f, 80.0f, 0.0f), CGauge::TYPE_BULLET_OPTION, (CGauge::PARENT)m_nPlayerNum);
		// バリアゲージUI
		CGauge::Create(D3DXVECTOR3(520.0f, 80.0f, 0.0f), CGauge::TYPE_BARRIER, (CGauge::PARENT)m_nPlayerNum);

		// スコアの生成
		m_pScore = CScore::Create(D3DXVECTOR3(250.0f, 25.0f, 0.0f), D3DXVECTOR2(30.0f, 30.0f), 20);
		// ライフの生成
		m_pLife = CLife::Create(D3DXVECTOR3(CRenderer::SCREEN_WIDTH / 2 - 100.0f, 30.0f, 0.0f), D3DXVECTOR2(30.0f, 30.0f));

		break;

	case PLAYER_2:
		// PLAYER2のUI
		CUi::Create(D3DXVECTOR3(820.0f, 25.0f, 0.0f), PLAYER_UI_SIZE, CUi::TYPE_PLAYER2, CUi::ANIM_NONE, (CUi::PARENT)m_nPlayerNum);
		// 残機UI
		CUi::Create(D3DXVECTOR3(CRenderer::SCREEN_WIDTH - 100.0f, 30.0f, 0.0f), LIFE_UI_SIZE, CUi::TYPE_LIFE2, CUi::ANIM_NONE, (CUi::PARENT)m_nPlayerNum);
		// メイン弾UI
		CUi::Create(D3DXVECTOR3(730.0f, 80.0f, 0.0f), LEVEL_UI_SIZE, CUi::TYPE_BULLET, CUi::ANIM_NONE, (CUi::PARENT)m_nPlayerNum);
		// オプション弾UI
		CUi::Create(D3DXVECTOR3(920.0f, 80.0f, 0.0f), LEVEL_UI_SIZE, CUi::TYPE_BULLET_OPTION, CUi::ANIM_NONE, (CUi::PARENT)m_nPlayerNum);
		// バリアUI
		CUi::Create(D3DXVECTOR3(1110.0f, 80.0f, 0.0f), LEVEL_UI_SIZE, CUi::TYPE_BARRIER, CUi::ANIM_NONE, (CUi::PARENT)m_nPlayerNum);

		// メイン弾ゲージUI
		CGauge::Create(D3DXVECTOR3(830.0f, 80.0f, 0.0f), CGauge::TYPE_BULLET, (CGauge::PARENT)m_nPlayerNum);
		// オプション弾ゲージUI
		CGauge::Create(D3DXVECTOR3(1020.0f, 80.0f, 0.0f), CGauge::TYPE_BULLET_OPTION, (CGauge::PARENT)m_nPlayerNum);
		// バリアゲージUI
		CGauge::Create(D3DXVECTOR3(1210.0f, 80.0f, 0.0f), CGauge::TYPE_BARRIER, (CGauge::PARENT)m_nPlayerNum);

		// スコアの生成
		m_pScore = CScore::Create(D3DXVECTOR3(950.0f, 25.0f, 0.0f), D3DXVECTOR2(30.0f, 30.0f), 20);
		// ライフの生成
		m_pLife = CLife::Create(D3DXVECTOR3(CRenderer::SCREEN_WIDTH - 50.0f, 30.0f, 0.0f), D3DXVECTOR2(30.0f, 30.0f));

		break;

	default:
		break;
	}

	// ライフの設定
	m_pLife->Add(DEFAULT_LIFE);

	return S_OK;
}

//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void CPlayer::Uninit()
{
	CObject2D::Uninit();
}

//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void CPlayer::Update()
{
	// 位置情報を取得
	D3DXVECTOR3 pos = CObject2D::GetPosition();

	//操作できる状態なら
	if (m_bControl == true)
	{
		//移動処理
		pos = Move(pos);

		// キーボード情報の取得
		CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
		CInputJoypad *pJoypad = CManager::GetInputJoypad();

		if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_ATTACK) == true ||
			pJoypad->GetPress(CInputJoypad::JOYKEY_A, m_nPlayerNum))
		{//攻撃キー押下

			//攻撃カウンターの加算
			m_nCntAttack++;

			if (m_nCntAttack > ATTACK_INTERVAL)
			{
				// プレイヤーの強化状態によって出す弾を設定
				switch (m_BulletLevel)
				{
				case CPlayer::LEVEL_1:
					//弾の設定
					CBullet::Create(D3DXVECTOR3(pos.x + posBullet.x, pos.y + posBullet.y, pos.z), D3DXVECTOR3(15.0f, 0.0f, 0.0f),
						5, CBullet::TYPE_PLAYER_BLUE)->SetParent((CBullet::EParent)m_nPlayerNum);
					break;

				case CPlayer::LEVEL_2:
					//弾1の設定
					CBullet::Create(D3DXVECTOR3(pos.x + posBullet.x, pos.y + posBullet.y - 15.0f, pos.z), D3DXVECTOR3(15.0f, 0.0f, 0.0f),
						5, CBullet::TYPE_PLAYER_BLUE)->SetParent((CBullet::EParent)m_nPlayerNum);
					//弾2の設定
					CBullet::Create(D3DXVECTOR3(pos.x + posBullet.x, pos.y + posBullet.y + 15.0f, pos.z), D3DXVECTOR3(15.0f, 0.0f, 0.0f),
						5, CBullet::TYPE_PLAYER_BLUE)->SetParent((CBullet::EParent)m_nPlayerNum);
					break;

				case CPlayer::LEVEL_3:
					//弾1の設定
					CBullet::Create(D3DXVECTOR3(pos.x + posBullet.x, pos.y + posBullet.y, pos.z), D3DXVECTOR3(15.0f, 0.0f, 0.0f),
						5, CBullet::TYPE_PLAYER_BLUE)->SetParent((CBullet::EParent)m_nPlayerNum);
					//弾2の設定
					CBullet::Create(D3DXVECTOR3(pos.x + posBullet.x, pos.y + posBullet.y - 30.0f, pos.z), D3DXVECTOR3(15.0f, 0.0f, 0.0f),
						5, CBullet::TYPE_PLAYER_BLUE)->SetParent((CBullet::EParent)m_nPlayerNum);
					//弾3の設定
					CBullet::Create(D3DXVECTOR3(pos.x + posBullet.x, pos.y + posBullet.y + 30.0f, pos.z), D3DXVECTOR3(15.0f, 0.0f, 0.0f),
						5, CBullet::TYPE_PLAYER_BLUE)->SetParent((CBullet::EParent)m_nPlayerNum);
					break;

				default:
					break;
				}

				// オプション弾の攻撃処理
				for (int nCnt = 0; nCnt < MAX_OPTION; nCnt++)
				{
					if (m_pOption[nCnt] != nullptr)
					{
						m_pOption[nCnt]->Attack();
					}
				}

				//攻撃カウンターをリセット
				m_nCntAttack = 0;

				//サウンド再生
				CSound::Play(CSound::SOUND_LABEL_SE_SHOT);
			}
		}
		else
		{
			m_nCntAttack = 10;
		}

		//可動範囲を画面内に制限
		SteyInScreen2D(&pos, CObject2D::GetSize());
	}

	//水しぶきを設定
	SetSpray();

	//位置情報更新
	CObject2D::SetPosition(pos);

	//状態管理
	State();

	// カウントを増やす
	m_nCntAnim++;
	if (m_nCntAnim % ANIM_INTERVAL == 0)
	{
		// 今のアニメーションを1つ進める
		m_nPatternAnim++;
	}

	if (m_nPatternAnim == MAX_ANIM)
	{// アニメーションが終わったら
		// アニメーションパターンを初期化する
		m_nPatternAnim = 0;
	}
	else
	{
		//頂点座標の設定
		CObject2D::SetVertex();

		//テクスチャアニメーション
		CObject2D::SetAnimation(m_nPatternAnim, m_nTexRotType, DIVISION_U, DIVISION_V);
	}
}

//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void CPlayer::Draw()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャブレンディングを加算合成にする
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	//描画
	CObject2D::Draw();

	//テクスチャブレンディングの設定を元に戻す
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
}

//-----------------------------------------------------------------------------
// 移動処理
//-----------------------------------------------------------------------------
D3DXVECTOR3 CPlayer::Move(D3DXVECTOR3 pos)
{
	// キーボード情報の取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	// ジョイパッド情報の取得
	CInputJoypad *pJoypad = CManager::GetInputJoypad();

	if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_LEFT) == true ||
		pJoypad->GetPress(CInputJoypad::JOYKEY_LEFT, m_nPlayerNum) == true ||
		pJoypad->GetStick(CInputJoypad::JOYKEY_LEFT_STICK, m_nPlayerNum).x <= -0.2f)
	{//左キー押下
		if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_UP) == true ||
			pJoypad->GetPress(CInputJoypad::JOYKEY_UP, m_nPlayerNum) == true ||
			pJoypad->GetStick(CInputJoypad::JOYKEY_LEFT_STICK, m_nPlayerNum).y <= -0.2f)
		{//上キー押下
			//移動量加算
			pos.x += GetSinVec(-0.75f, MOVE_DEFAULT);
			pos.y += GetCosVec(-0.75f, MOVE_DEFAULT);
			//アニメーション変更
			SetAnimNum(TYPE_DOWN, TYPE_UP);
			m_nCntAnimMove++;
			//弾の発射位置を設定
			posBullet = D3DXVECTOR2(20.0f, 10.0f);
		}
		else if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_DOWN) == true ||
			pJoypad->GetPress(CInputJoypad::JOYKEY_DOWN, m_nPlayerNum) == true ||
			pJoypad->GetStick(CInputJoypad::JOYKEY_LEFT_STICK, m_nPlayerNum).y >= 0.2f)
		{//下キー押下
			pos.x += GetSinVec(-0.25f, MOVE_DEFAULT);
			pos.y += GetCosVec(-0.25f, MOVE_DEFAULT);
			SetAnimNum(TYPE_UP, TYPE_DOWN);
			m_nCntAnimMove++;
			//弾の発射位置を設定
			posBullet = D3DXVECTOR2(20.0f, -5.0f);
		}
		else
		{
			pos.x += GetSinVec(-0.5f, MOVE_DEFAULT);
			pos.y += GetCosVec(-0.5f, MOVE_DEFAULT);
			m_nTexRotType = TYPE_NEUTRAL;
			m_nCntAnimMove = 0;
			//弾の発射位置を設定
			posBullet = D3DXVECTOR2(20.0f, 10.0f);
		}
	}
	else if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_RIGHT) == true ||
		pJoypad->GetPress(CInputJoypad::JOYKEY_RIGHT, m_nPlayerNum) == true ||
		pJoypad->GetStick(CInputJoypad::JOYKEY_LEFT_STICK, m_nPlayerNum).x >= 0.2f)
	{//右キー押下
		if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_UP) == true ||
			pJoypad->GetPress(CInputJoypad::JOYKEY_UP, m_nPlayerNum) == true ||
			pJoypad->GetStick(CInputJoypad::JOYKEY_LEFT_STICK, m_nPlayerNum).y <= -0.2f)
		{//上キー押下
			pos.x += GetSinVec(0.75f, MOVE_DEFAULT);
			pos.y += GetCosVec(0.75f, MOVE_DEFAULT);
			SetAnimNum(TYPE_DOWN, TYPE_UP);
			m_nCntAnimMove++;
			//弾の発射位置を設定
			posBullet = D3DXVECTOR2(20.0f, 10.0f);
		}
		else if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_DOWN) == true ||
			pJoypad->GetPress(CInputJoypad::JOYKEY_DOWN, m_nPlayerNum) == true ||
			pJoypad->GetStick(CInputJoypad::JOYKEY_LEFT_STICK, m_nPlayerNum).y >= 0.2f)
		{//下キー押下
			pos.x += GetSinVec(0.25f, MOVE_DEFAULT);
			pos.y += GetCosVec(0.25f, MOVE_DEFAULT);
			SetAnimNum(TYPE_UP, TYPE_DOWN);
			m_nCntAnimMove++;
			//弾の発射位置を設定
			posBullet = D3DXVECTOR2(20.0f, -5.0f);
		}
		else
		{
			pos.x += GetSinVec(0.5f, MOVE_DEFAULT);
			pos.y += GetCosVec(0.5f, MOVE_DEFAULT);
			m_nTexRotType = TYPE_NEUTRAL;
			m_nCntAnimMove = 0;
			//弾の発射位置を設定
			posBullet = D3DXVECTOR2(20.0f, 10.0f);
		}
	}
	else if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_UP) == true ||
		pJoypad->GetPress(CInputJoypad::JOYKEY_UP, m_nPlayerNum) == true ||
		pJoypad->GetStick(CInputJoypad::JOYKEY_LEFT_STICK, m_nPlayerNum).y <= -0.2f)
	{//上キー押下
		pos.x += GetSinVec(1.0f, MOVE_DEFAULT);
		pos.y += GetCosVec(1.0f, MOVE_DEFAULT);
		SetAnimNum(TYPE_DOWN, TYPE_UP);
		m_nCntAnimMove++;
		//弾の発射位置を設定
		posBullet = D3DXVECTOR2(20.0f, 10.0f);
	}
	else if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_DOWN) == true ||
		pJoypad->GetPress(CInputJoypad::JOYKEY_DOWN, m_nPlayerNum) == true ||
		pJoypad->GetStick(CInputJoypad::JOYKEY_LEFT_STICK, m_nPlayerNum).y >= 0.2f)
	{//下キー押下
		pos.x += GetSinVec(0.0f, MOVE_DEFAULT);
		pos.y += GetCosVec(0.0f, MOVE_DEFAULT);
		SetAnimNum(TYPE_UP, TYPE_DOWN);
		m_nCntAnimMove++;
		//弾の発射位置を設定
		posBullet = D3DXVECTOR2(20.0f, -5.0f);
	}
	else
	{
		//弾の発射位置を設定
		posBullet = D3DXVECTOR2(20.0f, 10.0f);
		m_nCntAnimMove = 0;
		m_nTexRotType = TYPE_NEUTRAL;
	}

	return pos;
}

//-----------------------------------------------------------------------------
// 水しぶきの設定
//-----------------------------------------------------------------------------
void CPlayer::SetSpray()
{
	//海の水面オブジェクト情報の取得
	CObject2D *pObject2D = CBg::GetObjectBg(CBg::BG_A_WAVE1);

	// 位置情報を取得
	D3DXVECTOR3 posWave = pObject2D->GetPosition();		//水面の位置を取得
	D3DXVECTOR3 pos = CObject2D::GetPosition();			//プレイヤーの位置を取得

	// 海に入っていないなら
	if (m_bInSea == false)
	{
		//自機の高さが水面より下
		if (posWave.y <= pos.y)
		{
			//入水フラグを立てる
			m_bInSea = true;
			//水しぶきの生成
			CSpray::Create(D3DXVECTOR3(pos.x, pos.y - 30.0f, pos.z));
			//自機を少し青くする
			CObject2D::SetColor(D3DXCOLOR(0.0f, 0.0f, 0.3f, 1.0f));
		}
	}
	// 海に入っているなら
	else if (m_bInSea == true)
	{
		//自機の高さが水面より上
		if (posWave.y > pos.y)
		{
			//入水フラグを折る
			m_bInSea = false;
			//自機の色を元に戻す
			CObject2D::SetColor(DEFAULT_COL);
		}
		//海に入ってるのに色が初期値だったら
		else if (GetColor() == DEFAULT_COL)
		{
			//自機を少し青くする
			CObject2D::SetColor(D3DXCOLOR(0.0f, 0.0f, 0.3f, 1.0f));
		}
	}
}

//-----------------------------------------------------------------------------
// アニメーション番号の設定
//-----------------------------------------------------------------------------
void CPlayer::SetAnimNum(ANIMTYPE AnimIn, ANIMTYPE AnimOut)
{
	if (m_nTexRotType == AnimIn)
	{
		m_nTexRotType = TYPE_NEUTRAL;
		m_nCntAnimMove = 0;
	}
	else if (m_nCntAnimMove >= 10)
	{
		m_nTexRotType = AnimOut;
	}
}

//-----------------------------------------------------------------------------
// 強化状態の設定
//-----------------------------------------------------------------------------
void CPlayer::SetLevel(CItem::EType type)
{
	// 位置の取得
	D3DXVECTOR3 pos = GetPosition();

	// 取得したアイテムによって強化状態を変更
	switch (type)
	{
		// メイン弾強化
	case CItem::TYPE_RED:

		// オプション弾状態がLEVEL_3(最大強化)以外の場合
		if (m_BulletLevel != CPlayer::LEVEL_3)
		{// 強化状態を進める
			m_BulletLevel = (LEVEL)(m_BarrierLevel + 1);
		}

		break;
		// オプション弾強化
	case CItem::TYPE_BLUE:

		switch (m_OptionLevel)
		{
		case CBulletOption::LEVEL_NONE:
			m_pOption[0] = CBulletOption::Create(0.0f, (CBulletOption::PARENT)m_nPlayerNum);
			break;

		case CBulletOption::LEVEL_1:
			m_pOption[0]->SetRad(0.0f);
			m_pOption[1] = CBulletOption::Create(D3DX_PI, (CBulletOption::PARENT)m_nPlayerNum);
			break;

		case CBulletOption::LEVEL_2:
			m_pOption[0]->SetRad(0.0f);
			m_pOption[1]->SetRad(D3DX_PI);
			m_pOption[2] = CBulletOption::Create(D3DX_PI / 2, (CBulletOption::PARENT)m_nPlayerNum);
			m_pOption[3] = CBulletOption::Create(D3DX_PI * 1.5f, (CBulletOption::PARENT)m_nPlayerNum);
			break;
		case CBulletOption::LEVEL_3:
			break;
		default:
			break;
		}

		// オプション弾状態がLEVEL_3(最大強化)以外の場合
		if (m_OptionLevel != CBulletOption::LEVEL_3)
		{// 強化状態を進める
			m_OptionLevel = (CBulletOption::LEVEL)(m_OptionLevel + 1);
		}
		break;

		// バリア強化
	case CItem::TYPE_GREEN:

		// 現在の段階によって強化状態を変化
		switch (m_BarrierLevel)
		{
		case CBarrier::LEVEL_NONE:
			// バリアの生成
			m_pBarrier = CBarrier::Create(pos, (CBarrier::PARENT)m_nPlayerNum);
			break;
		case CBarrier::LEVEL_GREEN:
			m_pBarrier->SetBarrier(CBarrier::LEVEL_SILVER);
			break;
		case CBarrier::LEVEL_SILVER:
			m_pBarrier->SetBarrier(CBarrier::LEVEL_GOLD);
			break;
		case CBarrier::LEVEL_GOLD:
			break;
		default:
			break;
		}
		
		// バリア状態がGOLD(最大強化)以外の場合
		if (m_BarrierLevel != CBarrier::LEVEL_GOLD)
		{// 強化状態を進める
			m_BarrierLevel = (CBarrier::LEVEL)(m_BarrierLevel + 1);
		}
		break;

	case CItem::TYPE_ORANGE:
		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------------
// 状態管理
//-----------------------------------------------------------------------------
void CPlayer::State()
{
	// 対角線(サイズ)の取得
	D3DXVECTOR2 size = CObject2D::GetSize();
	// 位置の取得
	D3DXVECTOR3 pos = CObject2D::GetPosition();

	switch (m_state)
	{
	case STATE_NORMAL:			//プレイヤーの状態が通常の場合
		break;
	case STATE_ENTRY:			//登場状態の場合

		m_nCntState++;

		// 対角線(サイズ)の減算
		size -= D3DXVECTOR2(4.46f, 1.98f);
		// 対角線の取得
		CObject2D::SetSize(size);

		if (pos.y <= CRenderer::SCREEN_HEIGHT / 2)
		{
			m_bControl = true;
			m_state = STATE_RESPAWN;
			m_nCntState = 150;
		}
		else if (pos.y <= CRenderer::SCREEN_HEIGHT / 1.5f)
		{
			pos += D3DXVECTOR3(-20.0f, -3.0f, 0.0f);
		}
		else
		{
			pos += D3DXVECTOR3(22.0f, -3.0f, 0.0f);
		}

		if (m_nCntState >= 96)
		{
			m_nTexRotType = TYPE_NEUTRAL;
		}
		else if (m_nCntState >= 72)
		{
			m_nTexRotType = TYPE_DOWN;
		}
		else if (m_nCntState >= 48)
		{
			m_nTexRotType = TYPE_NEUTRAL;
		}
		else if (m_nCntState >= 24)
		{
			m_nTexRotType = TYPE_UP;
		}

		CObject2D::SetPosition(pos);

		break;
	case STATE_RESPAWN:			//プレイヤーがダメージ状態の場合
		//状態カウンターの減算
		m_nCntState--;
		//右から出現させる
		if (m_nCntState >= 150)
		{
			// 位置の加算
			pos.x += 10.0f;
			// 位置の設定
			CObject2D::SetPosition(pos);

			// カウンターが一定
			if (m_nCntState == 150)
			{
				//操作可能にする
				m_bControl = true;
			}
		}

		//点滅させる
		if (m_nCntState % 4 == 0)
		{//黄色
			CObject2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
		}
		else
		{//色なし
			CObject2D::SetColor(DEFAULT_COL);
		}

		//状態を通常に戻す
		if (m_nCntState <= 0)
		{
			m_state = STATE_NORMAL;

			CObject2D::SetColor(DEFAULT_COL);
		}
		break;
	case STATE_DIE:			//プレイヤーが死亡状態の場合
		//状態カウンターの減算
		m_nCntState--;

		if (m_nCntState <= 0)
		{
			m_state = STATE_RESPAWN;
			m_nCntState = 180;
		}
		break;
	}
}

//-----------------------------------------------------------------------------
// ダメージ処理
//-----------------------------------------------------------------------------
void CPlayer::Damage()
{
	if (m_BarrierLevel == CBarrier::LEVEL_NONE)
	{
		// 死亡処理
		Die();
	}
	else
	{
		m_BarrierLevel = (CBarrier::LEVEL)(m_BarrierLevel - 1);

		if (m_BarrierLevel == CBarrier::LEVEL_NONE)
		{
			if (m_pBarrier != nullptr)
			{
				m_pBarrier->Uninit();
				m_pBarrier = nullptr;
			}
		}
		else
		{
			m_pBarrier->SetBarrier(m_BarrierLevel);
		}
		m_state = STATE_RESPAWN;
		m_nCntState = 150;
	}
}

//-----------------------------------------------------------------------------
// 死亡処理
//-----------------------------------------------------------------------------
void CPlayer::Die()
{
	// ライフが破棄されていなければ
	if (m_pLife != nullptr)
	{
		// ライフを減らす
		m_pLife->Add(-1);

		// ライフが0未満
		if (m_pLife->GetLife() < 0)
		{
			// 最終スコアの保存
			CRank::SetScore(m_pScore->GetScore(), m_nPlayerNum);

			// スコアの破棄
			if (m_pScore != nullptr)
			{
				m_pScore->Uninit();
				m_pScore = nullptr;
			}

			// ライフの破棄
			m_pLife->Uninit();
			m_pLife = nullptr;

			// プレイヤーを死亡状態にする
			m_bDie = true;

			// サイズの取得
			D3DXVECTOR2 size = GetSize();
			// 爆発の生成
			CExplosion::Create(CObject2D::GetPosition(), D3DXVECTOR2(size.x, size.y * 2));

			return;
		}

		// 操作不能にする
		m_bControl = false;
		// リスポーン状態にする
		m_state = STATE_DIE;
		m_nCntState = 60;

		// 変数のリセット
		m_nTexRotType = TYPE_NEUTRAL;		//アニメーション番号をリセットする

		// バリアの破棄
		if (m_pBarrier != nullptr)
		{
			m_pBarrier->Uninit();
			m_pBarrier = nullptr;
		}

		// オプションの破棄
		for (int nCnt = 0; nCnt < MAX_OPTION; nCnt++)
		{
			if (m_pOption[nCnt] != nullptr)
			{
				m_pOption[nCnt]->Uninit();
				m_pOption[nCnt] = nullptr;
			}
		}

		// 各強化状態を初期化する
		m_OptionLevel = CBulletOption::LEVEL_NONE;
		m_BarrierLevel = CBarrier::LEVEL_NONE;
		m_BulletLevel = CPlayer::LEVEL_1;

		// サイズの取得
		D3DXVECTOR2 size = GetSize();
		// 爆発の生成
		CExplosion::Create(CObject2D::GetPosition(), D3DXVECTOR2(size.x, size.y * 2));

		//プレイヤーの位置を左端に設定する
		if (m_nPlayerNum == PLAYER_1)
		{// 1Pの場合
			CObject2D::SetPosition(D3DXVECTOR3(-SIZE_X, CRenderer::SCREEN_HEIGHT / 2, 0.0f));
		}
		else
		{// 2Pの場合
			CObject2D::SetPosition(D3DXVECTOR3(-SIZE_X, CRenderer::SCREEN_HEIGHT / 2 + SIZE_Y, 0.0f));
		}

	}
}
