//////////////////////////////////
//バルブ

#pragma once

#include	"CBreakGimmick.h"
#include	"PmRender.h"
#include	"CHand.h"
#include	"math.h"
#include	"CommonDefine.h"
#include	"CVRPlayerCamerah.h"

#define		COLLISIONRADIUS			3.0f		//バルブの当たり判定の大きさ

enum EHANDSTATE
{
	RIGHTHAND = 0,
	LEFTHAND,
	HANDNUM,
};

//バルブの当たりサーチ
class CValveSearch : public PmCollisionSearch
{
private:
	PmObject*	m_pHandObj[2];
public:
	CValveSearch(){ m_pHandObj[RIGHTHAND] = NULL; m_pHandObj[LEFTHAND] = NULL; };
	virtual ~CValveSearch(){};

	void onTrigger(PmObject* obj);

	//手がバルブに当たって居ればオブジェが入っている（それ以外はNULL）
	PmObject* getHandObj(int handNum){ return m_pHandObj[handNum]; };
};

//バルブ管理クラス
class CValveSummary : public CBreakGimmick
{
private:
	CMeshContainer		m_ValuveMesh;	//バルブ描画用メッシュ
	PmObjectDynamic*	m_pCollision;	//バルブ用当たり判定
	CValveSearch*		m_pValveSearch;	//バルブの衝突サーチ

	float				m_HisAng[HANDNUM];		//最初に握っていた手の角度
	
	CVector3			m_Rotation;		//バルブの回転値
	bool				m_bprevflag;	//バルブを握ったときのフラグ

	//テスト用のパイプメッシュ
	CMeshContainer		m_PipeMesh;		//パイプメッシュ
	PmObject*			m_pPipeObj;

public:
	CValveSummary(){};
	~CValveSummary(){};

	void Create();
	void Update();
	void RearUpdate();
	void Render();
	void DebugRender();
	void Release();

	void gimmickBreak();
};