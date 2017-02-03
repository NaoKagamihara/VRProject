
#include		"PmManager.h"


/////////////////////////////////////////////////////////////////////////////////////////////
//PmManager���g�ɕK�v�Ȋ֐�

PmManager::PmManager() : 
	m_AllocatorCallback(),
	m_ErrorCallback(),
	m_pFoundation(NULL),
	m_pProfileZoneManager(NULL),
	m_pPhysics(NULL),
	m_pDispatcher(NULL),
	m_pScene(NULL),
	m_pCreate(NULL)
{

}

PmManager::~PmManager()
{

}

void PmManager::Initialize()
{
	//�Ȃ񂩕K�v(Fountdation and Physics)
	m_pFoundation = PxCreateFoundation(
		PX_PHYSICS_VERSION,
		m_AllocatorCallback,
		m_ErrorCallback);

	if (!m_pFoundation)
	{
		ERMS("PxCreateFoundation���s");
	}

	bool recorMemoryAllocations = true;
	/*m_ProfileZoneManager = &PxProfileZoneManager::createProfileZoneManager(m_Foundation);
	if (!m_ProfileZoneManager)
	{
		ERMS("PxProfileZoneManager::createProfileZoneManager���s");
	}*/
	//������Ȃ񂩕K�v
	m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, PxTolerancesScale(), recorMemoryAllocations, NULL);
	if (!m_pPhysics)
	{
		ERMS("PxCreatePhysics���s");
	}

	///////////////////////////////////////////////////
	////�g�p����V�[���̐ݒ�

	PxSceneDesc sceneDesc(m_pPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);				//�d�͒l�ݒ�
	m_pDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = m_pDispatcher;


	//////////////////�����蔻��p�̃t�B���^�[�[�[�[�����ō��////////////
	//sceneDesc.filterShader = PxDefaultSimulationFilterShader;	//�����蔻��̃t�B���^
	////////////////////////////////////////////////////////////////////////

	//��������œ����蔻��̃t�B���^���o���Ă�͂�
	sceneDesc.filterShader = simpleFilterShader;

	sceneDesc.simulationEventCallback = this;		//�g���K�[�̐ݒ�			
	//sceneDesc.flags |= PxSceneFlag::eREQUIRE_RW_LOCK;

	m_pScene = m_pPhysics->createScene(sceneDesc);

	//�R���g���[���̃f�X�N��n��
	PxCapsuleControllerDesc desc;
	desc.behaviorCallback = this;										//���ݍ�p
	desc.reportCallback = this;	

	m_pCreate = new PmObjectCreate(m_pPhysics, m_pFoundation, m_pScene, mv_Object,m_pConManager, desc);

	m_pScene->setGravity(PxVec3(0.0f, -30.0f, 0.0f));

	//�R���g���[���[�̃}�l�[�W���[���쐬
	m_pConManager = PxCreateControllerManager(*m_pScene);
	if (!m_pConManager)
	{
		ERMS("PxCreateControllerManager");
	}

	m_pConManager->setOverlapRecoveryModule(true);	//�d���h�~�@�\�̃t���O��on��

	//particle�ݒ�
	physx::PxU32 maxParticles = 20000;
	bool perParticleRestOffset = false;

	// create particle system in PhysX SDK
	m_pFluid = m_pPhysics->createParticleFluid(maxParticles, perParticleRestOffset);
	physx::PxReal particleDistance = 0.05;
	m_pFluid->setGridSize(5.0f);
	m_pFluid->setMaxMotionDistance(0.3f);
	m_pFluid->setRestOffset(particleDistance*0.3f);
	m_pFluid->setContactOffset(particleDistance*0.3f * 2);
	m_pFluid->setDamping(0.0f);
	m_pFluid->setRestitution(0.3f);
	m_pFluid->setDynamicFriction(0.001f);
	m_pFluid->setRestParticleDistance(particleDistance);
	m_pFluid->setViscosity(60.0f);
	m_pFluid->setStiffness(45.0f);

	//�ݒ肵��Fluid���V�[���ɒǉ�����
	m_pScene->addActor(*m_pFluid);
}

void PmManager::Update()
{
	m_pScene->simulate(1.0f / 60.0f);
	m_pScene->fetchResults(true);
}

void PmManager::Release()
{		
	m_pCreate->release();
	MOF_SAFE_DELETE(m_pCreate);

	m_pConManager->purgeControllers();
	m_pConManager->release();
	m_pScene->release();
	m_pDispatcher->release();
	m_pPhysics->release();
	m_pFoundation->release();

	for (vector<PmObject*>::iterator itr = mv_Object.begin(); itr != mv_Object.end(); ++itr)
	{
		MOF_SAFE_DELETE(*itr);
	}
}



//
//////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////
// Implements PxSimulationEventCallback
//�Փˌ��m�p�̊֐�

void PmManager::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
	PmObject* obj[2];
	bool flag1 = false;
	bool flag2 = false;
	for (vector<PmObject*>::iterator itr = mv_Object.begin(); itr != mv_Object.end(); ++itr)
	{
		if (flag1 == true && flag2 == true) break;
		if (pairHeader.actors[0] == (*itr)->getActor() && flag1 == false){ obj[0] = (*itr); flag1 = true; }
		if (pairHeader.actors[1] == (*itr)->getActor() && flag2 == false){ obj[1] = (*itr); flag2 = true; }
	}
	if (flag1 == true && flag2 == true)
	{
		if (obj[0]->isOnCollisionSearch() == true) obj[0]->m_CollSearch->onContact(obj[1]);
		if (obj[1]->isOnCollisionSearch() == true) obj[1]->m_CollSearch->onContact(obj[0]);
	}
}

void PmManager::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	PmObject* obj[2];

	for (PxU32 i = 0; i < count; i++)
	{
		bool flag1 = false;
		bool flag2 = false;

		// ignore pairs when shapes have been deleted
		if (pairs[i].flags & (PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER |
			PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
			continue;

		for (vector<PmObject*>::iterator itr = mv_Object.begin(); itr != mv_Object.end(); ++itr)
		{
			//if ((*itr)->isTrigger(1) == false) continue;
			if (flag1 == true && flag2 == true) break;
			if ((pairs[i].otherActor == (*itr)->getActor()) && flag1 == false){ obj[0] = (*itr); flag1 = true; }
			if ((pairs[i].triggerActor == (*itr)->getActor()) && flag2 == false){ obj[1] = (*itr); flag2 = true; }
		}

		//CollisionSearch���L���ɂȂ��Ă���������Ăяo��
		if (flag1 == true && flag2 == true)
		{
			if (obj[0]->isOnCollisionSearch() == true) obj[0]->m_CollSearch->onTrigger(obj[1]);
			if (obj[1]->isOnCollisionSearch() == true) obj[1]->m_CollSearch->onTrigger(obj[0]);
		}
	}
}

//
//////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
// Implements PxSimulationEventCallback
//
//���ݍ�p
PxControllerBehaviorFlags PmManager::getBehaviorFlags(const PxShape& shape, const PxActor& actor)
{
	return PxControllerBehaviorFlag::eCCT_CAN_RIDE_ON_OBJECT;
}

PxControllerBehaviorFlags PmManager::getBehaviorFlags(const PxController& controller)
{
	return PxControllerBehaviorFlags(0);
}

PxControllerBehaviorFlags PmManager::getBehaviorFlags(const PxObstacle& obstacle)
{
	return PxControllerBehaviorFlag::eCCT_CAN_RIDE_ON_OBJECT;
}

//
//////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
// Implements PxUserControllerHitReport
//

void PmManager::onShapeHit(const PxControllerShapeHit& hit)
{
	defaultCCTInteraction(hit);
}

//manager���Ŏg�p����L�����N�^�ƃI�u�W�F�N�g�̑��ݍ�p
void PmManager::defaultCCTInteraction(const PxControllerShapeHit& hit)
{
	PxRigidDynamic* actor = hit.shape->getActor()->is<PxRigidDynamic>();
	if (actor)
	{
		if (actor->getRigidBodyFlags() & PxRigidBodyFlag::eKINEMATIC)
			return;

		if (0)
		{
			const PxVec3 p = actor->getGlobalPose().p + hit.dir * 10.0f;

			PxShape* shape;
			actor->getShapes(&shape, 1);
			PxRaycastHit newHit;
			PxU32 n = PxShapeExt::raycast(*shape, *shape->getActor(), p, -hit.dir, 20.0f, PxHitFlag::ePOSITION, 1, &newHit, false);
			if (n)
			{
				// We only allow horizontal pushes. Vertical pushes when we stand on dynamic objects creates
				// useless stress on the solver. It would be possible to enable/disable vertical pushes on
				// particular objects, if the gameplay requires it.
				const PxVec3 upVector = hit.controller->getUpDirection();
				const PxF32 dp = hit.dir.dot(upVector);
				//		shdfnd::printFormatted("%f\n", fabsf(dp));
				if (fabsf(dp)<1e-3f)
					//		if(hit.dir.y==0.0f)
				{
					const PxTransform globalPose = actor->getGlobalPose();
					const PxVec3 localPos = globalPose.transformInv(newHit.position);
					addForceAtLocalPos(*actor, hit.dir*hit.length*1000.0f, localPos, PxForceMode::eACCELERATION);
				}
			}
		}

		PxShape* shape;
		actor->getShapes(&shape, 1);
		PxFilterData fData = shape->getSimulationFilterData();
		//m_TagInteraction = PmTag::TEST;
		if (fData.word0 & m_TagInteraction) return;

		// We only allow horizontal pushes. Vertical pushes when we stand on dynamic objects creates
		// useless stress on the solver. It would be possible to enable/disable vertical pushes on
		// particular objects, if the gameplay requires it.
		const PxVec3 upVector = hit.controller->getUpDirection();
		const PxF32 dp = hit.dir.dot(upVector);
		//		shdfnd::printFormatted("%f\n", fabsf(dp));
		if (fabsf(dp)<1e-3f)
			//		if(hit.dir.y==0.0f)
		{
			float force = 1000.0f * (1 - actor->getMass());

			const PxTransform globalPose = actor->getGlobalPose();
			const PxVec3 localPos = globalPose.transformInv(toVec3(hit.worldPos));
			//addForceAtLocalPos(*actor, hit.dir*hit.length*1000.0f, localPos, PxForceMode::eACCELERATION);
			addForceAtLocalPos(*actor, (hit.dir*hit.length*1000.0f) * 0.05f, localPos, PxForceMode::eACCELERATION);

		}
	}
}

void PmManager::addForceAtLocalPos(PxRigidBody& body, const PxVec3& force, const PxVec3& pos, PxForceMode::Enum mode, bool wakeup)
{
	//transform pos to world space
	const PxVec3 globalForcePos = body.getGlobalPose().transform(pos);

	addForceAtPosInternal(body, force, globalForcePos, mode, wakeup);
}

void PmManager::addForceAtPosInternal(PxRigidBody& body, const PxVec3& force, const PxVec3& pos, PxForceMode::Enum mode, bool wakeup)
{
	/*	if(mode == PxForceMode::eACCELERATION || mode == PxForceMode::eVELOCITY_CHANGE)
	{
	Ps::getFoundation().error(PxErrorCode::eINVALID_PARAMETER, __FILE__, __LINE__,
	"PxRigidBodyExt::addForce methods do not support eACCELERATION or eVELOCITY_CHANGE modes");
	return;
	}*/

	const PxTransform globalPose = body.getGlobalPose();
	const PxVec3 centerOfMass = globalPose.transform(body.getCMassLocalPose().p);

	const PxVec3 torque = (pos - centerOfMass).cross(force);
	body.addForce(force, mode, wakeup);
	body.addTorque(torque, mode, wakeup);
}


////////////////////////////////////////////////
//�����蔻��̃t�B���^�[
PxFilterFlags simpleFilterShader(PxFilterObjectAttributes attri0, PxFilterData filData0,
	PxFilterObjectAttributes attri1, PxFilterData filData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	//�ǂ��炩��Trigger�������ꍇ(onTrigger)
	if (PxFilterObjectIsTrigger(attri0) || PxFilterObjectIsTrigger(attri1))
	{
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
		return PxFilterFlag::eDEFAULT;
	}

	//(onContact)
	pairFlags = PxPairFlag::eCONTACT_DEFAULT;
	//�t�B���^�[
	if ((filData0.word0 & filData1.word1) && (filData1.word0 & filData0.word1))
	{
		//�G��Ă�Ƃ�
		pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
	}
	
	//�e�X�g�p������
	//pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;

	return PxFilterFlag::eDEFAULT;

}

