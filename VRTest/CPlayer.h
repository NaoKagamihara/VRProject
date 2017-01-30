///////////////////////////
//

#pragma once

#include	"PmGameSummary.h"
#include	"CHand.h"
#include	"CVRPlayerCamerah.h"
#include	"CommonDefine.h"

//手の当たり判定の大きさ
#define		HANDCOLLR		0.5f

////////////////////////////////////////////////
//プレイヤーの操作管理クラス
class CPlayer : public PmGameSummary
{
private:

	//手
	CHand*	m_pRightHand;
	CHand*	m_pLeftHand;
	//VR用カメラ
	CVRPlayerCamera*	m_pCamera;

	//プレイヤーの座標
	CVector3	m_Pos;
	//移動できるインターバル
	int			m_MoveCount;

	HandData	m_rHandData;
	HandData	m_lHandData;

public:

	CPlayer(CVRCamera* camera);
	virtual ~CPlayer();

	virtual void Create();

	virtual void Initialize();

	virtual void Update();

	virtual void RearUpdate();

	virtual void Render();

	virtual void RearRender();

	virtual void DebugRender();

	virtual void Release();

private:
	void handUpdate();
	void debugUpdate();
};