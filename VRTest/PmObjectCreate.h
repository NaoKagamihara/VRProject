/////////////////////////////////
//PmObjectを作成するためのクラス

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
	
	//Physics関係のインスタンスを作成する
	PmPhysxCreateTool* createPhysxCreateTool();

	//PmObjectをマネージャーとシーンの中から削除する
	//引数で渡したオブジェクトを削除する
	void detachObject(PmObject* obj);

	/////////////////////////////////////////////////
	//オブジェクトの作成(作成時にシーンに追加される)

	//Planeを作成する
	void createPlane(CVector3 vec, PxMaterial* mate);

	//Geometryが空のPmObjectを作成する
	PmObject* createEmpObject(CVector3 pos, PxMaterial* material, bool dynamic);

	//PmObjectにBoxGeometryを追加し作成する
	//pos		:座標
	//material	:材質
	//x,y,z		:の大きさ（２分の１になる）
	//dynamic	:動くオブジェクトかどうか(false:動かない, true:動く)
	PmObject* createBoxObject(CVector3 pos, PxMaterial* material, MofFloat x, MofFloat y, MofFloat z, bool dynamic);

	//PmObjectにSphereGeometryを追加し作成する
	//pos		:座標
	//material	:材質
	//r			:半径
	//dynamic	:動くオブジェクトかどうか(false:動かない, true:動く)
	PmObject* createSphereObject(CVector3 pos, PxMaterial* material, MofFloat r, bool dynamic);

	//PmObjectにCapsuleGeometryを追加し作成する
	//pos		:座標
	//material	:材質
	//r			:半径
	//h			:高さ
	//dynamic	:動くオブジェクトかどうか(false:動かない, true:動く)
	PmObject* createCapsuleObject(CVector3 pos, PxMaterial* material, MofFloat r, MofFloat h, bool dynamic);

	//PmObjectにConvexGeometryを追加し作成する
	//pos		:座標
	//material	:材質
	//mesh		:メッシュ
	//conTri	:コンベックスかトライアングル
	//dynamic	:動くオブジェクトかどうか(false:動かない, true:動く)
	PmObject* createConvexObject(CVector3 pos, PxMaterial* material, CMeshContainer mesh, bool dynamic, CVector3 scale = CVector3(1.0f, 1.0f, 1.0f));

	//PmObjectにTriangleGeometryを追加し作成する
	//pos		:座標
	//material	:材質
	//mesh		:メッシュ
	PmObject* createTriangleObject(CVector3 pos, PxMaterial* material, CMeshContainer mesh, CVector3 scale = CVector3(1.0f, 1.0f, 1.0f));

	//CItemCreate
	void createCItemObject(PmObjectDynamic* obj);

private:
	//PmObjectを作成する
	//pos		:座標
	//dynamic	:動くオブジェクトかどうか(false:動かない, true:動く)
	PmObject* createObject(CVector3 pos, bool dynamic);
};