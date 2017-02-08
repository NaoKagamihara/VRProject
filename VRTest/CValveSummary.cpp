
//�o���u�N���X��`

#include	"CValveSummary.h"


//////////////////////////////////////
//�o���u�Ǘ��N���X
void CValveSummary::Create()
{
	PmPhysxCreateTool* tool = createPhysxCreateTool();

	//�܂Ă肠��
	PxMaterial* mate;
	mate = tool->createMaterial(CVector3(0.5f, 0.5f, 0.5f));

	CVector3 mainPos(0, 10, 20);

	//�����蔻��쐬
	m_pCollision = (PmObjectDynamic*)createSphereObject(mainPos, mate, COLLISIONRADIUS, true);
	delete tool;

	//�T�[�`����������
	m_pValveSearch = new CHandCollSearch();
	m_pCollision->releaseCollisionSearch();
	m_pCollision->setCollisionSearch(m_pValveSearch);
	m_pCollision->setKinematic(true);
	m_pCollision->setTrigger(1, true);

	//���f���ǂݍ���
	if (!m_ValuveMesh.Load("valve.mom"))
	{
		ERMS("valveLoad ERROR");
	}

	//////////////////////////////////////////////////////////
	//�e�X�g�p�ɂ����Œ�`

	//�����蔻��̍��W��ύX
	PxShape* shape = m_pCollision->getShape(1);
	PxTransform tra = shape->getLocalPose();
	CVector3 vec;
	tra.p.x = 2.0f;
	shape->setLocalPose(tra);

	//������ύX
	m_Rotation = CVector3(0, MOF_ToRadian(90), 0);
	m_pCollision->setGlobalRot(m_Rotation);


	if (!m_PipeMesh.Load("pipe/pipe.mom"))
	{
		ERMS("pipeLoad ERROR");
	}
	m_pPipeObj = createTriangleObject(CVector3(mainPos.x,mainPos.y - 1,mainPos.z), mate, m_PipeMesh, CVector3(OBJALLSCALE, OBJALLSCALE, OBJALLSCALE));
	m_pPipeObj->setGlobalRot(CVector3(0, MOF_ToRadian(90), 0));
	m_bprevflag = false;
}

void CValveSummary::Update()
{
	if (m_pValveSearch->getHandObj(RIGHTHAND) != NULL)
	{	
		//m_Rotation.x += MOF_ToRadian(1);
		//m_pCollision->setGlobalRot(m_Rotation);	
	}

	//���Ă���Ԃ͐���ǉ�
	if (m_bBreakFlag == true)
	{
		m_pInundation->addWater(DEFADDWATER);
	}
}

void CValveSummary::RearUpdate()
{
	//����ŉ�]�������邩�e�X�g�p
	bool flagGrasp = false;

	//���E�̎�ň����Ă���Ƃ�������
	PmObject* hand[HANDNUM] = { NULL };
	for (int i = 0; i < HANDNUM; i++)
	{
		//���E�̎肪�o���u�ɓ������Ă��邩 || �����Ă��邩
		hand[i] = m_pValveSearch->getHandObj(i);
		if (hand[i] == NULL || !hand[i]->checkObjectTag(EPmObjectTag::OBJ_HANDGRASP))
		{
			flagGrasp = false;
			break;
		}
		flagGrasp = true;
	}

	//����ň����Ă����
	if (flagGrasp == true)
	{
		//�o���u��pose���擾����
		CMatrix44 mat;

		//���W�Ǝp�����擾����
		PxTransform tra = m_pCollision->getActor()->getGlobalPose();

		///////////////////////////////////////
		//��]
		//�O���[�o����]�擾
		//CQuaternion pq(tra.q.x, tra.q.y, tra.q.z, tra.q.w);
		//pq.ConvertMatrixTranspose(mat);
		mat.RotationZXY(CVector3(0, MOF_ToRadian(90), 0));

		//shape��pose�����o��
		PxShape* shape;
		m_pCollision->getActor()->getShapes(&shape, 1);
		//���[�J�����W�擾
		CVector3 lpv(shape->getLocalPose().p.x, shape->getLocalPose().p.y, shape->getLocalPose().p.z);

		//�O���[�o���ɍ��킹�ă��[�J������]������
		lpv *= mat;

		///////////////////////////////////////
		//���W
		//global���l������shape�̍��W
		CVector3 pv(tra.p.x + lpv.x, tra.p.y + lpv.y, tra.p.z + lpv.z);

		//�s��ɕϊ�
		mat.SetTranslation(pv);

		//�t�s������߂�
		CMatrix44 matInv;
		mat.Inverse(matInv);

		//////////////////////////////////
		//����̊p�x�����߂�
		CVector3 handVec[HANDNUM];
		for (int i = 0; i < HANDNUM; i++)
		{
			//�e��ɋt�s���������
			handVec[i] = hand[i]->getGlobalPos();
			handVec[i] *= matInv;
		}
		//�p�x���v�Z
		//�p�x�����߂�
		float r = atan2f(handVec[RIGHTHAND].y - handVec[LEFTHAND].y, (handVec[RIGHTHAND].z - handVec[LEFTHAND].z));

		//��̓����ɍ��킹�ăo���u����]������
		if (m_HisAng[LEFTHAND] != 1000.0f && m_bprevflag)
		{
			m_Rotation.x += m_HisAng[LEFTHAND] - r;
			m_pCollision->setGlobalRot(m_Rotation);
		}
		//���W��ۑ�
		m_HisAng[LEFTHAND] = r;

		//�񂵂�������C������
		if (MOF_ToDegree(m_Rotation.x) >= 360)
		{
			m_bBreakFlag = false;
			m_Rotation.x = 0.0f;
		}

		//�����Ă���ꏊ�Ɏ���Œ肷��
		for (int i = 0; i < HANDNUM; i++)
		{
			//��ɋt�s���������
			CVector3 vec = hand[i]->getGlobalPos();
			vec *= matInv;

			//�p�x�����߂�
			float r = atan2f(vec.y, vec.z);
			CVector3 npos(0, sinf(r) * COLLISIONRADIUS, cosf(r) * COLLISIONRADIUS);
			npos *= mat;

			//����o���u�ɌŒ肷��
			hand[i]->setGlobalPos(CVector3(npos.x, npos.y, npos.z));
		}
	}
	m_bprevflag = flagGrasp;
}

void CValveSummary::Render()
{
	//
	CMatrix44 mat;
	mat.SetTranslation(m_pCollision->getGlobalPos());

	CMatrix44 sca;
	sca.Scaling(0.4f);

	CMatrix44 rot;
	m_pCollision->getGlobalQuoRot().ConvertMatrixTranspose(rot);
	
	//�e�X�g
	CMatrix44 test;
	CMatrix44 test2;
	test2.SetTranslation(CVector3(2.0f, 0, 0));
	test *= test2;

	m_ValuveMesh.Render(sca * rot * mat);
	CMatrix44 pMat = m_pPipeObj->getActorMatrix44(CVector3(OBJALLSCALE, OBJALLSCALE, OBJALLSCALE));
	m_PipeMesh.Render(pMat);
}

void CValveSummary::DebugRender()
{
	PmRenderShape::RenderShape(m_pCollision, Mof::Color(1, 0, 0, 0.8f));

	CGraphicsUtilities::RenderString(10, 50, "%f",MOF_ToDegree(m_Rotation.x));
}

void CValveSummary::Release()
{
	m_ValuveMesh.Release();
}

void CValveSummary::gimmickBreak()
{
	m_bBreakFlag = true;
}