
#pragma once

#include	"PmPreparation.h"
#include	"PmObjectDynamic.h"

////////////////////////////////////////
//Itemの状態
enum EItemState
{
	ITEM_FREE = (1 << 0),
	ITEM_HAVE = (1 << 1),
	ITEM_USE = (1 << 2),
};

////////////////////////////////////////
//Itemクラス
class CItem : public PmObjectDynamic
{
protected:

	EItemState		m_eItemState;

	//持たれていない状態にする
	void changeModeFREE();

	//持たれている状態にする
	void changeModeHave();

public:
	CItem(){};
	virtual ~CItem(){};

	//状態を返す
	EItemState getState(){ return m_eItemState; };

	//アイテムを持つ
	virtual void itemHave(const CVector3& pos, const CVector3& rot){};
	virtual void itemHave(const CVector3& pos, const CQuaternion& rot){};

	//アイテムを離す
	virtual void itemLost(const CVector3& vec){};

	//アイテムを使用する
	virtual void itemUse(){};

	//描画
	virtual void render() = 0;
	virtual void debugRender() = 0;
};