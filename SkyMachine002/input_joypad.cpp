//=============================================================================
//
//	ジョイパッド入力処理 [inputJoypad.cpp]
//	Author : SHUGO KURODA
//
//=============================================================================

//======================================================
//	インクルード
//======================================================
#include "input_joypad.h"

//======================================================
//	コンストラクタ
//======================================================
CInputJoypad::CInputJoypad()
{
}

//======================================================
//	デストラクタ
//======================================================
CInputJoypad::~CInputJoypad()
{
}

//======================================================
//	ジョイパッドの初期化処理
//======================================================
HRESULT CInputJoypad::Init(HINSTANCE hinstance, HWND hWnd)
{
	//XInputのステートを設定（有効にする）
	XInputEnable(true);

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		//メモリーのクリア
		memset(&g_JoyKeyState[nCnt], 0, sizeof(XINPUT_STATE));
		memset(&g_JoyKeyStateTrigger[nCnt], 0, sizeof(XINPUT_STATE));

		//ジョイパッドの振動制御の０クリア
		ZeroMemory(&g_JoyMoter[nCnt], sizeof(XINPUT_VIBRATION));

		//振動制御用の初期化
		g_nStrength[nCnt] = 0;
		g_nTime[nCnt] = 0;
	}

	return S_OK;
}

//======================================================
//	ジョイパッドの終了処理
//======================================================
void CInputJoypad::Uninit()
{
	//XInputのステートを無効にする
	XInputEnable(false);
}

//======================================================
//	ジョイパッドの更新処理
//======================================================
void CInputJoypad::Update()
{
	XINPUT_STATE JoyKeyState[MAX_PLAYER];		//ジョイパッド入力情報

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		//ジョイパッドの状態を取得
		if (XInputGetState(nCnt, &JoyKeyState[nCnt]) == ERROR_SUCCESS)
		{
			g_JoyKeyStateTrigger[nCnt].Gamepad.wButtons
				= ~g_JoyKeyState[nCnt].Gamepad.wButtons
				& JoyKeyState[nCnt].Gamepad.wButtons;		//トリガー情報を保存
			g_JoyKeyState[nCnt] = JoyKeyState[nCnt];		//プレス処理
		}

		//ジョイパッドの振動
		g_JoyMoter[nCnt].wLeftMotorSpeed = g_nStrength[nCnt];
		g_JoyMoter[nCnt].wRightMotorSpeed = g_nStrength[nCnt];
		XInputSetState(nCnt, &g_JoyMoter[nCnt]);

		if (g_nTime[nCnt] > 0)
		{
			g_nTime[nCnt]--;
		}
		else
		{
			g_nStrength[nCnt] = 0;
			g_nTime[nCnt] = 0;
		}
	}
}

//======================================================
//	ジョイパッドのプレス処理
//======================================================
bool CInputJoypad::GetPress(JOYKEY Key, int nPlayer)
{
	return (g_JoyKeyState[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//======================================================
//	ジョイパッドのトリガー処理
//======================================================
bool CInputJoypad::GetTrigger(JOYKEY Key, int nPlayer)
{
	return (g_JoyKeyStateTrigger[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//======================================================
//	ジョイパット（スティックプレス）処理
//======================================================
D3DXVECTOR3 CInputJoypad::GetStick(JOYKEY Key, int nPlayer)
{
	switch (Key)
	{
	case JOYKEY_LEFT_STICK:
		g_JoyStickPos[nPlayer] = D3DXVECTOR3(g_JoyKeyState[nPlayer].Gamepad.sThumbLX / 32767.0f, -g_JoyKeyState[nPlayer].Gamepad.sThumbLY / 32767.0f, 0.0f);
		break;
	case JOYKEY_RIGHT_STICK:
		g_JoyStickPos[nPlayer] = D3DXVECTOR3(g_JoyKeyState[nPlayer].Gamepad.sThumbRX / 32767.0f, -g_JoyKeyState[nPlayer].Gamepad.sThumbRY / 32767.0f, 0.0f);
		break;
	}

	return g_JoyStickPos[nPlayer];
}

//======================================================
//	ジョイパット（トリガーペダル）処理
//======================================================
int CInputJoypad::GetTriggerPedal(JOYKEY Key, int nPlayer)
{
	int nJoypadTriggerPedal = 0;
	switch (Key)
	{
	case JOYKEY_LEFT_TRIGGER:
		nJoypadTriggerPedal = g_JoyKeyState[nPlayer].Gamepad.bLeftTrigger;
		break;
	case JOYKEY_RIGHT_TRIGGER:
		nJoypadTriggerPedal = g_JoyKeyState[nPlayer].Gamepad.bRightTrigger;
		break;
	}

	return nJoypadTriggerPedal;
}

//======================================================
//	コントローラーの振動制御
//======================================================
void CInputJoypad::Vibration(int nTime, WORD nStrength, int nPlayer)
{
	g_nTime[nPlayer] = nTime;			//振動持続時間
	g_nStrength[nPlayer] = nStrength;	//振動の強さ
}
