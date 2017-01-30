//////////////////////////////////////////////
//

#include		"PmRender.h"

void PmRenderShape::RenderShape(PmObject* obj, Mof::Color color)
{

	PxRigidActor* actor = obj->getActor();

	//�A�N�^�[�ɒǉ�����Ă���`��̐��ƌ`����擾����
	PxU32 nShapes = actor->getNbShapes();
	PxShape** shapes = new PxShape*[nShapes];
	actor->getShapes(shapes, nShapes);

	//GlobalPose
	PxTransform tra = actor->getGlobalPose();
	//�A�N�^�[�̃|�W�V������
	CVector3 gloPv(tra.p.x, tra.p.y, tra.p.z);
	//�������[�̂������ɂ���
	CQuaternion gloPq(tra.q.x, tra.q.y, tra.q.z, tra.q.w);
	CMatrix44 grmat;
	gloPq.ConvertMatrixTranspose(grmat);
	for (int i = 0; i < nShapes; i++)
	{
		//�`��̃��[�J���|�[�Y
		PxTransform shapeTra = shapes[i]->getLocalPose();
		CVector3 shaPv(shapeTra.p.x, shapeTra.p.y, shapeTra.p.z);
		CQuaternion shaPq(shapeTra.q.x, shapeTra.q.y, shapeTra.q.z, shapeTra.q.w);

		//�O���[�o���|�[�Y�ƃ��[�J���|�[�Y�����킹��
		CVector3 pv = gloPv + shaPv * grmat;
		CQuaternion pq = gloPq * shaPq;

		//���̕`��
		switch (shapes[i]->getGeometryType())
		{

			//���̕`��
		case PxGeometryType::eBOX:
		{
			PxBoxGeometry bGeo;
			shapes[i]->getBoxGeometry(bGeo);

			COBB box;
			//���W������
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

			//���̕`��
		case PxGeometryType::eSPHERE:
		{
			PxSphereGeometry sGeo;
			shapes[i]->getSphereGeometry(sGeo);
			//���a���擾
			float sphereR = sGeo.radius;

			CMatrix44 mat;
			pq.ConvertMatrixTranspose(mat);
			mat.Scaling(sphereR, mat);
			mat.SetTranslation(pv);
			CGraphicsUtilities::RenderSphere(mat, Vector4(color.r, color.g, color.b, color.a));

			break;
		}
			//�J�v�Z���̕`��
		case PxGeometryType::eCAPSULE:
		{
			PxCapsuleGeometry cGeo;
			shapes[i]->getCapsuleGeometry(cGeo);
			//���a���擾
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