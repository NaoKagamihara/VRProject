///////////////////////////////////
//PmObject��`

#include "PmObject.h"
#include "PmCollisionSearch.h"

PmObject::PmObject() : m_bCollSearch(true)
{
	m_CollSearch = new PmCollisionSearch;
	m_eObjTag = 0;
}

//pos�̏ꏊ�ɍ��W���Z�b�g����
void PmObject::setGlobalPos(CVector3 pos)
{
	PxTransform tra = m_RigidBody->getGlobalPose();
	tra.p.x = pos.x;
	tra.p.y = pos.y;
	tra.p.z = pos.z;
	m_RigidBody->setGlobalPose(tra);
}

//���W���擾����
CVector3 PmObject::getGlobalPos()
{
	PxTransform tra = m_RigidBody->getGlobalPose();
	CVector3 pv(tra.p.x, tra.p.y, tra.p.z);
	return pv;
}

//rot�̊p�x�ɃZ�b�g����
void PmObject::setGlobalRot(CVector3 rot)
{
	PxTransform tra = m_RigidBody->getGlobalPose();
	CMatrix33 RotMat;
	RotMat.RotationXYZ(rot);
	Quaternion qua;
	RotMat.Transpose();
	RotMat.GetRotation(qua);
	tra.q.x = qua.x;
	tra.q.y = qua.y;
	tra.q.z = qua.z;
	tra.q.w = qua.w;
	m_RigidBody->setGlobalPose(tra);
}
void PmObject::setGlobalRot(const CQuaternion& rot)
{
	PxTransform tra = m_RigidBody->getGlobalPose();
	tra.q.x = rot.x;
	tra.q.y = rot.y;
	tra.q.z = rot.z;
	tra.q.w = rot.w;
	m_RigidBody->setGlobalPose(tra);
}

//�p�x���擾����
CVector3 PmObject::getGlobalVecRot()
{
	PxTransform tra = m_RigidBody->getGlobalPose();
	CQuaternion pq(tra.q.x, tra.q.y, tra.q.z, tra.q.w);
	CMatrix33 mat3;
	pq.ConvertMatrixTranspose(mat3);
	CVector3 rot;
	mat3.GetRotation(rot);
	return rot;
}

//�p�x���擾����
CQuaternion PmObject::getGlobalQuoRot()
{
	PxTransform tra = m_RigidBody->getGlobalPose();
	CQuaternion pq(tra.q.x, tra.q.y, tra.q.z, tra.q.w);
	return pq;
}

//m_RigidBody��Actor��Matrix44���擾����
Matrix44 PmObject::getActorMatrix44(CVector3 scale)
{
	PxTransform tra = m_RigidBody->getGlobalPose();
	//�A�N�^�[�̃|�W�V������
	CVector3 pv(tra.p.x, tra.p.y, tra.p.z);
	//�������[�̂������ɂ���
	CQuaternion pq(tra.q.x, tra.q.y, tra.q.z, tra.q.w);

	CMatrix44 mat;
	//���W
	CVector3 plvec(tra.p.x, tra.p.y, tra.p.z);
	//��]
	CQuaternion plquo(tra.q.x, tra.q.y, tra.q.z, tra.q.w);

	//�g��
	CMatrix44 matSca;
	matSca.Scaling(scale);
	mat *= matSca;

	//��]
	CMatrix44 matRot;
	plquo.ConvertMatrixTranspose(matRot);
	mat *= matRot;

	//�ړ�
	CMatrix44 matVec;
	matVec.Translation(plvec);
	mat *= matVec;

	return mat;
}

//m_RigidBody��nbShape�Ԗڂ̃}�g���b�N�X���擾����
Matrix44 PmObject::getMatrix44(int nbShape, CVector3 scale)
{

	PxShape* shape;
	//�`���擾����
	m_RigidBody->getShapes(&shape, nbShape);

	PxTransform tra = m_RigidBody->getGlobalPose();
	//�A�N�^�[�̃|�W�V������
	CVector3 pv(tra.p.x, tra.p.y, tra.p.z);
	//�������[�̂������ɂ���
	CQuaternion pq(tra.q.x, tra.q.y, tra.q.z, tra.q.w);

	//�`���Ƃ�matrix���擾����
	CMatrix44 mat;
 	switch (shape->getGeometryType())
	{

		//���̌`��
	case PxGeometryType::eBOX:
	{
		PxBoxGeometry bGeo;
		shape->getBoxGeometry(bGeo);

		COBB box;
		//���W������
		box.Position = pv;
		box.Size.x = bGeo.halfExtents.x * 2;
		box.Size.y = bGeo.halfExtents.y * 2;
		box.Size.z = bGeo.halfExtents.z * 2;

		pq.ConvertMatrixTranspose(mat);
		mat.Scaling(box.Size.x, box.Size.y, box.Size.z, mat);
		mat.SetTranslation(pv);

		break;
	}

		//���̌`��
	case PxGeometryType::eSPHERE:
	{
		PxSphereGeometry sGeo;
		shape[0].getSphereGeometry(sGeo);
		//���a���擾
		float sphereR = sGeo.radius;

		CMatrix44 mat;
		pq.ConvertMatrixTranspose(mat);
		mat.Scaling(sphereR, mat);
		mat.SetTranslation(pv);

		break;
	}

		//�J�v�Z���̌`��
	case PxGeometryType::eCAPSULE:
	{
		PxCapsuleGeometry cGeo;
		shape->getCapsuleGeometry(cGeo);
		//���a���擾
		float radius = cGeo.radius;
		float height = cGeo.halfHeight * 2;

		//�}�g���b�N�X���쐬
		pq.ConvertMatrixTranspose(mat);
		mat.SetTranslation(pv);
		CMatrix33 rotz;
		rotz.RotationZ(MOF_MATH_HALFPI);
		mat = rotz * mat;

		break;
	}

		//�߂�����̌`��
	case PxGeometryType::eCONVEXMESH:
	case PxGeometryType::eTRIANGLEMESH:
	{

		//���W
		CVector3 plvec(tra.p.x, tra.p.y, tra.p.z);
		//��]
		CQuaternion plquo(tra.q.x, tra.q.y, tra.q.z, tra.q.w);

		//�g��
		CMatrix44 matSca;
		matSca.Scaling(scale);
		mat *= matSca;

		//��]
		CMatrix44 matRot;
		plquo.ConvertMatrixTranspose(matRot);
		mat *= matRot;

		//�ړ�
		CMatrix44 matVec;
		matVec.Translation(plvec);
		mat *= matVec;

		break;

	}
	}

	return mat;
}

//m_RigidBody��nbShape�Ԗڂ�shape���擾����
PxShape* PmObject::getShape(int nbShape)
{
	PxShape* shape;
	m_RigidBody->getShapes(&shape, nbShape);

	return shape;
}

//m_RigidBody��nbShape�Ԗڂ̌`�̎�ނ��擾����
PxGeometryType::Enum PmObject::getGeometryType(int nbShape)
{
	PxShape* shape;
	m_RigidBody->getShapes(&shape, nbShape);
	
	return shape->getGeometryType();
}

//m_RigidBody��nbShape�Ԗڂ����̃I�u�W�F�N�g�ƏՓ˂���̂����ׂ�
bool PmObject::isCollision(int nbShape)
{
	PxShape* shape;
	m_RigidBody->getShapes(&shape, nbShape);

	PxShapeFlags flag = shape->getFlags();

	return (flag & PxShapeFlag::eSIMULATION_SHAPE) ? true : false;
}

//m_RigidBody��nbShape�Ԗڂ̏Փ˂���̂��t���O��ݒ肷��:true�Ȃ�Փ�
void PmObject::setCollision(int nbShape, bool flag)
{
	PxShape* shape;
	m_RigidBody->getShapes(&shape, nbShape);

	shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, flag);
}

//m_RigidBody��nbShape�Ԗڂ�Trigger�p���擾����
bool PmObject::isTrigger(int nbShape)
{
	PxShape* shape;
	m_RigidBody->getShapes(&shape, nbShape);

	PxShapeFlags flag = shape->getFlags();

	if (flag & PxShapeFlag::eSIMULATION_SHAPE ? false : true)	//eSCENE_QUERY_SHAPE��false��
	{
		if (flag & PxShapeFlag::eTRIGGER_SHAPE ? true : false)	//eTRIGGER_SHAPE��true�̏ꍇtrue��Ԃ�
		{
			return true;
		}
	}

	return false;
}

//m_RigidBody��nbShape�Ԗڂ̃g���K�[���g�p����̂��t���O��ݒ肷��:true�Ȃ�g�p����
void PmObject::setTrigger(int nbShape, bool flag)
{
	PxShape* shape;
	m_RigidBody->getShapes(&shape, nbShape);

	if (flag)
	{
		shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
		shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	}
	else
	{
		shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
		shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);
	}
	
}

void PmObject::release()
{
	if (m_CollSearch) MOF_SAFE_DELETE(m_CollSearch);
	if(!m_RigidBody) m_RigidBody->release();
}