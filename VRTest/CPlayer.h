///////////////////////////
//

#pragma once

#include	"PmGameSummary.h"
#include	"CHand.h"
#include	"CVRPlayerCamerah.h"
#include	"CommonDefine.h"

//��̓����蔻��̑傫��
#define		HANDCOLLR		0.5f

////////////////////////////////////////////////
//�v���C���[�̑���Ǘ��N���X
class CPlayer : public PmGameSummary
{
private:

	//��
	CHand*	m_pRightHand;
	CHand*	m_pLeftHand;
	//VR�p�J����
	CVRPlayerCamera*	m_pCamera;

	//�v���C���[�̍��W
	CVector3	m_Pos;
	//�ړ��ł���C���^�[�o��
	int			m_MoveCount;

	HandData	m_rHandData;
	HandData	m_lHandData;

public:

	CPlayer(CVRCamera* camera);
	virtual ~CPlayer();

	virtual void Create();

	virtual void Initialize();

	virtual void Update();

	virtual void RearUpdate();

	virtual void Render();

	virtual void RearRender();

	virtual void DebugRender();

	virtual void Release();

private:
	void handUpdate();
	void debugUpdate();
};