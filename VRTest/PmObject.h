//////////////////////////
//Physx���g�p���邽�߂̃N���X
//PmCollisionSearch�͉��������ĂȂ��i�����Ȃ��Ɓj
//

#pragma once

#include		"PmPreparation.h"
#include		"Mof.h"
#include		"PmCollisionSearch.h"
#include		"PmObjectTags.h"

class PmCollisionSearch;

class PmObject
{
private:
	
	PxRigidActor*		m_RigidBody;
	bool				m_bCollSearch;
	DWORD				m_eObjTag;

public:
	
	//�����蔻�莞�ɌĂяo���p�̃N���X
	PmCollisionSearch* m_CollSearch;

	//Actor���擾����
	PxRigidActor* getActor(){ return m_RigidBody; };

	//�A�N�^�[���Z�b�g����
	void setActor(PxRigidActor& actor){ m_RigidBody = &actor; };

	//pos�̏ꏊ�ɍ��W���Z�b�g����
	void setGlobalPos(CVector3 pos);

	//���W���擾����
	CVector3 getGlobalPos();

	//rot�̊p�x�ɃZ�b�g����
	void setGlobalRot(CVector3 rot);
	void setGlobalRot(const CQuaternion& rot);

	//�p�x���擾����
	CVector3 getGlobalVecRot();
	CQuaternion getGlobalQuoRot();

	//m_RigidBody��Actor��Matrix44���擾����
	Matrix44 getActorMatrix44(CVector3 scale = CVector3(1.0f, 1.0f, 1.0f));

	//m_RigidBody��nbShape�Ԗڂ̃}�g���b�N�X���擾����
	Matrix44 getMatrix44(int nbShape, CVector3 scale = CVector3(1.0f,1.0f,1.0f));

	//m_RigidBody��Shape�̐����擾����
	MofU32 getNbShapes(){ return m_RigidBody->getNbShapes(); };

	//m_RigidBody��nbShape�Ԗڂ�shape���擾����
	PxShape* getShape(int nbShape);

	//m_RigidBody��shape��ǉ�����
	void addShape(PxShape& shape){ m_RigidBody->attachShape(shape); };

	//m_RigidBody��PxSpahe��؂藣��
	void detachShape(PxShape* shape){ m_RigidBody->detachShape(*shape); };

	//m_RigidBody��nbShape�Ԗڂ̌`�̎�ނ��擾����
	PxGeometryType::Enum getGeometryType(int nbShape);

	//m_RigidBody��nbShape�Ԗڂ����̃I�u�W�F�N�g�ƏՓ˂���̂����ׂ�
	bool isCollision(int nbShape);

	//m_RigidBody��nbShape�Ԗڂ̏Փ˂���̂��t���O��ݒ肷��:true�Ȃ�Փ�
	void setCollision(int nbShape, bool flag);

	//m_RigidBody��Trigger�p�̃I�u�W�F�N�g���擾����
	//�f�t�H���g��false
	bool isTrigger(int nbShape);

	//m_RigidBody��Trigger�p�̃I�u�W�F�N�g�ɂ���
	void setTrigger(int nbShape, bool flag);
	
	//m_RigidBody�̃^�C�v���擾����::Static��Dynamic
	PxActorType::Enum getRigidBodyType(){ return m_RigidBody->getType(); };

	//CollisionSearch���g�p���邩�ǂ������擾����
	bool isOnCollisionSearch(){ return m_bCollSearch; };

	//CollisionSearch���g�p���邩�ǂ������Z�b�g����
	void setOnCollisionSearch(bool flag){ m_bCollSearch = flag; };

	//m_CollisionSearch���擾����
	PmCollisionSearch* getCollisionSearch(){ return m_CollSearch; }

	//m_CollisionSearch���Z�b�g����
	void setCollisionSearch(PmCollisionSearch* search){ m_CollSearch = search; }

	//m_eObjTag���擾����
	DWORD getObjectTag(){ return m_eObjTag; };
	
	//m_eObjTag���Z�b�g����
	void setObjectTag(DWORD tag){ m_eObjTag = tag; };

	//m_eObjeTag���`�F�b�N����
	int checkObjectTag(EPmObjectTag tag){ return m_eObjTag & tag; };

	//m_eObjeTag�̃t���O��؂�ւ���
	void changeObjectTag(EPmObjectTag tag, bool flag){ flag ? m_eObjTag |= tag : m_eObjTag &= ~tag; };

	//m_CollisionSearch�̃��������J������
	void releaseCollisionSearch(){ delete m_CollSearch; }

	//PmObject�̃��������J������
	void release();

	////////////////////////////////////////
	//��肠�����K�v�Ȋ֐��͂��ꂭ�炢

	PmObject();

	virtual ~PmObject(){ this->release(); };
	
};