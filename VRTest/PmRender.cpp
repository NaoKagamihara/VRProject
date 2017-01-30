//////////////////////////////////////////////
//

#include		"PmRender.h"

void PmRenderShape::RenderShape(PmObject* obj, Mof::Color color)
{

	PxRigidActor* actor = obj->getActor();

	//アクターに追加されている形状の数と形状を取得する
	PxU32 nShapes = actor->getNbShapes();
	PxShape** shapes = new PxShape*[nShapes];
	actor->getShapes(shapes, nShapes);

	//GlobalPose
	PxTransform tra = actor->getGlobalPose();
	//アクターのポジションを
	CVector3 gloPv(tra.p.x, tra.p.y, tra.p.z);
	//あくたーのくおたにおん
	CQuaternion gloPq(tra.q.x, tra.q.y, tra.q.z, tra.q.w);
	CMatrix44 grmat;
	gloPq.ConvertMatrixTranspose(grmat);
	for (int i = 0; i < nShapes; i++)
	{
		//形状のローカルポーズ
		PxTransform shapeTra = shapes[i]->getLocalPose();
		CVector3 shaPv(shapeTra.p.x, shapeTra.p.y, shapeTra.p.z);
		CQuaternion shaPq(shapeTra.q.x, shapeTra.q.y, shapeTra.q.z, shapeTra.q.w);

		//グローバルポーズとローカルポーズを合わせる
		CVector3 pv = gloPv + shaPv * grmat;
		CQuaternion pq = gloPq * shaPq;

		//球の描画
		switch (shapes[i]->getGeometryType())
		{

			//箱の描画
		case PxGeometryType::eBOX:
		{
			PxBoxGeometry bGeo;
			shapes[i]->getBoxGeometry(bGeo);

			COBB box;
			//座標を入れる
			box.Position = pv;
			box.Size.x = bGeo.halfExtents.x * 2;
			box.Size.y = bGeo.halfExtents.y * 2;
			box.Size.z = bGeo.halfExtents.z * 2;

			CMatrix44 mat;
			pq.ConvertMatrixTranspose(mat);
			mat.Scaling(box.Size.x, box.Size.y, box.Size.z, mat);
			mat.SetTranslation(pv);

			CGraphicsUtilities::RenderBox(mat, Vector4(color.r, color.g, color.b, color.a));
			break;
		}

			//球の描画
		case PxGeometryType::eSPHERE:
		{
			PxSphereGeometry sGeo;
			shapes[i]->getSphereGeometry(sGeo);
			//半径を取得
			float sphereR = sGeo.radius;

			CMatrix44 mat;
			pq.ConvertMatrixTranspose(mat);
			mat.Scaling(sphereR, mat);
			mat.SetTranslation(pv);
			CGraphicsUtilities::RenderSphere(mat, Vector4(color.r, color.g, color.b, color.a));

			break;
		}
			//カプセルの描画
		case PxGeometryType::eCAPSULE:
		{
			PxCapsuleGeometry cGeo;
			shapes[i]->getCapsuleGeometry(cGeo);
			//半径を取得
			float radius = cGeo.radius;
			float height = cGeo.halfHeight * 2;

			CMatrix44 matWorld;
			pq.ConvertMatrixTranspose(matWorld);
			matWorld.SetTranslation(pv);
			CMatrix33 rotz;
			rotz.RotationZ(MOF_MATH_HALFPI);
			matWorld = rotz * matWorld;

			CMatrix44 mat;
			mat.Scaling(radius);
			mat.SetTranslation(0.0f, -height / 2.0f, 0);
			CGraphicsUtilities::RenderSphere(mat * matWorld, color);
			mat.SetTranslation(0.0f, height / 2.0f, 0.0f);
			CGraphicsUtilities::RenderSphere(mat * matWorld, color);

			mat.Scaling(radius, height, radius);
			CGraphicsUtilities::RenderCylinder(mat * matWorld, color);
			break;
		}
		}
	}

	delete []shapes;
}