//////////////////////////////////
//Phsyx��Geometry�̍쐬
//�ǂݍ���mom�t�@�C������mesh�f�[�^���쐬����

#pragma once

#include		"PmObject.h"
#include		"PmObjectCreate.h"
#include		"Mof.h"

class PmObjectCreate;

class PmPhysxCreateTool
{
private:
	PxPhysics* m_Physics;
	PxCooking* m_Cooking;

public:

	PmPhysxCreateTool(PxPhysics* physics, PxCooking* cooking);

	~PmPhysxCreateTool();

	///////////////////////////////////////
	//Material���쐬����
	PxMaterial* createMaterial(CVector3 material);

	//////////////////////////////////////
	//Shape���쐬����
	PxShape* createSphere(float radius, PxMaterial* material);

	PxShape* createCapsule(float radius, float height, PxMaterial* material);

	PxShape* createBox(CVector3 boxSize, PxMaterial* material);

	PxShape* createMesh(CMeshContainer mesh, PxMaterial* material, CVector3 scale = CVector3(1.0f,1.0f,1.0f));

	static void setupFiltering(PmObject* obj, PxU32 filterGroup, PxU32 filterMask);
};