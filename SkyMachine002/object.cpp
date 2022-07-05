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

//=============================================================================
// �g�R���X�g���N�^
//=============================================================================
CObject::CObject()
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (m_apObject[nCntObject] == nullptr)
		{
			// �I�u�W�F�N�g�����i�[
			m_apObject[nCntObject] = this;
			// �I�u�W�F�N�g�ԍ��̕ۑ�
			m_nID = nCntObject;
			// ���v�I�u�W�F�N�g�������Z
			m_nNumAll++;
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
			// �I�u�W�F�N�g�̏I������
			m_apObject[nCntUninit]->Uninit();

			// �������̊J��
			delete m_apObject[nCntUninit];
			m_apObject[nCntUninit] = nullptr;
		}
	}
}

//=============================================================================
//	�X�V����
//=============================================================================
void CObject::UpdateAll()
{
	for (int nCntUpdate = 0; nCntUpdate < MAX_OBJECT; nCntUpdate++)
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
	for (int nCntDraw = 0; nCntDraw < MAX_OBJECT; nCntDraw++)
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
		m_nNumAll--;
	}
}
