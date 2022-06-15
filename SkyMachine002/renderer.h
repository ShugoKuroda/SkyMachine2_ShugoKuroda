//=============================================================================
//
// �����_���[����[renderer.h]
// Author : SHUGO KURODA
//
//=============================================================================
#ifndef _RENDERER_
#define _RENDERER_

#include "main.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	LPDIRECT3DDEVICE9 GetDevice();

#ifdef _DEBUG
	void DrawFPS();
#endif // _DEBUG

private:	//�����o�ϐ�
	// Direct3D�I�u�W�F�N�g
	LPDIRECT3D9 m_pD3D = nullptr;
	// Device�I�u�W�F�N�g
	LPDIRECT3DDEVICE9 m_pD3DDevice = nullptr;

#ifdef _DEBUG
	// �t�H���g
	LPD3DXFONT m_pFont = nullptr;
#endif // _DEBUG

};

#endif // _RENDERER_
