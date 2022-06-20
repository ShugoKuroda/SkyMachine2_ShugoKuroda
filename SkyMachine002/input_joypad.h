//=============================================================================
//
// ジョイパッド入力処理 [input_joypad.h]
// Author: SHUGO KURODA
//
//=============================================================================
#ifndef _INPUTJOYPAD_H_		//このマクロ定義がされなかったら
#define _INPUTJOYPAD_H_		//2重インクルード防止のマクロ定義

//----------------------------------------------------------------------------
//	インクルード
//----------------------------------------------------------------------------
#include "input.h"

//----------------------------------------------------------------------------
//	ジョイパッド列挙型
//----------------------------------------------------------------------------
class CInputJoypad
{
public:
	enum JOYKEY
	{
		JOYKEY_UP = 0,				//十字ボタン上
		JOYKEY_DOWN,				//十字ボタン下
		JOYKEY_LEFT,				//十字ボタン左
		JOYKEY_RIGHT,				//十字ボタン右
		JOYKEY_START,				//スタートボタン
		JOYKEY_BACK,				//バックボタン
		JOYKEY_LEFT_THUMB,			//左スティック押込み
		JOYKEY_RIGHT_THUMB,			//右スティック押込み
		JOYKEY_LEFT_SHOULDER,		//L１ボタン
		JOYKEY_RIGHT_SHOULDER,		//R１ボタン
		JOYKEY_LEFT_TRIGGER,		//L２トリガー
		JOYKEY_RIGHT_TRIGGER,		//R２トリガー
		JOYKEY_A,					//Aボタン
		JOYKEY_B,					//Bボタン
		JOYKEY_X,					//Xボタン
		JOYKEY_Y,					//Yボタン
		JOYKEY_LEFT_STICK,			//左スティック
		JOYKEY_RIGHT_STICK,			//右スティック
		JOYKEY_MAX
	};

	CInputJoypad();
	~CInputJoypad();

	HRESULT Init(HINSTANCE hinstance, HWND hWnd);
	void Uninit();
	void Update();
	bool GetPress(JOYKEY Key, int nPlayer);
	bool GetTrigger(JOYKEY Key, int nPlayer);
	D3DXVECTOR3 GetStick(JOYKEY Key, int nPlayer);
	int GetTriggerPedal(JOYKEY Key, int nPlayer);
	void Vibration(int nTime, WORD nStrength, int nPlayer);

private:
	//最大プレイヤー人数
	static const int MAX_PLAYER = 4;

	XINPUT_STATE g_JoyKeyState[MAX_PLAYER];				//ジョイパットのプレス情報
	XINPUT_STATE g_JoyKeyStateTrigger[MAX_PLAYER];		//ジョイパットのトリガー情報
	D3DXVECTOR3 g_JoyStickPos[MAX_PLAYER];				//ジョイスティックの傾き
	XINPUT_VIBRATION g_JoyMoter[MAX_PLAYER];			//ジョイパッドのモーター
	int g_nTime[MAX_PLAYER];							//振動持続時間
	int g_nStrength[MAX_PLAYER];						//振動の強さ (0 - 65535)
}

#endif

/*************************************************************************************************************

//各関数の説明

{//引数にキーの列挙型を入れそれが押されたかどうかをboolで返す
bool GetJoypadPress(JOYKEY Key, int nPlayer);					//ジョイパッドプレス処理
bool GetJoypadTrigger(JOYKEY Key, int nPlayer);					//ジョイパッドトリガー処理
}

//返す値　スティックを倒せば倒すほど(1.0f or -1.0f)に近く
//スティックの遊びはifの条件式に組む
//例　if(GetJoypadStick(JOYKEY_RIGHT_STICK,0)  > 0.5f)
//    {//右に半分より多く倒したら

{第一引数はキー（スティック）,第二引数はプレイヤー番号
D3DXVECTOR3 GetJoypadStick(JOYKEY Key, int nPlayer);			//ジョイパッドスティック処理
}

//返す値　ペダルがどれだけ押し込まれたかをintで返す
//ペダルの遊びはifの条件式に組む
//例　if(GetJoypadTriggerPedal(JOYKEY_RIGHT_TRIGGER,0) > 100))
//    {//右ペダルを100より多く倒したら

{第一引数はキー（スティック）,第二引数はプレイヤー番号
int GetJoypadTriggerPedal(JOYKEY Key, int nPlayer);				//ジョイパッドトリガーペダル処理
}

{第一引数は揺らす時間, 第二引数はどれくらいの強さで揺らすか, 第三引数はプレイヤー番号
void JoypadVibration(int nTime, WORD nStrength, int nPlayer);	//ジョイパッド振動制御
}
************************************************************************************************************/