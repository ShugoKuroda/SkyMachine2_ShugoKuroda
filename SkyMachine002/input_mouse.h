//=============================================================================
//
// ジョイパッド処理 [input_joypad.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _INPUTMOUSE_H_
#define _INPUTMOUSE_H_

#include "input.h"

//*****************************************************************************
// 列挙型
//*****************************************************************************
typedef enum
{
	MOUSE_LEFT = 0,
	MOUSE_RIGHT,
	MOUSE_CENTER,
	MOUSE_MAX,
}MOUSE;

//*****************************************************************************
// ジョイパッドクラス(派生クラス)
//*****************************************************************************
class CInputMouse : public CInput
{
public:
	CInputMouse();
	~CInputMouse();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetPress(int nButton);
	bool GetTrigger(int nButton);
	bool GetRelese(int nButton);

	D3DXVECTOR2 GetMouseMove(void);
	float GetMouseWheel(void);

private:
	DIMOUSESTATE m_CurrentMouseState;
	DIMOUSESTATE m_PrevMouseState;
	POINT m_pos;
	POINT m_posOld;
	bool m_bLeftClick;
};
#endif