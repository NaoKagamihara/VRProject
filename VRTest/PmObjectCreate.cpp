/////////////////////////////////
//PmObject���쐬���邽�߂̃N���X

#include	"PmObjectCreate.h"



PmObjectCreate::PmObjectCreate(PxPhysics* physics, PxFoundation* foundation, PxScene* scene, vector<PmObject*>& obj, PxControllerManager* m_ConManager, PxCapsuleControllerDesc conDesc)
{
	m_pPhysics = physics;
	m_pScene = scene;
	mv_Object = &obj;
	m_pConManager = m_ConManager;
	m_ConDesc = conDesc;

	m_pCooking = PxCreateCooking(PX_PHYSICS_VERSION, *foundation, PxCookingParams(PxTolerancesScale()));
	if (!m_pCooking)
	{
		ERMS("PxCreateCooking���s");
	}
}

PmObjectCreate::~PmObjectCreate()
{

}

void PmObjectCreate::release()
{
	m_pCooking->release();
}

//Physics�֌W�̃C���X�^���X���쐬����
PmPhysxCreateTool* PmObjectCreate::createPhysxCreateTool(){ return new PmPhysxCreateTool(m_pPhysics, m_pCooking); };

//PmObject���}�l�[�W���[�ƃV�[���̒�����폜����
//�����œn�����I�u�W�F�N�g���폜����
void PmObjectCreate::detachObject(PmObject* obj)
{
	for (vector<PmObject*>::iterator itr = mv_Object->begin(); itr != mv_Object->end(); ++itr)
	{
		if ((*itr) == obj)
		{
			m_pScene->removeActor(*obj->getActor());
			mv_Object->erase(itr);
			break;
		}
	}
}

//Plane���쐬����
void PmObjectCreate::createPlane(CVector3 vec, PxMaterial* mate)
{
	PxRigidStatic* body = PxCreatePlane(*m_pPhysics, PxPlane(PxPlane(PxVec3(vec.x, vec.y, vec.z), PxVec3(0.0, 1.0, 0.0))), *mate);
	m_pScene->addActor(*body);
}

//Geometry�����PmObject���쐬����
PmObject* PmObjectCreate::createEmpObject(CVector3 pos, PxMaterial* material, bool dynamic)
{
	PmObject* Object;

	//�V�[���ɒǉ�������ԂŃA�N�^�[�쐬
	Object = createObject(pos, dynamic);

	PxRigidActor* actor = Object->getActor();										//PmObject�̎���Actor�̍쐬

	return Object;
}

//PmObject��BoxGeometry��ǉ����쐬����
PmObject* PmObjectCreate::createBoxObject(CVector3 pos, PxMaterial* material, MofFloat x, MofFloat y, MofFloat z, bool dynamic)
{
	PmObject* Object;

	//�V�[���ɒǉ�������ԂŃA�N�^�[�쐬
	Object = createObject(pos, dynamic);

	PxRigidActor* actor = Object->getActor();										//PmObject�̎���Actor�̍쐬
	actor->createShape(PxBoxGeometry(x, y, z), *material, PxTransform(0, 0, 0));	//Actor��Shape����������

	return Object;
}

//PmObject��SphereGeometry��ǉ����쐬����
PmObject* PmObjectCreate::createSphereObject(CVector3 pos, PxMaterial* material, MofFloat r, bool dynamic)
{
	PmObject* Object;

	//�V�[���ɒǉ�������ԂŃA�N�^�[�쐬
	Object = createObject(pos, dynamic);

	PxRigidActor* actor = Object->getActor();										//PmObject�̎���Actor�̍쐬
	actor->createShape(PxSphereGeometry(r), *material, PxTransform(0, 0, 0));		//Actor��Shape����������

	return Object;
}

//PmObject��CapsuleGeometry��ǉ����쐬����
PmObject* PmObjectCreate::createCapsuleObject(CVector3 pos, PxMaterial* material, MofFloat r, MofFloat h, bool dynamic)
{
	PmObject* Object;

	//�V�[���ɒǉ�������ԂŃA�N�^�[�쐬
	Object = createObject(pos, dynamic);

	PxRigidActor* actor = Object->getActor();										//PmObject�̎���Actor�̍쐬
	actor->createShape(PxCapsuleGeometry(r, h), *material, PxTransform(0, 0, 0));	//Actor��Shape����������

	return Object;
}

//PmObject��ConvexGeometry��ǉ����쐬����
PmObject* PmObjectCreate::createConvexObject(CVector3 pos, PxMaterial* material, CMeshContainer mesh, bool dynamic, CVector3 scale)
{
	PmObject* Object;

	//�V�[���ɒǉ�������ԂŃA�N�^�[�쐬
	Object = createObject(pos, dynamic);

	PxRigidActor* actor = Object->getActor();										//PmObject�̎���Actor�̍쐬

	//���b�V������W�I���g���擾
	LPGeometry Geo = mesh.GetGeometry(0);

	//�C���f�b�N�X�o�b�t�@�H
	LPIndexBuffer ib = Geo->GetIndexBuffer();

	//�g�p���郂�f���̏������Ă������
	PxConvexMeshDesc meshDesc;

	//�t���O?????????????????????????
	//	meshDesc.flags = 0;
	//if (ib->GetIndexU16())
	//	meshDesc.flags = PxConvexFlag::e16_BIT_INDICES;

	//���_�̐��𒲐����Ă����t���O
	meshDesc.flags |= PxConvexFlag::eCOMPUTE_CONVEX;

	///////////////////////////////////////////////////////////
	//���_�f�[�^�̍쐬
	//���_
	LPVertexBuffer vb = Geo->GetVertexBuffer();

	LPMofU32	m_pNind;
	float*		m_pPvx;

	//���_�̐�
	meshDesc.points.count = vb->GetVertexCount();
	//�P���_�̃T�C�Y(�o�C�g��)
	meshDesc.points.stride = vb->GetVertexSize();
	//���_���
	m_pPvx = (float*)malloc(meshDesc.points.stride * meshDesc.points.count);
	memcpy(m_pPvx, vb->GetVertex(), meshDesc.points.stride * meshDesc.points.count);
	char* pn = (char*)m_pPvx;
	for (MofU32 i = 0; i < vb->GetVertexCount(); i++)
	{
		Vector3* pvert = (Vector3*)pn;
		(*pvert) *= Geo->GetMatrix();
		pn += vb->GetVertexSize();
	}
	meshDesc.points.data = m_pPvx;

	///////////////////////////////////////////////////////////
	//triangle

	meshDesc.triangles.count = Geo->GetIndexBuffer()->GetIndexCount() / 3;	//�O�p�̐�
	meshDesc.triangles.stride = sizeof(MofU32) * 3;							//�T�C�Y

	//�C���f�b�N�X�̎�ނŕ���
	if (ib->GetIndexU16())
	{
		LPMofU16 pind = Geo->GetIndexBuffer()->GetIndexU16();
		m_pNind = new MofU32[Geo->GetIndexBuffer()->GetIndexCount()];		//���_�f�[�^�Ԃ�̈���m��
		for (MofU32 i = 0; i < Geo->GetIndexBuffer()->GetIndexCount(); i++)	//���_�f�[�^��MofU32�ɕϊ�
		{
			m_pNind[i] = *pind;	pind++;
		}
		meshDesc.triangles.data = m_pNind;
	}
	else
	{
		LPMofU32 pind = Geo->GetIndexBuffer()->GetIndexU32();
		m_pNind = new MofU32[Geo->GetIndexBuffer()->GetIndexCount()];
		memcpy(m_pNind, pind, sizeof(MofU32) * Geo->GetIndexBuffer()->GetIndexCount());
		meshDesc.triangles.data = m_pNind;
	}

	//�N�b�L���O
	PxDefaultMemoryOutputStream writeBuffer;
	//�N���X�I�u�W�F�N�g���V���A��������
	bool status = m_pCooking->cookConvexMesh(meshDesc, writeBuffer);
	if (!status)
	{
		ERMS("Cooking_errer");
	}

	PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());

	//�V���A���������f�[�^����ConvexMesh���쐬����
	PxConvexMesh* Mesh = m_pPhysics->createConvexMesh(readBuffer);
	PxQuat quat(0.0f, PxVec3(0, 0, 0));
	PxVec3 vec(scale.x, scale.y, scale.z);
	PxMeshScale sca(vec, quat);
	actor->createShape(PxConvexMeshGeometry(Mesh, sca), *material, PxTransform(0, 0, 0));

	MOF_SAFE_DELETE(m_pPvx);
	MOF_SAFE_DELETE(m_pNind);

	return Object;
}

PmObject* PmObjectCreate::createTriangleObject(CVector3 pos, PxMaterial* material, CMeshContainer mesh, CVector3 scale)
{
	PmObject* Object;

	//�V�[���ɒǉ�������ԂŃA�N�^�[�쐬
	Object = createObject(pos, false);

	PxRigidActor* actor = Object->getActor();										//PmObject�̎���Actor�̍쐬

	//���b�V������W�I���g���擾
	LPGeometry Geo = mesh.GetGeometry(0);

	//�C���f�b�N�X�o�b�t�@�H
	LPIndexBuffer ib = Geo->GetIndexBuffer();

	//�g�p���郂�f���̏������Ă������
	PxTriangleMeshDesc meshDesc;

	///////////////////////////////////////////////////////////
	//���_�f�[�^�̍쐬
	//���_
	LPVertexBuffer vb = Geo->GetVertexBuffer();

	LPMofU32	m_pNind;
	float*		m_pPvx;

	//���_�̐�
	meshDesc.points.count = vb->GetVertexCount();
	//�P���_�̃T�C�Y(�o�C�g��)
	meshDesc.points.stride = vb->GetVertexSize();
	//���_���
	m_pPvx = (float*)malloc(meshDesc.points.stride * meshDesc.points.count);
	memcpy(m_pPvx, vb->GetVertex(), meshDesc.points.stride * meshDesc.points.count);
	char* pn = (char*)m_pPvx;
	for (MofU32 i = 0; i < vb->GetVertexCount(); i++)
	{
		Vector3* pvert = (Vector3*)pn;
		(*pvert) *= Geo->GetMatrix();
		pn += vb->GetVertexSize();
	}
	meshDesc.points.data = m_pPvx;

	///////////////////////////////////////////////////////////
	//triangle

	meshDesc.triangles.count = Geo->GetIndexBuffer()->GetIndexCount() / 3;	//�O�p�̐�
	meshDesc.triangles.stride = sizeof(MofU32) * 3;						//�T�C�Y

	//�C���f�b�N�X�̎�ނŕ���
	if (ib->GetIndexU16())
	{
		LPMofU16 pind = Geo->GetIndexBuffer()->GetIndexU16();
		m_pNind = new MofU32[Geo->GetIndexBuffer()->GetIndexCount()];		//���_�f�[�^�Ԃ�̈���m��
		for (MofU32 i = 0; i < Geo->GetIndexBuffer()->GetIndexCount(); i++)	//���_�f�[�^��MofU32�ɕϊ�
		{
			m_pNind[i] = *pind;	pind++;
		}
		meshDesc.triangles.data = m_pNind;
	}
	else
	{
		LPMofU32 pind = Geo->GetIndexBuffer()->GetIndexU32();
		m_pNind = new MofU32[Geo->GetIndexBuffer()->GetIndexCount()];
		memcpy(m_pNind, pind, sizeof(MofU32) * Geo->GetIndexBuffer()->GetIndexCount());
		meshDesc.triangles.data = m_pNind;
	}

	//�N�b�L���O
	PxDefaultMemoryOutputStream writeBuffer;
	//�N���X�I�u�W�F�N�g���V���A��������
	bool status = m_pCooking->cookTriangleMesh(meshDesc, writeBuffer);
	if (!status)
	{
		ERMS("Cooking_errer");
	}

	PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());

	PxTriangleMesh* Mesh = m_pPhysics->createTriangleMesh(readBuffer);
	PxQuat quat(0.0f, PxVec3(0, 0, 0));
	PxVec3 vec(scale.x, scale.y, scale.z);
	PxMeshScale sca(vec, quat);
	PxTriangleMeshGeometry tgeom(Mesh, sca);
	actor->createShape(tgeom, *material, PxTransform(0, 0, 0));


	MOF_SAFE_DELETE(m_pPvx);
	MOF_SAFE_DELETE(m_pNind);

	return Object;
}

//PmObject���쐬����
//pos		:���W
//dynamic	:�����I�u�W�F�N�g���ǂ���(false:�����Ȃ�, true:����)
PmObject* PmObjectCreate::createObject(CVector3 pos, bool dynamic)
{
	//static�A�N�^�[�̍쐬
	if (dynamic == false)
	{
		PxRigidStatic* Body = m_pPhysics->createRigidStatic(PxTransform(pos.x, pos.y, pos.z));
		PmObject* obj = new PmObject();
		//�Ԃ�PmObject�ƃV�[���ɒǉ�����
		m_pScene->addActor(*Body);		//�V�[���ɃA�N�^�[��ǉ�����
		obj->setActor(*Body);
		mv_Object->push_back(obj);
		return obj;
	}

	//dynamic�A�N�^�[�̍쐬
	else if (dynamic == true)
	{
		PxRigidDynamic* Body = m_pPhysics->createRigidDynamic(PxTransform(pos.x, pos.y, pos.z));
		PmObjectDynamic* objDyn = new PmObjectDynamic();
		//�Ԃ�PmObject�ƃV�[���ɒǉ�����
		m_pScene->addActor(*Body);
		objDyn->setActor(*Body);
		mv_Object->push_back(objDyn);
		return objDyn;
	}

	return NULL;
}

//PmObjectCharacter���쐬����
PmObjectCharacter* PmObjectCreate::createCharacterCapsuleObject(CVector3 pos, float r, float h, PxMaterial* material,
	float slopeLimit,
	float contactOffset,
	float stepOffset)
{
	
	m_ConDesc.material = material;
	m_ConDesc.position = PxExtendedVec3(pos.x, pos.y, pos.z);				//���W
	m_ConDesc.height = h;													//�����i���S����Y����+��-�̕����ɉ��т钷���j
	m_ConDesc.radius = r;													//���a
	m_ConDesc.slopeLimit = slopeLimit;										//�o���p�x����
	m_ConDesc.contactOffset = contactOffset;								//�Փː��x

	PmObjectCapsuleCharacter* character = new PmObjectCapsuleCharacter(m_pConManager->createController(m_ConDesc));

	character->setHeight(h);
	character->setRadius(r);

	return character;
}

//CItemCreate
void PmObjectCreate::createCItemObject(PmObjectDynamic* obj)
{
	PxRigidDynamic* Body = m_pPhysics->createRigidDynamic(PxTransform(0,0,0));
	//�Ԃ�PmObject�ƃV�[���ɒǉ�����
	m_pScene->addActor(*Body);
	obj->setActor(*Body);
	mv_Object->push_back(obj);
	obj->changeObjectTag(EPmObjectTag::OBJ_ITEM,true);
}