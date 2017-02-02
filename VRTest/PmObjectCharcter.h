  /////////////////////////////////////////
//�L�����N�^�R���g���[���[

#pragma once

#include		"PmObjectTags.h"

#include		"PmPreparation.h"
#include		"Mof.h"
#include		<time.h>

class PmObjectCharacter
{
protected:
	PxController* m_Controller;
	clock_t m_time;
	PxControllerFilters m_filter;

	float m_MinDist;
	float m_Height;

public:

	//�L�����N�^���ړ�������,�߂�l�̃t���O�ŃL�����̏�Ԃ��擾
	PxControllerCollisionFlags move(CVector3 vec);

	//���W���擾����
	CVector3 getPosition()
	{
		PxExtendedVec3 vec = m_Controller->getPosition();
		return CVector3(vec.x, vec.y, vec.z);
	}

	//���W���Z�b�g����
	void setPosition(CVector3 vec){ m_Controller->setPosition(PxExtendedVec3(vec.x, vec.y, vec.z)); };

	//�ړ��̍ۂ̍ċA�I�ȌĂяo���̍ŏ����̒�����
	float getMinDist(){ return m_MinDist; };

	//�ړ��̍ۂ̍ċA�I�ȌĂяo���̍ŏ����̒����̃Z�b�g�idefault��0
	void setMinDist(float minDist){ m_MinDist = minDist; };

	//�L�����N�^�̍�����ύX����
	void resize(float height){ m_Controller->resize(height); m_Height = height; };

	//�J�v�Z���̍������擾����
	float getHeight(){ return m_Height; };

	//�J�v�Z���̍������Z�b�g����
	void setHeight(float height){ m_Height = height; };

	//�����̍��W���擾����
	CVector3 getFootPosition()
	{ 
		PxExtendedVec3 vec = m_Controller->getFootPosition();
		return CVector3(vec.x, vec.y, vec.z);
	}

	//�L�����N�^�̏�����̃x�N�g�����擾����
	CVector3 getUpDirection()
	{
		PxVec3 vec = m_Controller->getUpDirection();
		return CVector3(vec.x, vec.y, vec.z);
	}

	//�L�����N�^�̏�����̃x�N�g�����Z�b�g����
	void setUpDirection(CVector3 vec)
	{
		PxVec3 upVec(vec.x, vec.y, vec.z);
		m_Controller->setUpDirection(upVec);
	}

	//�R���g���[���[�̎��A�N�^�[���擾����
	PxRigidDynamic* getActor(){ return m_Controller->getActor(); };

	//�Փː��x���擾����
	MofU32 getContactOffset(){ return m_Controller->getContactOffset(); };

	//�Փː��x���Z�b�g����
	void setContactOffset(float con){ m_Controller->setContactOffset(con); };

	//������n�ʂ̊p�x���擾����
	MofU32 getSlopeLimit(){ return m_Controller->getSlopeLimit(); };

	//������n�ʂ̊p�x���Z�b�g����
	//static�I�u�W�F�N�g�݂̂ɗL��   
	void setSlopeLimit(float slope){ m_Controller->setSlopeLimit(MOF_COS(slope)); };

	//�����Ȃ��n�ʂ̊p�x�̎��̈ړ����@���擾����
	PxControllerNonWalkableMode::Enum getNonSlopeWalkMode(){ return m_Controller->getNonWalkableMode(); };

	//�����Ȃ��n�ʂ̊p�x�̎��̈ړ����@���Z�b�g����
	//PxControllerNonWalkableMode::eFORCE_SLIDING							//�f�t�H���g�B�����Ȃ�
	//PxControllerNonWalkableMode::ePREVENT_CLIMBING						//�Ζʂ���铮���݂̂𐧌�����
	//PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING		//�Ζʂ�o�铮���𐧌��{�Ζʂ����藎����
	void setNonSlopeWalkMode(PxControllerNonWalkableMode::Enum mode){ m_Controller->setNonWalkableMode(mode); };

	//�����œo���i���̍������擾����
	MofU32 getStepOffset(){ return m_Controller->getStepOffset(); };

	//�����œo���i���̍������Z�b�g����
	void setStepOffset(float step){ m_Controller->setStepOffset(step); };

	//�R���g���[���[�̃^�C�v���擾����
	//PxControllerShapeType::�@�ŃJ�v�Z�����{�b�N�X�����ׂ�
	PxControllerShapeType::Enum getShapeType(){ return m_Controller->getType(); };

	//�R���g���[���[��shape��`�悷��
	virtual void render(Mof::Color color) = 0;

	//���̃N���X�̃��������J������
	void release();

	//�R���X�g���N�^
	PmObjectCharacter(PxController* con){ m_Controller = con; };

	virtual ~PmObjectCharacter(){ release(); };
};

class PmObjectCapsuleCharacter : public PmObjectCharacter
{
private:
	float m_Radius;
public:

	//�J�v�Z���̔��a���擾����
	float getRadius(){ return m_Radius; };

	//�J�v�Z���̔��a���Z�b�g����i�`��p�j
	void setRadius(float radius){ m_Radius = radius; };

	//�J�v�Z����`�悷��
	void render(Mof::Color color);

	//�R���X�g���N�^
	PmObjectCapsuleCharacter(PxController* con) : PmObjectCharacter(con){};

	virtual ~PmObjectCapsuleCharacter(){};
};