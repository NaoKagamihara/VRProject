/////////////////////////////////
//PmObject���쐬���邽�߂̃N���X

#pragma once

#include	"PmObjectDynamic.h"
#include	"PmPhysxCreateTool.h"
#include	"CItem.h"

#include	<vector>

using namespace std;

class PmPhysxCreateTool;

class PmObjectCreate
{
private:
	PxPhysics*			m_pPhysics;
	PxScene*			m_pScene;
	PxCooking*			m_pCooking;

	vector<PmObject*>*	mv_Object;

public:
	
	PmObjectCreate(PxPhysics* physics, PxFoundation* foundation, PxScene* scene, vector<PmObject*>& obj);
	~PmObjectCreate();

	void release();
	
	//Physics�֌W�̃C���X�^���X���쐬����
	PmPhysxCreateTool* createPhysxCreateTool();

	//PmObject���}�l�[�W���[�ƃV�[���̒�����폜����
	//�����œn�����I�u�W�F�N�g���폜����
	void detachObject(PmObject* obj);

	/////////////////////////////////////////////////
	//�I�u�W�F�N�g�̍쐬(�쐬���ɃV�[���ɒǉ������)

	//Plane���쐬����
	void createPlane(CVector3 vec, PxMaterial* mate);

	//Geometry�����PmObject���쐬����
	PmObject* createEmpObject(CVector3 pos, PxMaterial* material, bool dynamic);

	//PmObject��BoxGeometry��ǉ����쐬����
	//pos		:���W
	//material	:�ގ�
	//x,y,z		:�̑傫���i�Q���̂P�ɂȂ�j
	//dynamic	:�����I�u�W�F�N�g���ǂ���(false:�����Ȃ�, true:����)
	PmObject* createBoxObject(CVector3 pos, PxMaterial* material, MofFloat x, MofFloat y, MofFloat z, bool dynamic);

	//PmObject��SphereGeometry��ǉ����쐬����
	//pos		:���W
	//material	:�ގ�
	//r			:���a
	//dynamic	:�����I�u�W�F�N�g���ǂ���(false:�����Ȃ�, true:����)
	PmObject* createSphereObject(CVector3 pos, PxMaterial* material, MofFloat r, bool dynamic);

	//PmObject��CapsuleGeometry��ǉ����쐬����
	//pos		:���W
	//material	:�ގ�
	//r			:���a
	//h			:����
	//dynamic	:�����I�u�W�F�N�g���ǂ���(false:�����Ȃ�, true:����)
	PmObject* createCapsuleObject(CVector3 pos, PxMaterial* material, MofFloat r, MofFloat h, bool dynamic);

	//PmObject��ConvexGeometry��ǉ����쐬����
	//pos		:���W
	//material	:�ގ�
	//mesh		:���b�V��
	//conTri	:�R���x�b�N�X���g���C�A���O��
	//dynamic	:�����I�u�W�F�N�g���ǂ���(false:�����Ȃ�, true:����)
	PmObject* createConvexObject(CVector3 pos, PxMaterial* material, CMeshContainer mesh, bool dynamic, CVector3 scale = CVector3(1.0f, 1.0f, 1.0f));

	//PmObject��TriangleGeometry��ǉ����쐬����
	//pos		:���W
	//material	:�ގ�
	//mesh		:���b�V��
	PmObject* createTriangleObject(CVector3 pos, PxMaterial* material, CMeshContainer mesh, CVector3 scale = CVector3(1.0f, 1.0f, 1.0f));

	//CItemCreate
	void createCItemObject(PmObjectDynamic* obj);

private:
	//PmObject���쐬����
	//pos		:���W
	//dynamic	:�����I�u�W�F�N�g���ǂ���(false:�����Ȃ�, true:����)
	PmObject* createObject(CVector3 pos, bool dynamic);
};