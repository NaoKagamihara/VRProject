//////////////////////////////
//�e�X�g�p�V�[���쐬

#pragma once

#include		"PmGameScene.h"
#include		"TestObject.h"
#include		"CPlayer.h"
#include		"VR/VRCamera.h"

//�A�C�e��
#include	"CWrenchSummary.h"
#include	"CFigureSummary.h"

//�M�~�b�N
#include	"CValveSummary.h"
#include	"CDrawerSummary.h"
#include	"CDirectionSet.h"

#include	"FluidTest.h"

class TestScene : public PmGameScene
{
private:
	CVRCamera*	m_pCamera;
public:
	TestScene(CVRCamera* camera){ m_pCamera = camera; };
	~TestScene(){};

	void Create()
	{
		addPmSummary(new PmTest);
		addSummary(new CameraPmTest);
		addPmSummary(new CPlayer(m_pCamera));

		////////////////////////////////////////////////////////
		//�A�C�e���ǉ�
		addPmSummary(new CWrenchSummary);
		//addPmSummary(new CFigureSummary);

		////////////////////////////////////////////////////////
		//�M�~�b�N�ǉ�
		//�o���u�ǉ�
		CBreakGimmick* valve = new CValveSummary();
		addPmSummary(valve);
		//�M�~�b�N�j��
		valve->gimmickBreak();

		//���̐ݒ�
		addPmSummary(new CDirectionSetSummary);

		//�Ђ������ǉ�
		addPmSummary(new CDrawerSummary(CVector3(0, 10, 25), CVector3(0, MOF_ToRadian(0), 0)));
	}
};