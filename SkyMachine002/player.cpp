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

#include "library.h"

#include "bullet.h"
#include "player.h"

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

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
// テクスチャのポインタ
LPDIRECT3DTEXTURE9 CPlayer::m_apTexture[2] = { nullptr };

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CPlayer::CPlayer() :
	m_move(0.0f, 0.0f, 0.0f), m_state(STATE_NORMAL), m_nCntState(0), m_nCntAttack(0),
	m_nCntAnim(0), m_nPatternAnim(0), m_nCntAnimMove(0), m_nTexRotType(TYPE_NEUTRAL), m_nPlayerNum(0)
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
	CObject2D::SetSize(D3DXVECTOR2(SIZE_X, SIZE_Y));

	CObject2D::Init();

	//テクスチャアニメーション
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

	//移動処理
	pos = Move(pos);

	// キーボード情報の取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	CInputJoypad *pJoypad = CManager::GetInputJoypad();

	if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_ATTACK) == true)
	{//SPACEキーを押された
		m_nCntAttack++;		//攻撃間隔のカウンター
		if (m_nCntAttack > 5)
		{
			//弾の設定
			CBullet::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), 5)->SetType(CBullet::EType::TYPE_PLAYER);
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

	//位置情報更新
	CObject2D::SetPosition(pos);
	
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
		// 終了する
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
	CObject2D::Draw();
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
			SetAnimNum(TYPE_DOWN,TYPE_UP);
			m_nCntAnimMove++;
		}
		else if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_DOWN) == true)
		{
			pos.x += sinf(-D3DX_PI * 0.25f) * MOVE_DEFAULT;
			pos.y += cosf(-D3DX_PI * 0.25f) * MOVE_DEFAULT;
			SetAnimNum(TYPE_UP, TYPE_DOWN);
			m_nCntAnimMove++;
		}
		else
		{
			pos.x += sinf(-D3DX_PI * 0.5f) * MOVE_DEFAULT;
			pos.y += cosf(-D3DX_PI * 0.5f) * MOVE_DEFAULT;
			m_nTexRotType = TYPE_NEUTRAL;
			m_nCntAnimMove = 0;
		}
	}
	else if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_RIGHT) == true)
	{//Dキーが押された
		if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_UP) == true)
		{
			pos.x += sinf(D3DX_PI *0.75f) * MOVE_DEFAULT;
			pos.y += cosf(D3DX_PI *0.75f) * MOVE_DEFAULT;
			SetAnimNum(TYPE_DOWN, TYPE_UP);
			m_nCntAnimMove++;
		}
		else if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_DOWN) == true)
		{
			pos.x += sinf(D3DX_PI * 0.25f) * MOVE_DEFAULT;
			pos.y += cosf(D3DX_PI * 0.25f) * MOVE_DEFAULT;
			SetAnimNum(TYPE_UP, TYPE_DOWN);
			m_nCntAnimMove++;
		}
		else
		{
			pos.x += sinf(D3DX_PI * 0.5f) * MOVE_DEFAULT;
			pos.y += cosf(D3DX_PI * 0.5f) * MOVE_DEFAULT;
			m_nTexRotType = TYPE_NEUTRAL;
			m_nCntAnimMove = 0;
		}
	}
	else if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_UP) == true)
	{//Wキーが押された
		pos.x += sinf(D3DX_PI * 1.0f) * MOVE_DEFAULT;
		pos.y += cosf(D3DX_PI * 1.0f) * MOVE_DEFAULT;
		SetAnimNum(TYPE_DOWN, TYPE_UP);
		m_nCntAnimMove++;
	}
	else if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_DOWN) == true)
	{//Sキーが押された
		pos.x += sinf(D3DX_PI * 0.0f) * MOVE_DEFAULT;
		pos.y += cosf(D3DX_PI * 0.0f) * MOVE_DEFAULT;
		SetAnimNum(TYPE_UP, TYPE_DOWN);
		m_nCntAnimMove++;
	}
	else
	{
		m_nCntAnimMove = 0;
		m_nTexRotType = TYPE_NEUTRAL;
	}

	return pos;
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
// ダメージ処理
//-----------------------------------------------------------------------------
void CPlayer::Damage()
{
	m_move.x += 123.0f;
}

//-----------------------------------------------------------------------------
// 死亡判定
//-----------------------------------------------------------------------------
void CPlayer::Die()
{
}
