/////////////////////////////////////////
//�L�����N�^�R���g���[���[

#include		"PmObjectCharcter.h"


//���̃N���X���g��Shape��`�悷��
//void PmObjectCharacter::Render(Mof::Color color)
//{
	//switch (m_Controller->getType())
	//{
	//	//�J�v�Z���̕`��
	//case PxControllerShapeType::eCAPSULE:

	//	PxExtendedVec3 pos = m_Controller->getPosition();
	//	CVector3 vec(pos.x, pos.y, pos.z);

	//	CMatrix44 matWorld;
	//	matWorld.Translation(vec);

	//	CMatrix44 mat;

	//	mat.Scaling(m_Controller->get);
	//	mat.SetTranslation(0, -height / 2.0f, 0);
	//	CGraphicsUtilities::RenderSphere(mat * mat1, Vector4(1, 0, 1, 1));
	//	mat.SetTranslation(0, height / 2.0f, 0);
	//	CGraphicsUtilities::RenderSphere(mat * mat1, Vector4(1, 0, 1, 1));

	//	mat.Scaling(radius, height, radius);
	//	CGraphicsUtilities::RenderCylinder(mat * mat1, Vector4(1, 0, 1, 1));

	//default:
	//	break;
	//}
//}

//�L�����N�^���ړ�������
//flag.operator&(PxControllerFlag::eCOLLISION_DOWN)
PxControllerCollisionFlags PmObjectCharacter::move(CVector3 vec)
{
	PxVec3 moveVec(vec.x, vec.y, vec.z);
	double time = (double)(clock() - m_time);
	PxControllerCollisionFlags flag = m_Controller->move(moveVec, m_MinDist, time, m_filter);
	time = clock();
	return flag;
}

//���̃N���X�̃��������J������
void PmObjectCharacter::release()
{
	if (!m_Controller) m_Controller->release();
}

//�J�v�Z����`�悷��
void PmObjectCapsuleCharacter::render(Mof::Color color)
{
	PxExtendedVec3 pos = m_Controller->getPosition();
	CVector3 vec(pos.x, pos.y, pos.z);

	CMatrix44 matWorld;
	matWorld.Translation(vec);

	CMatrix44 mat;

	mat.Scaling(m_Radius);
	mat.SetTranslation(0.0f, -m_Height / 2.0f, 0);
	CGraphicsUtilities::RenderSphere(mat * matWorld, color);
	mat.SetTranslation(0.0f, m_Height / 2.0f, 0.0f);
	CGraphicsUtilities::RenderSphere(mat * matWorld, color);

	mat.Scaling(m_Radius, m_Height, m_Radius);
	CGraphicsUtilities::RenderCylinder(mat * matWorld, color);
}