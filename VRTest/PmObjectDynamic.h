 /////////////////////////////////////
//���I�ȃI�u�W�F�N�g

#pragma once

#include		"PmObject.h"

class PmObjectDynamic : public PmObject
{
private:
	PxRigidDynamic* m_RigidDynamic;

public:

	//Actor���擾����
	PxRigidActor* getActor(){ return m_RigidDynamic; };

	//�A�N�^�[���Z�b�g����
	void setActor(PxRigidDynamic& actor){ m_RigidDynamic = &actor; PmObject::setActor(*m_RigidDynamic); };

	////////////////////////////////////////////////////////////////////////////////
	//���W�Ɗp�x��ς���

	//�w�肵��pos�ɍ��W���ړ�������(�ړ��̍ۂɂق��̃I�u�W�F�N�g�ɉe����^����)
	void setMovePos(CVector3 pos);

	CVector3 getMovePos();

	//�w�肵���p�x�ɉ�]������(�ړ��̍ۂɂق��̃I�u�W�F�N�g�ɉe����^����)
	void setMoveRot(CVector3 rot);
	void setMoveRot(CQuaternion rot);

	CVector3 getMoveRot();

	///////////////////////////////////////////////////
	//���x�֌W

	//���x���擾����
	CVector3 getLinearVec();
	//���x���Z�b�g����
	void setLinearVec(CVector3 vec);
	//�p���x���擾�i��]���x)
	CVector3 getAngularVec();
	//�p���x���Z�b�g�i��]���x�j�@�@�@
	void setAngularVec(CVector3 vec);

	//�͂�������
	//eVELOCITY_CHANGE	�Ȃ񂩂��ꂪ������₷��
	void addForce(CVector3 vec, PxForceMode::Enum mode);

	//�d�͂̉e���̗L���Ftrue�ŉe�����󂯂Ȃ�, false�ŏd�͂̉e�����󂯂�
	//��Ԃ��擾����
	bool getIsDisGravity();
	//��Ԃ��Z�b�g����
	void setIsDisGravity(bool flag){ m_RigidDynamic->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, flag); }

	//
	bool isKinematic();
	//
	void setKinematic(bool flag){ m_RigidDynamic->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, flag); }

	//��]���x�̌�������l0�Ȃ��]���������Ȃ�
	void setAnglarDamping(float ang){ m_RigidDynamic->setAngularDamping(ang); }

	//����(�O�Ȃ疳��)
	void setMass(float mass){ m_RigidDynamic->setMass(mass); }
	float getMass(){ return m_RigidDynamic->getMass(); }

	//���̓�������󂯂銵���̉e���̐ݒ�I�ȁH
	void setMassInertia(CVector3 vec){ m_RigidDynamic->setMassSpaceInertiaTensor(PxVec3(vec.x, vec.y, vec.z)); };

	PmObjectDynamic(){};
	~PmObjectDynamic(){};

};