//=============================================================================
//
// �Q�[����ʏ��� [game.h]
// Author : SHUGO KURODA
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "base.h"
#include "enemy_data.h"
#include "cloud_data.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CPlayer;

//-----------------------------------------------------------------------------
// �x�[�X�N���X(�h���N���X)
//-----------------------------------------------------------------------------
class CGame : public CBase
{
private:	//�萔
	static const int MAX_POS_CLOUD = 250;
	static const int MIN_POS_CLOUD = 180;

public:		//�����o�֐�
	CGame();
	~CGame() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;

	void CreateCloud();
	void CreateBubble();
	void CreateEnemy();
	static void SetCreateCloud(bool bCreate) { m_bCreateCloud = bCreate; }
	static void SetCreateBubble(bool bCreate) { m_bCreateBubble = bCreate; }
	static CPlayer *GetPlayer() { return m_pPlayer; }

private:
	// �e�N�X�`���ǂݍ���
	static void LoadAll();
	// �e�N�X�`���j��
	static void UnloadAll();

private:	//�����o�ϐ�
	static CPlayer *m_pPlayer;

	CloudInfo m_CloudInfo;
	// �_�𐶐����邩�ǂ���
	static bool m_bCreateCloud;

	// �_�����������܂ł̎���(����)
	int m_nRandBubble;
	// �_�����������܂ł̎��ԃJ�E���^�[
	int m_nCntBubble;
	// �A�𐶐����邩�ǂ���
	static bool m_bCreateBubble;

	// �G�̃Z�b�g���
	EnemySetInfo m_EnemySetInfo;
};

#endif	//_GAME_H_
