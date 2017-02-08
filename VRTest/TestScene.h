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
#include	"CDrawerSummary.h"
#include	"CDirectionSet.h"
#include	"CFuseSummary.h"
#include	"CMonitor.h"

//����M�~�b�N
#include	"CValveSummary.h"


#include	"FluidTest.h"

class TestScene : public PmGameScene
{
private:
	CVRCamera*		m_pCamera;		//�v���C���[�ɓn���u�q�p�J����
	CInundation*	m_pInun;		//�Z���Ǘ��N���X

	LPTexture		m_pMonitorTexture;		//���j�^�[�ɕ`�悷��e�N�X�`��
	LPTexture		m_pTex;
public:
	TestScene(CVRCamera* camera){ m_pCamera = camera; };
	~TestScene(){};

	void Create()
	{
		addPmSummary(new PmTest);				//�e�X�g�p�N���X
		addSummary(new CameraPmTest);			//�e�X�g�p�J�����N���X

		//���j�^�[�`��p�̃e�N�X�`������
		m_pMonitorTexture = new CTexture;
		m_pMonitorTexture->CreateTarget(g_pGraphics->GetTargetWidth(), g_pGraphics->GetTargetHeight(), PixelFormat::PIXELFORMAT_R8G8B8A8_UNORM, BufferAccess::BUFFERACCESS_GPUREADWRITE);
		//m_pMonitorTexture->CreateTarget(MONITORSIZEWIDTH, MONITORSIZEHEIGHT, PixelFormat::PIXELFORMAT_R8G8B8A8_UNORM, BufferAccess::BUFFERACCESS_GPUREADWRITE);

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
		addPmSummary(new CFuseSummary(CVector3(0, 0, 0), CVector3(0, 0, 0)));

		//���j�^�[�ǉ�
		addPmSummary(new CMonitor(m_pMonitorTexture));

		//�Ō�ɐ���ǉ�
		//addPmSummary(m_pInun);
		
		addPmSummary(new FluidTest(this->getFluid()));

		m_pTex = new CTexture;
		m_pTex->Load("back.jpg");
	}

	void SceneRearUpdate()
	{
		//�`�����ꎞ�I�ɕۑ����A�`�����Z�b�g
		LPRenderTarget prt = g_pGraphics->GetRenderTarget();
		g_pGraphics->SetRenderTarget(m_pMonitorTexture->GetRenderTarget(),g_pGraphics->GetDepthTarget());

		//��ʂ̃N���A
		g_pGraphics->ClearTarget(1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0);

		//���j�^�[�ɕ`��
		CGraphicsUtilities::RenderString(10, 10, "TETETETETESTaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
		CRectangle drec(0, 0, m_pMonitorTexture->GetWidth(), m_pMonitorTexture->GetHeight());
		CGraphicsUtilities::RenderTexture(drec, m_pTex);

		//�`����߂�
		g_pGraphics->SetRenderTarget(prt, g_pGraphics->GetDepthTarget());
	}

	void SceneRelease()
	{
		delete m_pMonitorTexture;
		delete m_pTex;
		delete m_pInun;
	}
};