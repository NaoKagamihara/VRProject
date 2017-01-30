/////////////////////////////////////
//基底クラス
//PmManagerに追加するオブジェクトを持ってるオブジェクト

#include		"PmGameSummary.h"

//PmPhysxCreateToolを作成する
PmPhysxCreateTool* PmGameSummary::createPhysxCreateTool()
{
	return m_pCreate->createPhysxCreateTool();
}

/////////////////////////////////////////////////
//オブジェクトの作成

//板を作成する
void PmGameSummary::createPlane(const CVector3& vec, PxMaterial* material)
{
	m_pCreate->createPlane(vec, material);
}

//Geometryが空のPmObjectを作成する
PmObject* PmGameSummary::createEmpObject(const CVector3& pos, PxMaterial* material,const bool dynamic)
{
	return m_pCreate->createEmpObject(pos, material, dynamic);
}

//PmObjectにBoxGeometryを追加し作成する
//pos		:座標
//material	:材質
//x,y,z		:の大きさ（中心からY軸に+と-の方向に延びる長さ）
//dynamic	:動くオブジェクトかどうか(false:動かない, true:動く)
PmObject* PmGameSummary::createBoxObject(const CVector3& pos, PxMaterial* material, const MofFloat x, const MofFloat y, const MofFloat z, const bool dynamic)
{
	return m_pCreate->createBoxObject(pos, material, x, y, z, dynamic);
}

//PmObjectにSphereGeometryを追加し作成する
//pos		:座標
//material	:材質
//r			:半径
//dynamic	:動くオブジェクトかどうか(false:動かない, true:動く)
PmObject* PmGameSummary::createSphereObject(const CVector3& pos, PxMaterial* material, const MofFloat r, const bool dynamic)
{
	return m_pCreate->createSphereObject(pos, material, r, dynamic);
}

//PmObjectにCapsuleGeometryを追加し作成する
//pos		:座標
//material	:材質
//r			:半径
//h			:高さ
//dynamic	:動くオブジェクトかどうか(false:動かない, true:動く)
PmObject* PmGameSummary::createCapsuleObject(const CVector3& pos, PxMaterial* material, const MofFloat r, const MofFloat h, const bool dynamic)
{
	return m_pCreate->createCapsuleObject(pos, material, r, h, dynamic);
}

//PmObjectにConvexGeometryを追加し作成する
//pos		:座標
//material	:材質
//mesh		:メッシュ
//dynamic	:動くオブジェクトかどうか(false:動かない, true:動く)
PmObject* PmGameSummary::createConvexObject(const CVector3& pos, PxMaterial* material, CMeshContainer mesh, const bool dynamic, const CVector3& scale)
{
	return m_pCreate->createConvexObject(pos, material, mesh, dynamic, scale);
}

//PmObjectにTriangleGeometryを追加し作成する
//pos		:座標
//material	:材質
//mesh		:メッシュ
//dynamic	:動くオブジェクトかどうか(false:動かない, true:動く)
PmObject* PmGameSummary::createTriangleObject(const CVector3& pos, PxMaterial* material, CMeshContainer mesh, const CVector3& scale)
{
	return m_pCreate->createTriangleObject(pos, material, mesh, scale);
}

//PmObjectをシーンから外す
//obj		:シーンから外すオブジェクト
void PmGameSummary::detachObject(PmObject* obj)
{
	m_pCreate->detachObject(obj);
}

//CItemを追加する
void PmGameSummary::createCItemObject(PmObjectDynamic* obj)
{
	m_pCreate->createCItemObject(obj);
}