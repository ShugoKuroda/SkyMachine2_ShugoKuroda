//===================================================================
//
// �l�X�ȏ������܂Ƃ߂��t�@�C�� (library.cpp)
// Author : SHUGO KURODA
//
//===================================================================
#include "library.h"

//*******************************************************************
// �}�N����`
//*******************************************************************
// �X�N���[���T�C�Y
#define SCREEN_WIDTH		(CRenderer::SCREEN_WIDTH)
#define SCREEN_HEIGHT		(CRenderer::SCREEN_HEIGHT)

//-----------------------------------------------------------------------------------------------
// ��ʊO����
//-----------------------------------------------------------------------------------------------
bool LibrarySpace::OutScreen2D(const D3DXVECTOR3 *pPos, const D3DXVECTOR2 size)
{
	// ��ʂ̒[����o�Ă��邩�ǂ���
	if (pPos->x < -(size.x / 2) ||
		pPos->x > SCREEN_WIDTH + size.x / 2 ||
		pPos->y < -(size.y / 2) ||
		pPos->y > SCREEN_HEIGHT + size.y / 2)
	{
		//��ʊO�ɏo��
		return true;
	}

	//�܂���ʓ�
	return false;
}

//-----------------------------------------------------------------------------------------------
// ��ʓ��͈̔͐���
//-----------------------------------------------------------------------------------------------
bool LibrarySpace::SteyInScreen2D(D3DXVECTOR3 *pPos, D3DXVECTOR2 size)
{
	// ��ʒ[���ǂ���
    bool bScreenIn = false;    

    // ��ʂ̍�
    if (pPos->x < size.x / 2)
    {
        // �����o��
        pPos->x = size.x / 2;
        // �����o���t���O��true��
        bScreenIn = true;
    }
    // ��ʂ̉E
    if (pPos->x > SCREEN_WIDTH - size.x / 2)
    {
		// �����o��
        pPos->x = SCREEN_WIDTH - size.x / 2;
        // �����o���t���O��true��
        bScreenIn = true;
    }

    // ��ʂ̏�
	if (pPos->y < 100.0f + size.y / 2)
	{
		// �����o��
		pPos->y = 100.0f + size.y / 2;
		// �����o���t���O��true��
		bScreenIn = true;
	}

    // ��ʂ̉�
    if (pPos->y > SCREEN_HEIGHT - size.y / 2)
    {
		// �����o��
        pPos->y = SCREEN_HEIGHT - size.y / 2;
        // �����o���t���O��true��
        bScreenIn = true;
    }

    return bScreenIn;
}

//-----------------------------------------------------------------------------------------------
// ��`�̓����蔻��2D
//-----------------------------------------------------------------------------------------------
bool LibrarySpace::BoxCollision2D(const D3DXVECTOR3 *pPos1, const D3DXVECTOR3*pPos2, const D3DXVECTOR3*pSize1, const D3DXVECTOR3*pSize2)
{
	// �l�ʂ̓����蔻��
	if (pPos1->x - pSize1->x / 2 <= pPos2->x + pSize2->x / 2 &&
		pPos1->x + pSize1->x / 2 >= pPos2->x - pSize2->x / 2 &&
		pPos1->y - pSize1->y / 2 <= pPos2->y + pSize2->y / 2 &&
		pPos1->y + pSize1->y / 2 >= pPos2->y - pSize2->y / 2)
	{
		// ��������
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------------------------
// ���̂̓����蔻��
//-----------------------------------------------------------------------------------------------
bool LibrarySpace::SphereCollision2D(const D3DXVECTOR3& posIn, const D3DXVECTOR3& posOut, const float& fsizeIn, const float& fsizeOut)
{
	//2�̍��W���������߂�
	D3DXVECTOR3 Length = posIn - posOut;

	//���W�������狗�������߂�
	float fLength = D3DXVec3Length(&Length);

	if (fsizeIn + fsizeOut > fLength)
	{//���݂��̃T�C�Y�̍��v��苗����������������
		return true;	//��������
	}

	return false;		//�������ĂȂ�
}

//-----------------------------------------------------------------------------------------------
// �����̎擾
//-----------------------------------------------------------------------------------------------
int LibrarySpace::GetRandNum(int nMax, int nMin)
{
	//�ő�l�`�ŏ��l���痐�����擾
	int nRandNum = rand() % (nMax - nMin + 1) + nMin;

	return nRandNum;
}

//-----------------------------------------------------------------------------------------------
// ����(��������)�̎擾
//-----------------------------------------------------------------------------------------------
float LibrarySpace::GetRandFloat(int nMax, int nMin, int nNumDiv)
{
	int nMaxNum = nMax * nNumDiv;
	int nMinNum = nMin * nNumDiv;
	
	//�ő�l�`�ŏ��l���痐�����擾
	float fRandNum = (float)(rand() % (nMaxNum - nMinNum + 1) + nMinNum);
	fRandNum = fRandNum / nNumDiv;

	return fRandNum;
}

//-----------------------------------------------------------------------------------------------
// �T�C���̎擾
//-----------------------------------------------------------------------------------------------
float LibrarySpace::GetSinVec(float fRot, float fNumMul)
{
	return sinf(D3DX_PI * fRot) * fNumMul;
}

//-----------------------------------------------------------------------------------------------
// �R�T�C���̎擾
//-----------------------------------------------------------------------------------------------
float LibrarySpace::GetCosVec(float fRot, float fNumMul)
{
	return cosf(D3DX_PI * fRot) * fNumMul;
}

//-----------------------------------------------------------------------------------------------
// ���W�Ԃ̋����v�Z
//-----------------------------------------------------------------------------------------------
float LibrarySpace::SphereRange(D3DXVECTOR3 posStart, D3DXVECTOR3 posEnd)
{
	//2�̍��W���������߂�
	D3DXVECTOR3 Length = posStart - posEnd;

	//���W�������狗�������߂�
	float fLength = D3DXVec3Length(&Length);

	return fLength;
}
