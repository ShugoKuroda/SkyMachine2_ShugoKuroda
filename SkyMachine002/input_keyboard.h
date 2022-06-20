//=============================================================================
//
// �L�[�{�[�h���� [input_keyboard.h]
// Author : ITSUKI TAKAIWA
//
//=============================================================================
#ifndef _INPUTKEYBOARD_H_
#define _INPUTKEYBOARD_H_

#include "input.h"

//-----------------------------------------------------------------------------
// �L�[�{�[�h�N���X(�h���N���X)
//-----------------------------------------------------------------------------
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();

	// �L�[���͏��
	typedef enum
	{
		KEYINFO_OK = 0,				// ����
		KEYINFO_UP,					// ��I��
		KEYINFO_DOWN,				// ���I��
		KEYINFO_RIGHT,				// �E�ړ�
		KEYINFO_LEFT,				// ���ړ�
		KEYINFO_PAUSE,				// �|�[�Y
		KEYINFO_UP,
		KEYINFO_DOWN,
		KEYINFO_RIGHT,
		KEYINFO_LEFT,
		KEYINFO_ATTACK,				// �U��
		KEYINFO_MAX,
	}KEYINFO;

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetReleace(int nKey);

private:
	static const int MAX_KEY = 256;

	BYTE m_aKeyState[MAX_KEY];
	BYTE m_aKeyStateTrigger[MAX_KEY];
	BYTE m_aKeyStateReleace[MAX_KEY];

	int m_aKeyInfo[KEYINFO_MAX] =
	{
		DIK_RETURN,		// ����
		DIK_W,			// ��I��
		DIK_S,			// ���I�� 
		DIK_D,			// �E�ړ�
		DIK_A,			// ���ړ�
		DIK_P,			// �|�[�Y
		DIK_UP,
		DIK_DOWN,
		DIK_RIGHT,
		DIK_LEFT,
		DIK_SPACE,		// �U��
	};
};
#endif