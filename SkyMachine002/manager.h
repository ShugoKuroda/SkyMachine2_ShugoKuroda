//=============================================================================
//
// �}�l�[�W���[����[manager.h]
// Author : SHUGO KURODA
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include <Windows.h>
#include "player.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CRenderer;
class CBase;
class CSound;				// �T�E���h�N���X
class CFade;				// �t�F�[�h�N���X
class CInputKeyboard;		// �L�[�{�[�h�N���X
class CInputJoypad;			// �W���C�p�b�h�N���X
class CInputMouse;			// �}�E�X�N���X

//*****************************************************************************
// �N���X��`(��{�N���X)
//*****************************************************************************
class CManager
{
public:
	// ���[�h�̎��
	enum MODE
	{
		MODE_TITLE = 0,		// �^�C�g��
		MODE_TUTORIAL,		// �`���[�g���A��
		MODE_GAME,			// �Q�[��
		MODE_RESULT,		// ���U���g
		MODE_MAX,
	};

	CManager();
	~CManager();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
	void Uninit();
	void Update();
	void Draw();
	
	static void SetMode(MODE mode);
	static MODE GetMode() { return m_mode; }

	static CRenderer *GetRenderer() { return m_pRenderer; }
	static CBase *GetBase() { return m_pBase; }
	static CFade *GetFade() { return m_pFade; }
	static CSound *GetSound() { return m_pSound; }
	static CInputKeyboard *GetInputKeyboard() { return m_pInputKeyboard; }
	static CInputJoypad *GetInputJoypad() { return m_pInputJoypad; }
	static CInputMouse *GetInputMouse() { return m_pInputMouse; }

	static bool GetPause() { return m_bPause; }
	static void SetPause(bool bPause) { m_bPause = bPause; }
	static bool GetEntry(int nNum) { return m_bEntry[nNum]; }
	static void SetEntry(int nNum, bool bEntry) { m_bEntry[nNum] = bEntry; }

private:
	static CRenderer *m_pRenderer;				// �����_���[���̃|�C���^
	static CInputKeyboard *m_pInputKeyboard;	// �L�[�{�[�h���̃|�C���^
	static CInputJoypad *m_pInputJoypad;		// �W���C�p�b�h���̃|�C���^
	static CInputMouse *m_pInputMouse;			// �}�E�X���̃|�C���^
	static CSound *m_pSound;					// �T�E���h���̃|�C���^
	static CBase *m_pBase;						// �x�[�X�V�[�����̃|�C���^
	static CFade *m_pFade;						// ��ʃt�F�[�h���̃|�C���^
	static MODE m_mode;							// ���[�h���
	static bool m_bPause;						// �|�[�Y���邩�ǂ���
	static bool m_bEntry[CPlayer::PLAYER_MAX];	// �Q�����Ă��邩�ǂ���
};

#endif // _MANAGER_
