 /////////////////////////////////////
//動的なオブジェクト

#pragma once

#include		"PmObject.h"

class PmObjectDynamic : public PmObject
{
private:
	PxRigidDynamic* m_RigidDynamic;

public:

	//Actorを取得する
	PxRigidActor* getActor(){ return m_RigidDynamic; };

	//アクターをセットする
	void setActor(PxRigidDynamic& actor){ m_RigidDynamic = &actor; PmObject::setActor(*m_RigidDynamic); };

	////////////////////////////////////////////////////////////////////////////////
	//座標と角度を変える

	//指定したposに座標を移動させる(移動の際にほかのオブジェクトに影響を与える)
	void setMovePos(CVector3 pos);

	CVector3 getMovePos();

	//指定した角度に回転させる(移動の際にほかのオブジェクトに影響を与える)
	void setMoveRot(CVector3 rot);
	void setMoveRot(CQuaternion rot);

	CVector3 getMoveRot();

	///////////////////////////////////////////////////
	//速度関係

	//速度を取得する
	CVector3 getLinearVec();
	//速度をセットする
	void setLinearVec(CVector3 vec);
	//角速度を取得（回転速度)
	CVector3 getAngularVec();
	//角速度をセット（回転速度）　　　
	void setAngularVec(CVector3 vec);

	//力を加える
	//eVELOCITY_CHANGE	なんかこれが分かりやすい
	void addForce(CVector3 vec, PxForceMode::Enum mode);

	//重力の影響の有無：trueで影響を受けない, falseで重力の影響を受ける
	//状態を取得する
	bool getIsDisGravity();
	//状態をセットする
	void setIsDisGravity(bool flag){ m_RigidDynamic->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, flag); }

	//
	bool isKinematic();
	//
	void setKinematic(bool flag){ m_RigidDynamic->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, flag); }

	//回転速度の減少する値0なら回転が減少しない
	void setAnglarDamping(float ang){ m_RigidDynamic->setAngularDamping(ang); }

	//質量(０なら無限)
	void setMass(float mass){ m_RigidDynamic->setMass(mass); }
	float getMass(){ return m_RigidDynamic->getMass(); }

	//他の動きから受ける慣性の影響の設定的な？
	void setMassInertia(CVector3 vec){ m_RigidDynamic->setMassSpaceInertiaTensor(PxVec3(vec.x, vec.y, vec.z)); };

	PmObjectDynamic(){};
	~PmObjectDynamic(){};

};