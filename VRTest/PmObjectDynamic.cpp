

#include		"PmObjectDynamic.h"

////////////////////////////////////////////////////////////////////////////////
//座標と角度を変える

//指定したposに座標を移動させる(移動の際にほかのオブジェクトに影響を与える)
void PmObjectDynamic::setMovePos(CVector3 pos)
{
	PxTransform tra;
	tra = m_RigidDynamic->getGlobalPose();
	//tra.p.x = pos.x;
	//tra.p.y = pos.y;
	//tra.p.z = pos.z;
	//m_RigidDynamic->setKinematicTarget(tra);
	m_RigidDynamic->setKinematicTarget(PxTransform(PxVec3(pos.x, pos.y, pos.z),tra.q));
}

CVector3 PmObjectDynamic::getMovePos()
{
	PxTransform tra;
	m_RigidDynamic->getKinematicTarget(tra);
	CVector3 pv(tra.p.x, tra.p.y, tra.p.z);
	return pv;
}

//指定した角度に回転させる(移動の際にほかのオブジェクトに影響を与える)
void PmObjectDynamic::setMoveRot(CVector3 rot)
{
	PxTransform tra;
	m_RigidDynamic->getKinematicTarget(tra);
	CMatrix44 RotMat;
	RotMat.RotationZYX(rot);
	Quaternion qua;
	RotMat.Transpose();
	RotMat.GetRotation(qua);
	tra.q.x = qua.x;
	tra.q.y = qua.y;
	tra.q.z = qua.z;
	tra.q.w = qua.w;
	m_RigidDynamic->setKinematicTarget(tra);
}
void PmObjectDynamic::setMoveRot(CQuaternion rot)
{
	PxTransform tra;
	m_RigidDynamic->getKinematicTarget(tra);
	//MOFから変換
	Quaternion qua;
	qua.x = rot.x;
	qua.y = rot.y;
	qua.z = rot.z;
	qua.w = rot.w;
	tra.q.x = qua.x;
	tra.q.y = qua.y;
	tra.q.z = qua.z;
	tra.q.w = qua.w;
	m_RigidDynamic->setKinematicTarget(tra);
}

CVector3 PmObjectDynamic::getMoveRot()
{
	PxTransform tra;
	m_RigidDynamic->getKinematicTarget(tra);
	CQuaternion pq(tra.q.x, tra.q.y, tra.q.z, tra.q.w);
	CMatrix33 mat3;
	pq.ConvertMatrix(mat3);
	CVector3 rot;
	mat3.GetRotation(rot);
	return rot;
}

///////////////////////////////////////////////////
//速度関係

//速度を取得する
CVector3 PmObjectDynamic::getLinearVec()
{
	PxVec3 pVec;
	pVec = m_RigidDynamic->getLinearVelocity();
	CVector3 cVec(pVec.x, pVec.y, pVec.z);
	return cVec;
}
//速度をセットする
void PmObjectDynamic::setLinearVec(CVector3 vec)
{
	m_RigidDynamic->setLinearVelocity(PxVec3(vec.x, vec.y, vec.z));
}
//角速度を取得（回転速度)
CVector3 PmObjectDynamic::getAngularVec()
{
	PxVec3 pVec;
	pVec = m_RigidDynamic->getAngularVelocity();
	CVector3 cVec(pVec.x, pVec.y, pVec.z);
	return cVec;
}
//角速度をセット（回転速度）　　　
void PmObjectDynamic::setAngularVec(CVector3 vec)
{
	m_RigidDynamic->setAngularVelocity(PxVec3(vec.x, vec.y, vec.z));
}

//力を加える
void PmObjectDynamic::addForce(CVector3 vec, PxForceMode::Enum mode)
{
	m_RigidDynamic->addForce(PxVec3(vec.x, vec.y, vec.z), mode);
}

//重力の影響の有無：trueで影響を受けない, falseで重力の影響を受ける
//状態を取得する
bool PmObjectDynamic::getIsDisGravity()
{
	if (m_RigidDynamic->getActorFlags() & PxActorFlag::eDISABLE_GRAVITY)
	{
		return true;
	}
	return false;
}

//
bool PmObjectDynamic::isKinematic()
{
	PxRigidBodyFlags flag = m_RigidDynamic->getRigidBodyFlags();
	return (flag & PxRigidBodyFlag::eKINEMATIC) ? true : false;
}