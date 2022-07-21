//===================================================================
//
//	�G�̐������[enemy_data.h]
//	Author : SHUGO KURODA
//
//===================================================================
#ifndef _ENEMY_DATA_H_
#define _ENEMY_DATA_H_

#include "renderer.h"

//===================================================================
// �}�N����`
//===================================================================
#define MAX_MOVE		(30)		//�ő�ړ���
#define MAX_WAVE		(30)		//�ő�E�F�[�u��(�����l)

//*******************************************************************
//	�G�̃��[�h���̒�`
//*******************************************************************
typedef struct
{
	D3DXVECTOR3 posEnd;		//�ڕW�̈ʒu
	int nFrameMove;			//�ړ�����(�t���[��)
}EnemyMove;

typedef struct
{
	EnemyMove MoveInfo[MAX_MOVE];	//�ړ����
	D3DXVECTOR3 pos;	//�o������ʒu
	int nMaxMove;		//�ő�ړ���
	int nFrame;			//�o������t���[��(����)
	int nType;			//�G�̎��
	int nNum;			//�o������G�̐�
	int nInterval;		//�A���œG���o���Ԋu
}EnemyInfo;

//�G�̐����J�E���^�[���
typedef struct
{
	// �G���A���Ő��������܂ł̎���
	int nCreateTime;
	// �G���������ꂽ��
	int nCreateNum;
	//�G�𐶐����Ȃ����ǂ���
	bool bCreate;
}EnemyCount;

typedef struct
{
	// �G�����������܂ł̎���
	int nCreatenCount;
	//�G�̐����J�E���^�[���
	EnemyCount CountInfo[MAX_WAVE];
	// �G�̐������
	EnemyInfo *pCreateInfo;
	// �ő�E�F�[�u��
	int nWave;
}EnemySetInfo;

#endif
