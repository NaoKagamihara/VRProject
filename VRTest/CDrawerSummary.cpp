
//////////////////////////////////////
//�Ђ������Ǘ��N���X��`

#include		"CDrawerSummary.h"

CDrawerSummary::CDrawerSummary(CVector3& pos, CVector3& rot)
{
	//�����o���̈ʒu�Ǝp��������
	m_Pos = pos;
	m_Rot = rot;
}

void CDrawerSummary::Create()
{
	PmPhysxCreateTool* tool = this->createPhysxCreateTool();
	PxMaterial* mate = tool->createMaterial(CVector3(0.5f, 0.5f, 0.5f));

	//�Ђ������{�̂̃��f���ǂݍ���
	if (!m_DrawerBody.Load("DrawerMom/drawer_Body.mom"))
		ERMS("DrawerBodyLoadError");
	m_pDrawerBodyObj = createTriangleObject(m_Pos, mate, m_DrawerBody, CVector3(OBJALLSCALE, OBJALLSCALE, OBJALLSCALE));
	m_pDrawerBodyObj->setGlobalRot(m_Rot);

	//�Ђ����������̃��f���ǂݍ���
	if (!m_Drawer.Load("DrawerMom/drawer.mom"))
		ERMS("DrawerLoadError");

	//�����o���̃n���h�����f���ǂݍ���
	if (!m_DrawerHandle.Load("DrawerMom/drawer_handle.mom"))
		ERMS("DrawerHandleLoadError");

	////////////////////////////////////////////////////////
	//�����o���̃{�[������n���h�����Z�b�g������W���擾����
	LPBoneArray handleBoneArray = m_Drawer.GetBone();
	LPBone handleBone = handleBoneArray->GetData(DRAWERHANDLEBONE);
	CMatrix44 bonePos = handleBone->GetMatrix();
	bonePos.GetTranslation(m_HandlePos);
	m_HandlePos *= OBJALLSCALE;
	m_HandlePos.z += 1 * OBJALLSCALE;			//������
	m_HandlePos.RotationY(m_Rot.y);				//�{�̂̉�]��������

	//�{�[����񂩂�����o����ݒu������W���擾
	LPBoneArray boneArray = m_DrawerBody.GetBone();
	int boneNum = DRAWERBONESTART;
	for (int i = 0; i < DRAWERNUM; i++)
	{
		CVector3 pos;
		LPBone bone = boneArray->GetData(boneNum);	//�{�[���擾
		CMatrix44 mat = bone->GetMatrix();			//�s��擾
		mat.GetTranslation(pos);					//�x�N�g���ɕϊ�
		pos.z += 18;								//�ŉ��ɂ��炵�Đݒu
		pos.RotationY(m_Rot.y);						//�{�̂̉�]�ɍ��킹��
		pos *= OBJALLSCALE;							//�傫�������킹��
		pos += m_Pos;								//�{�̂̍��W�ɐݒu
		//�{�̂̋t�s��������������ʒu���擾
		CMatrix44 iniMat,iniMatInv;
		iniMat = m_pDrawerBodyObj->getActorMatrix44();
		iniMat.Inverse(iniMatInv);
		m_DrawerIniPos[i] = pos * iniMatInv;
		//�ݒu������W�ɃI�u�W�F�N�g���쐬
		m_pDrawerObj[i] = (PmObjectDynamic*)createEmpObject(pos, mate, true);	//��̃I�u�W�F�N�g���쐬
		m_pDrawerObj[i]->setGlobalRot(m_Rot);

		///////////////////////////////////
		//��ǂ��쐬
		m_pDrawerObj[i]->setKinematic(true);		//�Œ�I�u�W�F�ɕύX
		CVector3 size(DRAWERLENX, DRAWERLENY, DRAWERLENZ);
		size *= OBJALLSCALE;
		PxShape* boxShape = tool->createBox(size, mate);
		///////////////////////////////////////////////////////////////////////////////////////////
		PxVec3 boxPos(0.5f, -5.0f, 0);											//�{�[���ύX�O
		boxPos *= OBJALLSCALE;
		boxShape->setLocalPose(PxTransform(boxPos));						
		///////////////////////////////////////////////////////////////////////////////////////////
		m_pDrawerObj[i]->addShape(*boxShape);

		//////////////////////////////////////////
		//���E�̕Ǎ쐬
		PxShape* shape;
		for (int tow = 0; tow < 2; tow++)
		{
			//���̕Ǎ쐬
			shape = tool->createBox(size, mate);
			PxTransform tra;
			CMatrix44 rot;
			//��]
			if (tow % 2 == 0) rot.RotationZ(MOF_ToRadian(60));
			else rot.RotationZ(MOF_ToRadian(-60));
			CQuaternion cq;
			rot.GetRotation(cq);
			PxQuat pq(cq.x, cq.y, cq.z, cq.w);
			tra.q = pq;

			//���W
			///////////////////////////////////////////////////////////////////////////////////////////
			if (tow % 2 == 0) tra.p = PxVec3(-4.5f, -2.1f, 0);							//�{�[���ύX�O
			else tra.p = PxVec3(4.5f, -2.1f, 0);
			tra.p *= OBJALLSCALE;
			///////////////////////////////////////////////////////////////////////////////////////////

			//�A�N�^�[�ɒǉ�
			shape->setLocalPose(tra);
			m_pDrawerObj[i]->addShape(*shape);

			///////////////////////////////
			//��O�̕Ǎ쐬
			PxShape* sideShape = tool->createBox(CVector3(DRAWERSIDEX, DRAWERSIDEY, DRAWERSIDEZ), mate);
			PxTransform sideTra = sideShape->getLocalPose();
			if (tow % 2 == 0) sideTra.p = PxVec3(0, 0, -17.5f);							//�{�[���ύX�O
			else sideTra.p = PxVec3(0,0,17);
			sideTra.p *= OBJALLSCALE;

			//�A�N�^�[�ɒǉ�
			sideShape->setLocalPose(sideTra);
			m_pDrawerObj[i]->addShape(*sideShape);
		}

		//�g���K�[�p�̃I�u�W�F�N�g�쐬
		float sphereSize = 5 * OBJALLSCALE;
		m_pDrawerHandleObj[i] = (PmObjectDynamic*)createSphereObject(CVector3(0, 0, 0), mate, sphereSize, true);
		m_pDrawerHandleObj[i]->setKinematic(true);
		m_pDrawerHandleObj[i]->setTrigger(1, true);
		//�{�[���̈ʒu�Ɉړ�������
		CVector3 handleSetPos = m_HandlePos + m_pDrawerObj[i]->getGlobalPos();
		m_pDrawerHandleObj[i]->setGlobalPos(handleSetPos);
		m_pDrawerHandleObj[i]->setGlobalRot(m_Rot);
		//�Փˌ��m�N���X���쐬���Ď�������
		m_pDrawerSearch[i] = new CDrawerCollSearch;
		delete m_pDrawerHandleObj[i]->getCollisionSearch();
		m_pDrawerHandleObj[i]->setCollisionSearch(m_pDrawerSearch[i]);

		boneNum += 2;	//���̃{�[���̔z��ɐi�߂�
	}

	

	delete tool;
}

void CDrawerSummary::Update()
{
	LPBoneArray boneArray = m_Drawer.GetBone();
	//�{�[����񂩂�����o����ݒu������W���擾
	if (g_pInput->IsKeyPush(MOFKEY_N))
		if (nb != 0) nb--;
	if (g_pInput->IsKeyPush(MOFKEY_M))
		if (boneArray->GetArrayCount() > nb) nb++;

	for (int i = 0; i < DRAWERNUM; i++)
	{
		m_pDrawerSearch[i]->update();

		if (m_pDrawerSearch[i]->getOnGrasp())
		{
			//�o���u��pose���擾����
			CMatrix44 mat = m_pDrawerBodyObj->getActorMatrix44();

			//�t�s������߂�
			CMatrix44 matInv;
			mat.Inverse(matInv);

			//�t�s���������
			CVector3 dPos = m_pDrawerSearch[i]->getdPos() * matInv;
			CVector3 pos = m_pDrawerSearch[i]->getPos() * matInv;
			CVector3 draPos = m_pDrawerObj[i]->getGlobalPos() * matInv;
			
			//�ړ��ʂ����߂�
			draPos.z += pos.z - dPos.z;
			//�ړ�����
			if (m_DrawerIniPos[i].z <= draPos.z)
				draPos.z = m_DrawerIniPos[i].z;
			if (m_DrawerIniPos[i].z - DRAWERLEN >= draPos.z)
				draPos.z = m_DrawerIniPos[i].z - DRAWERLEN;

			//�s������Ƃɖ߂��ē�����
			draPos *= mat;
			m_pDrawerObj[i]->setMovePos(draPos);
			
		}
	}
}

void CDrawerSummary::RearUpdate()
{
	for (int i = 0; i < DRAWERNUM; i++)
	{
		//�Ђ������𓮂�����Ƃ�����
		if (m_pDrawerSearch[i]->getOnGrasp())
		{
			//�n���h�����Ђ������̈ʒu�ɍ��킹��
			CVector3 handlePos = m_pDrawerObj[i]->getGlobalPos() + m_HandlePos;
			m_pDrawerHandleObj[i]->setGlobalPos(handlePos);
		}
	}
}

void CDrawerSummary::Render()
{
	//�Ђ������{�̕`��
	CMatrix44 matDrawerBody = m_pDrawerBodyObj->getMatrix44(1, CVector3(OBJALLSCALE, OBJALLSCALE, OBJALLSCALE));
	m_DrawerBody.Render(matDrawerBody);

	//�Ђ������`�� && �n���h���`��
	CMatrix44 matDrawer;
	for (int i = 0; i < DRAWERNUM; i++)
	{
		matDrawer = m_pDrawerObj[i]->getActorMatrix44(CVector3(OBJALLSCALE, OBJALLSCALE, OBJALLSCALE));
		m_Drawer.Render(matDrawer);
		matDrawer = m_pDrawerHandleObj[i]->getActorMatrix44(CVector3(OBJALLSCALE, OBJALLSCALE, OBJALLSCALE));
		m_DrawerHandle.Render(matDrawer);
	}
}

void CDrawerSummary::DebugRender()
{

	for (int i = 0; i < DRAWERNUM; i++)
	{
		PmRenderShape::RenderShape(m_pDrawerObj[i], CVector4(1, 0, 0, 0.3f));
		PmRenderShape::RenderShape(m_pDrawerHandleObj[i], CVector4(0, 1, 0, 0.3f));
	}

	////�{�[����񂩂�����o����ݒu������W���擾
	//LPBoneArray boneArray = m_Drawer.GetBone();
	//LPBone bone = boneArray->GetData(nb);
	//CMatrix44 matPos = bone->GetMatrix();
	//CVector3 pos;
	//matPos.GetTranslation(pos);

	//CSphere sp(pos, 0.5f);
	//CGraphicsUtilities::RenderSphere(sp, CVector4(0, 1, 0, 1));
	//CGraphicsUtilities::RenderString(10,70,"%d",nb);
	//CGraphicsUtilities::RenderString(10, 90, "%s", bone->GetName()->GetString());
	//CMatrix44 mat;
	//CMatrix44 handleMat;
	//pos.z -= 1;
	//handleMat.SetTranslation(pos);
	//m_DrawerHandle.Render(handleMat);
	//m_Drawer.Render(mat);

	//CMatrix44 mat;
	//CSphere shape(pos,1);
	//m_Drawer.Render(mat);
	//CGraphicsUtilities::RenderSphere(mat);
}

void CDrawerSummary::Release()
{
	m_DrawerBody.Release();
}
