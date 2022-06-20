//=============================================================================
//
// �I�u�W�F�N�g����[object.cpp]
// Author : SHUGO KURODA
//
//=============================================================================
#include "object.h"

//*****************************************************************************
//	�ÓI�����o�ϐ�
//*****************************************************************************
CObject* CObject::m_apObject[CObject::MAX_OBJECT] = { nullptr };
int CObject::m_nNumAll = 0;
int CObject::m_nID = 0;

//=============================================================================
// �g�R���X�g���N�^
//=============================================================================
CObject::CObject()
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (m_apObject[nCntObject] == nullptr)
		{
			m_apObject[nCntObject] = this;
			m_nNumAll++;
			m_nID = nCntObject;
			break;
		}
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CObject::~CObject()
{
}

//=============================================================================
//	�����[�X����
//=============================================================================
void CObject::ReleaseAll()
{
	for (int nCntUninit = 0; nCntUninit < MAX_OBJECT; nCntUninit++)
	{
		// �I�u�W�F�N�g�C���X�^���X�̔j��
		if (m_apObject[nCntUninit] != nullptr)
		{
			// �I������
			m_apObject[nCntUninit]->Uninit();
		}
	}
}

//=============================================================================
//	�X�V����
//=============================================================================
void CObject::UpdateAll()
{
	for (int nCntUpdate = 0; nCntUpdate < m_nNumAll; nCntUpdate++)
	{
		if (m_apObject[nCntUpdate] != nullptr)
		{
			// �|���S���̍X�V����
			m_apObject[nCntUpdate]->Update();
		}
	}
}

//=============================================================================
//	�R���X�g���N�^
//=============================================================================
void CObject::DrawAll()
{
	for (int nCntDraw = 0; nCntDraw < m_nNumAll; nCntDraw++)
	{
		if (m_apObject[nCntDraw] != nullptr)
		{
			//�|���S���̕`�揈��
			m_apObject[nCntDraw]->Draw();
		}
	}
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
void CObject::Release()
{
	// �I�u�W�F�N�g�C���X�^���X�̔j��
	if (m_apObject[m_nID] != nullptr)
	{
		// �I������
		int nID = m_nID;
		delete m_apObject[nID];
		m_apObject[nID] = nullptr;
		//m_nNumAll--;
	}
}
