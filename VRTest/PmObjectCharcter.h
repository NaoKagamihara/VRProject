  /////////////////////////////////////////
//キャラクタコントローラー

#pragma once

#include		"PmObjectTags.h"

#include		"PmPreparation.h"
#include		"Mof.h"
#include		<time.h>

class PmObjectCharacter
{
protected:
	PxController* m_Controller;
	clock_t m_time;
	PxControllerFilters m_filter;

	float m_MinDist;
	float m_Height;

public:

	//キャラクタを移動させる,戻り値のフラグでキャラの状態を取得
	PxControllerCollisionFlags move(CVector3 vec);

	//座標を取得する
	CVector3 getPosition()
	{
		PxExtendedVec3 vec = m_Controller->getPosition();
		return CVector3(vec.x, vec.y, vec.z);
	}

	//座標をセットする
	void setPosition(CVector3 vec){ m_Controller->setPosition(PxExtendedVec3(vec.x, vec.y, vec.z)); };

	//移動の際の再帰的な呼び出しの最小限の長さの
	float getMinDist(){ return m_MinDist; };

	//移動の際の再帰的な呼び出しの最小限の長さのセット（defaultで0
	void setMinDist(float minDist){ m_MinDist = minDist; };

	//キャラクタの高さを変更する
	void resize(float height){ m_Controller->resize(height); m_Height = height; };

	//カプセルの高さを取得する
	float getHeight(){ return m_Height; };

	//カプセルの高さをセットする
	void setHeight(float height){ m_Height = height; };

	//足元の座標を取得する
	CVector3 getFootPosition()
	{ 
		PxExtendedVec3 vec = m_Controller->getFootPosition();
		return CVector3(vec.x, vec.y, vec.z);
	}

	//キャラクタの上方向のベクトルを取得する
	CVector3 getUpDirection()
	{
		PxVec3 vec = m_Controller->getUpDirection();
		return CVector3(vec.x, vec.y, vec.z);
	}

	//キャラクタの上方向のベクトルをセットする
	void setUpDirection(CVector3 vec)
	{
		PxVec3 upVec(vec.x, vec.y, vec.z);
		m_Controller->setUpDirection(upVec);
	}

	//コントローラーの持つアクターを取得する
	PxRigidDynamic* getActor(){ return m_Controller->getActor(); };

	//衝突精度を取得する
	MofU32 getContactOffset(){ return m_Controller->getContactOffset(); };

	//衝突精度をセットする
	void setContactOffset(float con){ m_Controller->setContactOffset(con); };

	//歩ける地面の角度を取得する
	MofU32 getSlopeLimit(){ return m_Controller->getSlopeLimit(); };

	//歩ける地面の角度をセットする
	//staticオブジェクトのみに有効   
	void setSlopeLimit(float slope){ m_Controller->setSlopeLimit(MOF_COS(slope)); };

	//歩けない地面の角度の時の移動方法を取得する
	PxControllerNonWalkableMode::Enum getNonSlopeWalkMode(){ return m_Controller->getNonWalkableMode(); };

	//歩けない地面の角度の時の移動方法をセットする
	//PxControllerNonWalkableMode::eFORCE_SLIDING							//デフォルト。制限なし
	//PxControllerNonWalkableMode::ePREVENT_CLIMBING						//斜面を上る動きのみを制限する
	//PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING		//斜面を登る動きを制限＋斜面を滑り落ちる
	void setNonSlopeWalkMode(PxControllerNonWalkableMode::Enum mode){ m_Controller->setNonWalkableMode(mode); };

	//自動で登れる段差の高さを取得する
	MofU32 getStepOffset(){ return m_Controller->getStepOffset(); };

	//自動で登れる段差の高さをセットする
	void setStepOffset(float step){ m_Controller->setStepOffset(step); };

	//コントローラーのタイプを取得する
	//PxControllerShapeType::　でカプセルかボックスか調べる
	PxControllerShapeType::Enum getShapeType(){ return m_Controller->getType(); };

	//コントローラーのshapeを描画する
	virtual void render(Mof::Color color) = 0;

	//このクラスのメモリを開放する
	void release();

	//コンストラクタ
	PmObjectCharacter(PxController* con){ m_Controller = con; };

	virtual ~PmObjectCharacter(){ release(); };
};

class PmObjectCapsuleCharacter : public PmObjectCharacter
{
private:
	float m_Radius;
public:

	//カプセルの半径を取得する
	float getRadius(){ return m_Radius; };

	//カプセルの半径をセットする（描画用）
	void setRadius(float radius){ m_Radius = radius; };

	//カプセルを描画する
	void render(Mof::Color color);

	//コンストラクタ
	PmObjectCapsuleCharacter(PxController* con) : PmObjectCharacter(con){};

	virtual ~PmObjectCapsuleCharacter(){};
};