///////////////////////////////////
//PmObject定義

#include "PmObject.h"
#include "PmCollisionSearch.h"

PmObject::PmObject() : m_bCollSearch(true)
{
	m_CollSearch = new PmCollisionSearch;
	m_eObjTag = 0;
}

//posの場所に座標をセットする
void PmObject::setGlobalPos(CVector3 pos)
{
	PxTransform tra = m_RigidBody->getGlobalPose();
	tra.p.x = pos.x;
	tra.p.y = pos.y;
	tra.p.z = pos.z;
	m_RigidBody->setGlobalPose(tra);
}

//座標を取得する
CVector3 PmObject::getGlobalPos()
{
	PxTransform tra = m_RigidBody->getGlobalPose();
	CVector3 pv(tra.p.x, tra.p.y, tra.p.z);
	return pv;
}

//rotの角度にセットする
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

//角度を取得する
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

//角度を取得する
CQuaternion PmObject::getGlobalQuoRot()
{
	PxTransform tra = m_RigidBody->getGlobalPose();
	CQuaternion pq(tra.q.x, tra.q.y, tra.q.z, tra.q.w);
	return pq;
}

//m_RigidBodyのActorのMatrix44を取得する
Matrix44 PmObject::getActorMatrix44(CVector3 scale)
{
	PxTransform tra = m_RigidBody->getGlobalPose();
	//アクターのポジションを
	CVector3 pv(tra.p.x, tra.p.y, tra.p.z);
	//あくたーのくおたにおん
	CQuaternion pq(tra.q.x, tra.q.y, tra.q.z, tra.q.w);

	CMatrix44 mat;
	//座標
	CVector3 plvec(tra.p.x, tra.p.y, tra.p.z);
	//回転
	CQuaternion plquo(tra.q.x, tra.q.y, tra.q.z, tra.q.w);

	//拡大
	CMatrix44 matSca;
	matSca.Scaling(scale);
	mat *= matSca;

	//回転
	CMatrix44 matRot;
	plquo.ConvertMatrixTranspose(matRot);
	mat *= matRot;

	//移動
	CMatrix44 matVec;
	matVec.Translation(plvec);
	mat *= matVec;

	return mat;
}

//m_RigidBodyのnbShape番目のマトリックスを取得する
Matrix44 PmObject::getMatrix44(int nbShape, CVector3 scale)
{

	PxShape* shape;
	//形を取得する
	m_RigidBody->getShapes(&shape, nbShape);

	PxTransform tra = m_RigidBody->getGlobalPose();
	//アクターのポジションを
	CVector3 pv(tra.p.x, tra.p.y, tra.p.z);
	//あくたーのくおたにおん
	CQuaternion pq(tra.q.x, tra.q.y, tra.q.z, tra.q.w);

	//形ごとにmatrixを取得する
	CMatrix44 mat;
 	switch (shape->getGeometryType())
	{

		//箱の形状
	case PxGeometryType::eBOX:
	{
		PxBoxGeometry bGeo;
		shape->getBoxGeometry(bGeo);

		COBB box;
		//座標を入れる
		box.Position = pv;
		box.Size.x = bGeo.halfExtents.x * 2;
		box.Size.y = bGeo.halfExtents.y * 2;
		box.Size.z = bGeo.halfExtents.z * 2;

		pq.ConvertMatrixTranspose(mat);
		mat.Scaling(box.Size.x, box.Size.y, box.Size.z, mat);
		mat.SetTranslation(pv);

		break;
	}

		//球の形状
	case PxGeometryType::eSPHERE:
	{
		PxSphereGeometry sGeo;
		shape[0].getSphereGeometry(sGeo);
		//半径を取得
		float sphereR = sGeo.radius;

		CMatrix44 mat;
		pq.ConvertMatrixTranspose(mat);
		mat.Scaling(sphereR, mat);
		mat.SetTranslation(pv);

		break;
	}

		//カプセルの形状
	case PxGeometryType::eCAPSULE:
	{
		PxCapsuleGeometry cGeo;
		shape->getCapsuleGeometry(cGeo);
		//半径を取得
		float radius = cGeo.radius;
		float height = cGeo.halfHeight * 2;

		//マトリックスを作成
		pq.ConvertMatrixTranspose(mat);
		mat.SetTranslation(pv);
		CMatrix33 rotz;
		rotz.RotationZ(MOF_MATH_HALFPI);
		mat = rotz * mat;

		break;
	}

		//めっしゅの形状
	case PxGeometryType::eCONVEXMESH:
	case PxGeometryType::eTRIANGLEMESH:
	{

		//座標
		CVector3 plvec(tra.p.x, tra.p.y, tra.p.z);
		//回転
		CQuaternion plquo(tra.q.x, tra.q.y, tra.q.z, tra.q.w);

		//拡大
		CMatrix44 matSca;
		matSca.Scaling(scale);
		mat *= matSca;

		//回転
		CMatrix44 matRot;
		plquo.ConvertMatrixTranspose(matRot);
		mat *= matRot;

		//移動
		CMatrix44 matVec;
		matVec.Translation(plvec);
		mat *= matVec;

		break;

	}
	}

	return mat;
}

//m_RigidBodyのnbShape番目のshapeを取得する
PxShape* PmObject::getShape(int nbShape)
{
	PxShape* shape;
	m_RigidBody->getShapes(&shape, nbShape);

	return shape;
}

//m_RigidBodyのnbShape番目の形の種類を取得する
PxGeometryType::Enum PmObject::getGeometryType(int nbShape)
{
	PxShape* shape;
	m_RigidBody->getShapes(&shape, nbShape);
	
	return shape->getGeometryType();
}

//m_RigidBodyのnbShape番目が他のオブジェクトと衝突するのか調べる
bool PmObject::isCollision(int nbShape)
{
	PxShape* shape;
	m_RigidBody->getShapes(&shape, nbShape);

	PxShapeFlags flag = shape->getFlags();

	return (flag & PxShapeFlag::eSIMULATION_SHAPE) ? true : false;
}

//m_RigidBodyのnbShape番目の衝突するのかフラグを設定する:trueなら衝突
void PmObject::setCollision(int nbShape, bool flag)
{
	PxShape* shape;
	m_RigidBody->getShapes(&shape, nbShape);

	shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, flag);
}

//m_RigidBodyのnbShape番目がTrigger用か取得する
bool PmObject::isTrigger(int nbShape)
{
	PxShape* shape;
	m_RigidBody->getShapes(&shape, nbShape);

	PxShapeFlags flag = shape->getFlags();

	if (flag & PxShapeFlag::eSIMULATION_SHAPE ? false : true)	//eSCENE_QUERY_SHAPEがfalseで
	{
		if (flag & PxShapeFlag::eTRIGGER_SHAPE ? true : false)	//eTRIGGER_SHAPEがtrueの場合trueを返す
		{
			return true;
		}
	}

	return false;
}

//m_RigidBodyのnbShape番目のトリガーを使用するのかフラグを設定する:trueなら使用する
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