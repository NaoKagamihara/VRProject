


#include		"PmPhysxCreateTool.h"

PmPhysxCreateTool::PmPhysxCreateTool(PxPhysics* physics, PxCooking* cooking){ m_Physics = physics; m_Cooking = cooking; };

PmPhysxCreateTool::~PmPhysxCreateTool(){};

///////////////////////////////////////
//Materialを作成する
PxMaterial* PmPhysxCreateTool::createMaterial(CVector3 material){ return m_Physics->createMaterial(material.x, material.y, material.z); };

//////////////////////////////////////
//Shapeを作成する
PxShape* PmPhysxCreateTool::createSphere(float radius, PxMaterial* material)
{
	PxShape* shape = m_Physics->createShape(PxSphereGeometry(radius), *material);
	return shape;
}

PxShape* PmPhysxCreateTool::createCapsule(float radius, float height, PxMaterial* material)
{
	PxShape* shape = m_Physics->createShape(PxCapsuleGeometry(radius, height), *material);
	return shape;
}

PxShape* PmPhysxCreateTool::createBox(CVector3 boxSize, PxMaterial* material)
{
	PxShape* shape = m_Physics->createShape(PxBoxGeometry(boxSize.x / 2, boxSize.y / 2, boxSize.z / 2), *material);
	return shape;
}

PxShape* PmPhysxCreateTool::createMesh(CMeshContainer mesh, PxMaterial* material, CVector3 scale)
{
	//メッシュからジオメトリ取得
	LPGeometry Geo = mesh.GetGeometry(0);

	//インデックスバッファ？
	LPIndexBuffer ib = Geo->GetIndexBuffer();

	//使用するモデルの情報を入れていくやつ
	PxConvexMeshDesc meshDesc;

	//フラグ?????????????????????????
	//	meshDesc.flags = 0;
	//if (ib->GetIndexU16())
	//	meshDesc.flags = PxConvexFlag::e16_BIT_INDICES;

	//頂点の数を調整してくれるフラグ
	meshDesc.flags |= PxConvexFlag::eCOMPUTE_CONVEX;

	///////////////////////////////////////////////////////////
	//頂点データの作成
	//頂点
	LPVertexBuffer vb = Geo->GetVertexBuffer();

	LPMofU32	m_pNind;
	float*		m_pPvx;

	//頂点の数
	meshDesc.points.count = vb->GetVertexCount();
	//１頂点のサイズ(バイト数)
	meshDesc.points.stride = vb->GetVertexSize();
	//頂点情報
	m_pPvx = (float*)malloc(meshDesc.points.stride * meshDesc.points.count);
	memcpy(m_pPvx, vb->GetVertex(), meshDesc.points.stride * meshDesc.points.count);
	char* pn = (char*)m_pPvx;
	for (MofU32 i = 0; i < vb->GetVertexCount(); i++)
	{
		Vector3* pvert = (Vector3*)pn;
		(*pvert) *= Geo->GetMatrix();
		pn += vb->GetVertexSize();
	}
	meshDesc.points.data = m_pPvx;

	///////////////////////////////////////////////////////////
	//triangle

	meshDesc.triangles.count = Geo->GetIndexBuffer()->GetIndexCount() / 3;	//三角の数
	meshDesc.triangles.stride = sizeof(MofU32) * 3;							//サイズ

	//インデックスの種類で分岐
	if (ib->GetIndexU16())
	{
		LPMofU16 pind = Geo->GetIndexBuffer()->GetIndexU16();
		m_pNind = new MofU32[Geo->GetIndexBuffer()->GetIndexCount()];		//頂点データぶん領域を確保
		for (MofU32 i = 0; i < Geo->GetIndexBuffer()->GetIndexCount(); i++)	//頂点データをMofU32に変換
		{
			m_pNind[i] = *pind;	pind++;
		}
		meshDesc.triangles.data = m_pNind;
	}
	else
	{
		LPMofU32 pind = Geo->GetIndexBuffer()->GetIndexU32();
		m_pNind = new MofU32[Geo->GetIndexBuffer()->GetIndexCount()];
		memcpy(m_pNind, pind, sizeof(MofU32) * Geo->GetIndexBuffer()->GetIndexCount());
		meshDesc.triangles.data = m_pNind;
	}

	//クッキング
	PxDefaultMemoryOutputStream writeBuffer;
	//クラスオブジェクトをシリアル化する
	bool status = m_Cooking->cookConvexMesh(meshDesc, writeBuffer);
	if (!status)
	{
		ERMS("Cooking_errer");
	}

	PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());

	//シリアル化したデータからConvexMeshを作成する
	PxConvexMesh* Mesh = m_Physics->createConvexMesh(readBuffer);
	PxQuat quat(0.0f, PxVec3(0, 0, 0));
	PxVec3 vec(scale.x, scale.y, scale.z);
	PxMeshScale sca(vec, quat);

	PxShape* shape = m_Physics->createShape(PxConvexMeshGeometry(Mesh, sca), *material);

	MOF_SAFE_DELETE(m_pPvx);
	MOF_SAFE_DELETE(m_pNind);

	return shape;
}


void PmPhysxCreateTool::setupFiltering(PmObject* obj, PxU32 filterGroup, PxU32 filterMask)
{

	PxRigidActor* actor = obj->getActor();

	PxFilterData filterData;
	filterData.word0 = filterGroup;		//自分のID
	filterData.word1 = filterMask;		//フィルタリングするペアのID

	const PxU32 numShapes = actor->getNbShapes();
	PxShape** shapes = (PxShape**)malloc(sizeof(PxShape*)*numShapes);
	actor->getShapes(shapes, numShapes);
	for (PxU32 i = 0; i < numShapes; i++)
	{
		PxShape* shape = shapes[i];
		shape->setSimulationFilterData(filterData);
	}

	free(shapes);
}