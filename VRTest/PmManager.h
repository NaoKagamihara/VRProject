//////////////////////////////////////
//Manager��ɂ�Physx�̃V�[���������
//Physx�̃I�u�W�F�N�g���Ǘ�����N���X
//Physx�̃I�u�W�F�N�g�̍쐬
//�쐬�����I�u�W�F�N�g�̃g���K�[���s��

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

	//Physx���g�p����̂ɕK�v��

	PxDefaultAllocator				m_AllocatorCallback;		//�������Ǘ��̂��
	PxDefaultErrorCallback			m_ErrorCallback;			//�G���[�񍐂̂��

	PxFoundation*					m_pFoundation;

	PxProfileZoneManager*			m_pProfileZoneManager;
	PxPhysics*						m_pPhysics;

	PxDefaultCpuDispatcher*			m_pDispatcher;

	PxScene*						m_pScene;

	///////////////////////////////////////////
	//Physx�Ɏ������Ă���I�u�W�F�N�g���Ǘ�������
	vector<PmObject*>				mv_Object;

	PmObjectCreate*					m_pCreate;


public:



	////////////////////////////////////////////////////////////////////////////////
	//
	// Implements PxSimulationEventCallback
	//
	//�Փˌ��m�p�̊֐�

	virtual void							onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs);
	virtual void							onTrigger(PxTriggerPair* pairs, PxU32 count);
	virtual void							onConstraintBreak(PxConstraintInfo* infos, PxU32 count){};
	virtual void							onWake(PxActor**, PxU32){};
	virtual void							onSleep(PxActor**, PxU32){};

	////////////////////////////////////////////////////////////////////////////////
	//
	// Implements PxSimulationEventCallback
	//
	//���ݍ�p
	virtual PxControllerBehaviorFlags		getBehaviorFlags(const PxShape& shape, const PxActor& actor);
	virtual PxControllerBehaviorFlags		getBehaviorFlags(const PxController& controller);
	virtual PxControllerBehaviorFlags		getBehaviorFlags(const PxObstacle& obstacle);


	PmManager();
	~PmManager();

	void Initialize();
	void Update();

	//PmManager�̎����Ă���PmObject�͂����Ń�����������s��
	void Release();

	//PxPhysics��n��
	PxPhysics* getPhysics(){ return m_pPhysics; };

	PmObjectCreate* getObjectCreate(){ return m_pCreate; };

};

//�����蔻��̃t�B���^�[
PxFilterFlags simpleFilterShader(PxFilterObjectAttributes attri0, PxFilterData filData0,
	PxFilterObjectAttributes attri1, PxFilterData filData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize);