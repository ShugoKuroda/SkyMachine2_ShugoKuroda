//=============================================================================
//
// ジョイパッド処理 [input_joypad.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _INPUTMOUSE_H_
#define _INPUTMOUSE_H_

#include "renderer.h"
#include "input.h"

//*****************************************************************************
// マウスクラス(派生クラス)
//*****************************************************************************
class CInputMouse : public CInput
{
public:
	//マウス列挙型
	enum MOUSE
	{
		MOUSE_LEFT = 0,
		MOUSE_RIGHT,
		MOUSE_CENTER,
		MOUSE_MAX,
	};

	CInputMouse();
	~CInputMouse() override;

	HRESULT Init(HINSTANCE hInstance, HWND hWnd) override;
	void Uninit() override;
	void Update() override;

	bool GetPress(int nButton);
	bool GetTrigger(int nButton);
	bool GetRelese(int nButton);

	D3DXVECTOR2 GetMouseMove();
	float GetMouseWheel();

private:
	DIMOUSESTATE m_CurrentMouseState;
	DIMOUSESTATE m_PrevMouseState;
	POINT m_pos;
	POINT m_posOld;
	bool m_bLeftClick;
};

#endif
