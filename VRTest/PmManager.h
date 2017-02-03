//////////////////////////////////////
//Manager一つにつきPhysxのシーンを一つ持つ
//Physxのオブジェクトを管理するクラス
//Physxのオブジェクトの作成
//作成したオブジェクトのトリガーを行う

#pragma once

#include		"PmObject.h"
#include		"PmPreparation.h"
#include		"PmPhysxCreateTool.h"
#include		"particles\PxParticleFluid.h"
#include		"Mof.h"

#include		<vector>

using namespace std;

class PmManager : public PxSimulationEventCallback, 
					public PxControllerBehaviorCallback,
					public PxUserControllerHitReport
{

private:

	//Physxを使用するのに必要な

	PxDefaultAllocator				m_AllocatorCallback;		//メモリ管理のやつ
	PxDefaultErrorCallback			m_ErrorCallback;			//エラー報告のやつ

	PxFoundation*					m_pFoundation;

	PxProfileZoneManager*			m_pProfileZoneManager;
	PxPhysics*						m_pPhysics;

	PxDefaultCpuDispatcher*			m_pDispatcher;

	PxScene*						m_pScene;

	PxControllerManager*			m_pConManager;				//キャラクタコントローラ
	PxU32							m_TagInteraction;			//キャラクタとの相互作用を定義する用の変数

	PxParticleFluid*				m_pFluid;					//流体管理クラス

	///////////////////////////////////////////
	//Physxに持たせてあるオブジェクトを管理するやつ
	vector<PmObject*>				mv_Object;

	PmObjectCreate*					m_pCreate;


public:



	////////////////////////////////////////////////////////////////////////////////
	//
	// Implements PxSimulationEventCallback
	//
	//衝突検知用の関数

	virtual void							onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs);
	virtual void							onTrigger(PxTriggerPair* pairs, PxU32 count);
	virtual void							onConstraintBreak(PxConstraintInfo* infos, PxU32 count){};
	virtual void							onWake(PxActor**, PxU32){};
	virtual void							onSleep(PxActor**, PxU32){};

	////////////////////////////////////////////////////////////////////////////////
	//
	// Implements PxSimulationEventCallback
	//
	//相互作用
	virtual PxControllerBehaviorFlags		getBehaviorFlags(const PxShape& shape, const PxActor& actor);
	virtual PxControllerBehaviorFlags		getBehaviorFlags(const PxController& controller);
	virtual PxControllerBehaviorFlags		getBehaviorFlags(const PxObstacle& obstacle);

	////////////////////////////////////////////////////////////////////////////////
	//
	// Implements PxUserControllerHitReport
	//
	virtual void							onShapeHit(const PxControllerShapeHit& hit);
	virtual void							onControllerHit(const PxControllersHit& hit)		{};
	virtual void							onObstacleHit(const PxControllerObstacleHit& hit)	{};

	/////////////////////////////////////////////////////////////////////////
	//キャラクタの相互作用関係
	void defaultCCTInteraction(const PxControllerShapeHit& hit);

	void addForceAtLocalPos(PxRigidBody& body, const PxVec3& force, const PxVec3& pos, PxForceMode::Enum mode, bool wakeup = true);

	void addForceAtPosInternal(PxRigidBody& body, const PxVec3& force, const PxVec3& pos, PxForceMode::Enum mode, bool wakeup);

	PmManager();
	~PmManager();

	void Initialize();
	void Update();

	//PmManagerの持っているPmObjectはここでメモリ解放を行う
	void Release();

	//PxPhysicsを渡す
	PxPhysics* getPhysics(){ return m_pPhysics; };

	//PmObjectを渡す
	PmObjectCreate* getObjectCreate(){ return m_pCreate; };

	//PxParticleFluidを渡す
	PxParticleFluid* getFluid(){ return m_pFluid; };

};

//当たり判定のフィルター
PxFilterFlags simpleFilterShader(PxFilterObjectAttributes attri0, PxFilterData filData0,
	PxFilterObjectAttributes attri1, PxFilterData filData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize);