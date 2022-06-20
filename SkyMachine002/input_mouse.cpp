//=============================================================================
//
// ジョイパッド処理 (input_joypad.cpp)
// Author:Itsuki Takaiwa
//
//=============================================================================
#include "input_mouse.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define DEADZONE        (2500)      // 各軸の25%を無効ゾーンとする
#define RANGE_MAX       (10000)     // 有効範囲の最大値
#define RANGE_MIN       (-10000)    // 有効範囲の最小値

#define MAX_DIRECTION	(4)			// キー方向の最大数

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************	

//=============================================================================	
// CInputMouseのコンストラクタ
//=============================================================================	
CInputMouse::CInputMouse()
{
	memset(&m_CurrentMouseState, 0, sizeof(DIMOUSESTATE));
	memset(&m_PrevMouseState, 0, sizeof(DIMOUSESTATE));
}

//=============================================================================	
// CInputMouseのデストラクタ
//=============================================================================	
CInputMouse::~CInputMouse()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	// インプット処理の共通初期化処理
	CInput::Init(hInstance, hWnd);

	// マウスデバイスの取得
	if (FAILED(m_pInput->CreateDevice(
		GUID_SysMouse,
		&m_pDevice,
		NULL)))
	{
		return E_FAIL;
	}

	// デバイスのフォーマット設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}

	// 協調モードの設定
	if (FAILED(m_pDevice->SetCooperativeLevel(
		hWnd,
		DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CInputMouse::Uninit(void)
{
	// CInputの終了処理
	CInput::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CInputMouse::Update(void)
{
	HRESULT hr;

	if (m_pDevice != NULL)
	{
		hr = m_pDevice->Poll();

		// 現在の情報を前回の情報に保存
		m_PrevMouseState = m_CurrentMouseState;
		m_posOld = m_pos;
		// 現在のマウスの状態を取得
		if (SUCCEEDED(hr = m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE), &m_CurrentMouseState)))
		{
			GetCursorPos(&m_pos);
			//ScreenToClient(FindWindowA(CLASS_NAME, nullptr), &m_pos);
		}
		else
		{
			m_pDevice->Acquire();
		}
	}
}

bool CInputMouse::GetPress(int nButton)
{
	return (m_CurrentMouseState.rgbButtons[nButton] & 0x80) ? true : false;
}

bool CInputMouse::GetTrigger(int nButton)
{
	if (!(m_PrevMouseState.rgbButtons[nButton] & 0x80) && (m_CurrentMouseState.rgbButtons[nButton] & 0x80))
	{
		return true;
	}
	return false;
}

bool CInputMouse::GetRelese(int nButton)
{
	if ((m_PrevMouseState.rgbButtons[nButton] & 0x80) && !(m_CurrentMouseState.rgbButtons[nButton] & 0x80))
	{
		return true;
	}
	return false;
}

D3DXVECTOR2 CInputMouse::GetMouseMove(void)
{
	return D3DXVECTOR2((float)m_CurrentMouseState.lX, (float)m_CurrentMouseState.lY);
}

float CInputMouse::GetMouseWheel(void)
{
	return (float)m_CurrentMouseState.lZ;
}