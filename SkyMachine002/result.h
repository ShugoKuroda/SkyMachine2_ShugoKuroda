//=============================================================================
//
// ���U���g��ʏ��� [result.h]
// Author : SHUGO KURODA
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "base.h"

//*****************************************************************************
// ���U���g��ʃN���X(�h���N���X)
//*****************************************************************************
class CResult : public CBase
{
public:
	CResult();
	~CResult();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;

private:
	// �e�N�X�`���ǂݍ���
	static void LoadAll();
	// �e�N�X�`���j��
	static void UnloadAll();
};

#endif
