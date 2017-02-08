/////////////////////////////////////
//手

#pragma once

#include	"PmObject.h"
#include	"PmRender.h"
#include	"CItem.h"

//モデル描画時の大きさ
#define		MODELSCALE			0.1f

//////////////////////////////////////////////
//手の状態
enum EHandState
{
	HAND_FREE = (1 << 0),
	HAND_HAVE = (1 << 1),
	HAND_USE = (1 << 2),
	HAND_GRASP = (1 << 3),
};

////////////////////////////////////////////
//手の状態の更新に必要なデータ
struct HandData
{
	CVector3	pos;				//座標
	CQuaternion	handRot;			//手の角度
	bool		grasp;				//握るフラグ
	bool		button;				//使うフラグ

	//CVector3	poseVec;			//手の姿勢
};

///////////////////////////////////////////////////
//手の当たり判定サーチ
class CHandSearch : public PmCollisionSearch
{
private:
	//範囲内にあるアイテムのポインタ
	//何もない場合はNULL
	CItem*		m_pItem;
public:

	CHandSearch() : m_pItem(NULL) {};
	~CHandSearch(){};

	//範囲内のItemのポインタを保存する
	void onTrigger(PmObject* obj);

	//保存したItemを取得する
	CItem* getItem(){ return m_pItem; };

	//ItemをNULLにする
	void emp(){ m_pItem = NULL; };
};

///////////////////////////////////////////////////
//手の状態を管理するクラス
class CHand
{
private:
	CMeshContainer	m_Mesh;		//手のモデル
	PmObject*		m_pHandColl;	//手の当たり判定
	EHandState		m_eState;		//手の状態
	CItem*			m_pItem;		//持っているアイテム
	CHandSearch*	m_pSearch;		//トリガー
	CVector3		m_HysPos;		//１フレーム前の座標
	CQuaternion		m_HysRot;		//１フレーム前の向き
	CVector3		m_PoseVec;		//コントローラの姿勢

public:
	CHand(PmObject* obj,EPmObjectTag tag);
	~CHand();

	//読み込み
	void load();

	//更新
	void update(HandData& data);

	//描画
	void render();

	//デバッグ用の描画
	void debugRender();

	PmObject* getHandColl(){ return m_pHandColl; };

private:

	//手の状態に応じた処理
	void freeUpdate(HandData& data);
	void haveUpdate(HandData& data);
	void useUpdate(HandData& data);
	void graspUpdate(HandData& data);

};