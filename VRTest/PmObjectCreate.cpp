/////////////////////////////////
//PmObjectを作成するためのクラス

#include	"PmObjectCreate.h"



PmObjectCreate::PmObjectCreate(PxPhysics* physics, PxFoundation* foundation, PxScene* scene, vector<PmObject*>& obj, PxControllerManager* m_ConManager, PxCapsuleControllerDesc conDesc)
{
	m_pPhysics = physics;
	m_pScene = scene;
	mv_Object = &obj;
	m_pConManager = m_ConManager;
	m_ConDesc = conDesc;

	m_pCooking = PxCreateCooking(PX_PHYSICS_VERSION, *foundation, PxCookingParams(PxTolerancesScale()));
	if (!m_pCooking)
	{
		ERMS("PxCreateCooking失敗");
	}
}

PmObjectCreate::~PmObjectCreate()
{

}

void PmObjectCreate::release()
{
	m_pCooking->release();
}

//Physics関係のインスタンスを作成する
PmPhysxCreateTool* PmObjectCreate::createPhysxCreateTool(){ return new PmPhysxCreateTool(m_pPhysics, m_pCooking); };

//PmObjectをマネージャーとシーンの中から削除する
//引数で渡したオブジェクトを削除する
void PmObjectCreate::detachObject(PmObject* obj)
{
	for (vector<PmObject*>::iterator itr = mv_Object->begin(); itr != mv_Object->end(); ++itr)
	{
		if ((*itr) == obj)
		{
			m_pScene->removeActor(*obj->getActor());
			mv_Object->erase(itr);
			break;
		}
	}
}

//Planeを作成する
void PmObjectCreate::createPlane(CVector3 vec, PxMaterial* mate)
{
	PxRigidStatic* body = PxCreatePlane(*m_pPhysics, PxPlane(PxPlane(PxVec3(vec.x, vec.y, vec.z), PxVec3(0.0, 1.0, 0.0))), *mate);
	m_pScene->addActor(*body);
}

//Geometryが空のPmObjectを作成する
PmObject* PmObjectCreate::createEmpObject(CVector3 pos, PxMaterial* material, bool dynamic)
{
	PmObject* Object;

	//シーンに追加した状態でアクター作成
	Object = createObject(pos, dynamic);

	PxRigidActor* actor = Object->getActor();										//PmObjectの持つActorの作成

	return Object;
}

//PmObjectにBoxGeometryを追加し作成する
PmObject* PmObjectCreate::createBoxObject(CVector3 pos, PxMaterial* material, MofFloat x, MofFloat y, MofFloat z, bool dynamic)
{
	PmObject* Object;

	//シーンに追加した状態でアクター作成
	Object = createObject(pos, dynamic);

	PxRigidActor* actor = Object->getActor();										//PmObjectの持つActorの作成
	actor->createShape(PxBoxGeometry(x, y, z), *material, PxTransform(0, 0, 0));	//ActorにShapeを持たせる

	return Object;
}

//PmObjectにSphereGeometryを追加し作成する
PmObject* PmObjectCreate::createSphereObject(CVector3 pos, PxMaterial* material, MofFloat r, bool dynamic)
{
	PmObject* Object;

	//シーンに追加した状態でアクター作成
	Object = createObject(pos, dynamic);

	PxRigidActor* actor = Object->getActor();										//PmObjectの持つActorの作成
	actor->createShape(PxSphereGeometry(r), *material, PxTransform(0, 0, 0));		//ActorにShapeを持たせる

	return Object;
}

//PmObjectにCapsuleGeometryを追加し作成する
PmObject* PmObjectCreate::createCapsuleObject(CVector3 pos, PxMaterial* material, MofFloat r, MofFloat h, bool dynamic)
{
	PmObject* Object;

	//シーンに追加した状態でアクター作成
	Object = createObject(pos, dynamic);

	PxRigidActor* actor = Object->getActor();										//PmObjectの持つActorの作成
	actor->createShape(PxCapsuleGeometry(r, h), *material, PxTransform(0, 0, 0));	//ActorにShapeを持たせる

	return Object;
}

//PmObjectにConvexGeometryを追加し作成する
PmObject* PmObjectCreate::createConvexObject(CVector3 pos, PxMaterial* material, CMeshContainer mesh, bool dynamic, CVector3 scale)
{
	PmObject* Object;

	//シーンに追加した状態でアクター作成
	Object = createObject(pos, dynamic);

	PxRigidActor* actor = Object->getActor();										//PmObjectの持つActorの作成

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
	bool status = m_pCooking->cookConvexMesh(meshDesc, writeBuffer);
	if (!status)
	{
		ERMS("Cooking_errer");
	}

	PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());

	//シリアル化したデータからConvexMeshを作成する
	PxConvexMesh* Mesh = m_pPhysics->createConvexMesh(readBuffer);
	PxQuat quat(0.0f, PxVec3(0, 0, 0));
	PxVec3 vec(scale.x, scale.y, scale.z);
	PxMeshScale sca(vec, quat);
	actor->createShape(PxConvexMeshGeometry(Mesh, sca), *material, PxTransform(0, 0, 0));

	MOF_SAFE_DELETE(m_pPvx);
	MOF_SAFE_DELETE(m_pNind);

	return Object;
}

PmObject* PmObjectCreate::createTriangleObject(CVector3 pos, PxMaterial* material, CMeshContainer mesh, CVector3 scale)
{
	PmObject* Object;

	//シーンに追加した状態でアクター作成
	Object = createObject(pos, false);

	PxRigidActor* actor = Object->getActor();										//PmObjectの持つActorの作成

	//メッシュからジオメトリ取得
	LPGeometry Geo = mesh.GetGeometry(0);

	//インデックスバッファ？
	LPIndexBuffer ib = Geo->GetIndexBuffer();

	//使用するモデルの情報を入れていくやつ
	PxTriangleMeshDesc meshDesc;

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
	meshDesc.triangles.stride = sizeof(MofU32) * 3;						//サイズ

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
	bool status = m_pCooking->cookTriangleMesh(meshDesc, writeBuffer);
	if (!status)
	{
		ERMS("Cooking_errer");
	}

	PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());

	PxTriangleMesh* Mesh = m_pPhysics->createTriangleMesh(readBuffer);
	PxQuat quat(0.0f, PxVec3(0, 0, 0));
	PxVec3 vec(scale.x, scale.y, scale.z);
	PxMeshScale sca(vec, quat);
	PxTriangleMeshGeometry tgeom(Mesh, sca);
	actor->createShape(tgeom, *material, PxTransform(0, 0, 0));


	MOF_SAFE_DELETE(m_pPvx);
	MOF_SAFE_DELETE(m_pNind);

	return Object;
}

//PmObjectを作成する
//pos		:座標
//dynamic	:動くオブジェクトかどうか(false:動かない, true:動く)
PmObject* PmObjectCreate::createObject(CVector3 pos, bool dynamic)
{
	//staticアクターの作成
	if (dynamic == false)
	{
		PxRigidStatic* Body = m_pPhysics->createRigidStatic(PxTransform(pos.x, pos.y, pos.z));
		PmObject* obj = new PmObject();
		//返すPmObjectとシーンに追加する
		m_pScene->addActor(*Body);		//シーンにアクターを追加する
		obj->setActor(*Body);
		mv_Object->push_back(obj);
		return obj;
	}

	//dynamicアクターの作成
	else if (dynamic == true)
	{
		PxRigidDynamic* Body = m_pPhysics->createRigidDynamic(PxTransform(pos.x, pos.y, pos.z));
		PmObjectDynamic* objDyn = new PmObjectDynamic();
		//返すPmObjectとシーンに追加する
		m_pScene->addActor(*Body);
		objDyn->setActor(*Body);
		mv_Object->push_back(objDyn);
		return objDyn;
	}

	return NULL;
}

//PmObjectCharacterを作成する
PmObjectCharacter* PmObjectCreate::createCharacterCapsuleObject(CVector3 pos, float r, float h, PxMaterial* material,
	float slopeLimit,
	float contactOffset,
	float stepOffset)
{
	
	m_ConDesc.material = material;
	m_ConDesc.position = PxExtendedVec3(pos.x, pos.y, pos.z);				//座標
	m_ConDesc.height = h;													//高さ（中心からY軸に+と-の方向に延びる長さ）
	m_ConDesc.radius = r;													//半径
	m_ConDesc.slopeLimit = slopeLimit;										//登れる角度かな
	m_ConDesc.contactOffset = contactOffset;								//衝突精度

	PmObjectCapsuleCharacter* character = new PmObjectCapsuleCharacter(m_pConManager->createController(m_ConDesc));

	character->setHeight(h);
	character->setRadius(r);

	return character;
}

//CItemCreate
void PmObjectCreate::createCItemObject(PmObjectDynamic* obj)
{
	PxRigidDynamic* Body = m_pPhysics->createRigidDynamic(PxTransform(0,0,0));
	//返すPmObjectとシーンに追加する
	m_pScene->addActor(*Body);
	obj->setActor(*Body);
	mv_Object->push_back(obj);
	obj->changeObjectTag(EPmObjectTag::OBJ_ITEM,true);
}