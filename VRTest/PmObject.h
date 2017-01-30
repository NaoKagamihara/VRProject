//////////////////////////
//Physxを使用するためのクラス
//PmCollisionSearchは何も書いてない（書かないと）
//

#pragma once

#include		"PmPreparation.h"
#include		"Mof.h"
#include		"PmCollisionSearch.h"
#include		"PmObjectTags.h"

class PmCollisionSearch;

class PmObject
{
private:
	
	PxRigidActor*		m_RigidBody;
	bool				m_bCollSearch;
	DWORD				m_eObjTag;

public:
	
	//当たり判定時に呼び出す用のクラス
	PmCollisionSearch* m_CollSearch;

	//Actorを取得する
	PxRigidActor* getActor(){ return m_RigidBody; };

	//アクターをセットする
	void setActor(PxRigidActor& actor){ m_RigidBody = &actor; };

	//posの場所に座標をセットする
	void setGlobalPos(CVector3 pos);

	//座標を取得する
	CVector3 getGlobalPos();

	//rotの角度にセットする
	void setGlobalRot(CVector3 rot);
	void setGlobalRot(const CQuaternion& rot);

	//角度を取得する
	CVector3 getGlobalVecRot();
	CQuaternion getGlobalQuoRot();

	//m_RigidBodyのActorのMatrix44を取得する
	Matrix44 getActorMatrix44(CVector3 scale = CVector3(1.0f, 1.0f, 1.0f));

	//m_RigidBodyのnbShape番目のマトリックスを取得する
	Matrix44 getMatrix44(int nbShape, CVector3 scale = CVector3(1.0f,1.0f,1.0f));

	//m_RigidBodyのShapeの数を取得する
	MofU32 getNbShapes(){ return m_RigidBody->getNbShapes(); };

	//m_RigidBodyのnbShape番目のshapeを取得する
	PxShape* getShape(int nbShape);

	//m_RigidBodyにshapeを追加する
	void addShape(PxShape& shape){ m_RigidBody->attachShape(shape); };

	//m_RigidBodyのPxSpaheを切り離す
	void detachShape(PxShape* shape){ m_RigidBody->detachShape(*shape); };

	//m_RigidBodyのnbShape番目の形の種類を取得する
	PxGeometryType::Enum getGeometryType(int nbShape);

	//m_RigidBodyのnbShape番目が他のオブジェクトと衝突するのか調べる
	bool isCollision(int nbShape);

	//m_RigidBodyのnbShape番目の衝突するのかフラグを設定する:trueなら衝突
	void setCollision(int nbShape, bool flag);

	//m_RigidBodyがTrigger用のオブジェクトか取得する
	//デフォルトでfalse
	bool isTrigger(int nbShape);

	//m_RigidBodyをTrigger用のオブジェクトにする
	void setTrigger(int nbShape, bool flag);
	
	//m_RigidBodyのタイプを取得する::StaticかDynamic
	PxActorType::Enum getRigidBodyType(){ return m_RigidBody->getType(); };

	//CollisionSearchを使用するかどうかを取得する
	bool isOnCollisionSearch(){ return m_bCollSearch; };

	//CollisionSearchを使用するかどうかをセットする
	void setOnCollisionSearch(bool flag){ m_bCollSearch = flag; };

	//m_CollisionSearchを取得する
	PmCollisionSearch* getCollisionSearch(){ return m_CollSearch; }

	//m_CollisionSearchをセットする
	void setCollisionSearch(PmCollisionSearch* search){ m_CollSearch = search; }

	//m_eObjTagを取得する
	DWORD getObjectTag(){ return m_eObjTag; };
	
	//m_eObjTagをセットする
	void setObjectTag(DWORD tag){ m_eObjTag = tag; };

	//m_eObjeTagをチェックする
	int checkObjectTag(EPmObjectTag tag){ return m_eObjTag & tag; };

	//m_eObjeTagのフラグを切り替える
	void changeObjectTag(EPmObjectTag tag, bool flag){ flag ? m_eObjTag |= tag : m_eObjTag &= ~tag; };

	//m_CollisionSearchのメモリを開放する
	void releaseCollisionSearch(){ delete m_CollSearch; }

	//PmObjectのメモリを開放する
	void release();

	////////////////////////////////////////
	//取りあえず必要な関数はこれくらい

	PmObject();

	virtual ~PmObject(){ this->release(); };
	
};