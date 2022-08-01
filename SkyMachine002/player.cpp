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

//*****************************************************************************
// 定数宣言
//*****************************************************************************
const float CPlayer::SIZE_X = 90.0f;
const float CPlayer::SIZE_Y = 40.0f;
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
	m_nTexRotType(TYPE_NEUTRAL), m_nPlayerNum(0), posBullet(0.0f, 0.0f), m_bControl(false), m_bInSea(false)
{
	//オブジェクトの種類設定
	SetObjectType(EObject::OBJ_PLAYER);
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
CPlayer *CPlayer::Create(const D3DXVECTOR3& pos)
{
	//インスタンス生成
	CPlayer *pPlayer = new CPlayer;

	if (pPlayer != nullptr)
	{
		//位置設定
		pPlayer->SetPosition(pos);
		//生成処理
		pPlayer->Init();
		// テクスチャの設定
		pPlayer->BindTexture(m_apTexture[0]);
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
	for (int nCnt = 0; nCnt < MAX_TEX; nCnt++)
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
	//弾の発射位置を設定
	posBullet = D3DXVECTOR2(20.0f, 10.0f);
	//プレイヤーを操作可能にする
	m_bControl = true;

	//サイズの設定
	CObject2D::SetSize(D3DXVECTOR2(SIZE_X, SIZE_Y));
	//初期化
	CObject2D::Init();
	//色の設定
	CObject2D::SetColor(DEFAULT_COL);
	//テクスチャ座標の設定
	CObject2D::SetAnimation(m_nPatternAnim, m_nTexRotType, DIVISION_U, DIVISION_V);

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

		if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_ATTACK) == true)
		{//SPACEキーを押された

			//攻撃カウンターの加算
			m_nCntAttack++;		

			if (m_nCntAttack > 5)
			{
				//弾の設定
				CBullet::Create(D3DXVECTOR3(pos.x + posBullet.x, pos.y + posBullet.y, pos.z), D3DXVECTOR3(15.0f, 0.0f, 0.0f),
					5, CBullet::TYPE_PLAYER_BLUE)->SetParent(CBullet::PARENT_PLAYER);

				//攻撃カウンターをリセット
				m_nCntAttack = 0;

				//サウンド再生
				//CSound::Play(CSound::SOUND_LABEL_TITLE);
			}
		}
		else
		{
			m_nCntAttack = 10;
		}

		//可動範囲を画面内に制限
		LibrarySpace::SteyInScreen2D(&pos, CObject2D::GetSize());
	}

	//水しぶきを設定
	SetSpray();

	//位置情報更新
	CObject2D::SetPosition(pos);

	//状態管理
	State();
	
	//サイズ更新
	//CObject2D::SetSize(m_Scale.x, m_Scale.y);

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

	if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_LEFT) == true /*||
		pJoypad->GetStick(CInputJoypad::JOYKEY_LEFT_STICK, m_nPlayerNum).x*/)
	{//Aキーが押された
		if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_UP) == true)
		{
			pos.x += sinf(-D3DX_PI * 0.75f) * MOVE_DEFAULT;
			pos.y += cosf(-D3DX_PI * 0.75f) * MOVE_DEFAULT;
			SetAnimNum(TYPE_DOWN, TYPE_UP);
			m_nCntAnimMove++;
			//弾の発射位置を設定
			posBullet = D3DXVECTOR2(20.0f, 10.0f);
		}
		else if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_DOWN) == true)
		{
			pos.x += sinf(-D3DX_PI * 0.25f) * MOVE_DEFAULT;
			pos.y += cosf(-D3DX_PI * 0.25f) * MOVE_DEFAULT;
			SetAnimNum(TYPE_UP, TYPE_DOWN);
			m_nCntAnimMove++;
			//弾の発射位置を設定
			posBullet = D3DXVECTOR2(20.0f, -5.0f);
		}
		else
		{
			pos.x += sinf(-D3DX_PI * 0.5f) * MOVE_DEFAULT;
			pos.y += cosf(-D3DX_PI * 0.5f) * MOVE_DEFAULT;
			m_nTexRotType = TYPE_NEUTRAL;
			m_nCntAnimMove = 0;
			//弾の発射位置を設定
			posBullet = D3DXVECTOR2(20.0f, 10.0f);
		}
	}
	else if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_RIGHT) == true)
	{//Dキーが押された
		if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_UP) == true)
		{
			pos.x += sinf(D3DX_PI * 0.75f) * MOVE_DEFAULT;
			pos.y += cosf(D3DX_PI * 0.75f) * MOVE_DEFAULT;
			SetAnimNum(TYPE_DOWN, TYPE_UP);
			m_nCntAnimMove++;
			//弾の発射位置を設定
			posBullet = D3DXVECTOR2(20.0f, 10.0f);
		}
		else if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_DOWN) == true)
		{
			pos.x += sinf(D3DX_PI * 0.25f) * MOVE_DEFAULT;
			pos.y += cosf(D3DX_PI * 0.25f) * MOVE_DEFAULT;
			SetAnimNum(TYPE_UP, TYPE_DOWN);
			m_nCntAnimMove++;
			//弾の発射位置を設定
			posBullet = D3DXVECTOR2(20.0f, -5.0f);
		}
		else
		{
			pos.x += sinf(D3DX_PI * 0.5f) * MOVE_DEFAULT;
			pos.y += cosf(D3DX_PI * 0.5f) * MOVE_DEFAULT;
			m_nTexRotType = TYPE_NEUTRAL;
			m_nCntAnimMove = 0;
			//弾の発射位置を設定
			posBullet = D3DXVECTOR2(20.0f, 10.0f);
		}
	}
	else if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_UP) == true)
	{//Wキーが押された
		pos.x += sinf(D3DX_PI * 1.0f) * MOVE_DEFAULT;
		pos.y += cosf(D3DX_PI * 1.0f) * MOVE_DEFAULT;
		SetAnimNum(TYPE_DOWN, TYPE_UP);
		m_nCntAnimMove++;
		//弾の発射位置を設定
		posBullet = D3DXVECTOR2(20.0f, 10.0f);
	}
	else if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_DOWN) == true)
	{//Sキーが押された
		pos.x += sinf(D3DX_PI * 0.0f) * MOVE_DEFAULT;
		pos.y += cosf(D3DX_PI * 0.0f) * MOVE_DEFAULT;
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
// 状態管理
//-----------------------------------------------------------------------------
void CPlayer::State()
{
	switch (m_state)
	{
	case STATE_NORMAL:			//プレイヤーの状態が通常の場合
		break;
	case STATE_RESPAWN:			//プレイヤーがダメージ状態の場合
		//状態カウンターの減算
		m_nCntState--;
		//右から出現させる
		if (m_nCntState >= 150)
		{
			D3DXVECTOR3 pos = CObject2D::GetPosition();
			pos.x += 10.0f;
			CObject2D::SetPosition(pos);
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
void CPlayer::Damage(int nDamage)
{
	// 死亡処理
	Die();
}

//-----------------------------------------------------------------------------
// 死亡処理
//-----------------------------------------------------------------------------
void CPlayer::Die()
{
	// 操作不能にする
	m_bControl = false;
	// リスポーン状態にする
	m_state = STATE_DIE;
	m_nCntState = 60;
	//アニメーション番号をリセットする
	m_nTexRotType = TYPE_NEUTRAL;
	// 爆発の生成
	CExplosion::Create(CObject2D::GetPosition());
	//プレイヤーの位置を左端に設定する
	CObject2D::SetPosition(D3DXVECTOR3(-SIZE_X, CRenderer::SCREEN_HEIGHT / 2, 0.0f));
}
