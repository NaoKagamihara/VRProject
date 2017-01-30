/////////////////////////////////////
//���N���X
//PmManager�ɒǉ�����I�u�W�F�N�g�������Ă�I�u�W�F�N�g

#include		"PmGameSummary.h"

//PmPhysxCreateTool���쐬����
PmPhysxCreateTool* PmGameSummary::createPhysxCreateTool()
{
	return m_pCreate->createPhysxCreateTool();
}

/////////////////////////////////////////////////
//�I�u�W�F�N�g�̍쐬

//���쐬����
void PmGameSummary::createPlane(const CVector3& vec, PxMaterial* material)
{
	m_pCreate->createPlane(vec, material);
}

//Geometry�����PmObject���쐬����
PmObject* PmGameSummary::createEmpObject(const CVector3& pos, PxMaterial* material,const bool dynamic)
{
	return m_pCreate->createEmpObject(pos, material, dynamic);
}

//PmObject��BoxGeometry��ǉ����쐬����
//pos		:���W
//material	:�ގ�
//x,y,z		:�̑傫���i���S����Y����+��-�̕����ɉ��т钷���j
//dynamic	:�����I�u�W�F�N�g���ǂ���(false:�����Ȃ�, true:����)
PmObject* PmGameSummary::createBoxObject(const CVector3& pos, PxMaterial* material, const MofFloat x, const MofFloat y, const MofFloat z, const bool dynamic)
{
	return m_pCreate->createBoxObject(pos, material, x, y, z, dynamic);
}

//PmObject��SphereGeometry��ǉ����쐬����
//pos		:���W
//material	:�ގ�
//r			:���a
//dynamic	:�����I�u�W�F�N�g���ǂ���(false:�����Ȃ�, true:����)
PmObject* PmGameSummary::createSphereObject(const CVector3& pos, PxMaterial* material, const MofFloat r, const bool dynamic)
{
	return m_pCreate->createSphereObject(pos, material, r, dynamic);
}

//PmObject��CapsuleGeometry��ǉ����쐬����
//pos		:���W
//material	:�ގ�
//r			:���a
//h			:����
//dynamic	:�����I�u�W�F�N�g���ǂ���(false:�����Ȃ�, true:����)
PmObject* PmGameSummary::createCapsuleObject(const CVector3& pos, PxMaterial* material, const MofFloat r, const MofFloat h, const bool dynamic)
{
	return m_pCreate->createCapsuleObject(pos, material, r, h, dynamic);
}

//PmObject��ConvexGeometry��ǉ����쐬����
//pos		:���W
//material	:�ގ�
//mesh		:���b�V��
//dynamic	:�����I�u�W�F�N�g���ǂ���(false:�����Ȃ�, true:����)
PmObject* PmGameSummary::createConvexObject(const CVector3& pos, PxMaterial* material, CMeshContainer mesh, const bool dynamic, const CVector3& scale)
{
	return m_pCreate->createConvexObject(pos, material, mesh, dynamic, scale);
}

//PmObject��TriangleGeometry��ǉ����쐬����
//pos		:���W
//material	:�ގ�
//mesh		:���b�V��
//dynamic	:�����I�u�W�F�N�g���ǂ���(false:�����Ȃ�, true:����)
PmObject* PmGameSummary::createTriangleObject(const CVector3& pos, PxMaterial* material, CMeshContainer mesh, const CVector3& scale)
{
	return m_pCreate->createTriangleObject(pos, material, mesh, scale);
}

//PmObject���V�[������O��
//obj		:�V�[������O���I�u�W�F�N�g
void PmGameSummary::detachObject(PmObject* obj)
{
	m_pCreate->detachObject(obj);
}

//CItem��ǉ�����
void PmGameSummary::createCItemObject(PmObjectDynamic* obj)
{
	m_pCreate->createCItemObject(obj);
}