//////////////////////////////
//�e�X�g�p�V�[���쐬

#pragma once

#include		"PmGameScene.h"
#include		"TestObject.h"
#include		"CPlayer.h"
#include		"VR/VRCamera.h"

//�Z���N���X
#include	"CInundation.h"
#include	"FluidTest.h"

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
	CVRCamera*		m_pCamera;		//�v���C���[�ɓn���u�q�p�J����
	CInundation*	m_pInun;		//�Z���Ǘ��N���X
public:
	TestScene(CVRCamera* camera){ m_pCamera = camera; };
	~TestScene(){};

	void Create()
	{
		addPmSummary(new PmTest);				//�e�X�g�p�N���X
		addSummary(new CameraPmTest);			//�e�X�g�p�J�����N���X

		addPmSummary(new CPlayer(m_pCamera));	//�v���C���[
		m_pInun = new CInundation();			//�Z���Ǘ��N���X
		//���̐ݒ�
		addPmSummary(new CDirectionSetSummary);

		////////////////////////////////////////////////////////
		//�A�C�e���ǉ�
		addPmSummary(new CWrenchSummary);
		//addPmSummary(new CFigureSummary);

		////////////////////////////////////////////////////////
		//����M�~�b�N�ǉ�
		//�o���u�ǉ�
		CBreakGimmick* valve = new CValveSummary(m_pInun);
		addPmSummary(valve);
		//�M�~�b�N�j��
		valve->gimmickBreak();

		////////////////////////////////////////////////////////
		//�M�~�b�N�ǉ�
		//�Ђ������ǉ�
		addPmSummary(new CDrawerSummary(CVector3(23.5f, 5, -32), CVector3(0, MOF_ToRadian(180), 0)));

		//�Ō�ɐ���ǉ�
		addPmSummary(m_pInun);
		
		addPmSummary(new FluidTest(this->getFluid()));
	}
};