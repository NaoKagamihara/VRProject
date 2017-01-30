
#include		"PmManager.h"


/////////////////////////////////////////////////////////////////////////////////////////////
//PmManager自身に必要な関数

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
	//なんか必要(Fountdation and Physics)
	m_pFoundation = PxCreateFoundation(
		PX_PHYSICS_VERSION,
		m_AllocatorCallback,
		m_ErrorCallback);

	if (!m_pFoundation)
	{
		ERMS("PxCreateFoundation失敗");
	}

	bool recorMemoryAllocations = true;
	/*m_ProfileZoneManager = &PxProfileZoneManager::createProfileZoneManager(m_Foundation);
	if (!m_ProfileZoneManager)
	{
		ERMS("PxProfileZoneManager::createProfileZoneManager失敗");
	}*/
	//これもなんか必要
	m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, PxTolerancesScale(), recorMemoryAllocations, NULL);
	if (!m_pPhysics)
	{
		ERMS("PxCreatePhysics失敗");
	}

	///////////////////////////////////////////////////
	////使用するシーンの設定

	PxSceneDesc sceneDesc(m_pPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);				//重力値設定
	m_pDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = m_pDispatcher;


	//////////////////当たり判定用のフィルターーーーこれ後で作る////////////
	//sceneDesc.filterShader = PxDefaultSimulationFilterShader;	//当たり判定のフィルタ
	////////////////////////////////////////////////////////////////////////

	//多分これで当たり判定のフィルタが出来てるはず
	sceneDesc.filterShader = simpleFilterShader;

	sceneDesc.simulationEventCallback = this;		//トリガーの設定			
	//sceneDesc.flags |= PxSceneFlag::eREQUIRE_RW_LOCK;

	m_pScene = m_pPhysics->createScene(sceneDesc);

	m_pCreate = new PmObjectCreate(m_pPhysics, m_pFoundation, m_pScene, mv_Object);

	m_pScene->setGravity(PxVec3(0.0f, -30.0f, 0.0f));
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
//衝突検知用の関数

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

		//CollisionSearchが有効になっている方だけ呼び出す
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
//相互作用
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

////////////////////////////////////////////////
//当たり判定のフィルター
PxFilterFlags simpleFilterShader(PxFilterObjectAttributes attri0, PxFilterData filData0,
	PxFilterObjectAttributes attri1, PxFilterData filData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	//どちらかがTriggerだった場合(onTrigger)
	if (PxFilterObjectIsTrigger(attri0) || PxFilterObjectIsTrigger(attri1))
	{
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
		return PxFilterFlag::eDEFAULT;
	}

	//(onContact)
	pairFlags = PxPairFlag::eCONTACT_DEFAULT;
	//フィルター
	if ((filData0.word0 & filData1.word1) && (filData1.word0 & filData0.word1))
	{
		//触れてるとき
		pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
	}
	
	//テスト用消して
	//pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;

	return PxFilterFlag::eDEFAULT;

}