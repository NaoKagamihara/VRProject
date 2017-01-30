
/////////////////////////////////
//引き出しの当たり判定サーチ

#pragma once

#include		"PmCollisionSearch.h"
#include		"PmObject.h"

class CDrawerCollSearch : public PmCollisionSearch
{
private:
	bool		m_Poss;	//掴めるかどうか
	bool		m_On;	//掴める状態フラグ
	PmObject*	m_pHand;//当たって居る手
	CVector3	m_dPos;	//前の座標
	CVector3	m_Pos;	//座標
public:
	CDrawerCollSearch(){ m_Poss = false; m_On = false; };
	~CDrawerCollSearch(){};

	void onTrigger(PmObject* obj);
	void update();

	bool getOnGrasp(){ return m_On; };
	CVector3 getdPos(){ return m_dPos; };
	CVector3 getPos(){ return m_Pos; };
};