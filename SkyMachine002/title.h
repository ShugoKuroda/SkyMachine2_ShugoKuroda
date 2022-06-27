//=============================================================================
//
// �^�C�g����ʏ��� [title.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "base.h"

//*****************************************************************************
// �^�C�g����ʃN���X(�h���N���X)
//*****************************************************************************
class CTitle : public CBase
{
public:
	// �^�C�g���̃��[�h
	typedef enum
	{
		TITLEMODE_NORMAL = 0,		// �ʏ�
		TITLEMODE_TRANSITION,		// �J��
		TITLEMODE_MAX,
	}TITLEMODE;

	CTitle();
	~CTitle();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	TITLEMODE GetTitleMode(void) { return m_titlemode; }
	void SetTitleMode(TITLEMODE titlemode) { m_titlemode = titlemode; }

private:
	TITLEMODE m_titlemode;					// �^�C�g�����[�h
};
#endif