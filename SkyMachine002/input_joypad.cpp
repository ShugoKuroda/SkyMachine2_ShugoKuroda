//=============================================================================
//
//	�W���C�p�b�h���͏��� [inputJoypad.cpp]
//	Author : SHUGO KURODA
//
//=============================================================================

//======================================================
//	�C���N���[�h
//======================================================
#include "input_joypad.h"

//======================================================
//	�R���X�g���N�^
//======================================================
CInputJoypad::CInputJoypad()
{
}

//======================================================
//	�f�X�g���N�^
//======================================================
CInputJoypad::~CInputJoypad()
{
}

//======================================================
//	�W���C�p�b�h�̏���������
//======================================================
HRESULT CInputJoypad::Init(HINSTANCE hinstance, HWND hWnd)
{
	//XInput�̃X�e�[�g��ݒ�i�L���ɂ���j
	XInputEnable(true);

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		//�������[�̃N���A
		memset(&g_JoyKeyState[nCnt], 0, sizeof(XINPUT_STATE));
		memset(&g_JoyKeyStateTrigger[nCnt], 0, sizeof(XINPUT_STATE));

		//�W���C�p�b�h�̐U������̂O�N���A
		ZeroMemory(&g_JoyMoter[nCnt], sizeof(XINPUT_VIBRATION));

		//�U������p�̏�����
		g_nStrength[nCnt] = 0;
		g_nTime[nCnt] = 0;
	}

	return S_OK;
}

//======================================================
//	�W���C�p�b�h�̏I������
//======================================================
void CInputJoypad::Uninit()
{
	//XInput�̃X�e�[�g�𖳌��ɂ���
	XInputEnable(false);
}

//======================================================
//	�W���C�p�b�h�̍X�V����
//======================================================
void CInputJoypad::Update()
{
	XINPUT_STATE JoyKeyState[MAX_PLAYER];		//�W���C�p�b�h���͏��

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		//�W���C�p�b�h�̏�Ԃ��擾
		if (XInputGetState(nCnt, &JoyKeyState[nCnt]) == ERROR_SUCCESS)
		{
			g_JoyKeyStateTrigger[nCnt].Gamepad.wButtons
				= ~g_JoyKeyState[nCnt].Gamepad.wButtons
				& JoyKeyState[nCnt].Gamepad.wButtons;		//�g���K�[����ۑ�
			g_JoyKeyState[nCnt] = JoyKeyState[nCnt];		//�v���X����
		}

		//�W���C�p�b�h�̐U��
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
//	�W���C�p�b�h�̃v���X����
//======================================================
bool CInputJoypad::GetPress(JOYKEY Key, int nPlayer)
{
	return (g_JoyKeyState[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//======================================================
//	�W���C�p�b�h�̃g���K�[����
//======================================================
bool CInputJoypad::GetTrigger(JOYKEY Key, int nPlayer)
{
	return (g_JoyKeyStateTrigger[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//======================================================
//	�W���C�p�b�g�i�X�e�B�b�N�v���X�j����
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
//	�W���C�p�b�g�i�g���K�[�y�_���j����
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
//	�R���g���[���[�̐U������
//======================================================
void CInputJoypad::Vibration(int nTime, WORD nStrength, int nPlayer)
{
	g_nTime[nPlayer] = nTime;			//�U����������
	g_nStrength[nPlayer] = nStrength;	//�U���̋���
}
