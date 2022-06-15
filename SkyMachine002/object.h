//=============================================================================
//
// オブジェクト処理[object.h]
// Author : SHUGO KURODA
//
//=============================================================================
#ifndef _OBJECT_
#define _OBJECT_

#include "main.h"

//*****************************************************************************
// クラス定義(基本クラス)
//*****************************************************************************
class CObject
{
public:
	CObject();
	//CObject(CObject2D* pObject2D);
	virtual ~CObject();

	virtual HRESULT Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	static void ReleaseAll();
	static void UpdateAll();
	static void DrawAll();

	void Release();

	//最大オブジェクト数
	static const int MAX_OBJECT = 4;

private:
	static CObject *m_apObject[MAX_OBJECT];
	static int m_nNumAll;
	static int m_nID;
};

#endif // _OBJECT_
