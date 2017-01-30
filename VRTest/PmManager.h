//////////////////////////////////////
//Manager一つにつきPhysxのシーンを一つ持つ
//Physxのオブジェクトを管理するクラス
//Physxのオブジェクトの作成
//作成したオブジェクトのトリガーを行う

#pragma once

#include		"PmObject.h"
#include		"PmPreparation.h"
#include		"PmPhysxCreateTool.h"
#include		"Mof.h"

#include		<vector>

using namespace std;

class PmManager : public PxSimulationEventCallback
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


	PmManager();
	~PmManager();

	void Initialize();
	void Update();

	//PmManagerの持っているPmObjectはここでメモリ解放を行う
	void Release();

	//PxPhysicsを渡す
	PxPhysics* getPhysics(){ return m_pPhysics; };

	PmObjectCreate* getObjectCreate(){ return m_pCreate; };

};

//当たり判定のフィルター
PxFilterFlags simpleFilterShader(PxFilterObjectAttributes attri0, PxFilterData filData0,
	PxFilterObjectAttributes attri1, PxFilterData filData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize);