//=============================================================================
//
// �I�u�W�F�N�g����[object.h]
// Author : SHUGO KURODA
//
//=============================================================================
#ifndef _MANAGER_
#define _MANAGER_

#include "main.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CRenderer;
class CInputKeyboard;		// �L�[�{�[�h�N���X
class CInputJoypad;			// �W���C�p�b�h�N���X
class CInputMouse;			// �}�E�X�N���X

//*****************************************************************************
// �N���X��`(��{�N���X)
//*****************************************************************************
class CManager
{
public:
	CManager();
	~CManager();

	HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
	void Uninit();
	void Update();
	void Draw();

	static CRenderer *GetRenderer() { return m_pRenderer; }
	static CInputKeyboard *GetInputKeyboard() { return m_pInputKeyboard; }
	static CInputJoypad *GetInputJoypad() { return m_pInputJoypad; }
	static CInputMouse *GetInputMouse() { return m_pInputMouse; }

private:
	static CRenderer *m_pRenderer;
	static CInputKeyboard *m_pInputKeyboard;	// �L�[�{�[�h���̃|�C���^
	static CInputJoypad *m_pInputJoypad;		// �W���C�p�b�h���̃|�C���^
	static CInputMouse *m_pInputMouse;			// �}�E�X���̃|�C���^
};

#endif // _MANAGER_
