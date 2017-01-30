//////////////////////////////////
//PhsyxのGeometryの作成
//読み込んだmomファイルからmeshデータを作成する

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
	//Materialを作成する
	PxMaterial* createMaterial(CVector3 material);

	//////////////////////////////////////
	//Shapeを作成する
	PxShape* createSphere(float radius, PxMaterial* material);

	PxShape* createCapsule(float radius, float height, PxMaterial* material);

	PxShape* createBox(CVector3 boxSize, PxMaterial* material);

	PxShape* createMesh(CMeshContainer mesh, PxMaterial* material, CVector3 scale = CVector3(1.0f,1.0f,1.0f));

	static void setupFiltering(PmObject* obj, PxU32 filterGroup, PxU32 filterMask);
};