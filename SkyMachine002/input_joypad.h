//=============================================================================
//
// �W���C�p�b�h���͏��� [input_joypad.h]
// Author: SHUGO KURODA
//
//=============================================================================
#ifndef _INPUTJOYPAD_H_		//���̃}�N����`������Ȃ�������
#define _INPUTJOYPAD_H_		//2�d�C���N���[�h�h�~�̃}�N����`

//----------------------------------------------------------------------------
//	�C���N���[�h
//----------------------------------------------------------------------------
#include "input.h"

//----------------------------------------------------------------------------
//	�W���C�p�b�h�񋓌^
//----------------------------------------------------------------------------
class CInputJoypad
{
public:
	enum JOYKEY
	{
		JOYKEY_UP = 0,				//�\���{�^����
		JOYKEY_DOWN,				//�\���{�^����
		JOYKEY_LEFT,				//�\���{�^����
		JOYKEY_RIGHT,				//�\���{�^���E
		JOYKEY_START,				//�X�^�[�g�{�^��
		JOYKEY_BACK,				//�o�b�N�{�^��
		JOYKEY_LEFT_THUMB,			//���X�e�B�b�N������
		JOYKEY_RIGHT_THUMB,			//�E�X�e�B�b�N������
		JOYKEY_LEFT_SHOULDER,		//L�P�{�^��
		JOYKEY_RIGHT_SHOULDER,		//R�P�{�^��
		JOYKEY_LEFT_TRIGGER,		//L�Q�g���K�[
		JOYKEY_RIGHT_TRIGGER,		//R�Q�g���K�[
		JOYKEY_A,					//A�{�^��
		JOYKEY_B,					//B�{�^��
		JOYKEY_X,					//X�{�^��
		JOYKEY_Y,					//Y�{�^��
		JOYKEY_LEFT_STICK,			//���X�e�B�b�N
		JOYKEY_RIGHT_STICK,			//�E�X�e�B�b�N
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
	//�ő�v���C���[�l��
	static const int MAX_PLAYER = 4;

	XINPUT_STATE g_JoyKeyState[MAX_PLAYER];				//�W���C�p�b�g�̃v���X���
	XINPUT_STATE g_JoyKeyStateTrigger[MAX_PLAYER];		//�W���C�p�b�g�̃g���K�[���
	D3DXVECTOR3 g_JoyStickPos[MAX_PLAYER];				//�W���C�X�e�B�b�N�̌X��
	XINPUT_VIBRATION g_JoyMoter[MAX_PLAYER];			//�W���C�p�b�h�̃��[�^�[
	int g_nTime[MAX_PLAYER];							//�U����������
	int g_nStrength[MAX_PLAYER];						//�U���̋��� (0 - 65535)
}

#endif

/*************************************************************************************************************

//�e�֐��̐���

{//�����ɃL�[�̗񋓌^����ꂻ�ꂪ�����ꂽ���ǂ�����bool�ŕԂ�
bool GetJoypadPress(JOYKEY Key, int nPlayer);					//�W���C�p�b�h�v���X����
bool GetJoypadTrigger(JOYKEY Key, int nPlayer);					//�W���C�p�b�h�g���K�[����
}

//�Ԃ��l�@�X�e�B�b�N��|���Γ|���ق�(1.0f or -1.0f)�ɋ߂�
//�X�e�B�b�N�̗V�т�if�̏������ɑg��
//��@if(GetJoypadStick(JOYKEY_RIGHT_STICK,0)  > 0.5f)
//    {//�E�ɔ�����葽���|������

{�������̓L�[�i�X�e�B�b�N�j,�������̓v���C���[�ԍ�
D3DXVECTOR3 GetJoypadStick(JOYKEY Key, int nPlayer);			//�W���C�p�b�h�X�e�B�b�N����
}

//�Ԃ��l�@�y�_�����ǂꂾ���������܂ꂽ����int�ŕԂ�
//�y�_���̗V�т�if�̏������ɑg��
//��@if(GetJoypadTriggerPedal(JOYKEY_RIGHT_TRIGGER,0) > 100))
//    {//�E�y�_����100��葽���|������

{�������̓L�[�i�X�e�B�b�N�j,�������̓v���C���[�ԍ�
int GetJoypadTriggerPedal(JOYKEY Key, int nPlayer);				//�W���C�p�b�h�g���K�[�y�_������
}

{�������͗h�炷����, �������͂ǂꂭ�炢�̋����ŗh�炷��, ��O�����̓v���C���[�ԍ�
void JoypadVibration(int nTime, WORD nStrength, int nPlayer);	//�W���C�p�b�h�U������
}
************************************************************************************************************/