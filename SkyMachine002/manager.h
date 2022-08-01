//=============================================================================
//
// �}�l�[�W���[����[manager.h]
// Author : SHUGO KURODA
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include <Windows.h>

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CRenderer;
class CBase;
class CSound;				// �T�E���h�N���X
//class CFade;				// �t�F�[�h�N���X
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
		MODE_RANKING,		// �����L���O
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
	static CBase *GetBase(void) { return m_pBase; }
	//static CFade *GetFade(void) { return m_pFade; }
	static CSound *GetSound(void) { return m_pSound; }
	static CInputKeyboard *GetInputKeyboard() { return m_pInputKeyboard; }
	static CInputJoypad *GetInputJoypad() { return m_pInputJoypad; }
	static CInputMouse *GetInputMouse() { return m_pInputMouse; }

private:
	static CRenderer *m_pRenderer;				// �����_���[���̃|�C���^
	static CInputKeyboard *m_pInputKeyboard;	// �L�[�{�[�h���̃|�C���^
	static CInputJoypad *m_pInputJoypad;		// �W���C�p�b�h���̃|�C���^
	static CInputMouse *m_pInputMouse;			// �}�E�X���̃|�C���^
	static CSound *m_pSound;					// �T�E���h���̃|�C���^
	static CBase *m_pBase;						// �x�[�X�V�[�����̃|�C���^
	//static CFade *m_pFade;
	static MODE m_mode;							// ���[�h���
};

#endif // _MANAGER_
